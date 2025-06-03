import graphviz
import os
import sys

def parse_mst_file(mst_file):
    if not os.path.exists(mst_file):
        raise FileNotFoundError(f"MST file not found: {mst_file}")
    
    edges = []
    with open(mst_file, 'r') as f:
        for line in f:
            parts = line.strip().split()
            if len(parts) != 3:
                raise ValueError("Each MST line must be: u v weight")
            u, v, _ = map(int, parts)
            edges.append((u, v))
    return edges

def render_mst(mst_file, output_file='mst_graph', fmt='png'):
    edges = parse_mst_file(mst_file)

    g = graphviz.Graph('G', format=fmt)
    nodes = set()

    for u, v in edges:
        nodes.add(u)
        nodes.add(v)

    for node in nodes:
        g.node(str(node), style='filled', fillcolor='lightgoldenrod1')

    for u, v in edges:
        g.edge(str(u), str(v), color='forestgreen', penwidth='2')

    g.render(output_file, view=True)
    print(f"MST graph saved as {output_file}.{fmt}")

def main():
    try:
        if len(sys.argv) != 2:
            print("Usage: python3 draw_mst.py mst.txt")
            return

        render_mst(sys.argv[1])
    except Exception as e:
        print(f"Error: {e}")

if __name__ == "__main__":
    main()
