#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>

#include "GILUnlocker.h"

struct Matrix : public std::vector<std::vector<int> >
{
	Matrix(const std::initializer_list<std::vector<int> > &_matrix)
		: std::vector<std::vector<int> >(_matrix)
	{
		n = size();
		m = at(0).size();
	}

	Matrix(int _n = 0, int _m = 0)
		: std::vector<std::vector<int> >(_n, std::vector<int>(_m, 0))
		, n(_n)
		, m(_m)
	{}

	int n, m;
};


struct Empty
{};


template <typename V, typename E>
struct Vertex : public V, public std::unordered_map<int, E>
{
	// For readability purposes.
	std::unordered_map<int, E>& neighbors()
	{
		return *this;
	}

	std::vector<int> list_neighbors() const
	{
		std::vector<int> result;
		result.reserve(size());
		for (auto &p : *this) {
			result.push_back(p.first);
		}
		return result;
	}
};

template <typename V = Empty, typename E = Empty>
class Graph : public std::unordered_map<int, typename Vertex<V, E>>
{
public:
	Graph()
	{}

	void edge_directed(int _from, int _to, const E &_data = E())
	{
		(*this)[_from][_to] = _data;
	}

	void edge_undirected(int _from, int _to, const E &_data = E())
	{
		(*this)[_from][_to] = _data;
		(*this)[_to][_from] = _data;
	}

	// -----

	Graph<V, E> rename_vertices(std::unordered_map<int, int> const &_new_values) const
	{
		std::vector<std::pair<int, int>> inc_list = to_incidence_list();
		for (auto &p : inc_list) {
			p = std::make_pair(_new_values.at(p.first), _new_values.at(p.second));
		}
		return Graph<V, E>::from_incidence_list(inc_list);
	}

	std::vector<int> sort_by_degree() const
	{
		std::vector<std::pair<int, int>> vec_list;
		vec_list.reserve(this->size());
		for (auto &p : *this) {
			vec_list.emplace_back(p.second.size(), p.first);
		}
		std::sort(vec_list.begin(), vec_list.end(), std::greater<std::pair<int, int>>());
		std::vector<int> result;
		result.reserve(this->size());
		for (auto &p : vec_list) {
			result.emplace_back(p.second);
		}
		return result;
	}

	int find_smallest_degree() const
	{
		int result, minimum = std::numeric_limits<int>::max();
		for (auto &v : *this) {
			if (minimum > v.second.size()) {
				minimum = v.second.size();
				result = v.first;
			}
		}
		return result;
	}

	bool is_clique() const
	{
		int ver_count = this->size() - 1;
		for (auto &v : *this) {
			if (v.second.size() != ver_count) {
				return false;
			}
		}
		return true;
	}

	bool connected(int _from, int _to) const
	{
		return this->count(_from) && this->at(_from).count(_to);
	}

	std::vector<int> vertices()
	{
		std::vector<int> result;
		result.reserve(this->size());
		for (auto &v : *this) {
			result.push_back(v.first);
		}
		return result;
	}

	unsigned int vertices_count() const
	{
		return this->size();
	}

	unsigned int edges_count() const
	{
		unsigned int result = 0;
		for (auto &v : *this) {
			result += v.second.size();
		}
		return result / 2;
	}

	Graph<V, E> extract_undirected(std::vector<int> const &_vertices) const
	{
		Graph<V, E> result;
		for (int i = 0; i < _vertices.size(); ++i) {
			for (int j = i + 1; j < _vertices.size(); ++j) {
				if (this->connected(_vertices[i], _vertices[j])) {
					result.edge_undirected(_vertices[i], _vertices[j]);
				}
			}
		}

		return result;
	}


	void remove_undirected(int _vertex)
	{
		for (auto &c : (*this)[_vertex]) {
			(*this)[c.first].erase(_vertex);
		}
		this->erase(_vertex);
	}

	// -----

	static Graph<V, E> from_file(std::string const &_file)
	{
		// ------------------
		GILUnlocker unlocker;
		// ------------------

		Graph<V, E> graph;

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

	static Graph<V, E> from_file_matrix(std::string const &_file)
	{
		std::fstream file(_file, std::ios::in);
		int size;
		file >> size;
		Matrix matrix(size, size);
		for (int i = 0; i < size; ++i) {
			for (int j = 0; j < size; ++j) {
				file >> matrix[i][j];
			}
		}
		return Graph<V, E>::from_adjacency_matrix(matrix);
	}

	// ----- export functions

	Matrix to_adjacenc_matrix() const
	{
		int height = 0, width = 0;
		for (auto &ver : *this) {
			for (auto &edge : ver.second) {
				width = std::max(width, edge.first);
			}
			height = std::max(height, ver.first);
		}
		Matrix result(height, width);
		for (auto &ver : *this) {
			for (auto &edge : ver.second) {
				result[ver.first-1][edge.first-1] = 1;
			}
		}
		return result;
	}

	std::vector<std::pair<int, std::vector<int> > > to_adjacency_list() const
	{
		std::vector<std::pair<int, std::vector<int> > > result;
		for (auto &ver : *this) {
			result.emplace_back(ver.first, std::vector<int>());
			for (auto &edge : ver.second) {
				result.back().second.push_back(edge.first);
			}
		}
		return result;
	}

	std::vector<std::pair<int, int> > to_incidence_list() const
	{
		// ------------------
		GILUnlocker unlocker;
		// ------------------

		std::vector<std::pair<int, int> > result;
		for (auto &ver : *this) {
			for (auto &edge : ver.second) {
				result.emplace_back(ver.first, edge.first);
			}
		}
		return result;
	}

	// ----- import functions

	static Graph<V, E> from_adjacency_matrix(const Matrix &_matrix)
	{
		Graph<V, E> result;
		for (int i = 0; i < _matrix.size(); ++i) {
			for (int j = 0; j < _matrix[i].size(); ++j) {
				if (_matrix[i][j] == 1) {
					result.edge_undirected(i, j);
				}
			}
		}
		return result;
	}

	static Graph<V, E> from_adjacency_list(const std::vector<std::pair<int, std::vector<int> > > &_list)
	{
		Graph<V, E> result;
		for (auto &ver : _list) {
			for (auto &edge : ver.second) {
				result.edge_directed(ver.first, edge);
			}
		}
		return result;
	}

	static Graph<V, E> from_incidence_list(const std::vector<std::pair<int, int> > &_list)
	{
		Graph<V, E> result;
		for (auto &p : _list) {
			result.edge_directed(p.first, p.second);
		}
		return result;
	}
};

#endif // __GRAPH_H__