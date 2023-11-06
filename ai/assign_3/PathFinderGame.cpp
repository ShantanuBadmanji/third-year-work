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
    bool isBlocked(int x, int y) {
        bool outsideBoundry = x < 0 || x >= _length || y < 0 || y >= _breadth;
        return outsideBoundry || _playGround.at(x).at(y) == WALL;
    }

   public:
    PathFinderGame() : _start({0, 0}), _goal({0, 0}), _length(0), _breadth(0){};
    PathFinderGame(pair<int, int> start, pair<int, int> goal) : _start(start), _goal(goal), _length(0), _breadth(0){};
    void CreatePlayGound();
    void displayPlayGound();
    void getPlayGround(vector<vector<int>> &playGround);
    void AStar(pair<int, int> start, pair<int, int> goal);
    void displayCoordinate(int x, int y) { printf("(%d, %d)", y, x); }
};

int PathFinderGame::getHeuristicValue(pair<int, int> curr) {
    int dx = curr.first - _goal.first;
    int dy = curr.second - _goal.second;
    // absolute value
    dx = dx < 0 ? (-dx) : dx;
    dy = dy < 0 ? (-dy) : dy;
    cout << "h(n):" << dx + dy << " ";
    return dx + dy;
}

void PathFinderGame::getPlayGround(vector<vector<int>> &playGround) {
    if ((_length = playGround.size()) == 0) {
        return;
    };
    if ((_breadth = playGround.at(0).size()) == 0) {
        return;
    };
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
    cout << "Enter the coordinates (row wise: zero index based)." << endl;
    for (int i = 0; i < _length; i++) {
        auto &row = _playGround.at(i);
        cout << "Enter number of coordinates for " << i << " row: " << endl;
        int numCoordinates;
        cin >> numCoordinates;
        if (numCoordinates >= _breadth) {
            cout << "Entire row is marked!" << endl;
            continue;
        }
        //  Assigning PATH or WALL to all the recieved coordinates of a row
        cout << "Enter all the y coordinates for " << i << " row: " << endl;
        for (int j = 0; j < numCoordinates; j++) {
            int y;
            cin >> y;
            row.at(y) = obj;
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
        cout << "Enter start coordinates:";
        cin >> start.first >> start.second;
        cout << "Enter goal coordinates:";
        cin >> goal.first >> goal.second;
    }
    _start = start;
    _goal = goal;
    if (isBlocked(_start.first, _start.second) || isBlocked(_goal.first, _goal.second)) {
        cout << "The Start or Goal coordinates are not vaild." << endl;
        return;
    }
    // vector<pair<bool, int>> cols(_breadth, {false, -1});
    // cerr << "cols-size: " << cols.size();
    vector<vector<pair<bool, int>>> visited(_length, vector<pair<bool, int>>(_breadth));  // if true, store g(n)(dis from start to curr)
    // cerr << "visited size: " << visited.size() << "  ";
    // cerr << visited.at(0).size() << endl;
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
        // printf("(%d, %d)\n", curr.first, curr.second);
        displayCoordinate(curr.first, curr.second);
        pq.pop();
        // visited so
        // cout << "END!!" << endl;
        // visited[curr.first][curr.second].first = true;
        visited.at(curr.first).at(curr.second).first = true;
        // cout << "END!!" << endl;

        if (curr == _goal) {
            cout << "END!!" << endl;
            break;
        }
        // add the nbr to pq
        // cout << "END!!" << endl;

        // int possibleSteps[][2] = {{-1, -1}, {1, -1}, {-1, 1}, {1, 1}};
        int possibleSteps[][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        for (auto step : possibleSteps) {
            int x = curr.first + step[0];
            int y = curr.second + step[1];
            if (isBlocked(x, y)) {
                // cout << "isBlocked: " << x << y << endl;
                cout << "isBlocked: ";
                displayCoordinate(x, y);
                continue;
            }
            if (visited[x][y].first) {
                continue;
            }
            // cout << endl
            //      << "x:" << x << " y:" << y << " ";
            cout << endl;
            displayCoordinate(x, y);
            visited[x][y].second = visited[curr.first][curr.second].second + 1;  // the g(n) of coordinate(x,y)
            pq.push({getFitnessValue(visited[x][y].second, getHeuristicValue({x, y})), {x, y}});

            // if (!isBlocked(x, y) && !visited[x][y].first && _playGround[x][y] == PATH) {
            //     cout << endl
            //          << "x:" << x << " y:" << y << " ";
            //     visited[x][y].second = visited[curr.first][curr.second].second + 1;  // the g(n) of coordInate(x,y)
            //     pq.push({getFitnessValue(visited[x][y].second, getHeuristicValue({x, y})), {x, y}});
            // }
        }
    }
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

    vector<vector<int>> pg2({{1, 1, 1},
                             {0, 0, 1}});

    // g.getPlayGround(pg2);
    g.CreatePlayGound();

    g.AStar({0, 0}, {1, 2});
    return 0;
}
