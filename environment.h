#ifndef __ENVIRONMENT__
#define __ENVIRONMENT__

/**@brief models the simulation environment used to create 
 * default entities
 * @Author
 * @Date March 2014
 */

class CEnvironment : public CCallback {
	private:
		const int m_num_cells; /*!< number of cells*/
		const int m_initial_frogs; /*!< initial frogs*/
		const int m_infected_frogs; /*!< initial infected frogs*/

		float m_send_buffer[4];	/*!< out message buffer*/
		int m_send_count; /*!< message length*/
	
		/**Create grid cells
 		* @param actor - used to create cell process
 		*/
		void create_cells(CActor* actor);

		/**Create initial set of frogs
 		*@param actor - used to create frog processes 
 		*/
		void create_initial_frogs(CActor* actor);

		/**Create timer to notify monsoon events
		 *@param actor - uses to create new timer process
 		*/
		int create_timer(CActor* actor);
			
		/**start timer to notify monsoon events
 		*@param timerId - timer process identifier
		*@param actor - send start timer message 
 		*/
		void start_timer(int timerId, CActor* actor);
	public:
		
		/**constructor of this object
 		* @param num_cells - initial number of cells
 		* @param initial_frogs - initial number of cells 
 		* @param infected_frogs - initial infected frogs
 		*/
		CEnvironment(int num_cells,
			     int initial_frogs,
			     int infected_frogs);
		
		
		/**create initial set of cells,
		*initial number of frogs, initial number of infected frogs
		*create and start timer to notify monsoon
 		* @param actor - used to send
 		*/
		void on_load(CActor* actor);
};

#endif
