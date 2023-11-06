#include <iostream>
#include <queue>
#include <vector>

#define PATH true
#define WALL false

using namespace std;

class PathFinderGame {
   private:
    vector<vector<int>> _playGround;
    pair<int, int> _start, _goal;
    int _length, _breadth;
    int getHeuristicValue(pair<int, int> curr);
    void getRowColPair(int obj);

    int getFitnessValue(int g, int h) {
        cout << "f(n):" << g + h << " ";
        return g + h;
    }
    bool isBlocked(int r, int c) {
        bool outsideBoundry = r < 0 || r >= _length || c < 0 || c >= _breadth;
        return outsideBoundry || _playGround.at(r).at(c) == WALL;
    }

   public:
    PathFinderGame() : _start({0, 0}), _goal({0, 0}), _length(0), _breadth(0){};
    PathFinderGame(pair<int, int> start, pair<int, int> goal) : _start(start), _goal(goal), _length(0), _breadth(0){};
    void CreatePlayGound();
    void displayPlayGound();
    void getPlayGround(vector<vector<int>> &playGround, int length, int breadth);
    void AStar(pair<int, int> start, pair<int, int> goal);
};

int PathFinderGame::getHeuristicValue(pair<int, int> curr) {
    int dr = curr.first - _goal.first;
    int dc = curr.second - _goal.second;
    // absolute value
    dr = dr < 0 ? (-dr) : dr;
    dc = dc < 0 ? (-dc) : dc;
    cout << "h(n):" << dr + dc << " ";
    return dr + dc;
}

void PathFinderGame::getPlayGround(vector<vector<int>> &playGround, int length, int breadth) {
    _length = length;
    _breadth = breadth;
    _playGround.clear();
    for (auto row : playGround) {
        vector<int> newRow(row);
        _playGround.push_back(newRow);
    }
    cout << "_length: " << _length << "_breadth: " << _breadth << endl;
    displayPlayGound();
}

void PathFinderGame::CreatePlayGound() {
    cout << "Enter the length of play ground: " << endl;
    cin >> _length;
    cout << "Enter the breadth of play ground: " << endl;
    cin >> _breadth;
    cout << "You can either give coordinates of paths  or walls." << endl;
    cout << "Do you want to give coordinates of path? (y/n):";
    char isPath;
    cin >> isPath;
    if (isPath == 'y' || isPath == 'Y') {
        _playGround.resize(_length, vector<int>(_breadth, WALL));
        getRowColPair(PATH);
    } else {
        _playGround.resize(_length, vector<int>(_breadth, PATH));
        getRowColPair(WALL);
    }

    displayPlayGound();
}

void PathFinderGame::getRowColPair(int obj) {
    cout << "Enter the elements (row wise: zero index based)." << endl;
    for (int i = 0; i < _length; i++) {
        auto &row = _playGround.at(i);
        cout << "Enter number of indices of column for " << i << " row: " << endl;
        int numColumnIndex;
        cin >> numColumnIndex;
        if (numColumnIndex >= _breadth) {
            cout << "Entire row is marked!" << endl;
            continue;
        }
        //  Assigning PATH or WALL to all the recieved coordinates of a row
        cout << "Enter all the indices of column for " << i << " row: " << endl;
        for (int j = 0; j < numColumnIndex; j++) {
            int c;
            cin >> c;
            row.at(c) = obj;
        }
    }
}
void PathFinderGame::displayPlayGound() {
    cout << "The playground Map: " << endl;
    for (auto row : _playGround) {
        for (int p : row) {
            cout << p << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void PathFinderGame::AStar(pair<int, int> start = {0, 0}, pair<int, int> goal = {0, 0}) {
    pair<int, int> zeros({0, 0});
    if ((start == zeros) && (goal == zeros)) {
        cout << "Enter start index:";
        cin >> start.first >> start.second;
        cout << "Enter goal index:";
        cin >> goal.first >> goal.second;
    }
    _start = start;
    _goal = goal;
    if (isBlocked(_start.first, _start.second) || isBlocked(_goal.first, _goal.second)) {
        cout << "The Start or Goal indices are not vaild." << endl;
        return;
    }
    vector<vector<pair<bool, int>>> visited(
        _length, vector<pair<bool, int>>(_breadth, {false, -1}));  // if true, store g(n)(dis from start to curr)

    priority_queue<pair<int, pair<int, int>>,
                   vector<pair<int, pair<int, int>>>,
                   greater<pair<int, pair<int, int>>>>
        pq;  // estiCostForNode
    // int hueristicIndex = 0, disFromStartIndex = 1, currNodeIndex = 2;
    // Algo start
    int totalCost = 0;
    pq.push({getFitnessValue(0, getHeuristicValue(_start)), _start});
    pair<int, int> curr = {-1, -1};  // dummy location
    while (!pq.empty()) {
        curr = pq.top().second;
        printf("(%d, %d)\n", curr.first, curr.second);
        pq.pop();
        // visited so
        // cout << "END!!" << endl;
        visited.at(curr.first).at(curr.second).first = true;
        // cout << "END!!" << endl;

        if (curr == _goal) {
            totalCost = visited.at(_goal.first).at(_goal.second).second;
            cout << "Total Cost: " << totalCost << endl
                 << "END!!" << endl;
            return;
        }

        // add the nbr to pq
        // int possibleSteps[][2] = {{-1, -1}, {1, -1}, {-1, 1}, {1, 1}}; // for diagoanl nbrs
        int possibleSteps[][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        for (auto step : possibleSteps) {
            int r = curr.first + step[0];
            int c = curr.second + step[1];
            if (isBlocked(r, c)) {
                cout << "isBlocked: " << r << c << endl;
                continue;
            }
            if (visited[r][c].first) {
                continue;
            }
            cout << endl
                 << "r:" << r << " c:" << c << " ";
            visited[r][c].second = visited[curr.first][curr.second].second + 1;  // the g(n) of coordinate(r,c)
            pq.push({getFitnessValue(visited[r][c].second, getHeuristicValue({r, c})), {r, c}});
        }
    }
    cout << "Cannot reach Goal state." << endl;
}

int main(int argc, char const *argv[]) {
    PathFinderGame g;
    vector<vector<int>> pg({{1, 1, 1, 1, 1},
                            {1, 1, 0, 1, 1},
                            {1, 1, 0, 1, 1},
                            {1, 1, 0, 1, 1},
                            {1, 1, 0, 1, 1},
                            {1, 1, 1, 1, 1},
                            {1, 1, 1, 1, 1}});
    g.getPlayGround(pg, 6, 5);
    g.AStar({3, 0}, {3, 3});

    vector<vector<int>> pg2({{1, 0, 1},
                             {0, 0, 1}});
    g.getPlayGround(pg2, 2, 3);
    g.AStar({0, 0}, {1, 2});

    // g.CreatePlayGound();
    return 0;
}
