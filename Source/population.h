#ifndef __POPULATION_H__
#define __POPULATION_H__

#include <algorithm>
#include <vector>
#include <functional>
#include <random>
#include <set>
#include <boost/python.hpp>

#include "entity.h"
#include "graph.h"


/**
 * Struktura przechowuj�ca wynik dzia�ania algorytmu.
 */

struct Stats
{
	unsigned int generation;
	unsigned int fitness;
	std::vector<int> clique;
};


using container = std::multiset<Entity, std::greater<Entity>>;

/**
 * Klasa reprezentuj�ca populacj�. 
 */

class Population : private container
{
public:
	/**
	 * Konstruktor klasy Population.
	 * @param _graph graf dla kt�rego pr�bujemy rozwi�za� problem.
	 * @param _count wielko�� populacji.
	 * @param _generation_step liczba krok�w symulacji po kt�rej nast�pi zmiana parametr�w.
	 * @param _crosspoints_start pocz�tkowa ilo�� punkt�w krzy�owania chromosom�w.
	 * @param _crosspoints_end ko�cowa ilo�� punkt�w krzy�owania chromosom�w.
	 * @param _crosspoints_step krok z jakim zmienia si� ilo�� punkt�w krzy�owania chromosom�w.
	 * @param _stagnacy ilo�� generacji po kt�rej je�li nie nast�pi�a zmiana.
	 * @param _offspring_selection_prob prawdopodobie�stwo, �e nowy osobnik zostanie poddany mutacji.
	 * @param _offspring_mutation_prob_start startowe prawdopodobie�stwo mutacji per gen.
	 * @param _offspring_mutation_prob_end ko�cowe(minimalne) prawdopodobie�stwo mutacji per gen.
	 * @param _offspring_mutation_prob_step krok z jakim zmienia si� prawdopodobie�stwo mutacji per gen.
	 * @param _add_random dodawanie na ko�cu ka�dej generacji dodatkowego losowego osobnika.
	 */
	Population(Graph<> _graph,
			   unsigned int _count = 100,
			   unsigned int _generation_step = 20,
			   unsigned int _crosspoints_start = 10,
			   unsigned int _crosspoints_end = 2,
			   unsigned int _crosspoints_step = 1,
			   unsigned int _stagnacy = 50,
			   double _offspring_selection_prob = 0.7,
			   double _offspring_mutation_prob_start = 0.7,
			   double _offspring_mutation_prob_end = 0.2,
			   double _offspring_mutation_prob_step = 0.05,
			   bool _add_random = false);

	/**
	 * Funkcja kt�ra symuluje ewolucje populacji.
	 * @param _generations ilo�� krok�w do wykonania w symulacji
	 */
	Stats evolve(unsigned int _generations);

	/**
	 * Funkcja, kt�ra zwraca najlepszego osobnika danej populacji.
	 */
	Entity best_entity();

	/**
	 * Funkcja, kt�ra symuluje ewolucje populacji do momentu jej stagnacji.
	 */
	Stats evolve_until_done();

protected:
	/**
	 * Funkcja inicjuj�ca populacje.
	 * @param _graph graf dla kt�rego rozwi�zujemy problem, kt�rego wierzcho�ki s� posortowane wg stopnia
	 * @param _count wielko�� populacji
	 */
	void initialize(Graph<> &_graph, unsigned int _count);

	/**
	 * Funkcja wybieraj�ca odpowiedni� ilo�� osobnik�w z populacji wg ich dopasowania(fitness).
	 * @param _selected wektor wybranych osobnik�w.
	 * @param _count ilo�� osobnik�w do wybrania.
	 */
	void select_by_fitness(std::vector<container::iterator> &_selected, unsigned int _count);

	/**
	 * Funkcja krzy�uj�ca dwa osobniki.
	 * @param _parents osobniki kt�re podlegaj� krzy�owaniu.
	 * @param _offspring wynik krzy�owania _parents.
	 * @param _cross_points ilo�� punkt�w krzy�owania.
	 */
	void crossover(std::vector<container::iterator> &_parents, std::vector<Entity> &_offspring, unsigned int _cross_points);

	/**
	 * Funkcja mutuj�ca danego osobnika.
	 * @param _offspring osobnik poddany mutacji.
	 * @param _prob prawdopodobie�stwo mutacji per gen
	 */
	void mutate(Entity &_offspring, double _prob);

	/**
	 * Funkcja, kt�ra zach�annie optymalizuje rozwi�zania w nowych osobnikach.
	 * @param _offspring osobnik kt�rego rozwi�zanie ma zosta� poprawione.
	 */
	void clique_optimizer(Entity &_offspring);

	/**
	 * Funkcja dokonuj�ca podmiany osobnik�w w populacji.
	 * @param _offspring nowe osobniki.
	 */
	void replacement(std::vector<Entity> &_offspring);


private:
	unsigned int chrom_length;
	Graph<> graph;
	Graph<> sorted_graph;
	std::vector<int> order;
	std::unordered_map<int, int> mapping;

	// ----- Parameters

	unsigned int	count;
	unsigned int	no_generations;
	unsigned int	generation_step;
	unsigned int	crosspoints_start;
	unsigned int	crosspoints_end; 
	unsigned int	crosspoints_step; // every generation_step
	unsigned int	parents_count; // unused
	unsigned int	stagnacy;
	double			offspring_selection_prob;
	double			offspring_mutation_prob_start;
	double			offspring_mutation_prob_end;
	double			offspring_mutation_prob_step; // every generation_step
	bool			add_random;

	// -----

	unsigned int	crosspoints;
	double			offspring_mutation_prob;

	// -----

	std::mt19937 mersenne_twister;
	std::uniform_real_distribution<> real_dist;

};

#endif // __POPULATION_H__