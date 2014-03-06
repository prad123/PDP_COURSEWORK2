#ifndef __ACTOR__
#define __ACTOR__

/**@brief class implementing the Actor pattern using process pool module
 * @author
 * @date March 20014
 */

class CActor{
private:
	CCallbackFactory&  m_factory; /*!< factory to create callback objects*/
public:
	/**constructor to create this object
 	*@param callback_factory -  stores domain specific factory
 	*/
	CActor(CCallbackFactory& callback_factory);

	/**routine to send MPI messages to other actors
 	*@param buffer - message data
	*@param msg_length - message length
	*@param dest - destination process
	*@param message_id - message identifier
 	*/
	void send_message(void* buffer,int msg_length,int dest,int message_id);

	/**routine to receive MPI message from other actors
 	*@param buffer - receive buffer
	*@param msg_length - receive buffer length
	*@param source - sender identifier 	
 	*/
	void recv_message(void* buffer, int msg_length, int source);

	/**shutdown actor
 	*/
	void shutdown();
	
	/**create new process, uses process pool to create new actor
 	*/
	int spawn_process();
	
	/**kill or sleep process
 	*/
	void kill_process();

	/**actor main loop to receive message from other actors
 	*and handle messages 	
 	*/
	void run_message_loop();
};

#endif
