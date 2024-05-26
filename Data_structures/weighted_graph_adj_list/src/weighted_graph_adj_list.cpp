
#include "../include/weighted_graph_adj_list.h"

namespace my {
    weighted_graph::weighted_graph(std::size_t size)
        : m_graph(size)
    {
    }

    //-------------------------_add_vertex_---------------------//
    void weighted_graph::add_vertex()
    {
        m_graph.emplace_back();
    }

    //--------------------------_add_edge_-----------------------//
    void weighted_graph::add_edge(const vertex_t src_vertex1, const vertex_t vertex2, const weight_t weight)
    {
        if (src_vertex1 >= m_graph.size() || vertex2 >= m_graph.size())
        {
            std::ostringstream errorMessage;

            if (src_vertex1 >= m_graph.size() && vertex2 >= m_graph.size())
            {
                errorMessage << "Cannot add edge:\nvertex: " << src_vertex1 << " and vertex: " << vertex2
                             << " are out of range";
            }
            else if (src_vertex1 >= m_graph.size())
            {
                errorMessage << "Cannot add edge:\n vertex " << src_vertex1 << " is out of range";
            }
            else
            {
                errorMessage << "Cannot add edge:\n vertex " << vertex2 << " is out of range";
            }

            throw std::out_of_range(errorMessage.str());
        }

        std::vector<Edge>& edges = m_graph[src_vertex1];

        // check if the edge already exists
        auto it =
            std::find_if(edges.begin(), edges.end(), [vertex2](const Edge& edge) { return edge.first == vertex2; });

        if (it == edges.end()) // if no edge add it
        {
            m_graph[src_vertex1].emplace_back(vertex2, weight);
        }
        else // if edge exists change weight
        {
            it->second = weight;
        }
    }

    //-----------------------------_-dfs_rec-_----------------------//
    void weighted_graph::dfs(const vertex_t start_vert, bool print_preorder) const
    {
        std::vector<bool> visited(m_graph.size(), false);

        dfs(start_vert, visited, print_preorder);

        for (vertex_t i = 0; i < m_graph.size(); ++i)
        {
            if (visited[i] == false)
            {
                dfs(i, visited, print_preorder);
            }
        }

        std::cout << std::endl;
    }

    void weighted_graph::dfs(vertex_t src, std::vector<bool>& visited, bool print_preorder) const
    {
        visited[src] = true;

        if (print_preorder)
        {
            std::cout << src << " ";
        }

        for (const auto& u_vert : m_graph[src])
        {
            if (!visited[u_vert.first])
            {
                dfs(u_vert.first, visited, print_preorder);
            }
        }

        if (!print_preorder)
        {
            std::cout << src << " ";
        }
    }

    //-----------------------------_-bfs-_----------------------//
    void weighted_graph::bfs(const vertex_t start_vert) const
    {
        std::vector<bool> visited(m_graph.size(), false);

        bfs(start_vert, visited);

        for (vertex_t vertex = 0; vertex < m_graph.size(); ++vertex)
        {
            if (!visited[vertex])
            {
                bfs(vertex, visited);
            }
        }
        std::cout << std::endl;
    }

    void weighted_graph::bfs(vertex_t start_vertex, std::vector<bool>& visited) const
    {
        std::queue<vertex_t> qu;

        qu.push(start_vertex);
        visited[start_vertex] = true;

        while (!qu.empty())
        {
            vertex_t current_vert = qu.front();
            std::cout << current_vert << " ";
            qu.pop();

            for (const auto& neighbor : m_graph[current_vert])
            {
                if (!visited[neighbor.first])
                {
                    qu.push(neighbor.first);
                    visited[neighbor.first] = true;
                }
            }
        }
    }

    /*---------------------------------------------------------------------*/
    /*---------------------------*_kosaraju_scc_*--------------------------*/
    /*---------------------------------------------------------------------*/
    void weighted_graph::kosaraju_scc() const
    {
        std::cout << "kosaraju_scc: " << std::endl;

        std::stack<vertex_t> finish_stack;
        std::vector<bool>    visited(m_graph.size(), false);

        // Step 1: Perform DFS and push vertices to finish_stack
        for (vertex_t i = 0; i < m_graph.size(); ++i)
        {
            if (!visited[i])
            {
                dfs_kosaraju(i, visited, finish_stack);
            }
        }

        // Step 2: Transpose the graph
        AdjacencyList transposed_graph = this->transpose();

        std::fill(visited.begin(), visited.end(), false);

        // Step 3: Perform DFS on the transposed graph
        while (!finish_stack.empty())
        {
            vertex_t v = finish_stack.top();
            finish_stack.pop();

            if (!visited[v])
            {
                std::vector<vertex_t> component;
                dfs_transposed(v, transposed_graph, visited, component);

                // Output the current SCC
                std::cout << "SCC: ";
                for (vertex_t vert : component)
                {
                    std::cout << vert << " ";
                }
                std::cout << "\n";
            }
        }
    }

    //---------------------------_transpose_--------------------------//
    weighted_graph::AdjacencyList weighted_graph::transpose() const
    {
        AdjacencyList transposed(m_graph.size());

        for (vertex_t v = 0; v < m_graph.size(); ++v)
        {
            for (const auto& neighbor : m_graph[v])
            {
                transposed[neighbor.first].emplace_back(v, neighbor.second);
            }
        }

        return transposed;
    }

    //---------------------------_dfs_kosaraju_--------------------------//
    void weighted_graph::dfs_kosaraju(vertex_t vertex, std::vector<bool>& visited,
                                      std::stack<vertex_t>& finish_stack) const
    {
        visited[vertex] = true;

        for (const auto& neighbor : m_graph[vertex])
        {
            if (!visited[neighbor.first])
            {
                dfs_kosaraju(neighbor.first, visited, finish_stack);
            }
        }

        finish_stack.push(vertex);
    }

    //---------------------------_dfs_transposed_--------------------------//
    void weighted_graph::dfs_transposed(vertex_t vertex, const AdjacencyList& transposed_graph,
                                        std::vector<bool>& visited, std::vector<vertex_t>& component) const
    {
        visited[vertex] = true;
        component.push_back(vertex);
        for (const auto& neighbor : transposed_graph[vertex])
        {
            if (!visited[neighbor.first])
            {
                dfs_transposed(neighbor.first, transposed_graph, visited, component);
            }
        }
    }

    /*---------------------------------------------------------------------*/
    /*----------------------------*_TARJAN_SCC_-*--------------------------*/
    /*---------------------------------------------------------------------*/
    void weighted_graph::tarjan_scc() const
    {
        std::vector<long>                  ids(m_graph.size(), -1);
        std::vector<long>                  low(m_graph.size(), -1);
        std::vector<bool>                  on_stack(m_graph.size(), false);
        std::stack<vertex_t>               stack;
        std::vector<std::vector<vertex_t>> sccs;
        vertex_t                           id = 0;

        for (vertex_t vertex = 0; vertex < m_graph.size(); ++vertex)
        {
            if (ids[vertex] == -1)
            {
                tarjan_scc_util(vertex, ids, low, on_stack, stack, sccs, id);
            }
        }

        std::cout << "tarjan_scc: " << std::endl;

        for (const auto& scc : sccs)
        {
            std::cout << "SCC: ";
            for (vertex_t vertex : scc)
            {
                std::cout << vertex << " ";
            }
            std::cout << std::endl;
        }
    }

    void weighted_graph::tarjan_scc_util(vertex_t vertex, std::vector<long>& ids, std::vector<long>& low,
                                         std::vector<bool>& on_stack, std::stack<vertex_t>& stack,
                                         std::vector<std::vector<vertex_t>>& sccs, vertex_t& id) const
    {
        ids[vertex] = low[vertex] = static_cast<long>(id++);
        stack.push(vertex);
        on_stack[vertex] = true;

        for (const auto& neighbor : m_graph[vertex])
        {
            vertex_t next_vertex = neighbor.first;

            if (ids[next_vertex] == -1)
            {
                tarjan_scc_util(next_vertex, ids, low, on_stack, stack, sccs, id);
            }

            if (on_stack[next_vertex])
            {
                low[vertex] = std::min(low[vertex], low[next_vertex]);
            }
        }

        if (ids[vertex] == low[vertex])
        {
            std::vector<vertex_t> scc;
            vertex_t              top_vertex;

            do
            {
                top_vertex = stack.top();
                stack.pop();
                on_stack[top_vertex] = false;
                scc.push_back(top_vertex);

            } while (top_vertex != vertex);

            sccs.push_back(scc);
        }
    }

    //---------------------------_dijkstra_--------------------------//
    void weighted_graph::dijkstra(const vertex_t start_vert, std::vector<inf_t>& res) const
    {
        std::vector<inf_t> distances(m_graph.size(), INF);
        distances[start_vert] = 0;

        using dist_vert_Pair = std::pair<inf_t, vertex_t>;
        std::priority_queue<dist_vert_Pair, std::vector<dist_vert_Pair>, std::greater<>> pq;

        pq.emplace(0, start_vert);

        while (!pq.empty())
        {
            auto [current_dist, U_vert] = pq.top();
            pq.pop();

            if (current_dist > distances[U_vert])
            {
                continue;
            }

            for (const auto& [V_vert, weight] : m_graph[U_vert])
            {
                if (weight < 0)
                {
                    throw std::runtime_error("Graph contains a negative weight edge.");
                }

                inf_t new_dist;
                if (__builtin_add_overflow(current_dist, weight, &new_dist))
                {
                    new_dist = INF;
                }

                if (new_dist < distances[V_vert])
                {
                    distances[V_vert] = new_dist;
                    pq.emplace(new_dist, V_vert);
                }
            }
        }

        res.clear();
        res.resize(distances.size());
        for (size_t i = 0; i < distances.size(); ++i)
        {
            if (distances[i] >= INF)
            {
                res[i] = INF;
            }
            else
            {
                res[i] = distances[i];
            }
        }

        this->print_paths(start_vert, res);
    }

    //---------------------------_print_dijkstra_--------------------------//
    void weighted_graph::print_paths(vertex_t start_vert, const std::vector<inf_t>& vec) const
    {
        vertex_t dest_vert = 0;

        for (inf_t weight : vec)
        {
            if (weight == INF)
            {
                ++dest_vert;
                continue;
            }
            std::cout << "shortest path from  " << start_vert << "  to  " << dest_vert << "  is  " << weight;
            ++dest_vert;
            std::cout << std::endl;
        }
    }

    //---------------------------_bellman_ford_--------------------------//
    bool weighted_graph::bellman_ford(vertex_t start_vert, std::vector<inf_t>& res) const
    {
        res.assign(m_graph.size(), INF);
        res[start_vert] = 0;

        for (std::size_t i = 1; i < m_graph.size(); ++i)
        {
            for (vertex_t U_vert = 0; U_vert < m_graph.size(); ++U_vert)
            {
                for (const auto& [V_vert, weight] : m_graph[U_vert])
                {
                    if (res[U_vert] != INF && res[U_vert] + weight < res[V_vert])
                    {
                        res[V_vert] = res[U_vert] + weight;
                    }
                }
            }
        }

        for (std::size_t U_vert = 0; U_vert < m_graph.size(); ++U_vert)
        {
            for (const auto& [V_vert, weight] : m_graph[U_vert])
            {
                if (res[U_vert] != INF && res[U_vert] + weight < res[V_vert]) // Negative cycle detected
                {
                    return false; 
                }
            }
        }

        this->print_paths(start_vert, res);

        return true;
    }

    //---------------------------_print_--------------------------//
    void weighted_graph::print() const
    {
        for (std::size_t i = 0; i < m_graph.size(); ++i)
        {
            std::cout << i << " -> ";

            for (const auto& elem : m_graph[i])
            {
                std::cout << "e:" << elem.first << " w:" << elem.second << " | ";
            }
            std::cout << std::endl;
        }
    }

} // namespace my
