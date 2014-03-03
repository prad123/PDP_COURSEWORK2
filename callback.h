#ifndef __CALLBACK__
#define __CALLBACK__

class CActor;
class CCallback{
public:
	//virtual void* get_send_buffer() {return (void*)0;}
	virtual void* get_recv_buffer() {return (void*)0;}
	//virtual int   get_send_buffer_count() {return 0;}
	virtual int   get_recv_buffer_count() {return 0;}
	//int   get_send_dest() {return -1;}

	virtual bool on_new_message(int source, int message_id, CActor* actor) 
				{ return false; };
	virtual void on_load(CActor* actor){}
	virtual void on_stop(CActor* actor){};
};

#endif
