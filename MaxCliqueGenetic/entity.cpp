#include "entity.h"


Entity::Entity(unsigned int _chrom_length)
	: chromosome(_chrom_length, 0)
	, fitness(0)
{ }

void Entity::set(unsigned int _idx)
{
	fitness += 1 - chromosome[_idx];
	chromosome[_idx] = 1;
}

void Entity::clear(unsigned int _idx)
{
	fitness -= chromosome[_idx];
	chromosome[_idx] = 0;
}

void Entity::flip(unsigned int _idx) 
{
	fitness += chromosome[_idx] ? -1 : 1;
	chromosome[_idx] = !chromosome[_idx];
}

void Entity::null_chrom()
{
	std::fill(chromosome.begin(), chromosome.end(), 0);
	fitness = 0;
}

// ------

bool operator<(Entity const &_a, Entity const &_b)
{
	return _a.fitness < _b.fitness;
}

bool operator>(Entity const &_a, Entity const &_b)
{
	return _a.fitness > _b.fitness;
}