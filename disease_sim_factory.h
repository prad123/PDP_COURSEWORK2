#ifndef __DISEASE_SIM_FACTORY
#define __DISEASE_SIM_FACTORY

/**@brief concreate implementation of CCallbackFactory to create
 * Biologist's simulation entities
 * @Author
 * @Date March 2014
 */
class CDiseaseSimFactory : public CCallbackFactory {
	private:
		const int m_num_cells; /*!< total number of cells*/
		const int m_initial_frogs; /*!< initial number of frogs*/
		const int m_infected_frogs; /*!< initial infected frogs*/
		const int m_iterations; /*!< number of iterations*/
	public:
	
		/**constructor to create this object
 		*@param num_cells - number of grid cells
		*@param initial_frogs - initial number of frogs
		*@param infected_frogs - initial infected frogs
		*@param iterations - number of iterations 	
 		*/
		CDiseaseSimFactory(int num_cells,
				   int initial_frogs,	
				   int infected_frogs,
				   int iterations);

		/**function to create simulatin entities 
		*@param processId - process identifier used to created objects
		*@return simulation entities
 		*/
		CCallback* make_callback(unsigned int processId);
};

#endif
