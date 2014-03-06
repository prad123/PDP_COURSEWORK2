#include "callback.h"
#include "factory.h"
#include "actor.h"
#include "frog.h"
#include "frog-functions.h"
#include "message_defs.h"

#include <stdio.h>

CFrog::CFrog(int rank) 
	: m_hops(0), m_pos_x(0), m_pos_y(0), m_infected(0), m_rank(rank){
	m_state = -1 - m_rank;
	m_send_count = sizeof(m_send_buffer)/sizeof(m_send_buffer[0]);
	m_recv_count = sizeof(m_recv_buffer)/sizeof(m_recv_buffer[0]);
}

void CFrog::hop(CActor* actor){

	m_hops++;

	frogHop(m_pos_x, m_pos_y, &m_pos_x, &m_pos_y, &m_state);
	int newPosition = getCellFromPosition(m_pos_x, m_pos_y);	
	m_send_buffer[0] = m_infected;	
	
	actor->send_message(m_send_buffer, m_send_count, 
				newPosition+2, MSG_VISIT_CELL);
	/*Note: +2 added to new position to adjust for cell MPI process id
 	* process 0 is environment	
 	* process 1 is timer
 	* process 2 onwards cells
 	* */
}

void CFrog::on_load(CActor* actor){
	for(int i = 0; i < POP_CELLS; ++i){
		m_population[i] = 0.0f;
	}
	for(int i = 0; i < INF_CELLS; ++i){
		m_inf_level[i] = 0.0f;
	}

	initialiseRNG(&m_state);

	/*get ramdom default position*/
	frogHop(0,0, &m_pos_x, &m_pos_y, &m_state);
}

void CFrog::spawn(CActor* actor){

	int child_id = actor->spawn_process();
	//child inherits parents position
	m_send_buffer[0] = m_pos_x;//pos x
	m_send_buffer[1] = m_pos_y;//pos y
	m_send_buffer[2] = 0; //child not infected

	//creat new frog at parents location
	actor->send_message(m_send_buffer, m_send_count, 
					child_id, MSG_SPAWN_NEW_FROG);
}

bool CFrog::on_new_message(int source, int message_id, CActor* actor){
	
 
	if(message_id == MSG_CELL_INFO){//hoped to new cell
		m_population[m_hops%POP_CELLS] = m_recv_buffer[0];
		m_inf_level[m_hops%INF_CELLS]  = m_recv_buffer[1];

		//can spawn?
		if(m_hops >= POP_CELLS){
			float avg_pop = 0.0f;
			for(int i = 0; i < POP_CELLS; ++i) {
				avg_pop += m_population[i];
			}
			avg_pop /= POP_CELLS;

			int can_spawn = willGiveBirth(avg_pop, &m_state);
			if(can_spawn) {
			#ifdef VERBOSE
				printf("Frog %d spawning\n", m_rank);
			#endif
				//spawn(actor);
			}
		}	

		//is infected?
		if((m_infected == 0) && (m_hops >= INF_CELLS)){
			float avg_inf_level = 0.0f;
			for(int i=0; i<INF_CELLS; ++i){
				avg_inf_level += m_inf_level[i];
			}
			avg_inf_level /= INF_CELLS;
			m_infected = willCatchDisease(avg_inf_level, &m_state);
			#ifndef VERBOSE
			if(m_infected)
				printf("Frog %ld is infected\n", m_rank);
			#endif
		}

		//will die?		
		if((m_infected == 1) && (m_hops%700 == 0)){
			if(willDie(&m_state)){
				#ifndef VERBOSE
				printf("Frog %ld is dead\n", m_rank);
				#endif
				return false;
			}
		}
		
		
	} else if(message_id == MSG_NEW_FROG_BORN) { //new frog is born
		m_pos_x    = m_recv_buffer[0]; //x position
		m_pos_y    = m_recv_buffer[1]; //y position
		m_infected = m_recv_buffer[2];	//is infected?
		#ifdef VERBOSE
		printf("Frog born at position(%f,%f)\n", 
						m_pos_x, m_pos_y);
		#endif
	}
	hop(actor);//hop to new location
	return true;
}	

void CFrog::on_stop(CActor* actor){
	#ifdef VERBOSE
	printf("Frog %ld is deleted\n", m_rank);
	#endif	
}


void* CFrog::get_recv_buffer(){
	return (void*)m_recv_buffer;
}

int CFrog::get_recv_buffer_count(){
	return m_recv_count;
}
