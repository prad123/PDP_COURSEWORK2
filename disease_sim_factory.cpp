#include "callback.h"
#include "factory.h"
#include "disease_sim_factory.h"
#include "environment.h"
#include "frog.h"
#include "cell.h"
#include "timer.h"

#include <stdio.h>

CDiseaseSimFactory::CDiseaseSimFactory(int num_cells,
				       int initial_frogs,
				       int infected_frogs,
				       int iterations) : 
					m_num_cells(num_cells),
					m_initial_frogs(initial_frogs),
					m_infected_frogs(infected_frogs),
					m_iterations(iterations){}

CCallback* CDiseaseSimFactory::make_callback(unsigned int processId){

	if(processId == 0){
		return new CEnvironment(m_num_cells,
					m_initial_frogs,
					m_infected_frogs);
	} else if(processId == 1){

		const int first_cell_id = 2;
		const int last_cell_id  = m_num_cells + 1;
		const int timer_interval = 1; //secnds

		return new CTimer(processId, 
				  first_cell_id, 
			          last_cell_id, 
				  timer_interval, 10/*m_iterations*/);

	} else if(processId <= m_num_cells+1 ){
		return new CCell(processId);
	} else{
		return new CFrog(processId);
	}
}
