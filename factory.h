#ifndef __FACTORY__
#define __FACTORY__

class CCallbackFactory{
	public:
		virtual CCallback* make_callback(unsigned int process_id) = 0;
};

#endif
