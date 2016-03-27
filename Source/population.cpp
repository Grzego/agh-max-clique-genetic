#include "population.h"
#include "GILUnlocker.h"


Population::Population(Graph<> _graph,
					   unsigned int _count,
					   unsigned int _generation_step,
					   unsigned int _crosspoints_start,
					   unsigned int _crosspoints_end,
					   unsigned int _crosspoints_step,
					   unsigned int _stagnacy,
					   double _offspring_selection_prob,
					   double _offspring_mutation_prob_start,
					   double _offspring_mutation_prob_end,
					   double _offspring_mutation_prob_step,
					   bool _add_random)
	: graph(_graph)
	, count(_count)
	, no_generations(0)
	, generation_step(_generation_step)
	, crosspoints_start(_crosspoints_start)
	, crosspoints_end(_crosspoints_end)
	, crosspoints_step(_crosspoints_step)
	, parents_count(2)
	, stagnacy(_stagnacy)
	, offspring_selection_prob(_offspring_selection_prob)
	, offspring_mutation_prob_start(_offspring_mutation_prob_start)
	, offspring_mutation_prob_end(_offspring_mutation_prob_end)
	, offspring_mutation_prob_step(_offspring_mutation_prob_step)
	, add_random(_add_random)
	, mersenne_twister(std::random_device()())
	, real_dist(0.0, 1.0)
{
	order = graph.sort_by_degree();
	for (int i = 0; i < order.size(); ++i) {
		mapping[order[i]] = i;
	}
	sorted_graph = graph.rename_vertices(mapping);

	crosspoints = crosspoints_start;
	offspring_mutation_prob = offspring_mutation_prob_start;

	this->initialize(sorted_graph, count);
}

void Population::initialize(Graph<> &_graph, unsigned int _count)
{
	chrom_length = _graph.vertices_count();

	std::uniform_int_distribution<> dist(0, _graph.vertices_count() - 1);

	for (unsigned int i = 0; i < _count; ++i) {
		int rver = dist(mersenne_twister);
		std::vector<int> neighbors = _graph[rver].list_neighbors();
		std::random_shuffle(neighbors.begin(), neighbors.end());

		std::vector<int> chrom;
		chrom.push_back(rver);

		while (!neighbors.empty()) {
			int back = neighbors.back();
			neighbors.pop_back();

			if (std::all_of(chrom.begin(), chrom.end(), 
				[&](int _i) {
					return _graph.connected(back, _i);
				})) {
				chrom.push_back(back);
			}
		}

		Entity entity(_graph.vertices_count());
		for (auto &g : chrom) {
			entity.set(g);
		}
		this->insert(entity);
	}
}

Stats Population::evolve(unsigned int _generations)
{
	// ------------------
	GILUnlocker unlocker;
	// ------------------

	std::vector<container::iterator> parents(2);
	for (unsigned int i = 0; i < _generations; ++i) {
		std::vector<Entity> offspring(2, chrom_length);
		this->select_by_fitness(parents, parents_count);
		this->crossover(parents, offspring, crosspoints);
		for (auto &o : offspring) {
			if (real_dist(mersenne_twister) > offspring_selection_prob) {
				this->mutate(o, offspring_mutation_prob);
			}
		}
		for (auto &o : offspring) {
			this->clique_optimizer(o);
		}
		this->replacement(offspring);

		if ((++no_generations) % generation_step == 0) {
			crosspoints = std::max(crosspoints - crosspoints_step, crosspoints_end);
			offspring_mutation_prob = std::max(offspring_mutation_prob - offspring_mutation_prob_step, offspring_mutation_prob_end);
		}
	}

	Entity const &best = this->best_entity();

	Stats result;
	result.generation = no_generations;
	result.fitness = best.fitness;
	for (int i = 0; i < best.chromosome.size(); ++i) {
		if (best.chromosome[i] == 1) {
			result.clique.push_back(order[i]);
		}
	}		
		
	return result;
}

Stats Population::evolve_until_done()
{
	// ------------------
	GILUnlocker unlocker;
	// ------------------

	int last_solution = -1, current_solution = 0, stagnacy_count = 0;
	std::vector<container::iterator> parents(2);
	while (stagnacy_count <= stagnacy) {
		if (last_solution == current_solution) {
			++stagnacy_count;
		}
		last_solution = current_solution;
		std::vector<Entity> offspring(2, chrom_length);
		this->select_by_fitness(parents, parents_count);
		this->crossover(parents, offspring, crosspoints);
		for (auto &o : offspring) {
			if (real_dist(mersenne_twister) > offspring_selection_prob) {
				this->mutate(o, offspring_mutation_prob);
			}
		}
		for (auto &o : offspring) {
			this->clique_optimizer(o);
		}
		this->replacement(offspring);

		if ((++no_generations) % generation_step == 0) {
			crosspoints = std::max(crosspoints - crosspoints_step, crosspoints_end);
			offspring_mutation_prob = std::max(offspring_mutation_prob - offspring_mutation_prob_step, offspring_mutation_prob_end);
		}
		current_solution = this->best_entity().fitness;
	}

	Entity const &best = this->best_entity();

	Stats result;
	result.generation = no_generations;
	result.fitness = best.fitness;
	for (int i = 0; i < best.chromosome.size(); ++i) {
		if (best.chromosome[i] == 1) {
			result.clique.push_back(order[i]);
		}
	}		
		
	return result;
}

Entity Population::best_entity()
{
	return *this->begin();
}

void Population::select_by_fitness(std::vector<container::iterator> &_selected, unsigned int _count)
{
	unsigned int fit_sum = 0;
	for (auto &e : *this) {
		fit_sum += e.fitness;
	}

	std::uniform_int_distribution<> dist(0, fit_sum);
		
	for (unsigned int s = 0; s < _count; ++s) {
		int rands = dist(mersenne_twister);
		fit_sum = 0;
		for (auto it = this->begin(); it != this->end(); ++it) {
			fit_sum += it->fitness;
			if (rands <= fit_sum) {
				_selected[s] = it;
			}
		}
	}
}

void Population::crossover(std::vector<container::iterator> &_parents, std::vector<Entity> &_offspring, unsigned int _cross_points)
{
	std::uniform_int_distribution<> dist(1, chrom_length - 2);

	std::vector<int> cuts(_cross_points + 1, chrom_length);
	std::generate(cuts.begin(), cuts.end() - 1, std::bind(dist, mersenne_twister));
	std::sort(cuts.begin(), cuts.end());
	cuts.erase(std::unique(cuts.begin(), cuts.end()), cuts.end());

	int s = 0;
	for (int i = 0; i < chrom_length; ++i) {
		if (i == cuts[s]) ++s;
		_offspring[0].chromosome[i] = _parents[s&1]->chromosome[i];
		_offspring[1].chromosome[i] = _parents[(s+1)&1]->chromosome[i];

		_offspring[0].fitness += _offspring[0].chromosome[i];
		_offspring[1].fitness += _offspring[1].chromosome[i];
	}
}

void Population::mutate(Entity &_offspring, double _prob)
{
	for (int i = 0; i < chrom_length; ++i) {
		if (real_dist(mersenne_twister) < _prob) {
			_offspring.flip(i);
		}
	}
}

void Population::clique_optimizer(Entity &_offspring)
{
	if (_offspring.fitness == 0) return;

	std::vector<int> vertices;
	vertices.reserve(_offspring.fitness);
	for (int i = 0; i < chrom_length; ++i) {
		if (_offspring.chromosome[i]) {
			vertices.push_back(i);
		}
	}
	Graph<> sub = graph.extract_undirected(vertices);

	while (!sub.is_clique()) {
		sub.remove_undirected(sub.find_smallest_degree());
	}

	std::vector<int> clique = sub.vertices();

	std::uniform_int_distribution<> dist(0, chrom_length - 1);

	int pivot = dist(mersenne_twister);
	for (int i = pivot; i < chrom_length; ++i) {
		if (!sub.count(i)) {
			if (std::all_of(clique.begin(), clique.end(), 
				[&](int _i) {
					return graph.connected(i, _i);
				})) {
				clique.push_back(i);
			}
		}
	}
	for (int i = 0; i < pivot; ++i) {
		if (!sub.count(i)) {
			if (std::all_of(clique.begin(), clique.end(), 
				[&](int _i) {
					return graph.connected(i, _i);
				})) {
				clique.push_back(i);
			}
		}
	}
		
	_offspring.null_chrom();
	for (auto &g : clique) {
		_offspring.set(g);
	}
}

void Population::replacement(std::vector<Entity> &_offspring)
{
	if (add_random) {
		std::uniform_int_distribution<> dist(0, sorted_graph.vertices_count() - 1);

		int rver = dist(mersenne_twister);
		std::vector<int> neighbors = sorted_graph[rver].list_neighbors();
		std::random_shuffle(neighbors.begin(), neighbors.end());

		std::vector<int> chrom;
		chrom.push_back(rver);

		while (!neighbors.empty()) {
			int back = neighbors.back();
			neighbors.pop_back();

			if (std::all_of(chrom.begin(), chrom.end(), 
				[&](int _i) {
					return sorted_graph.connected(back, _i);
				})) {
				chrom.push_back(back);
			}
		}

		Entity entity(sorted_graph.vertices_count());
		for (auto &g : chrom) {
			entity.set(g);
		}
		this->insert(entity);
	}

	// -----

	this->insert(_offspring.begin(), _offspring.end());

	std::uniform_int_distribution<> dist((this->size() * 2) / 10 + 1, this->size() - _offspring.size() - add_random);

	for (int i = 0; i < _offspring.size() + add_random; ++i) {
		int randrm = dist(mersenne_twister);
		auto it = this->begin();
		std::advance(it, randrm);
		this->erase(it);			
	}
}
