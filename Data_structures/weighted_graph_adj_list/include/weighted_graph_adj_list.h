
#ifndef WEIGHTED_GRAPH_ADJ_LIST_INCLUDE_WEIGHTED_GRAPH_ADJ_LIST_H
#define WEIGHTED_GRAPH_ADJ_LIST_INCLUDE_WEIGHTED_GRAPH_ADJ_LIST_H

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <limits>
#include <queue>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <utility>
#include <vector>

namespace my {
    class weighted_graph
    {
    public:
        using vertex_t      = std::size_t;
        using weight_t      = std::int32_t;
        using AdjacencyList = std::vector<std::vector<std::pair<vertex_t, weight_t>>>;
        using inf_t         = std::int64_t;

        static constexpr int64_t INF = std::numeric_limits<int64_t>::max();

    public:
        weighted_graph(std::size_t size); //

    public:
        void add_vertex();                                                                            //
        void add_edge(const vertex_t src_vertex1, const vertex_t vertex2, const weight_t weight = 0); //

        void dfs(const vertex_t start_vert, bool print_preorder = true) const; //
        void bfs(const vertex_t start_vert) const;                             //

        AdjacencyList transpose() const; //

        void kosaraju_scc() const; //
        void tarjan_scc() const;   //

        void dijkstra(const vertex_t start_vert, std::vector<inf_t>& res) const;      //
        void print_dijkstra(vertex_t start_vert, const std::vector<inf_t> vec) const; //

        void print() const; //

    private:
        void dfs(vertex_t src, std::vector<bool>& visited, bool print_preorder) const; //
        void bfs(vertex_t start_vertex, std::vector<bool>& visited) const;             //

        void dfs_kosaraju(vertex_t vertex, std::vector<bool>& visited,
                          std::stack<vertex_t>& finish_stack) const; //

        void dfs_transposed(vertex_t vertex, const AdjacencyList& transposed_graph, std::vector<bool>& visited,
                            std::vector<vertex_t>& component) const; //

        void tarjan_scc_util(vertex_t vertex, std::vector<long>& ids, std::vector<long>& low,
                             std::vector<bool>& on_stack, std::stack<vertex_t>& stack,
                             std::vector<std::vector<vertex_t>>& sccs, vertex_t& id) const; //

    private:
        AdjacencyList m_graph;
    };

} // namespace my

#endif // WEIGHTED_GRAPH_ADJ_LIST_INCLUDE_WEIGHTED_GRAPH_ADJ_LIST_H
