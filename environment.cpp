#include "callback.h"
#include "factory.h"
#include "actor.h"
#include "environment.h"
#include "frog-functions.h"
#include <stdio.h>

CEnvironment::CEnvironment(int num_cells,
			   int initial_frogs,
			   int infected_frogs):
				m_num_cells(num_cells),
				m_initial_frogs(initial_frogs),
				m_infected_frogs(infected_frogs){}

void CEnvironment::create_cells(CActor* actor){
	for(int i = 0; i<m_num_cells; ++i){ //create grid cells
		actor->spawn_process();
	}
}

void CEnvironment::create_initial_frogs(CActor* actor){
	float pos_x 	=  0.0f;
	float pos_y 	=  0.0f;
	for(int i = 0; i < m_initial_frogs; ++i){ //initial frogs
		int procId = actor->spawn_process();
		long state = i;	

		//generate random positions
		frogHop(0.0f, 0.0f, &pos_x, &pos_y, &state);

		m_send_buffer[0] = pos_x; //x position
		m_send_buffer[1] = pos_y; //y position
		
		if(i<m_infected_frogs) 	m_send_buffer[2] = 1; //infected?
		else    		m_send_buffer[2] = 0;

		//actor->send_message(procId, 3, this);
		actor->send_message(m_send_buffer, 4, procId, 3);
	}
}

int CEnvironment::create_timer(CActor* actor){
	return actor->spawn_process();
}

void CEnvironment::start_timer(int timerId, CActor* actor){
	//actor->send_message(timerId, 4, this);
	actor->send_message(m_send_buffer, 4, timerId, 4);
}

void CEnvironment::on_load(CActor* actor){

	long seed = -10;
	initialiseRNG(&seed);

	int timerId = create_timer(actor);
	create_cells(actor);
	create_initial_frogs(actor);
	start_timer(timerId, actor);
}
/*
void* CEnvironment::get_send_buffer(){
	return (void*)m_send_buffer;
}

int CEnvironment::get_send_buffer_count(){
	return 4;
}
*/
