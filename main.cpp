#include <stdio.h>
#include "callback.h"
#include "factory.h"
#include "disease_sim_factory.h"
#include "actor.h"

int main(){
	int num_cells 	   = 16;
	int initial_frogs  = 34;
	int infected_frogs = 4;
	int iterations	   = 100;

	CDiseaseSimFactory simFactory(num_cells, 
					initial_frogs, 
					infected_frogs,
					iterations);
	CActor actor(simFactory);
	
	actor.run_message_loop();
	return 0;
}
