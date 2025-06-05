#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <tuple>
#include <string>

class Graph {
public:
    struct WeightedEdge {
        int u, v, weight;
        bool operator<(const WeightedEdge& other) const { return weight < other.weight; }
    };

    Graph();
    explicit Graph(const std::string& file_path, bool is_json = false); 
    Graph(const std::vector<std::tuple<int, int, int>>& edges);

    Graph(const Graph& other) = default;
    Graph(Graph&& other) noexcept = default;
    Graph& operator=(const Graph& other) = default;
    Graph& operator=(Graph&& other) noexcept = default;
    ~Graph() = default;

    void add_edge(int u, int v, int weight = 1);
    std::vector<WeightedEdge> minimum_spanning_tree() const;
    void export_to_dot(const std::string& filename) const;

private:
    std::vector<std::vector<int>> adj_;
    std::vector<WeightedEdge> weighted_edges_;
    size_t max_vertex_;

    void validate_graph() const;
};

#endif // GRAPH_HPP
