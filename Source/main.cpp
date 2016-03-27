#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <string>
#include <sstream>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include "population.h"
#include "graph.h"


BOOST_PYTHON_MODULE(genetics)
{
	using namespace boost::python;
	class_<std::vector<int>>("vector")
		.def(vector_indexing_suite<std::vector<int>>())
		;
	class_<std::vector<std::pair<int, int>>>("edges")
		.def(vector_indexing_suite<std::vector<std::pair<int, int>>>())
		;

	class_<std::pair<int, int>>("int_pair")
		.def_readwrite("first", &std::pair<int, int>::first)
		.def_readwrite("second", &std::pair<int, int>::second)
		;
		
	class_<Graph<>>("Graph")
		.def("from_file", &Graph<>::from_file)
		.staticmethod("from_file")
		.def("to_incidence_list", &Graph<>::to_incidence_list)
		.def("vertices_count", &Graph<>::vertices_count)
		;

	class_<Population>("Population", init<Graph<>, optional<unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, double, double, double, double, bool>>())
		.def("evolve", &Population::evolve)
		.def("evolve_until_done", &Population::evolve_until_done)
		;

	class_<Stats>("Stats")
		.def_readonly("generation", &Stats::generation)
		.def_readonly("fitness", &Stats::fitness)
		.def_readonly("clique", &Stats::clique)
		;
}



int main()
{
	return 0;
}