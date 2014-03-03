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

		return new CTimer(processId, 
				  2, m_num_cells+1, 
				  1, 10/*m_iterations*/);

	} else if(processId <= m_num_cells+1 ){
		return new CCell(processId);
	} else{
		return new CFrog(processId);
	}
}
