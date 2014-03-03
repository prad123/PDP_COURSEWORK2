#ifndef __CELL__
#define __CELL__

class CCell : public CCallback {
	private:
		float m_send_buffer[4];
		int m_send_count;

		float m_recv_buffer[4];
		int m_recv_count;

		float m_population;
		float m_infection_level;
		int m_rank;
	public:
		CCell(int rank);
		void visit();
		void wipe_disease();
		void send_cell_info();

		//void* get_send_buffer();
		//int   get_send_buffer_count();
		void* get_recv_buffer();
		int   get_recv_buffer_count();

		/*EVENT HANDLERS*/
		bool on_new_message(int source, int message_id, CActor* actor);
		void on_load(CActor* actor);
		void on_stop(CActor* actor);
};

#endif
