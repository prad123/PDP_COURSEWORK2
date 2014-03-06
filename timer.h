#ifndef __TIMER__
#define __TIMER__

/**@brief timer class to notify monsoon arrival
 *@Author
 *@Date March 2014
 */

class CTimer : public CCallback {
	private:
		float m_send_buffer[4]; /*!< out message buffer*/
		int m_send_count; /*!< out message length*/

		float m_recv_buffer[4]; /*!< in message buffer*/
		int m_recv_count; /*!< in message length*/

		const int m_interval; /*!< timer interval*/
		const int m_iterations; /*!< number of simulations*/
		const int m_rank; /*!< MPI process id*/

		const int m_cell_begin; /*!< process identifier of first cell*/
		const int m_cell_end; /*!< process identifier of last cell*/

	public:

		/**constructor to initialize this object
		*@param rank - MPI process id
		*@param cell_begin - process identifier of first cell
		*@param cell_end - process identifier of last cell
		*@param interval - timer period
		*@param iterations - number of time to run the simulation
		*/
		CTimer(int rank, int cell_begin,
		       int cell_end,
		       int interval, int iterations);

		/**callback to specify recv buffer
 		* @return in message buffer
 		*/
		void* get_recv_buffer();
	
		/**callback to get recv buffer length
		 * @return in message length
 		*/
		int   get_recv_buffer_count();

		/**start timer message received form envirnment
 		* notify cells of monsoon events
 		* @param source - source process identifier
 		* @param message_id - message identifier
 		* @param actor - actor to send/recv message
 		* @return 1 to continue handling message 0 to quit
 		*/
		bool on_new_message(int source, int message_id, CActor* actor);
		
		/**finialize this object
 */
		void on_stop(CActor* actor);
};

#endif
