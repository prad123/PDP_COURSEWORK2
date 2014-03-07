#include <stdlib.h>
#include <stdio.h>
#include "callback.h"
#include "factory.h"
#include "disease_sim_factory.h"
#include "actor.h"

int main(int argc, char* argv[]){
	int num_cells 	   = 0;
	int initial_frogs  = 0;
	int infected_frogs = 0;
	int iterations	   = 0;

	if(argc < 5){
		printf("Usage: ./sim grid_cell_count initial_frog_count ");
		printf("initial_infected_frog_count iteration_count");
		printf(
		"Example: ./sim 16 34 4 100\n");
		return -1;
	}

	num_cells 	= atoi(argv[1]);
	initial_frogs 	= atoi(argv[2]);
	infected_frogs  = atoi(argv[3]);
	iterations	= atoi(argv[4]);

	CDiseaseSimFactory simFactory(num_cells, 
					initial_frogs, 
					infected_frogs,
					iterations);
	CActor actor(simFactory);
	
	actor.run_message_loop();
	return 0;
}
