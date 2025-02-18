#include <bits/stdc++.h>
using namespace std;
//# define INF 0xffff

typedef pair<int, int> iPair;

class Graph {
	int numVertices, numEdges;
	list<pair<int, int>> *adjLists;
	int *distance; // keep the distance of all vertices from a source vertex
	int *parent;   // keep the parent of all vertices during a traversal
	bool *visited;
	bool directed;

public:
	Graph(int vertices);
	//~Graph();
	void addEdge(int src, int dest, int weight);
	void removeEdge(int src, int dest);
	int getInDegree(int src);
	void getDistDijkstra(int src);
	void getDistBellmanFord(int src);
	void printPath(int v);
	void printGraph();
};

Graph::Graph(int vertices) {
	numVertices = vertices;
	numEdges = 0;
	directed = true;
	distance = new int[numVertices];
	visited = new bool[numVertices];
	parent = new int[numVertices];
	adjLists = new list<iPair>[vertices];
}

void Graph::addEdge(int src, int dest, int weight) {
	if (src < 0 || src >= numVertices || dest < 0 || dest >= numVertices)
		return;
	// Weight (or distance) is used used as first item  of pair
	// as first item is by default used to compare two pairs
	adjLists[src].push_back(make_pair(weight, dest));
	numEdges++;
	if (!directed) {
		adjLists[dest].push_back(make_pair(weight, src));
	}
}

void Graph::removeEdge(int src, int dest) {
	list<iPair>::iterator it;
	for (it = adjLists[src].begin(); it != adjLists[src].end(); ++it) {
		if ((*it).second == dest) {
			adjLists[src].erase(it);
			cout << "Removed" << endl;
			break;
		}
	}

	if (!directed) {
		for (it = adjLists[dest].begin(); it != adjLists[dest].end(); ++it) {
			if ((*it).second == src) {
				adjLists[dest].erase(it);
				cout << "Removed" << endl;
				return;
			}
		}
	}
	cout << "Element not found\n";
	return;
}

int Graph::getInDegree(int src) {
	// returns the degree of vertex src

	int inDegree = 0;

	list<iPair>::iterator it;
	for (int i = 0; i < numVertices; i++) {
		for (it = adjLists[i].begin(); it != adjLists[i].end(); ++it) {
			if ((*it).second == src) {
				inDegree++;
			}
		}
	}

	return inDegree;
}

void Graph::printGraph() {
	list<iPair>::iterator j;
	printf(
		"\nNumber of vertices: %d, Number of edges: %d\n",
		numVertices,
		numEdges);
	for (int i = 0; i < numVertices; i++) {
		cout << i << ": ";
		for (j = adjLists[i].begin(); j != adjLists[i].end(); ++j) {
			cout << "-->" << (*j).second << "(" << (*j).first << ")";
		}
		cout << endl;
	}
}

// Prints shortest paths from src to all other vertices
// You can insert the same node with multiple different
// weights but that is not a problem. Since they will be ultimately popped
// from the priority queue but their adjacent nodes would not be inserted
// since distance to those nodes through this node will not satisfy the
// condition distance[v] > distance[u] + weight
void Graph::getDistDijkstra(int src) {
  
    // Step 1: Initialize distances of all vertices as infinite
    for (int i = 0; i < numVertices; i++) {
        distance[i] = INT_MAX;
    }

    // Step 2: Create an empty priority_queue pq.
    priority_queue<iPair, vector<iPair>, greater<iPair>> pq;

    // Step 3: Insert source vertex into pq and make its distance as 0.
    distance[src] = 0;
    pq.push(make_pair(distance[src], src));

    // Step 4: While pq isn't empty
    while (!pq.empty()) {
        // Extract minimum distance vertex from pq
        int u = pq.top().second;
        pq.pop();

        // Loop through all adjacent of u and update the distances of the neighbors if a shorter path is found.
      list<iPair>::iterator it;
      
        for (it = adjLists[u].begin(); it != adjLists[u].end(); it++) {
            int v = it->first;
            int weight = it->second;
            if (distance[v] > distance[u] + weight) {
                distance[v] = distance[u] + weight;
                pq.push(make_pair(distance[v], v));
            }
        }
    }

    // Step 5: Print distance array to print shortest paths to all vertices
    cout << "Vertex\tDistance from Source" << endl;
    for (int i = 0; i < numVertices; i++) {
        cout << i << "\t\t" << distance[i] << endl;
    }
}

void Graph::getDistBellmanFord(int src) {
	list<iPair>::iterator it;

	for (int i = 0; i < numVertices; i++) {
		distance[i] = INT_MAX;
		visited[i] = false;
		parent[i] = -1;
	}
	distance[src] = 0;

	for (int i = 0; i < numVertices - 1; i++) {
		for (int j = 0; j < numVertices; j++) {
			for (it = adjLists[j].begin(); it != adjLists[j].end(); it++) {
				int v = it->first;
				int weight = it->second;
				if (distance[j] != INT_MAX &&
					distance[j] + weight < distance[v]) {
					distance[v] = distance[j] + weight;
					parent[v] = j;
				}
			}
		}
	}

	for (int i = 0; i < numVertices; i++) {
		for (it = adjLists[i].begin(); it != adjLists[i].end(); it++) {
			int v = it->first;
			int weight = it->second;
			if (distance[i] != INT_MAX && distance[i] + weight < distance[v]) {
				cout << "Graph contains negative weight cycle" << endl;
				return;
			}
		}
	}

	cout << "Shortest distances from vertex " << src
		 << " to all other vertices:" << endl;
	for (int i = 0; i < numVertices; i++) {
		cout << i << "\t\t" << distance[i] << endl;
	}
	return;
}

// Recurive Function to print path of given vertex v from source vertex
void Graph::printPath(int v) {
	if (parent[v] == -1) {
		cout << v << " ";
		return;
	}

	printPath(parent[v]);
	cout << v << " ";
}

/*
Graph::~Graph()
{
	//write your destructor here
}
*/

int main() {
	int nVertex, nEdge, choice, i, u, v, w;
	ifstream file;

	file.open("inputdijkstra.txt", ios::in);

	if (!file) {
		cout << "Error in creating file!!!";
		return 0;
	}
	cout << "File opened successfully: ";

	cout << "Enter number of vertices: " << endl;
	file >> nVertex;
	Graph g(nVertex);
	file >> nEdge;

	for (i = 0; i < nEdge; i++) {
		file >> u;
		file >> v;
		file >> w;
		g.addEdge(u, v, w);
	}

	g.printGraph();

	while (1) {
		cout << "\n----------------------" << endl;
		cout << "Operations on Graph" << endl;
		cout << "\n----------------------" << endl;
		cout << "1.Add Edge" << endl;
		cout << "2.Remove Edge" << endl;
		cout << "3.Get Degree of a vertex" << endl;
		cout << "4.Find Distance: Dijkstra" << endl;
		cout << "5.Find Distance: Bellman Ford" << endl;
		cout << "6.Print Graph" << endl;
		cout << "7. Exit" << endl;
		cout << "Enter your choice: ";
		cin >> choice;
		switch (choice) {
		case 1:
			// add edge
			cout
				<< "Please enter the source,destination and weight for the edge"
				<< endl;
			cin >> u >> v >> w;
			g.addEdge(u, v, w);
			break;

		case 2:
			// remove edge
			cout << "Please enter the source and destination of the edge"
				 << endl;
			cin >> u >> v;
			g.removeEdge(u, v);
			break;

		case 3:
			// Get Degree of a vertex
			cout << "Please enter the source vertex" << endl;
			cin >> u;
			cout << "Indegree of " << u << ": " << g.getInDegree(u);
			break;

		case 4:
			// Get Distance of other nodes uisng Dijkstra
			cout << "Please enter the source vertex" << endl;
			cin >> u;
			g.getDistDijkstra(u);
			break;

		case 5:
			// Get Distance of other nodes uisng Bellman Ford
			cout << "Please enter the source vertex" << endl;
			cin >> u;
			g.getDistBellmanFord(u);
			break;

		case 6:
			// Print Graph
			g.printGraph();
			break;

		case 7:
			// exit
			exit(1);
			break;

		default:
			cout << "\nEnter correct option\n";
		}
	}

	return 0;
}
/*
7
9
0 1
0 2
1 2
1 5
2 3
5 3
5 4
6 1
6 5
*/