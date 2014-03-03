#ifndef __TIMER__
#define __TIMER__

class CTimer : public CCallback {
	private:
		float m_send_buffer[4];
		int m_send_count;

		float m_recv_buffer[4];
		int m_recv_count;

		const int m_interval;
		const int m_iterations;
		const int m_rank;

		const int m_cell_begin;
		const int m_cell_end;

	public:
		CTimer(int rank, int cell_begin,
		       int cell_end,
		       int interval, int iterations);

		//void* get_send_buffer();
		//int   get_send_buffer_count();
		void* get_recv_buffer();
		int   get_recv_buffer_count();

		/*EVENT HANDLERS*/
		bool on_new_message(int source, int message_id, CActor* actor);
		//void on_load(CActor* actor);
		void on_stop(CActor* actor);
};

#endif
