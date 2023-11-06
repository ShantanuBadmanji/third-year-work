#include <iostream>
#include <queue>
#include <vector>

using namespace std;

class Graph {
   private:
    int _numNodes;
    vector<vector<pair<int, int>>> _adjList;  //{nbr,edge-weight}
    vector<int> _heuristicValues;

   public:
    Graph(int numNodes = 0) : _numNodes(numNodes) {}
    void createGraph();
    void getHeuristicValue();
    void bestFirstSearch();
    void displayAdjList();
};
void Graph::createGraph() {
    cout << "Enter the number of nodes:" << endl;
    cin >> _numNodes;

    _adjList.resize(_numNodes);
    for (int i = 0; i < _numNodes; i++) {
        // read all the nbrs of curr node
        auto& nbrs = _adjList.at(i);
        int nbrSize;
        cout << "Enter the count of neighbours for " << i << ": " << endl;
        cin >> nbrSize;

        cout << "Enter the nbr and weight of edges with " << i << ": " << endl;
        for (int j = 0; j < nbrSize; j++) {
            int nbrNode;
            cin >> nbrNode;
            int edgeWeight;
            cin >> edgeWeight;
            nbrs.push_back({nbrNode, edgeWeight});
        }
    }
}
void Graph::getHeuristicValue() {
    cout << "Enter the heuristic value of each node:";
    for (int i = 0; i < _numNodes; i++) {
        int val;
        cin >> val;
        _heuristicValues.push_back(val);
    }
    cout << "Done." << endl;
}

void Graph::bestFirstSearch() {
    int start, goal;
    cout << "Enter The Goal state: ";
    cin >> goal;
    cout << "Enter The Start state: ";
    cin >> start;
    priority_queue<
        pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>>
        pq;
    vector<pair<bool, int>> visited(_numNodes, {false, -1});  //{bool,dis from start}
    pq.push({_heuristicValues.at(start), start});
    visited.at(start) = {true, 0};
    cout << endl;
    while (!pq.empty()) {
        int curr = pq.top().second;
        pq.pop();
        cout << "curr: " << curr << " ";

        if (curr == goal) {
            int totalCost = visited.at(curr).second;
            cout << "Best-First-Search Completed" << endl;
            cout << "Total cost: " << totalCost << endl;
            break;
        }

        for (pair<int, int> nbr : _adjList.at(curr)) {
            int nbrNode = nbr.first;
            int edgeWeight = nbr.second;

            if (visited.at(nbrNode).first) continue;
            cout << "Nbr:" << nbrNode << " ";
            pq.push({_heuristicValues.at(nbrNode), nbrNode});
            visited.at(nbrNode) = {true, visited.at(curr).second + edgeWeight};
        }
        cout << endl;
    }
}

void Graph::displayAdjList() {
    for (int i = 0; i < _adjList.size(); i++) {
        vector<pair<int, int>>& list = _adjList.at(i);
        cout << i << ": ";
        for (pair<int, int> nbr : list) {
            int nbrNode = nbr.first;
            int edgWt = nbr.second;
            printf("(%d, %d) ", nbrNode, edgWt);
        }
        cout << endl;
    }
}

int main(int argc, char const* argv[]) {
    Graph g;
    g.createGraph();
    g.displayAdjList();

    g.getHeuristicValue();
    g.bestFirstSearch();
    return 0;
}

/*
inputs:

10
2
1 3     2 2
2
3 4     4 1
2
5 3     6 1
0
0
1
7 5
2
8 2     9 3
0
0
0


13 12 4 7 3 8 2 4 9 0
9
0



*/