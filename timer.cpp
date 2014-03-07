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
	//initialize timer state
	#ifdef VERBOSE
	printf("Timer %ld is created\n", m_rank);
	#endif

	m_send_count = sizeof(m_send_buffer)/sizeof(m_send_buffer[0]);
	m_recv_count = sizeof(m_recv_buffer)/sizeof(m_recv_buffer[0]);
}

bool CTimer::on_new_message(int source, int message_id, CActor* actor){

	if(message_id == MSG_START_TIMER){//start timer

		printf("Starting simulation...\n");
	

		ofstream out_file; //file to write cell details
		char file_name[255];

		for(int i=0; i < m_iterations; ++i){
			//sleep for specified interval
			sleep(m_interval);

			printf("Year %ld\n", i+1);			
			//printf("Total frogs in the simulation %ld\n",
			//	actor->get_total_actors());
			sprintf(file_name, "sim_year_%ld.txt", i+1);		
			
			out_file.open(file_name);

			for(int j = m_cell_begin; j <= m_cell_end; ++j){
				int source, msg_id;
				//notify cells arrival of monsoon
			  	actor->send_message(m_send_buffer,j,
							MSG_MONSOON_ARRIVED);
				actor->recv_message(m_recv_buffer, &source, 
								&msg_id);
			
				//record data in file
				out_file << m_recv_buffer[0] << ", ";
				out_file << m_recv_buffer[1] << endl;
			/*	
				printf("cell %ld= %f, %f\n",
					j, m_recv_buffer[0], 
					   m_recv_buffer[1]);
			*/
			}

			out_file.close();
		}
	}
	
	printf("End Simulation\n");
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

