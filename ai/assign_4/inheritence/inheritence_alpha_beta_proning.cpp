#include <climits>
#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

enum NodeType_t {
    NoneType,
    TerminalType,
    MaxType,
    MinType
};
// Node Structures

struct Node {
    NodeType_t type;
    Node() : type(NoneType) {}
};
struct MinNode : Node {
    int beta;
    Node *left, *right;
    MinNode() : beta(INT_MAX), left(NULL), right(NULL) { type = MinType; }
};
struct MaxNode : Node {
    int alpha;
    Node *left, *right;
    MaxNode() : alpha(INT_MIN), left(NULL), right(NULL) { type = MaxType; }
};
struct TerminalNode : Node {
    int value;
    TerminalNode(int terVal) : value(terVal) { type = TerminalType; }
};

class AlphaBetaProning {
   private:
    MaxNode *_root;
    vector<int> *_terminals;
    int _internalNodesDepth;

    bool getInternalNodeDepth();
    bool traverseMore(int alpha, int beta) { return alpha < beta; }
    Node *getNewNode(int &terminalIndex, int depth);
    void displayNode(Node *curr);
    int depthFirstSearch(Node *curr, int parentProneValue);

   public:
    AlphaBetaProning(MaxNode *root = NULL) : _root(root), _terminals(NULL), _internalNodesDepth(0){};
    bool CreateGraph(vector<int> &terminals);
    bool getOptimalPath();
};
// public Methods
bool AlphaBetaProning::CreateGraph(vector<int> &terminals) {
    _terminals = new vector(terminals);
    if (getInternalNodeDepth() == false) return false;
    int terminalIndex = 0;
    cout << endl;
    _root = (MaxNode *)getNewNode(terminalIndex, 0);
    // displayNode(_root);
    return true;
}

bool AlphaBetaProning::getOptimalPath() {
    if (_root == NULL) {
        cout << "No Nodes provided!";
        return false;
    }
    int bestChance = depthFirstSearch(_root, INT_MAX);
    cout << "bestChange: " << bestChance << endl;
    displayNode(_root);
    return true;
}

// private Methods
Node *AlphaBetaProning::getNewNode(int &terminalIndex, int depth) {
    depth++;
    if (depth > _internalNodesDepth) {
        // terminal node
        // cout << "ter " << _terminals->at(terminalIndex) << endl;
        return new TerminalNode(_terminals->at(terminalIndex++));
    } else if (depth % 2 != 0) {
        // max node
        MaxNode *curr = new MaxNode();
        curr->left = getNewNode(terminalIndex, depth);
        curr->right = getNewNode(terminalIndex, depth);
        return curr;
    } else {
        // min node
        MinNode *curr = new MinNode();
        curr->left = getNewNode(terminalIndex, depth);
        curr->right = getNewNode(terminalIndex, depth);
        return curr;
    }
}
bool AlphaBetaProning::getInternalNodeDepth() {
    int terminalSize = _terminals->size();
    if (terminalSize == 0) {
        _internalNodesDepth = 0;
    } else {
        double logOfSize = log2(terminalSize);
        if (int(logOfSize) == logOfSize)
            _internalNodesDepth = int(logOfSize);
        else {
            cerr << "the terminal values should be power of 2!" << endl;
            return false;
        }
    }
    cout << "_internalNodesDepth: " << _internalNodesDepth << endl;
    return true;
}
void AlphaBetaProning::displayNode(Node *curr) {
    if (curr->type == TerminalType) {
        cout << ((TerminalNode *)curr)->value << endl;
    } else if (curr->type == MaxType) {
        cout << ((MaxNode *)curr)->alpha << endl;
        displayNode(((MaxNode *)curr)->left);
        displayNode(((MaxNode *)curr)->right);
    } else {
        cout << ((MinNode *)curr)->beta << endl;
        displayNode(((MinNode *)curr)->left);
        displayNode(((MinNode *)curr)->right);
    }
}
int AlphaBetaProning::depthFirstSearch(Node *curr, int parentProneValue) {
    if (curr->type == TerminalType) {
        // Terminal value
        return ((TerminalNode *)curr)->value;
    } else if (curr->type == MaxType) {
        // MaxNode value
        MaxNode *maxNode = (MaxNode *)curr;
        maxNode->alpha = depthFirstSearch(maxNode->left, maxNode->alpha);
        if (traverseMore(maxNode->alpha, parentProneValue)) {
            maxNode->alpha = max(maxNode->alpha, depthFirstSearch(maxNode->right, maxNode->alpha));
        }
        return maxNode->alpha;
    } else {
        // MinNode value
        MinNode *minNode = (MinNode *)curr;
        minNode->beta = depthFirstSearch(minNode->left, minNode->beta);
        if (traverseMore(parentProneValue, minNode->beta)) {
            minNode->beta = min(minNode->beta, depthFirstSearch(minNode->right, minNode->beta));
        }
        return minNode->beta;
    }
}

int main(int argc, char const *argv[]) {
    // vector<int> terminals = {2, 3, 5, 9, 0, 1, 7, 5};
    vector<int> terminals = {3, 4, 2, 1, 7, 8, 9, 10, 12, 11, 1, 12, 14, 9, 13, 16};

    AlphaBetaProning a;
    a.CreateGraph(terminals);
    a.getOptimalPath();

    return 0;
}
