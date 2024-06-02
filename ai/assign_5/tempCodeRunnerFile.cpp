#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <vector>

using namespace std;

struct Node {
    string color, name;
    set<Node *> neighbours;
    Node(string paraName) : name(paraName) {}
};

class ColorSorting {
    set<string> *_colors;
    vector<Node *> *_nodes;
    map<string, int> _nodeNames;
    bool insertEdge(int node1Index, int node2Index);
    int getNodeIndex(string &nodeName) { return _nodeNames[nodeName]; }

   public:
    ColorSorting() : _colors(NULL), _nodes(NULL) {}
    bool CreateGraph();
    bool getColors(set<string> *paraColors);
    bool setNodeColors();
    void displayNodes();
};
bool ColorSorting::insertEdge(int node1Index, int node2Idex) {
    // check if nodes are assiged
    if (_nodes == NULL) {
        cout << "nodes not assigned!" << endl;
        return false;
    }

    Node *node1 = _nodes->at(node1Index);
    Node *node2 = _nodes->at(node2Idex);

    set<Node *>::iterator it;
    if ((it = node1->neighbours.find(node2)) != node1->neighbours.end()) {
        cout << "Edge already exists." << (*it)->name << endl;
        return false;
    }
    node1->neighbours.insert(node2);
    node2->neighbours.insert(node1);
    return true;
}

bool ColorSorting::CreateGraph() {
    int numNodes;
    cout << "Enter the number of Nodes: " << endl;
    cin >> numNodes;
    if (numNodes < 0) {
        cout << "Only enter Whole Number!" << endl;
        return false;
    }

    // created the new _nodes vector with all elements pointing to NULL
    _nodes = new vector<Node *>(numNodes, NULL);

    for (int i = 0; i < numNodes; i++) {
        string name;
        name.push_back(i + '0');

        transform(name.begin(), name.end(), name.begin(), ::tolower);
        if (_nodeNames.find(name) == _nodeNames.end()) {
            _nodeNames[name] = i;
            _nodes->at(i) = new Node(name);
        } else {
            cout << "Node name already exists!" << endl;
            return false;
        }
    }

    // Make Edges by adding neighbours
    int numEdges;
    cout << "Enter the number of edges between nodes: " << endl;
    cin >> numEdges;
    cout << "Enter the 2 nodes with space in between(nodes1 nodes2): " << endl;
    for (int i = 0; i < numEdges; i++) {
        string node1Name, node2Name;
        cin >> node1Name >> node2Name;
        if (!insertEdge(getNodeIndex(node1Name), getNodeIndex(node2Name))) return false;
    }
    cout << "done." << endl;
    return true;
}

bool ColorSorting::getColors(set<string> *paraColors = NULL) {
    if (paraColors != NULL) {
        _colors = new set(*paraColors);
        return true;
    }
    int numColors;
    cin >> numColors;
    cout << "Enter the number of colors" << endl;
    for (int i = 0; i < numColors; i++) {
        string color;
        cin >> color;
        transform(color.begin(), color.end(), color.begin(), ::tolower);
        if (!_colors->insert(color).second) {
            cout << "Color already exists!" << endl;
            return false;
        };
    }
    cout << "done." << endl;
    return true;
}
bool ColorSorting::setNodeColors() {
    // check if nodes are assiged
    if (_nodes == NULL) {
        cout << "nodes not assigned!" << endl;
        return false;
    }

    for (Node *node : (*_nodes)) {
        if (node->color != "") continue;  // skip of name already given

        set<string> colors = *_colors;  // creating a temp copy of colors to work with

        // remove the mathing colors from set
        for (Node *nbr : node->neighbours) {
            if (nbr->color != "") colors.erase(nbr->color);
        }

        // check if colors are left
        if (colors.size() == 0) {
            cout << "Out of options of colors!" << endl;
            return false;
        }
        node->color = *(colors.begin());  // set the first leftover color to node
    }
    return true;
}
void ColorSorting::displayNodes() {
    cout << "The colors of nodes are: " << endl;
    for (Node *node : *(_nodes)) {
        cout << node->name << ":" << node->color << "  ";
    }
    cout << "done." << endl;
}
int main(int argc, char const *argv[]) {
    ColorSorting cg;
    cg.CreateGraph();
    cg.getColors(new set<string>({"red", "blue", "green"}));
    cg.setNodeColors();
    cg.displayNodes();
    return 0;
}
