#ifndef NAVIGATOR_HPP
#define NAVIGATOR_HPP

#include <vector>
#include <list>
#include <queue>
#include <limits>
#include <iostream>
#include <functional>
#include <unordered_map>
#include <cmath>

class Navigator {
public:
    using Node = int;
    using Weight = double;

    Navigator(int n);
    void add_edge(Node u, Node v, Weight w);
    bool bellman_ford(Node source);
    std::vector<Node> a_star(Node source, Node target, const std::function<Weight(Node, Node)>& heuristic);
    void print_shortest_distances() const;
    Weight distance_between(Node source, Node target);

private:
    std::vector<std::list<std::pair<Node, Weight>>> adj_list;
    std::vector<Weight> dist;

    void reset_distances();
};

#endif // NAVIGATOR_HPP
