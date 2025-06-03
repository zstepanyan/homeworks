#include "Graph.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <algorithm>

void print_adjacency_matrix(const std::vector<std::vector<int>>& matrix) {
    std::cout << "\nAdjacency Matrix:\n";
    for (const auto& row : matrix) {
        for (int val : row) {
            std::cout << val << " ";
        }
        std::cout << "\n";
    }
}

std::string get_file_extension(const std::string& filename) {
    auto dot_pos = filename.find_last_of('.');
    if (dot_pos == std::string::npos) return "";
    std::string ext = filename.substr(dot_pos + 1);
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    return ext;
}

int main() {
    std::string choice;
    std::cout << "Enter 1 to exit or any other key to continue: ";
    std::getline(std::cin, choice);
    if (choice == "1") {
        std::cout << "Exiting program.\n";
        return 0;
    }

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

    try {
        bool is_json = (ext == "json");
        Graph g(file_path, is_json);
        std::cout << "\nGraph loaded successfully.\n";

        std::cout << "Number of vertices: " << g.get_vertex_count() << "\n";
        std::cout << "Number of edges: " << g.get_edge_count() << "\n";
        std::cout << "Is graph connected? " << (g.is_connected() ? "Yes" : "No") << "\n";

        print_adjacency_matrix(g.get_adjacency_matrix());

        g.export_to_dot(dot_output);

        std::cout << "\nDOT file written to: " << dot_output << "\n";

        // Draw only the final tree (e.g., MST)
        std::string render_command = "python3 graph.py \"" + dot_output + "\"";
        std::system(render_command.c_str());

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
