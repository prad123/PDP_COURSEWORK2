#ifndef __ACTOR__
#define __ACTOR__

class CActor{
private:
	CCallbackFactory&  m_factory;
public:
	CActor(CCallbackFactory& callback_factory);
	//void send_message(int dest, int message_id, CCallback* callback);
	void send_message(void* buffer,int msg_length,int dest,int message_id);
	void recv_message(void* buffer, int msg_length, int source);
	void shutdown();
	int spawn_process();
	void kill_process();
	void run_message_loop();
};

#endif
