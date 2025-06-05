#include "Graph.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <cstdlib>

void writeDOT(const std::vector<Graph::WeightedEdge>& edges, const std::string& filename) {
    std::ofstream out(filename);
    if (!out.is_open()) {
        std::cerr << "Error: Could not open " << filename << " for writing.\n";
        return;
    }

    out << "graph G {\n";
    for (const auto& [u, v, w] : edges) {
        out << "    " << u << " -- " << v << " [label=" << w << "];\n";
    }

    out << "}\n";
    out.close();
}

std::string get_file_extension(const std::string& filename) {
    auto dot_pos = filename.find_last_of('.');
    if (dot_pos == std::string::npos) return "";
    std::string ext = filename.substr(dot_pos + 1);
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    return ext;
}

int main() {
    std::string file_path;
    std::cout << "Enter the input file name (e.g., graph.txt or graph.json): ";
    std::getline(std::cin, file_path);
    if (file_path.empty()) {
        std::cerr << "Error: File path cannot be empty\n";
        return 1;
    }

    std::string ext = get_file_extension(file_path);
    if (ext != "txt" && ext != "json") {
        std::cerr << "Error: Unsupported file type. Please provide a .txt or .json file.\n";
        return 1;
    }

    const std::string dot_output = "output.dot";
    const std::string mst_dot_output = "mst.dot";

    try {
        bool is_json = (ext == "json");
        Graph g(file_path, is_json);

        g.export_to_dot(dot_output);

        std::vector<Graph::WeightedEdge> mst_edges = g.minimum_spanning_tree();
        writeDOT(mst_edges, mst_dot_output);

        std::string render_command = "python3 visualizer.py \"" + dot_output + "\" \"" + mst_dot_output + "\"";
        std::system(render_command.c_str());

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
