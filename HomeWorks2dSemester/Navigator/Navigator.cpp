#include "navigator.hpp"

Navigator::Navigator(int n) : adj_list(n), dist(n, std::numeric_limits<Weight>::infinity()) {}

void Navigator::add_edge(Node u, Node v, Weight w) {
    adj_list[u].push_back({v, w});
}

bool Navigator::bellman_ford(Node source) {
    reset_distances();
    dist[source] = 0;

    for (size_t i = 1; i < dist.size(); ++i) {
        for (Node u = 0; u < adj_list.size(); ++u) {
            for (const auto& edge : adj_list[u]) {
                Node v = edge.first;
                Weight weight = edge.second;
                if (dist[u] != std::numeric_limits<Weight>::infinity() && dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                }
            }
        }
    }

    for (Node u = 0; u < adj_list.size(); ++u) {
        for (const auto& edge : adj_list[u]) {
            Node v = edge.first;
            Weight weight = edge.second;
            if (dist[u] != std::numeric_limits<Weight>::infinity() && dist[u] + weight < dist[v]) {
                return false;
            }
        }
    }

    return true;
}

std::vector<Navigator::Node> Navigator::a_star(Node source, Node target, const std::function<Weight(Node, Node)>& heuristic) {
    reset_distances();
    std::vector<Node> prev(dist.size(), -1);
    dist[source] = 0;

    auto cmp = [&](Node left, Node right) {
        return dist[left] + heuristic(left, target) > dist[right] + heuristic(right, target);
    };

    std::priority_queue<Node, std::vector<Node>, decltype(cmp)> pq(cmp);
    pq.push(source);

    while (!pq.empty()) {
        Node u = pq.top();
        pq.pop();

        if (u == target) break;

        for (const auto& edge : adj_list[u]) {
            Node v = edge.first;
            Weight weight = edge.second;
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                prev[v] = u;
                pq.push(v);
            }
        }
    }

    std::vector<Node> path;
    for (Node at = target; at != -1; at = prev[at]) {
        path.push_back(at);
    }
    
    std::reverse(path.begin(), path.end());
    return path;
}

void Navigator::print_shortest_distances() const {
    for (size_t i = 0; i < dist.size(); ++i) {
        if (dist[i] == std::numeric_limits<Weight>::infinity()) {
            std::cout << "Node " << i << ": unreachable\n";
        } 
        else {
            std::cout << "Node " << i << ": " << dist[i] << '\n';
        }
    }
}

Navigator::Weight Navigator::distance_between(Node source, Node target) {
    if (bellman_ford(source)) {
        return dist[target];
    } 
    else {
        std::cout << "Negative cycle detected!\n";
        return std::numeric_limits<Weight>::infinity();
    }
}

void Navigator::reset_distances() {
    std::fill(dist.begin(), dist.end(), std::numeric_limits<Weight>::infinity());
}
