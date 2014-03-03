#include "callback.h"
#include "factory.h"
#include "actor.h"
#include "cell.h"

#include <stdio.h>

CCell::CCell(int rank) : m_rank(rank){
	#ifdef VERBOSE
	printf("Cell %ld is created\n", m_rank);
	#endif
}

void CCell::on_load(CActor* actor){
	m_population 		= 0.0f;
	m_infection_level 	= 0.0f;
}

bool CCell::on_new_message(int source, int message_id, CActor* actor){

	if(message_id == 1){//frog arrived

		#ifdef VERBOSE
		printf("Frog %d arrived in cell %d\n", source, m_rank);
		#endif
		
		m_send_buffer[0] = m_population;
		m_send_buffer[1] = m_infection_level;

		m_population += 1.0f;
		if(m_recv_buffer[0] != 0){//frog is infected
			m_infection_level += 1.0f;
		}
		
		//actor->send_message(source, 0, this);
		actor->send_message(m_send_buffer, 4, source, 0);

	} else if (message_id == 2){//monsoon arrived

		#ifdef VERBOSE
		printf("Monsoon arrived in cell %ld\n", m_rank);
		#endif
		m_send_buffer[0] = m_population;
		m_send_buffer[1] = m_infection_level;

		actor->send_message(m_send_buffer, 2, source, 0);

		m_population      = 0.0f;
		m_infection_level = 0.0f;

	}
	
	return true;
}

void CCell::on_stop(CActor* actor){
	#ifdef VERBOSE
	printf("Cell %ld is deleted\n", m_rank);
	#endif
}
/*
void* CCell::get_send_buffer(){
	return (void*)m_send_buffer;
}*/

void* CCell::get_recv_buffer(){
	return (void*)m_recv_buffer;
}
/*
int CCell::get_send_buffer_count(){
	return 4;
}
*/

int CCell::get_recv_buffer_count(){
	return 4;
}

