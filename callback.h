#ifndef __CALLBACK__
#define __CALLBACK__

/**@brief base class to add domain specific behaviour to actor framework
 * @author
 * @date March 2014
 */

class CActor;
class CCallback{
public:
	/**callback and default implementation
 	* to get receive buffer for new message
 	*@return returns receive buffer	 
 	*/
	virtual void* get_recv_buffer() {return (void*)0;}
	
	/**callback and default implementation
 	* to get receive buffer length
 	*@return returns receive buffer length 
 	*/
	virtual int   get_recv_buffer_count() {return 0;}

	/**callback and default implementation
 	*to handle new message
	*@param source - sending process identifier
	*@param message_id - message identifier
	*@param actor - used to send/receive message
	*@return 1 to continue processing message 0 to exit 
 	*/
	virtual bool on_new_message(int source, int message_id, CActor* actor) 
				{ return false; };
	
	/**callback and default implementation
 	*to initialize this object
	*@param actor - used to send/receive message
 	*/
	virtual void on_load(CActor* actor){}

	/**callback and default implementation
 	*to notify this object to finalize before destruction
	*@param actor - used to send/receive message 
 	*/
	virtual void on_stop(CActor* actor){}
};

#endif
