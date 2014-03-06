#include "callback.h"
#include "factory.h"
#include "actor.h"
#include "timer.h"

#include <unistd.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include "message_defs.h"

using namespace std;

CTimer::CTimer(int rank, int cell_begin, int cell_end, 
		int interval, int iterations) : 
			m_rank(rank), 
			m_cell_begin(cell_begin),
			m_cell_end(cell_end),
			m_interval(interval),
			m_iterations(iterations){
	#ifdef VERBOSE
	printf("Timer %ld is created\n", m_rank);
	#endif

	m_send_count = sizeof(m_send_buffer)/sizeof(m_send_buffer[0]);
	m_recv_count = sizeof(m_recv_buffer)/sizeof(m_recv_buffer[0]);
}

bool CTimer::on_new_message(int source, int message_id, CActor* actor){

	if(message_id == MSG_START_TIMER){//start timer

		#ifdef VERBOSE
		printf("Starting timer\n");
		#endif

		ofstream out_pop; //file to write cell population
		ofstream out_inf; //file to write cell infection
		char pop_file_name[255];
		char inf_file_name[255];

		for(int i=0; i < m_iterations; ++i){
			sleep(m_interval);

			sprintf(pop_file_name, "population_year_%ld.txt", i);	
			sprintf(inf_file_name, "infection_year_%ld.txt", i);
			
			out_pop.open(pop_file_name);
			out_inf.open(inf_file_name);

			for(int j = m_cell_begin; j <= m_cell_end; ++j){
				//notify arrival of monsoon
			  	actor->send_message(m_send_buffer,
					m_send_count, j,MSG_MONSOON_ARRIVED);
				actor->recv_message(m_recv_buffer, 2, j);
			
				/*record data in file*/
				out_pop << m_recv_buffer[0] << endl;
				out_inf << m_recv_buffer[1] << endl;

				printf("cell %ld= %f, %f\n",
					j, m_recv_buffer[0], 
					   m_recv_buffer[1]);
			}

			out_pop.close();
			out_inf.close();
		}
		
		printf("End timer\n");
	}
	actor->shutdown();
	
	return false;
}

void CTimer::on_stop(CActor* actor){
	#ifdef VERBOSE
	printf("Timer %ld is deleted\n", m_rank);
	#endif
}

void* CTimer::get_recv_buffer(){
	return (void*)m_recv_buffer;
}

int CTimer::get_recv_buffer_count(){
	return 4;
}

