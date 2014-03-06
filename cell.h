#ifndef __CELL__
#define __CELL__

/**@brief class used to model the grid cells in the simulation
 * @Author
 * @Date March 2014
 */
class CCell : public CCallback {
	private:
		float m_send_buffer[4]; /*!< out message buffer*/
		int m_send_count; /*!< out message buffer length*/

		float m_recv_buffer[4]; /*!< in message buffer*/
		int m_recv_count; /*!< in message buffer length*/

		float m_population; /*!< population level in the cell*/
		float m_infection_level; /*!< infection level of the cell*/
		int m_rank; /*!< MPI process rank*/

		/**handles frogs visit to the cell by 
 		* updating the population level 
 		* and infection level of the cell
 		* @param source - visiting frogs process id
 		* @param actor - object used to send/receive message
 		*/
		void visit(int source, CActor* actor);
	
		/**
 		* handles the monsoon event by clearing the population level and
 		* infection level of the cell
 		* @param source - timer id sending the message
 		* @param actor - object used to send/receive message
 		*/
		void wipe_disease(int source, CActor* actor);

	public:
		/**constructor 
 		* @param rank - process id	
 		*/
		CCell(int rank);

		/**callback to receive in message buffer
		* @return in message buffer pointer
 		*/
		void* get_recv_buffer();

		/**callback to get in message buffer length
 		* @return in message buffer length
 		*/
		int   get_recv_buffer_count();

		/**event handler to handle frogs visit and monsoon events
 		* @param source - identifier of process sending the message
 		* @param message_id - message identifier
		* @param actor - used to send/receive messages
 		* @return 1 to continue handling event 0 to exit
 		*/
		bool on_new_message(int source, int message_id, CActor* actor);
		
		/**event handler to handle loading of the object
 		* @param actor - used to send/receive messages
 		*/
		void on_load(CActor* actor);
		
		/**event handler to handle destruction of object
 		* @param actor - used to send/receive message
 		*/
		void on_stop(CActor* actor);
};

#endif
