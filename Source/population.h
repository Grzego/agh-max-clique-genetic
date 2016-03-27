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
 * Struktura przechowuj¹ca wynik dzia³ania algorytmu.
 */

struct Stats
{
	unsigned int generation;
	unsigned int fitness;
	std::vector<int> clique;
};


using container = std::multiset<Entity, std::greater<Entity>>;

/**
 * Klasa reprezentuj¹ca populacjê. 
 */

class Population : private container
{
public:
	/**
	 * Konstruktor klasy Population.
	 * @param _graph graf dla którego próbujemy rozwi¹zaæ problem.
	 * @param _count wielkoœæ populacji.
	 * @param _generation_step liczba kroków symulacji po której nast¹pi zmiana parametrów.
	 * @param _crosspoints_start pocz¹tkowa iloœæ punktów krzy¿owania chromosomów.
	 * @param _crosspoints_end koñcowa iloœæ punktów krzy¿owania chromosomów.
	 * @param _crosspoints_step krok z jakim zmienia siê iloœæ punktów krzy¿owania chromosomów.
	 * @param _stagnacy iloœæ generacji po której jeœli nie nast¹pi³a zmiana.
	 * @param _offspring_selection_prob prawdopodobieñstwo, ¿e nowy osobnik zostanie poddany mutacji.
	 * @param _offspring_mutation_prob_start startowe prawdopodobieñstwo mutacji per gen.
	 * @param _offspring_mutation_prob_end koñcowe(minimalne) prawdopodobieñstwo mutacji per gen.
	 * @param _offspring_mutation_prob_step krok z jakim zmienia siê prawdopodobieñstwo mutacji per gen.
	 * @param _add_random dodawanie na koñcu ka¿dej generacji dodatkowego losowego osobnika.
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
	 * Funkcja która symuluje ewolucje populacji.
	 * @param _generations iloœæ kroków do wykonania w symulacji
	 */
	Stats evolve(unsigned int _generations);

	/**
	 * Funkcja, która zwraca najlepszego osobnika danej populacji.
	 */
	Entity best_entity();

	/**
	 * Funkcja, która symuluje ewolucje populacji do momentu jej stagnacji.
	 */
	Stats evolve_until_done();

protected:
	/**
	 * Funkcja inicjuj¹ca populacje.
	 * @param _graph graf dla którego rozwi¹zujemy problem, którego wierzcho³ki s¹ posortowane wg stopnia
	 * @param _count wielkoœæ populacji
	 */
	void initialize(Graph<> &_graph, unsigned int _count);

	/**
	 * Funkcja wybieraj¹ca odpowiedni¹ iloœæ osobników z populacji wg ich dopasowania(fitness).
	 * @param _selected wektor wybranych osobników.
	 * @param _count iloœæ osobników do wybrania.
	 */
	void select_by_fitness(std::vector<container::iterator> &_selected, unsigned int _count);

	/**
	 * Funkcja krzy¿uj¹ca dwa osobniki.
	 * @param _parents osobniki które podlegaj¹ krzy¿owaniu.
	 * @param _offspring wynik krzy¿owania _parents.
	 * @param _cross_points iloœæ punktów krzy¿owania.
	 */
	void crossover(std::vector<container::iterator> &_parents, std::vector<Entity> &_offspring, unsigned int _cross_points);

	/**
	 * Funkcja mutuj¹ca danego osobnika.
	 * @param _offspring osobnik poddany mutacji.
	 * @param _prob prawdopodobieñstwo mutacji per gen
	 */
	void mutate(Entity &_offspring, double _prob);

	/**
	 * Funkcja, która zach³annie optymalizuje rozwi¹zania w nowych osobnikach.
	 * @param _offspring osobnik którego rozwi¹zanie ma zostaæ poprawione.
	 */
	void clique_optimizer(Entity &_offspring);

	/**
	 * Funkcja dokonuj¹ca podmiany osobników w populacji.
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