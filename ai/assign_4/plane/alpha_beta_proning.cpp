#include <climits>
#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

struct MaxNode;
struct MinNode {
    int beta;
    MaxNode *left, *right;
    MinNode(int b = INT_MAX) : beta(b), left(NULL), right(NULL) {}
};
struct MaxNode {
    int alpha;
    MinNode *left, *right;
    MaxNode(int a = INT_MIN) : alpha(a), left(NULL), right(NULL) {}
};

class AlphaBetaProning {
   private:
    MaxNode *_root;
    vector<int> *_terminals;
    int _internalNodesDepth;

    bool getInternalNodeDepth();
    bool traverseMore(int alpha, int beta) { return alpha < beta; }

    MaxNode *getNewMaxNode(int &terminalIndex, int depth);
    MinNode *getNewMinNode(int &terminalIndex, int depth);

    void displayMinNode(MinNode *curr);
    void displayMaxNode(MaxNode *curr);

    int dfsMaxNode(MaxNode *curr, int parentBeta);
    int dfsMinNode(MinNode *curr, int parentAlpha);

   public:
    AlphaBetaProning(MaxNode *root = NULL) : _root(root), _terminals(NULL), _internalNodesDepth(0){};
    bool CreateGraph(vector<int> &terminals);
    bool getOptimalPath();
};

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
    return true;
}

bool AlphaBetaProning::CreateGraph(vector<int> &terminals) {
    _terminals = new vector(terminals);
    if (getInternalNodeDepth() == false) return false;
    int terminalIndex = 0;
    _root = getNewMaxNode(terminalIndex, 0);
    displayMaxNode(_root);
    return true;
}

bool AlphaBetaProning::getOptimalPath() {
    if (_root == NULL) {
        cout << "No Nodes provided!";
        return false;
    }
    int bestChance = dfsMaxNode(_root, INT_MIN);
    cout << bestChance << endl;
    return true;
}
MaxNode *AlphaBetaProning::getNewMaxNode(int &terminalIndex, int depth) {
    if (terminalIndex >= _terminals->size()) return NULL;
    depth++;
    // cout << "depth: " << depth << " ";

    // terminal nodes
    if (depth > _internalNodesDepth) {
        // cout << "terminal: " << _terminals->at(terminalIndex) << endl;
        return new MaxNode(_terminals->at(terminalIndex++));
    } else {
        MaxNode *curr = new MaxNode();
        curr->left = getNewMinNode(terminalIndex, depth);
        curr->right = getNewMinNode(terminalIndex, depth);
        return curr;
    }
}
MinNode *AlphaBetaProning::getNewMinNode(int &terminalIndex, int depth) {
    if (terminalIndex >= _terminals->size()) return NULL;
    depth++;
    // cout << "depth: " << depth << " ";

    // terminal nodes
    if (depth > _internalNodesDepth) {
        // cout << "terminal: " << _terminals->at(terminalIndex) << endl;
        return new MinNode(_terminals->at(terminalIndex++));
    } else {
        MinNode *curr = new MinNode();
        curr->left = getNewMaxNode(terminalIndex, depth);
        curr->right = getNewMaxNode(terminalIndex, depth);
        return curr;
    }
}

void AlphaBetaProning::displayMinNode(MinNode *curr) {
    if (curr == NULL) return;
    cout << curr->beta << " ";
    displayMaxNode(curr->left);
    displayMaxNode(curr->right);
}

void AlphaBetaProning::displayMaxNode(MaxNode *curr) {
    if (curr == NULL) return;
    cout << curr->alpha << " ";
    displayMinNode(curr->left);
    displayMinNode(curr->right);
}
int AlphaBetaProning::dfsMaxNode(MaxNode *curr, int parentBeta) {
    if (curr->left == NULL && curr->right == NULL) return curr->alpha;
    // left traverse
    curr->alpha = dfsMinNode(curr->left, curr->alpha);
    // curr->alpha = dfsMinNode(curr->right, curr->alpha);

    // right traverse
    if (traverseMore(curr->alpha, parentBeta)) {
        cout << "trasversing  from " << curr->alpha << " with parent " << parentBeta << endl;
        curr->alpha = max(curr->alpha, dfsMinNode(curr->right, curr->alpha));
        // curr->alpha = max(curr->alpha, dfsMinNode(curr->left, curr->alpha));
    } else {
        cout << "cut from " << curr->alpha << " with parent " << parentBeta << endl;
    }
    return curr->alpha;
}
int AlphaBetaProning::dfsMinNode(MinNode *curr, int parentAlpha) {
    if (curr->left == NULL && curr->right == NULL) return curr->beta;
    // right traverse
    curr->beta = dfsMaxNode(curr->left, curr->beta);
    // curr->beta = dfsMaxNode(curr->right, curr->beta);

    // left traverse
    if (traverseMore(parentAlpha, curr->beta)) {
        cout << "trasversing  from " << curr->beta << " with parent " << parentAlpha << endl;
        curr->beta = min(curr->beta, dfsMaxNode(curr->right, curr->beta));
        // curr->beta = min(curr->beta, dfsMaxNode(curr->left, curr->beta));
    } else {
        cout << "cut from " << curr->beta << " with parent " << parentAlpha << endl;
    }
    return curr->beta;
}

int main(int argc, char const *argv[]) {
    cout << INT_MIN << endl;

    vector<int> terminals = {2, 3, 5, 9, 0, 1, 7, 5};
    // vector<int> terminals = {3, 4, 2, 1, 7, 8, 9, 10, 12, 11, 1, 12, 14, 9, 13, 16};
    AlphaBetaProning a;
    a.CreateGraph(terminals);
    cout << endl;
    a.getOptimalPath();
    return 0;
}

// void AlphaBetaProning::getInternalNodeDepth() {
//     int terminalSize = _terminals->size();
// if (terminalSize == 0) {
//     _internalNodesDepth = 0;
// } else if (terminalSize <= 2) {
//     _internalNodesDepth = 1;
// } else {
//     double logOfSize = log2(terminalSize);
//     if (int(logOfSize) == logOfSize)
//         _internalNodesDepth = int(logOfSize);
//     else {
//         _internalNodesDepth = int(logOfSize) + 1;
//     }
// }
// }