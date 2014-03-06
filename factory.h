#ifndef __FACTORY__
#define __FACTORY__

/**@brief factory implementation to create callback objects
 * @author
 * @Date March 2014
 */

class CCallbackFactory{
	public:
		/**factory function to create new callback object
 		*@param process_id -  uses to decided which object to create
		*@return new callback object
 		*/
		virtual CCallback* make_callback(unsigned int process_id) = 0;
};

#endif
