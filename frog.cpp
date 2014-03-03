#include "callback.h"
#include "factory.h"
#include "actor.h"
#include "frog.h"
#include "frog-functions.h"

#include <stdio.h>

CFrog::CFrog(int rank) 
	: m_hops(0), m_pos_x(0), m_pos_y(0), m_infected(0), m_rank(rank){
	//printf("Frog %ld is created\n", m_rank);
}

void CFrog::hop(CActor* actor){
	long state = m_hops;

	m_hops++;

	frogHop(m_pos_x, m_pos_y, &m_pos_x, &m_pos_y, &state);
	int newPosition = getCellFromPosition(m_pos_x, m_pos_y);	
	m_send_buffer[0] = m_infected;
	//actor->send_message(newPosition + 2, 1, this);	
	actor->send_message(m_send_buffer, 4, newPosition+2, 1);
}

void CFrog::on_load(CActor* actor){
	for(int i = 0; i < POP_CELLS; ++i){
		m_population[i] = 0.0f;
	}
	for(int i = 0; i < INF_CELLS; ++i){
		m_inf_level[i] = 0.0f;
	}

	long seed  = -1 - m_rank;
	long state = m_rank;
	initialiseRNG(&seed);

	/*get default position*/
	frogHop(0,0, &m_pos_x, &m_pos_y,&state);
}

void CFrog::spawn(CActor* actor){
	int child_id = actor->spawn_process();
	//child inherits parents position
	m_send_buffer[0] = m_pos_x;//pos x
	m_send_buffer[1] = m_pos_y;//pos y
	m_send_buffer[2] = 0; //child not infected
	//actor->send_message(child_id, 3, this);//child is born
	actor->send_message(m_send_buffer, 4, child_id, 3);
}

bool CFrog::on_new_message(int source, int message_id, CActor* actor){
	
	//long state = -1 - m_rank;
	long state = -10; //using trial & error 
	if(message_id == 0){//hoped to new cell
		m_population[m_hops%POP_CELLS] = m_recv_buffer[0];
		m_inf_level[m_hops%INF_CELLS]  = m_recv_buffer[1];

		//can spawn
		if(m_hops >= POP_CELLS){
			float avg_pop = 0.0f;
			for(int i = 0; i < POP_CELLS; ++i) {
				avg_pop += m_population[i];
			}
			avg_pop /= POP_CELLS;

			int can_spawn = willGiveBirth(avg_pop, &state);
			if(can_spawn) {
			#ifdef VERBOSE
				printf("Frog %d spawning\n", m_rank);
			#endif
				//spawn(actor);
			}
		}	

		//is infected
		if((m_infected == 0) && (m_hops >= INF_CELLS)){
			float avg_inf_level = 0.0f;
			for(int i=0; i<INF_CELLS; ++i){
				avg_inf_level += m_inf_level[i];
			}
			avg_inf_level /= INF_CELLS;
			m_infected = willCatchDisease(avg_inf_level, &state);
			#ifdef VERBOSE
			if(m_infected)
				printf("Frog %ld is infected\n", m_rank);
			#endif
		}

		//will die?		
		if((m_infected == 1) && (m_hops%700 == 0)){
			if(willDie(&state)){
				printf("Frog %ld is dead\n", m_rank);
				//actor->kill_process();
				return false;
			}
		}
		
		
	} else if(message_id == 3) { //new frog is born
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
/*
void* CFrog::get_send_buffer(){
	return (void*)m_send_buffer;
}
*/

void* CFrog::get_recv_buffer(){
	return (void*)m_recv_buffer;
}
/*
int CFrog::get_send_buffer_count(){
	return 4;
}
*/

int CFrog::get_recv_buffer_count(){
	return 4;
}
