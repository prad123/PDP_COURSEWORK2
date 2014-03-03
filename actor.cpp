#include <stdio.h>
#include <mpi.h>

//#include <unistd.h>

#include "callback.h"
#include "factory.h"
#include "actor.h"

#include "pool.h"

CActor::CActor(CCallbackFactory& callback_factory)
	: m_factory(callback_factory){}

int CActor::spawn_process(){
	return startWorkerProcess();
}

void CActor::kill_process(){
	workerSleep();
}
/*
void CActor::send_message(int dest, int message_id, CCallback* callback){
	MPI_Ssend(callback->get_send_buffer(),
		  callback->get_send_buffer_count(),
		  MPI_FLOAT, dest,
		  message_id, MPI_COMM_WORLD);
}*/

void CActor::send_message(void* buffer, int length, int dest, int message_id){
	MPI_Ssend(buffer, length, MPI_FLOAT, dest, message_id, MPI_COMM_WORLD);
}

void CActor::recv_message(void* buffer, int length, int source){
	MPI_Status status;
	MPI_Recv(buffer, length, MPI_FLOAT,  
		 source, MPI_ANY_TAG, 
		 MPI_COMM_WORLD, &status);
}

void CActor::shutdown(){
	shutdownPool();
}

void CActor::run_message_loop(){

	MPI_Init(NULL, NULL);

	int myRank;
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	
	int statusCode = processPoolInit();
		
	CCallback* callback = m_factory.make_callback(myRank);	
	callback->on_load(this);

	if(statusCode == 1){
		while(1){
			MPI_Status status;
			MPI_Recv(callback->get_recv_buffer(),
			 	callback->get_recv_buffer_count(),
			 	MPI_FLOAT,  MPI_ANY_SOURCE,
			 	MPI_ANY_TAG, MPI_COMM_WORLD, &status);

			bool done = callback->on_new_message(status.MPI_SOURCE,
							     status.MPI_TAG,
							     this);
			if(!done){
				printf("Killing %ld\n", myRank);
				callback->on_stop(this);
				delete callback;
				int work_available  = workerSleep();
				if(work_available){
					printf("New incarnation for %ld\n",
						myRank);
					callback = m_factory.make_callback(
								myRank);
					callback->on_load(this);
					continue;
					
				} else { 
					printf("Killing %ld\n", myRank);
					break;
				}
			}
			//1. might sleep and wake to create a new frog
			//2. 

		}
	} else {
		//callback->on_load(this);
		int masterStatus = 1;
		while(masterStatus){
			masterStatus = masterPoll();
		}	
		printf("MASTER DEAD\n");
	}

	callback->on_stop(this);

	processPoolFinalise();
	MPI_Finalize();
}
