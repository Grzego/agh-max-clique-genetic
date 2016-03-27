#ifndef __GENETIC_H__
#define __GENETIC_H__

#include <random>
#include <fstream>
#include <string>
#include <chrono>

#include "graph.h"
#include "population.h"


namespace genetic
{

Graph<> read_graph(std::string const &_file)
{
	Graph<> graph;

	std::fstream file(_file, std::ios::in);
	int ver_count, edge_count;
	file >> ver_count >> edge_count;
	for (int i = 0; i < edge_count; ++i) {
		int ver1, ver2;
		file >> ver1 >> ver2;
		graph.edge_undirected(ver1, ver2);
	}

	return graph;
}

void run()
{
}

//std::chrono::high_resolution_clock::time_point tp;
//
//std::chrono::high_resolution_clock::time_point start()
//{
//	return (tp = std::chrono::high_resolution_clock::now());
//}
//
//double stop(/*std::string const &_msg*/)
//{
//	auto duration = static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - tp).count()) / 1000000000.0;
//	//std::cout << duration << "s - " << _msg << '\n';
//	return duration;
//}
//
//#define max(a, b) ((a) > (b) ? (a) : (b))

//void run()
//{
//	std::vector<std::string> files;
//	for (int i = 20; i <= 20; ++i) {
//		files.push_back("graph" + std::to_string(i) + ".txt");
//	}
//
//	const int count = 1;
//	const double dcount = static_cast<double>(count);
//	for (auto &f : files) {
//		double fitn = 0.0;
//		double time = 0.0;
//		double gens = 0.0;
//		int best = 0;
//
//		Graph<> graph = Graph<>::from_file_matrix(f);
//		for (int i = 0; i < count; ++i) {
//			//std::cout << '\n' << f << '\n';
//			start();
//			Population population(graph, 2, 50, 10, 10, 0, 100, 0.4, 0.8, 0.7, 0.0, false);
//			Stats stats = population.evolve_until_done();
//			/*std::cout << "Fitness:     " << stats.fitness << '\n';
//			std::cout << "Generations: " << stats.generation << '\n';
//			std::cout << "[ ";
//			for (auto &v : stats.clique) {
//				std::cout << v << " ";
//			}*/
//			//std::cout << "]\n";
//			time += stop();
//			fitn += stats.fitness;
//			gens += stats.generation;
//			best = max(best, stats.fitness);
//		}
//		std::cout << "File:             " << f << '\n';
//		std::cout << "Best:             " << best << '\n';
//		std::cout << "Avr. fitness:     " << (fitn / dcount) << '\n';
//		std::cout << "Avr. generations: " << (gens / dcount) << '\n';
//		std::cout << "Avr. time:        " << (time / dcount) << "s\n\n";
//	}
//}

//#define FOR(it, v) for (auto &it : v)
//
//struct params
//{
//	int pop_count;
//	int gen_step;
//	int cut_start;
//	int cut_end;
//	int cut_step;
//	int stagnacy;
//	double sel_prob;
//	double mut_prob_start;
//	double mut_prob_end;
//	double mut_prob_step;
//	bool add_random;
//};
//
//std::ostream& operator<<(std::ostream &_stream, params const &_params)
//{
//	return _stream << _params.pop_count << " "
//				   << _params.gen_step << " "
//				   << _params.cut_start << " " 
//				   << _params.cut_end << " "
//				   << _params.cut_step << " "
//				   << _params.stagnacy << " " 
//				   << _params.sel_prob << " "
//				   << _params.mut_prob_start << " "
//				   << _params.mut_prob_end << " "
//				   << _params.mut_prob_step << " "
//				   << _params.add_random;
//}
//
//void run()
//{
//	Graph<> graph = Graph<>::from_file_matrix("graph20.txt");
//
//	std::vector<int> population_count = { 10, 100, 1000 };
//	std::vector<int> gen_step = { 20, 100 };
//	std::vector<int> cuts_start = { 10, 25, 50 };
//	std::vector<int> cuts_end = { 2, 5 };
//	std::vector<int> cuts_step = { 1, 3 };
//	std::vector<int> stagnacy = { 50, 150 };
//	std::vector<double> sel_prob = { 0.0, 0.3, 0.7, 1.0 };
//	std::vector<double> mut_prob_start = { 0.0, 0.3, 0.7, 1.0 };
//	std::vector<double> mut_prob_end = { 0.0, 0.2, 0.5 };
//	std::vector<double> mut_prob_step = { 0.0, 0.05, 0.2 };
//	std::vector<bool> add_random = { true, false };
//
//	const int TEST_COUNT = 1;
//	const double D_TEST_COUNT = static_cast<double>(TEST_COUNT);
//	int best_fitness = 0;
//	params best_params{};
//	std::fstream results("results.txt", std::ios::out);
//
//	long long acc_count = 0;
//	long long max_perc = population_count.size() * gen_step.size() * cuts_start.size() * cuts_end.size() *
//					  cuts_step.size() * stagnacy.size() * sel_prob.size() * mut_prob_start.size() *
//					  mut_prob_end.size() * mut_prob_step.size() * add_random.size();
//
//	auto sstart = start();
//	FOR(i1, population_count) {
//	FOR(i2, gen_step) {
//	FOR(i3, cuts_start){
//	FOR(i4, cuts_end) {
//	FOR(i5, cuts_step) {
//	FOR(i6, stagnacy) {
//	FOR(i7, sel_prob) {
//	FOR(i8, mut_prob_start) {
//	FOR(i9, mut_prob_end) {
//	FOR(i10, mut_prob_step) {
//	FOR(i11, add_random) {
//		int c_best_fitness = 0;
//		double c_avr_fitness = 0.0;
//		double c_avr_time = 0.0;
//		double c_avr_gen = 0.0;
//		for (int i = 0; i < TEST_COUNT; ++i) {
//			start();
//			Population population(graph, i1, i2, i3, i4, i5, i6, i7, i8, max(0.0, i8 - i9), i10, i11);
//			Stats stats = population.evolve_until_done();
//			c_avr_time += stop();
//			c_best_fitness = max(c_best_fitness, stats.fitness);
//			c_avr_fitness += static_cast<double>(stats.fitness);
//			c_avr_gen += static_cast<double>(stats.generation);
//
//		}
//		c_avr_time /= D_TEST_COUNT;
//		c_avr_fitness /= D_TEST_COUNT;
//		c_avr_gen /= D_TEST_COUNT;
//
//		params temp = params{i1, i2, i3, i4, i5, i6, i7, i8, i9, i10, i11}; 
//		results << temp << " " << c_avr_gen << " " << c_best_fitness << " " << c_avr_fitness << " " << c_avr_time << "\n";
//
//		if (best_fitness < c_best_fitness) {
//			best_fitness = c_best_fitness;
//			best_params = temp;
//		}
//
//		acc_count += 1.0;
//		std::cout << "\r" << acc_count << "/" << max_perc;
//	}}}}}}}}}}}
//
//	auto duration = static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - sstart).count()) / 1000000000.0;
//	std::cout << "Total time: " << duration << "s\n";
//	std::cout << "Best params:\n";
//	std::cout << "\tFitness: " << best_fitness << "\n";
//	std::cout << "\t" << best_params << '\n';
//}


//void run()
//{
//	std::vector<std::string> files = { "brock400-1", "brock800-1", "c-fat200-1", "c-fat500-1", 
//									   "hamming8-2", "hamming10-2", "keller4", "keller5", "keller6",
//									   "MANN-a27", "MANN-a45", "p-hat500-1", "p-hat700-1", 
//									   "p-hat1000-1", "p-hat1500-1" };
//	std::vector<int> max_clique = { 27, 23, 12, 14, 128, 512, 11, 27, 59, 126, 345, 9, 11, 10, 12 };
//	unsigned int idx = 0;
//
//	std::fstream data("data.tex", std::ios::out);
//	data.setf(std::ios::fixed);
//	data.precision(1);
//
//	const int test_count = 5;
//	const double d_test_count = static_cast<double>(test_count);
//
//	for (auto &filename : files) {
//		Graph<> graph = Graph<>::from_file("graph/" + filename + ".mtx");
//		
//		data << filename << " & " << graph.vertices_count() << " & "
//			 << graph.edges_count() << " & " << max_clique[idx++] << " & ";
//
//		double avr_time = 0.0;
//		double avr_fitn = 0.0;
//		int best_fitn = 0;
//
//		for (int i = 1; i <= test_count; ++i) {
//			std::cout << "\rfile: " << filename << " " << i << "/" << test_count;
//			start();
//			Population population(graph);
//			Stats stats = population.evolve_until_done();
//			avr_time += stop();
//			avr_fitn += stats.fitness;
//			best_fitn = max(best_fitn, stats.fitness);
//		}
//		std::cout << '\n';
//
//		data << best_fitn << " & " << (avr_fitn / d_test_count) << " & " << (avr_time / d_test_count) << " \\\\\n";
//	}
//}


}




#endif // __GENETIC_H__