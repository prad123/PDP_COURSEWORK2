#ifndef __FROG__
#define __FROG_

#define POP_CELLS 			300l
#define INF_CELLS			500l

class CFrog : public CCallback{

	float m_send_buffer[4];
	int m_send_count;

	float m_recv_buffer[4];
	int m_recv_count;

	float m_population[POP_CELLS];
	float m_inf_level[INF_CELLS];
	float m_pos_x;
	float m_pos_y;
	int m_rank;
	int m_hops;
	int m_infected;
	
	void hop(CActor* actor);
	void spawn(CActor* actor);
	void die();
public:
	CFrog(int rank);

	/*send/receive callbacks*/
	//void* get_send_buffer();
	void* get_recv_buffer();
	//int get_send_buffer_count();
	int get_recv_buffer_count();	

	/*callback events*/	
	bool on_new_message(int source, int message_id, CActor* actor);
	void on_load(CActor* actor);
	void on_stop(CActor* actor);
};

#endif
