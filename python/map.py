import networkx as nx

with open('test.con') as fp:
    line = fp.readline()
    while line:
        print("{}".format(line.strip()))
        line = fp.readline()


G = nx.Graph();

G.add_node(0);
G.add_node(1);
G.add_node(2);
G.add_node(3);

G.add_edge(0,1);
G.add_edge(1,0);
G.add_edge(1,2);
G.add_edge(2,1);
G.add_edge(2,3);
G.add_edge(3,2);

c=nx.eigenvector_centrality(G)

print(['%s %0.6f'%(node,c[node]) for node in c])

