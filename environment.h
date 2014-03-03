#ifndef __ENVIRONMENT__
#define __ENVIRONMENT__

class CEnvironment : public CCallback {
	private:
		const int m_num_cells;
		const int m_initial_frogs;
		const int m_infected_frogs;

		float m_send_buffer[4];
		int m_send_buffer_count;
	
		void create_cells(CActor* actor);
		void create_initial_frogs(CActor* actor);
		int create_timer(CActor* actor);
		void start_timer(int timerId, CActor* actor);
	public:
		CEnvironment(int num_cells,
			     int initial_frogs,
			     int infected_frogs);
		//void* get_send_buffer();
		//int get_send_buffer_count();
		/*event handler*/
		void on_load(CActor* actor);
};

#endif
