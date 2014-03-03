#ifndef __DISEASE_SIM_FACTORY
#define __DISEASE_SIM_FACTORY

class CDiseaseSimFactory : public CCallbackFactory {
	private:
		const int m_num_cells;
		const int m_initial_frogs;
		const int m_infected_frogs;
		const int m_iterations;
	public:
		CDiseaseSimFactory(int num_cells,
				   int initial_frogs,	
				   int infected_frogs,
				   int iterations);

		CCallback* make_callback(unsigned int processId);
};

#endif
