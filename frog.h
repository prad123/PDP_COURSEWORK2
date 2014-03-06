#ifndef __FROG__
#define __FROG_

#define POP_CELLS 			300l
#define INF_CELLS			500l

/**
 * @brief class used to model frogs in
 * the simulation
 * @author:
 * @date March 2014
 */

class CFrog : public CCallback{

	float m_send_buffer[4]; /*!< send buffer*/
	int m_send_count;

	float m_recv_buffer[4]; /*!< buffer to receive message*/
	int m_recv_count;

	float m_population[POP_CELLS]; /*!< population of last 300 cells*/
	float m_inf_level[INF_CELLS];  /*!< infection of last 300 cells*/
	float m_pos_x; /*!< frogs cell position - x*/
	float m_pos_y; /*!< frogs cell position - y*/
	int m_rank; /*!< MPI process id*/
	int m_hops; /*!< total hops*/
	int m_infected; /*!< is frog infected*/
	long m_state; /*!< initial seed for biologists function*/
	
	/**hop to a new location
 	*@param actor - actor to send message to cell 
 	*/ 
	void hop(CActor* actor);

	/**spawn new frogs
 	*@param actor - actor to wake new processes  
 	*/	 
	void spawn(CActor* actor);
public:
	/**constructor to initialize frog
 	*@param rank - MPI process id 
 	*/ 
	CFrog(int rank);

	/**callback function called by actor to receive new message
 	*@return pointer to receive buffer 	
	 */
	void* get_recv_buffer();

	/**callback function called by actor to get receive buffer length
 	*@return receive buffer count
 	*/ 
	int get_recv_buffer_count();	

	/**callback function called by actor to notify new message
 	*@param source - identifier of process sending the message
	*@param message_id - message identifier
	*@param actor - used by function to send/receive message
	*@return 1 to continue handling message 0 to terminate
 	*/
	bool on_new_message(int source, int message_id, CActor* actor);

	/**callback function called by actor to initialize object
 	*@param actor - used by function to send/receive message 
 	*/
	void on_load(CActor* actor);

	/**callback function called by actor before destroying object
 	*@param actor - used by function to send/receive message 
 	*/
	void on_stop(CActor* actor);
};

#endif
