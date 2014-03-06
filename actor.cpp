#include <stdio.h>
#include <mpi.h>

//#include <unistd.h>

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
	sendMessage(buffer, dest, message_id);
	
	/*
	static MPI_Request request;
	struct PP_Control_Package message;

	message.command = PP_CUSTOM;
	message.param_a = ((float*)buffer)[0];
	message.param_b = ((float*)buffer)[1];
	message.param_c = ((float*)buffer)[2];

	MPI_Isend(&message, 1, PP_COMMAND_TYPE, dest, 
				message_id, MPI_COMM_WORLD, &request);
	*/

	//MPI_Ssend(&message, 1, PP_COMMAND_TYPE, dest, 
					//message_id, MPI_COMM_WORLD);
	//MPI_Ssend(buffer, length, MPI_FLOAT, dest, message_id, MPI_COMM_WORLD);
}

void CActor::recv_message(void* buffer, int length, int* source, int* msg_id){
	recvMessage(buffer, source, msg_id);


	/*
	MPI_Status status;
	struct PP_Control_Package message;

	MPI_Recv(&message, 1, PP_COMMAND_TYPE, 
					MPI_ANY_SOURCE, MPI_ANY_TAG,
					MPI_COMM_WORLD, &status);

	((float*)buffer)[0] = message.param_a;
	((float*)buffer)[1] = message.param_b;
	((float*)buffer)[2] = message.param_c;

	*source = status.MPI_SOURCE;

//if(*source == 0 && message.command != PP_CUSTOM)	
printf("MSG %ld from %ld\n", message.command, status.MPI_SOURCE);

	if(status.MPI_SOURCE == 0 && message.command == PP_STOP){
		*msg_id = PP_STOP;
	} else {
		*msg_id = status.MPI_TAG;
	}
	*/
	//printf("RANK %ld\n", message.command);
	/*MPI_Status status;
	MPI_Recv(buffer, length, MPI_FLOAT,  
		 source, MPI_ANY_TAG, 
		 MPI_COMM_WORLD, &status);
	*/
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
			/*MPI_Status status;
			MPI_Recv(callback->get_recv_buffer(),
			 	callback->get_recv_buffer_count(),
			 	MPI_FLOAT,  MPI_ANY_SOURCE,
			 	MPI_ANY_TAG, MPI_COMM_WORLD, &status);*/
	
			int source, msg_id;	
			//printf("%d waiting for message\n", myRank);
			recv_message(callback->get_recv_buffer(), 0, 
							&source, &msg_id);
			//printf("%d got message\n", myRank);

			if(msg_id == PP_STOP) {
			//	printf("RECEIVED MESSAGE TO DIE %ld\n", myRank);
				break;
			}

			bool done = callback->on_new_message(source, msg_id,
									this);

			/*bool done = callback->on_new_message(status.MPI_SOURCE,
							     status.MPI_TAG,
							     this);
			*/

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
					printf("No work available Killing %ld\n", myRank);
					break;
				}
			}
		}
	} else if (statusCode == 2){
		//callback->on_load(this);
		int masterStatus = 1;
		while(masterStatus){
			masterStatus = masterPoll();
		}	
		callback->on_stop(this);
		printf("MASTER DEAD\n");
	}

	//callback->on_stop(this);

	processPoolFinalise();
	MPI_Finalize();
}
