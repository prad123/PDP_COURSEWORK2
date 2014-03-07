#include <stdio.h>
#include <mpi.h>

#include "callback.h"
#include "factory.h"
#include "actor.h"

#include "pool.h"

extern MPI_Datatype PP_COMMAND_TYPE;

CActor::CActor(CCallbackFactory& callback_factory)
	: m_factory(callback_factory){}

int CActor::spawn_process(){
	return startWorkerProcess();
}

int CActor::get_total_actors(){
	return getProcCount();
}

void CActor::send_message(void* buffer, int dest, int message_id){
	sendMessage(buffer, dest, message_id);
}

void CActor::recv_message(void* buffer, int* source, int* msg_id){
	recvMessage(buffer, source, msg_id);
}

void CActor::shutdown(){
	shutdownPool();
}

void CActor::run_message_loop(){

	MPI_Init(NULL, NULL);

	int myRank;
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	
	int statusCode = processPoolInit();
	//create object based on process id
	CCallback* callback = m_factory.make_callback(myRank);	
	callback->on_load(this);

	if(statusCode == 1){
		while(1){
			int source = 0, msg_id =0;	
			recv_message(callback->get_recv_buffer(), 
							&source, &msg_id);

			if(msg_id == PP_STOP) {
				break;
			}
			//notify actor of new message
			bool done = callback->on_new_message(source, msg_id,
									this);
			if(!done){
				//task complete, release resources
				callback->on_stop(this);
				delete callback;
				callback = NULL;

				int work_available  = workerSleep();
				if(work_available){
					#ifdef VERBOSE
					printf("New incarnation of %ld\n",
						myRank);
					#endif
					callback = m_factory.make_callback(
									myRank);
					callback->on_load(this);
					continue;
					
				} else { 
					//got a shutdown message from pool
					break;
				}
			}
		}
	} else if (statusCode == 2){
		int masterStatus = 1;
		while(masterStatus){
			masterStatus = masterPoll();
		}
		#ifdef VERBOSE
		printf("Master terminating\n");
		#endif
	}

	if(callback != NULL){
		callback->on_stop(this);
		delete callback;
	}

	processPoolFinalise();
	MPI_Finalize();
}
