import graphviz
import os
import sys
import re

def parse_dot_file(dot_file):
    if not os.path.exists(dot_file):
        raise FileNotFoundError(f"DOT file not found: {dot_file}")
    
    edges = []
    with open(dot_file, 'r') as f:
        for line in f:
            match = re.match(r'\s*(\d+)\s*--\s*(\d+)\s*\[label=(\d+)\];', line)
            if match:
                u, v, w = map(int, match.groups())
                edges.append((u, v, w))
    return edges

def render_dot_edges(dot_file, output_file, fmt='png'):
    edges = parse_dot_file(dot_file)
    
    g = graphviz.Graph('G', format=fmt)
    nodes = set()

    for u, v, _ in edges:
        nodes.add(u)
        nodes.add(v)

    for node in nodes:
        g.node(str(node), style='filled', fillcolor='lightgoldenrod1')

    for u, v, w in edges:
        g.edge(str(u), str(v), label=str(w), color='forestgreen', penwidth='2')

    output_path = g.render(output_file, view=False)
    print(f"Graph rendered and saved as {output_path}")

def main():
    try:
        if len(sys.argv) < 2:
            print("Usage: python3 visualizer.py input.dot [mst.dot]")
            return

        # Render original graph
        render_dot_edges(sys.argv[1], 'graph_rendered')

        # Render MST graph if provided
        if len(sys.argv) > 2:
            render_dot_edges(sys.argv[2], 'mst_rendered')

    except Exception as e:
        print(f"Error: {e}")

if __name__ == "__main__":
    main()
