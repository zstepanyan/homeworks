#include "Graph.hpp"
#include <fstream>
#include <stdexcept>
#include <set>
#include <queue>
#include <algorithm>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

Graph::Graph() : max_vertex_(0) {}

Graph::Graph(const std::string& file_path, bool is_json) : max_vertex_(0) {
    if (is_json) {
        std::ifstream file(file_path);
        if (!file.is_open()) throw std::runtime_error("Unable to open JSON file: " + file_path);

        json j;
        try {
            file >> j;
        } catch (const json::parse_error& e) {
            throw std::invalid_argument("Invalid JSON: " + std::string(e.what()));
        }

        if (!j.contains("edges") || !j["edges"].is_array())
            throw std::invalid_argument("JSON must contain an 'edges' array");

        for (const auto& edge : j["edges"]) {
            if (!edge.is_array() || edge.size() != 3)
                throw std::invalid_argument("Each edge must be an array of three integers");

            int u = edge[0].get<int>();
            int v = edge[1].get<int>();
            int w = edge[2].get<int>();

            add_edge(u, v, w);
        }

    } else {
        std::ifstream file(file_path);
        if (!file.is_open()) throw std::runtime_error("Unable to open file: " + file_path);

        int n, m;
        if (!(file >> n >> m)) {
            throw std::invalid_argument("Expected vertex and edge count");
        }

        for (int i = 0; i < m; ++i) {
            int u, v, w;
            if (!(file >> u >> v >> w))
                throw std::invalid_argument("Expected edge with three integers");

            add_edge(u, v, w);
        }
    }

    validate_graph();
}

Graph::Graph(const std::vector<std::tuple<int, int, int>>& edges) : max_vertex_(0) {
    for (const auto& [u, v, w] : edges) {
        add_edge(u, v, w);
    }
    validate_graph();
}

Graph::Graph(const std::vector<std::pair<int, int>>& edges) : max_vertex_(0) {
    for (const auto& [u, v] : edges) {
        add_edge(u, v, 1);
    }
    validate_graph();
}

Graph::Graph(const std::vector<std::vector<int>>& adj_matrix) : max_vertex_(0) {
    size_t n = adj_matrix.size();
    for (size_t i = 0; i < n; ++i) {
        if (adj_matrix[i].size() != n)
            throw std::invalid_argument("Adjacency matrix must be square");

        for (size_t j = i; j < n; ++j) {
            if (adj_matrix[i][j] != 0 && adj_matrix[i][j] != 1)
                throw std::invalid_argument("Adjacency matrix must contain only 0 or 1");

            if (adj_matrix[i][j] == 1)
                add_edge(i, j, 1);
        }
    }
    
    validate_graph();
}

Graph::Graph(const std::vector<std::vector<int>>& weight_matrix, bool is_weighted)
    : max_vertex_(0) {
    if (!is_weighted || weight_matrix.empty()) return;

    size_t n = weight_matrix.size();
    for (size_t i = 0; i < n; ++i) {
        if (weight_matrix[i].size() != n)
            throw std::invalid_argument("Weighted adjacency matrix must be square");
        for (size_t j = i; j < n; ++j) {
            int w = weight_matrix[i][j];
            if (w > 0) {
                add_edge(i, j, w);
            }
        }
    }

    validate_graph();
}

void Graph::add_edge(int u, int v, int weight) {
    if (u < 0 || v < 0 || weight < 0)
        throw std::invalid_argument("Vertices and weight must be non-negative");

    size_t max_index = std::max(static_cast<size_t>(u), static_cast<size_t>(v));
    if (max_index >= adj_.size()) {
        adj_.resize(max_index + 1);
    }

    max_vertex_ = std::max(max_vertex_, max_index);

    adj_[u].push_back(v);
    adj_[v].push_back(u);
    weighted_edges_.push_back({u, v, weight});
}

void Graph::clear() {
    adj_.clear();
    weighted_edges_.clear();
    max_vertex_ = 0;
}

std::vector<std::vector<int>> Graph::get_adjacency_matrix() const {
    if (adj_.empty()) return {};

    std::vector<std::vector<int>> matrix(max_vertex_ + 1, std::vector<int>(max_vertex_ + 1, 0));
    for (const auto& [u, v, w] : weighted_edges_) {
        matrix[u][v] = w;
        matrix[v][u] = w;
    }

    return matrix;
}

size_t Graph::get_vertex_count() const {
    size_t count = 0;
    for (size_t i = 0; i <= max_vertex_; ++i) {
        if (!adj_[i].empty()) ++count;
    }
    return count;
}

size_t Graph::get_edge_count() const {
    return weighted_edges_.size();
}

bool Graph::is_connected() const {
    if (adj_.empty()) return true;

    std::vector<bool> visited(max_vertex_ + 1, false);
    std::queue<int> q;

    size_t start = 0;
    while (start <= max_vertex_ && adj_[start].empty()) ++start;
    if (start > max_vertex_) return true;

    q.push(start);
    visited[start] = true;
    size_t visited_count = 1;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v : adj_[u]) {
            if (!visited[v]) {
                visited[v] = true;
                ++visited_count;
                q.push(v);
            }
        }
    }

    return visited_count == get_vertex_count();
}

void Graph::export_to_dot(const std::string& filename) const {
    std::ofstream out(filename);
    if (!out.is_open())
        throw std::runtime_error("Unable to open file: " + filename);

    out << "graph G {\n";
    std::set<std::pair<int, int>> printed;

    for (const auto& [u, v, w] : weighted_edges_) {
        int a = std::min(u, v);
        int b = std::max(u, v);
        if (printed.insert({a, b}).second) {
            out << "    " << a << " -- " << b << " [label=" << w << "];\n";
        }
    }

    out << "}\n";
    out.close();
}

void Graph::validate_graph() const {
    for (size_t u = 0; u <= max_vertex_; ++u) {
        for (int v : adj_[u]) {
            if (v < 0 || static_cast<size_t>(v) > max_vertex_)
                throw std::invalid_argument("Invalid vertex index");
            if (std::find(adj_[v].begin(), adj_[v].end(), u) == adj_[v].end())
                throw std::invalid_argument("Graph must be bidirectional");
        }
    }

    for (const auto& [u, v, w] : weighted_edges_) {
        if (u < 0 || v < 0 || w < 0)
            throw std::invalid_argument("Invalid edge or weight");
    }
}

std::vector<Graph::WeightedEdge> Graph::minimum_spanning_tree() const {
    std::vector<WeightedEdge> result;
    if (weighted_edges_.empty()) return result;

    std::vector<int> parent(max_vertex_ + 1);
    std::iota(parent.begin(), parent.end(), 0);

    auto find = [&](int x) {
        while (x != parent[x]) x = parent[x];
        return x;
    };

    auto unite = [&](int x, int y) {
        x = find(x);
        y = find(y);
        if (x == y) return false;
        parent[x] = y;
        return true;
    };

    std::vector<WeightedEdge> edges = weighted_edges_;
    std::sort(edges.begin(), edges.end());

    for (const auto& edge : edges) {
        if (unite(edge.u, edge.v)) {
            result.push_back(edge);
        }
    }

    return result;
}
