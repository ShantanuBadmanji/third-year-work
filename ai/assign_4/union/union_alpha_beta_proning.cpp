#include <climits>
#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

enum NodeType {
    MinType,
    MaxType,
    TerminalType
};

struct MaxNode;
struct MinNode {
    int beta;
    MaxNode *left, *right;
    MinNode() : beta(INT_MAX), left(NULL), right(NULL) {}
};
struct MaxNode {
    int alpha;
    MinNode *left, *right;
    MaxNode() : alpha(INT_MIN), left(NULL), right(NULL) {}
};
class Node {
    void deleteNode() {
        switch (type) {
            case TerminalType:
                terminalValue = 0;
                break;
            case MaxType:
                mx.MaxNode::~MaxNode();
                break;
            case MinType:
                mn->MinNode::~MinNode();
                break;

            default:
                cerr << "Error: Unknown NodeType passed to Node destructor" << endl;
                break;
        }
    }

   public:
    NodeType type;

    union {
        MaxNode mx;
        MinNode *mn;
        int terminalValue;
    };
    Node(NodeType nodeType) {
        switch (nodeType) {
            case MaxType:
                type = MaxType;
                new (&mx) MaxNode();
                break;
            case MinType:
                type = MinType;
                mn = new MinNode();
                break;
            default:
                if (nodeType == TerminalType)
                    cerr << "Error: Terminal NodeType passed to min-max Node constructor" << endl;
                else
                    cerr << "Error: Unknown NodeType passed" << endl;
                break;
        }
    }
    Node(int value) : type(TerminalType), terminalValue(value) {}
    ~Node() { deleteNode(); }
};

int main(int argc, char const *argv[]) {
    {
        Node n(MaxType);
        n.mn->beta = 16;
        n.terminalValue = 31;
        cout << n.mn->beta << endl;
    }

    return 0;
}
