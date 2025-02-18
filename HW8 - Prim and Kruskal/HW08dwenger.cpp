#include <bits/stdc++.h>
#include <limits.h>
#define INF 0x3f3f3f3f
using namespace std;

typedef pair<int, int> iPair;

class Graph {
	int numVertices, numEdges;
	list<int> *adjLists;
	vector<pair<int, int>> *adj;
  vector<pair<int, vector<pair<int, int>>>> *adjList;
	int *distance; // keep the distance of all vertices from a source vertex
	int *parent;   // keep the parent of all vertices during a traversal
	bool directed;
	int *keys;
	bool *visited;		// keep track of the vertices that are visited
	int *discoveryTime; // keep track of the vertices as they are discovered
	int *finishTime;	// keep track of the vertices as they are finished
						// processing
	int *color;
	bool *inMST;
	list<int> finish;

public:
	Graph(int vertices);
	~Graph();
	void addEdge(int src, int dest, int weight);
	void BFS(int startVertex);
	bool isBipartite(int startVertex);
	void DFSUtil(int startVertex);
	void DFS(int startVertex);
	void topologicalSortingUtil(int startVertex);
	void topologicalSorting(int startVertex);
	void removeEdge(int src, int dest);
	bool isEdge(int src, int dest);
	int getInDegree(int src);
	void printAdjVertices(int src);
	bool hasCommonAdjacent(int src, int dest);
	void printGraph();
	// void removeVertexEdges(int src);
	void nonReachable(int src);
	void getPrimMST(int src);
	void getKruskalMST(int src);
	int find(vector<int> &parent, int i);
	void Union(vector<int> &parent, vector<int> &rank, int x, int y);
};

Graph::Graph(int vertices) {
	numVertices = vertices;
	numEdges = 0;
	directed = false;
	adjLists = new list<int>[vertices];
	adj = new vector<pair<int,int>>[vertices];
  adjList = new vector<pair<int, vector<pair<int, int>>>>[vertices];
}

void Graph::getPrimMST(int src) {
	// Initalize keys of all vertices as INF, all vertices of a Boolean array
	// inMST as false, and parent of every vertex as -1;
	// for (int i = 0; i < numVertices; i++) {
	// 	keys[i] = INF;
	// 	parent[i] = -1;
	// 	inMST[i] = false;
	// }
	vector<int> parent(numVertices, -1);
	vector<int> keys(numVertices, INF);
	vector<bool> inMST(numVertices, false);

	// Create an empty priority_queue pq. Every item of pq is a pair (weight,
	// vertex). Weight (or key) is used as first item of pair as first item is
	// by default used to compare two pairs.
	priority_queue<iPair, vector<iPair>, greater<iPair>> pq;

	// Insert source vertex into pq and make its key as 0.
	pq.push(make_pair(0, src));
	keys[src] = 0;

	// While pq isn't empty
	while (!pq.empty()) {
		// Extract minimum key vertex from pq(u)
		// Include u in MST using inMST[u] = true;
		int u = pq.top().second;
		pq.pop();
		inMST[u] = true;

		// Loop through all adjacents of u and do the following for every vertex
		// v.
		vector<pair<int, int>>::iterator it;
		for (it = adj[u].begin(); it != adj[u].end(); it++) {
			// If v is not currently in MST and its key is greater than weight
			// of the
			int v = (*it).first;
			int weight = (*it).second;
			// current edge (u,v)
			if (inMST[v] == false && keys[v] > weight) {
				// Update key of v with weight(u,v);
				keys[v] = weight;
				// Insert v into the pq
				pq.push(make_pair(keys[v], v));
				// Make u the parent of v
				parent[v] = u;
			}
		}

		// Print edges of MST using parent array
		cout << "Edges that are a part of  the MST: " << endl;
		for (int i = 0; i < numVertices; i++) {
			cout << parent[i] << " - " << i << endl;
		}
	}
}

void Graph::getKruskalMST(int src) {
	// Create a priority queue to store edges sorted by their weight
	priority_queue<
		pair<int, pair<int, int>>,
		vector<pair<int, pair<int, int>>>,
		greater<pair<int, pair<int, int>>>>
		pq;

	vector<pair<int, vector<pair<int, int>>>>::iterator it;
	// Add all edges to the priority queue
	for (it = adjList.begin(); it != adjList.end(); it++) {
		int u = it->first;
		for (auto v : it->second) {
			int w = v.first;
			pq.push(make_pair(w, make_pair(u, v.second)));
		}
	}

	// Create a vector to store the parent of each vertex
	vector<int> parent(numVertices);

	// Create a vector to store the rank of each vertex (for union-find)
	vector<int> rank(numVertices);

	// Initialize parent and rank vectors
	for (int i = 0; i < numVertices; i++) {
		parent[i] = i;
		rank[i] = 0;
	}

	// Initialize MST weight to 0
	int MSTweight = 0;

	// Create a vector to store the edges in the MST
	vector<pair<int, int>> MST;

	// Loop until all vertices are in the MST
	while (!pq.empty() && MST.size() < V - 1) {
		// Get the edge with the smallest weight
		pair<int, pair<int, int>> edge = pq.top();
		pq.pop();

		int u = edge.second.first;
		int v = edge.second.second;
		int w = edge.first;

		// Check if adding this edge creates a cycle
		int uroot = find(parent, u);
		int vroot = find(parent, v);
		if (uroot != vroot) {
			// Add the edge to the MST
			MST.push_back(make_pair(u, v));
			MSTweight += w;

			// Merge the two trees using union-by-rank
			Union(parent, rank, uroot, vroot);
		}
	}

	// Print the edges in the MST and their total weight
	cout << "Edges in the Minimum Spanning Tree:\n";
	for (auto edge : MST) {
		cout << "(" << edge.first << "," << edge.second << ")\n";
	}
	cout << "Total weight of the Minimum Spanning Tree: " << MSTweight << "\n";
}

int Graph::find(vector<int> &parent, int i) {
	if (parent[i] != i) {
		parent[i] = find(parent, parent[i]); // Path compression
	}
	return parent[i];
}

void Graph::Union(vector<int> &parent, vector<int> &rank, int x, int y) {
	int xroot = find(parent, x);
	int yroot = find(parent, y);

	// Attach smaller rank tree under root of higher rank tree (Union by Rank)
	if (rank[xroot] < rank[yroot]) {
		parent[xroot] = yroot;
	} else if (rank[xroot] > rank[yroot]) {
		parent[yroot] = xroot;
	} else {
		parent[yroot] = xroot;
		rank[xroot]++;
	}
}

void Graph::nonReachable(int src) {
	int count = 0;

	DFS(src);

	for (int i = 0; i < numVertices; i++) {
		if (visited[i] == false) {
			count = count + 1;
		}
	}

	cout << "Non reachable vertices from the vertex " << src
		 << " are: " << count << endl;
	return;
}

void Graph::addEdge(int src, int dest, int weight) {
	// In class
	if ((src < 0) || (src >= numVertices) || (dest < 0) ||
		(dest >= numVertices)) {
		return;
	}
	// 2,5
	adj[src].push_back(make_pair(src, weight));
	adj[dest].push_back(make_pair(dest, weight));

	adjLists[src].push_back(dest);
	numEdges++;
	if (!directed) {
		adjLists[dest].push_back(src);
	}
}

void Graph::removeEdge(int src, int dest) {
	// write this function
	/*
	//option 1
	adjLists[src].remove(dest);
	cout<<"Removed"<< endl;
	numEdges--;
	if(!directed)
	{
		adjLists[dest].remove(src);
	}
	return;
	// 0: 1-->2-->3-->4-->D
	list <int>::iterator it;
	it = find(adjLists[src].begin(), adjLists[src].end(), dest);

	if (it != adjLists[src].end())
	{
		adjLists[src].remove(dest);
		cout << "Removed"<< endl;
		numEdges--;
		if(!directed)
		{
			adjLists[dest].remove(src);
		}
	}
	else
	{
		cout << "Element not found\n";
	}
	*/

	list<int>::iterator it;
	for (it = adjLists[src].begin(); it != adjLists[src].end(); it++) {
		if (*it == dest) {
			adjLists[src].erase(it);
			cout << "Removed" << endl;
			numEdges--;
		}
	}
	cout << "Element not found\n";
	return;
}

// void Graph::removeVertexEdges(int src)
// {
//   list<int>::iterator it;
//       for(int i = 0; i < numVertices; i++)
//         {
//           if(isEdge(*it, i) == true)
//           {
//             removeEdge(*it, i);
//           }
//         }
//     }
// }

bool Graph::isEdge(int src, int dest) {
	// returns true if (src,dest) is an edge, otherwise should return false
	if ((src < 0) || (src >= numVertices) || (dest < 0) ||
		(dest >= numVertices)) {
		return false;
	}

	if (!adjLists[src].empty()) {
		list<int>::iterator it;
		for (it = adjLists[src].begin(); it != adjLists[src].end(); it++) {
			if (*it == dest) {
				return true;
			}
		}
	}
	return false;
}

int Graph::getInDegree(int src) {
	// returns the degree of vertex src
	// in class
	list<int>::iterator it;
	int InDegree = 0;

	for (int i = 0; i < numVertices; i++) {
		for (it = adjLists[src].begin(); it != adjLists[src].end(); it++) {
			if (*it == src) {
				InDegree++;
			}
		}
	}
	return InDegree;
}

void Graph::printAdjVertices(int src) {
	// prints all adjacent vertices of a vertex src
	list<int>::iterator it;
	for (it = adjLists[src].begin(); it != adjLists[src].end(); ++it) {
		cout << *it << " ";
	}
	cout << endl;
}

bool Graph::hasCommonAdjacent(int src, int dest) {
	// returns true if vertices src and dest have common adjacent vertices
	list<int>::iterator i;
	list<int>::iterator j;
	for (i = adjLists[src].begin(); i != adjLists[src].end(); i++) {
		for (j = adjLists[dest].begin(); j != adjLists[dest].end(); j++) {
			if (*i == *j) {
				return true;
			}
		}
	}
	return false;
}

void Graph::printGraph() {
	// in class
	list<int>::iterator j;
	for (int i = 0; i < numVertices; i++) {
		cout << i << ":";
		for (j = adjLists[i].begin(); j != adjLists[i].end(); j++) {
			cout << "-->" << *j;
		}
		cout << endl;
	}
}

Graph::~Graph() {
	// write your destructor here
	delete[] adjLists;
	delete[] visited;
	delete[] distance;
	delete[] parent;
	delete[] keys;
	delete[] inMST;
	delete[] discoveryTime;
	delete[] finishTime;
	delete[] color;
}

void Graph::BFS(int startVertex) {
	int i;
	visited = new bool[numVertices];
	// this will be needed for to implement the getDistance function
	// distance = new int[numVertices];

	// initialize visited and distance array
	for (i = 0; i < numVertices; i++) {
		visited[i] = false;
		distance[i] = 0;
	}

	// declare a queue
	queue<int> myQueue;
	// make the visited array index for startVertex true and push it in queue
	visited[startVertex] = true;
	myQueue.push(startVertex);
	list<int>::iterator it;

	while (!myQueue.empty()) {
		int currVertex = myQueue.front();
		myQueue.pop();
		cout << currVertex << "-->";

		for (it = adjLists[currVertex].begin();
			 it != adjLists[currVertex].end();
			 it++) {
			int adjVertex = *it;
			// if adjVertex is not visited
			// mark it as visited
			// push adjVertex in the queue

			if (!visited[adjVertex]) {
				visited[adjVertex] = true;
				myQueue.push(adjVertex);
				// distance[adjVertex] = distance[currVertex] + 1;
			}
		}
	}
	return;
}

// check bipartiteness of a graph
bool Graph::isBipartite(int startVertex) {
}

void Graph::DFSUtil(int startVertex) {
	visited[startVertex] = true;
	// cout << startVertex << "-->";

	list<int>::iterator it;
	for (it = adjLists[startVertex].begin(); it != adjLists[startVertex].end();
		 it++) {
		int adjVertex = *it;

		if (!visited[adjVertex]) {
			DFSUtil(adjVertex);
		}
	}
}

// DFS traversal of the vertices reachable from startVertex
// It uses recursive DFSUtil()
void Graph::DFS(int startVertex) {
	// Mark all the vertices as not visited
	int i;
	visited = new bool[numVertices];

	// initialize visited array
	for (i = 0; i < numVertices; i++) {
		visited[i] = false;
	}

	// Call the recursive helper function to print DFS traversal
	DFSUtil(startVertex);

	// for (i = 0; i < numVertices; i++) {
	// 	if (!visited[i]) {
	// 		DFSUtil(i);
	// 	}
	// }
	// int count = 0;
	// for (int i = 0; i < numVertices; i++)
	//   {
	//     if(visited[i] == false)
	//     {
	//       count = count + 1;
	//     }
	//   }
	return;
}

int main() {
	int nVertex, nEdge, choice, i, u, v, w;
	ifstream file;

	file.open("input.txt", ios::in);

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

	while (1) {
		cout << "\n----------------------" << endl;
		cout << "Operations on Graph" << endl;
		cout << "\n----------------------" << endl;
		cout << "1.Add Edge" << endl;
		cout << "2.Remove Edge" << endl;
		cout << "3.Find Edge" << endl;
		cout << "4.Get Degree of a vertex" << endl;
		cout << "5.Find Adjacent Vertices" << endl;
		cout << "6.Find Common Adjacent Vertices" << endl;
		cout << "7.Print Graph" << endl;
		cout << "8.BFS" << endl;
		cout << "9.DFS" << endl;
		cout << "10.Check Bipartiteness" << endl;
		cout << "11.Topological Sorting" << endl;
		cout << "12.Exit" << endl;
		cout << "13.Remove Vertex and all Edges attached to it" << endl;
		cout << "14. NonReachable" << endl;
		cout << "15. Prim MST" << endl;
		cout << "Enter your choice: ";
		cin >> choice;
		switch (choice) {
		case 1:
			// int u, v;
			cout << "Please enter the source and destination vertex of the edge"
				 << endl;
			cin >> u >> v >> w;
			g.addEdge(u, v, w);
			break;

		case 2:
			// remove edge
			cout << "Please enter the source and destination vertex of the edge"
				 << endl;
			cin >> u >> v;
			g.removeEdge(u, v);
			break;

		case 3:
			// Find Edge
			break;

		case 4:
			// Get Degree of a vertex
			cout << "Please enter the source vertex" << endl;
			cin >> u;
			cout << "Indegree of " << u << ": " << g.getInDegree(u);
			break;

		case 5:
			// Find Adjacent Vertices
			cout << "Please enter the source vertex" << endl;
			cin >> u;

			break;

		case 6:
			// Find Common Adjacent Vertices
			cout << "Enter the source and destination vertices" << endl;
			cin >> u >> v;
			g.hasCommonAdjacent(u, v);
			break;

		case 7:
			g.printGraph();
			break;

		case 8:
			// BFS
			cout << "Please enter the source vertex" << endl;
			cin >> u;
			g.BFS(u);
			break;

		case 9:
			// DFS
			cout << "Please enter the source vertex" << endl;
			cin >> u;
			g.DFS(u);
			break;

		case 10:
			// check bipartiteness

			break;

		case 11:
			// Topological sorting
			break;

		case 12:
			exit(1);
			break;

		case 13:
			cout << "Enter the source vertex you want to delete" << endl;
			cin >> u;
			// g.removeVertexEdges(u);
			break;

		case 14:
			cout << "Enter the source vertex you want to check" << endl;
			cin >> u;
			g.nonReachable(u);
			break;

		case 15:
			cout << "Enter the source you want to check" << endl;
			cin >> u;
			g.getPrimMST(u);
			break;

		default:
			cout << "\nEnter correct option\n";
		}
	}

	return 0;
}