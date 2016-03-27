# Maximum clique genetic algorithm
Genetic algorithm for finding maximum clique in graph. Written in C++ for academic purpose. Creates a library file for python which exposes ```Graph```, ```Population```, ```Stats``` classes.

#### Requirements
- Boost Python

##### Example usage in Python
```Python
from genetics import *
graph = Graph.from_file("graph.txt")
population = Population(graph)
stats = population.evolve_until_done()

print("Generations:", stats.generation)
print("Fitness:", stats.fitness)
print("Vertices:", [x for x in stats.clique])
```
