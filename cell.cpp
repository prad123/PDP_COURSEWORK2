#include "callback.h"
#include "factory.h"
#include "actor.h"
#include "cell.h"
#include "message_defs.h"

#include <stdio.h>

CCell::CCell(int rank) : m_rank(rank){
	#ifdef VERBOSE
	printf("Cell %ld is created\n", m_rank);
	#endif
	m_send_count = sizeof(m_send_buffer)/sizeof(m_send_buffer[0]);
	m_recv_count = sizeof(m_recv_buffer)/sizeof(m_recv_buffer[0]);
}

void CCell::visit(int source, CActor* actor){
	m_send_buffer[0] = m_population;
	m_send_buffer[1] = m_infection_level;

	m_population += 1.0f;
	if(m_recv_buffer[0] != 0){//frog is infected
		m_infection_level += 1.0f;
	}
		
	actor->send_message(m_send_buffer, m_send_count, 
						source, MSG_CELL_INFO);
}

void CCell::wipe_disease(int source, CActor* actor){
	m_send_buffer[0] = m_population;
	m_send_buffer[1] = m_infection_level;

	actor->send_message(m_send_buffer, 2/*m_send_count*/, 
						source, MSG_CELL_INFO);

	m_population      = 0.0f;
	m_infection_level = 0.0f;
}

void CCell::on_load(CActor* actor){
	m_population 		= 0.0f;
	m_infection_level 	= 0.0f;
}

bool CCell::on_new_message(int source, int message_id, CActor* actor){

	if(message_id == MSG_VISIT_CELL){//frog arrived

		#ifdef VERBOSE
		printf("Frog %d arrived in cell %d\n", source, m_rank);
		#endif
	
		visit(source, actor);
	/*	
		m_send_buffer[0] = m_population;
		m_send_buffer[1] = m_infection_level;

		m_population += 1.0f;
		if(m_recv_buffer[0] != 0){//frog is infected
			m_infection_level += 1.0f;
		}
		
		actor->send_message(m_send_buffer, m_send_count, 
						source, MSG_CELL_INFO);
	*/

	} else if (message_id == MSG_MONSOON_ARRIVED){//monsoon arrived

		#ifdef VERBOSE
		printf("Monsoon arrived in cell %ld\n", m_rank);
		#endif
		wipe_disease(source, actor);
		/*
		m_send_buffer[0] = m_population;
		m_send_buffer[1] = m_infection_level;

		actor->send_message(m_send_buffer, m_send_count, 
						source, MSG_CELL_INFO);

		m_population      = 0.0f;
		m_infection_level = 0.0f;
		*/
	}
	
	return true;
}

void CCell::on_stop(CActor* actor){
	#ifdef VERBOSE
	printf("Cell %ld is deleted\n", m_rank);
	#endif
}

void* CCell::get_recv_buffer(){
	return (void*)m_recv_buffer;
}

int CCell::get_recv_buffer_count(){
	return m_recv_count;
}

