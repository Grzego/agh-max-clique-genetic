#ifndef __ENTITY_H__
#define __ENTITY_H__

#include <vector>


struct Entity
{
	Entity(unsigned int _chrom_length);

	void set(unsigned int _idx);

	void clear(unsigned int _idx);

	void flip(unsigned int _idx);

	void null_chrom();

	friend bool operator<(Entity const &, Entity const &);
	friend bool operator>(Entity const &, Entity const &);

	// -----

	std::vector<int>	chromosome;
	unsigned int		fitness;
};

bool operator<(Entity const &_a, Entity const &_b);

bool operator>(Entity const &_a, Entity const &_b);

#endif // __ENTITY_H__