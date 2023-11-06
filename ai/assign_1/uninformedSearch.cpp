#include <iostream>
#include <queue>
#include <vector>

using namespace std;

class Graph {
   private:
    int _numNodes;
    vector<vector<int>> _adjList;
    void dfsHelper(int curr, vector<bool>& visited);

   public:
    Graph(int numNodes = 0) : _numNodes(numNodes) {}
    void createGraph();
    void breathFirstSearch(int src);
    void depthFirstSearch(int src);
    void displayAdjList();
};
void Graph::createGraph() {
    cout << "Enter the number of nodes:" << endl;
    cin >> _numNodes;

    _adjList.resize(_numNodes);
    for (int i = 0; i < _numNodes; i++) {
        // read all the nbrs of curr node
        vector<int>& nbrs = _adjList.at(i);
        int nbrSize;
        cout << "Enter the count of neighbours for " << i << ": " << endl;
        cin >> nbrSize;
        for (int j = 0; j < nbrSize; j++) {
            int nbrNode;
            cin >> nbrNode;
            nbrs.push_back(nbrNode);
        }
    }
}

void Graph::breathFirstSearch(int src) {
    queue<int> que;
    vector<bool> visited(_numNodes, false);
    visited.at(src) = true;
    que.push(src);

    while (!que.empty()) {
        int curr = que.front();
        que.pop();
        cout << curr << " ";
        for (int nbr : _adjList.at(curr)) {
            if (!visited.at(nbr)) {
                visited.at(nbr) = true;
                que.push(nbr);
            }
        }
    }
    cout << "End!" << endl;
}

void Graph::depthFirstSearch(int src) {
    vector<bool> visited(_numNodes, false);
    dfsHelper(src, visited);
    cout << "End!" << endl;
}

void Graph::dfsHelper(int curr, vector<bool>& visited) {
    visited.at(curr) = true;
    cout << curr << " ";
    for (int nbr : _adjList.at(curr)) {
        if (!visited.at(nbr))
            dfsHelper(nbr, visited);
    }
}
void Graph::displayAdjList() {
    cout << endl;
    for (int i = 0; i < _numNodes; i++) {
        vector<int>& list = _adjList.at(i);
        cout << i << ": ";
        for (int nbr : list) {
            cout << nbr << " ";
        }
        cout << endl;
    }
    cout << endl;
}

int main(int argc, char const* argv[]) {
    Graph g;
    g.createGraph();
    g.displayAdjList();

    int start;
    cout << "Enter The Start state: ";
    cin >> start;
    g.breathFirstSearch(start);
    g.depthFirstSearch(start);
    return 0;
}

/*
inputs:

10
2
1 2
2
3 4
2
5 6
0
0
1
7
2
8 9
0
0
0




*/