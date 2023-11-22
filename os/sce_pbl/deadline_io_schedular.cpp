#include <unistd.h>

#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

static const int readDeadline = 500;
static const int writeDeadline = 5000;
static const int writeStarved = 2;  // for every 2 read req one write req is displatched after expiration
static const int fifoBatch = 5;     // max capacity of the dispatcher

struct Request {
    int sector;
    int expireAt;
    bool isPending;
    int reqID;
    Request(int paraReqID, int paraExpireAt, int paraSector) : reqID(paraReqID), expireAt(paraExpireAt), sector(paraSector), isPending(true) {}
};

class Deadline_IO_Schedular {
   private:
    int diskSeekPointer, readPriorityCount;
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> *writeRequestArrivalArray;  //{{arrival,sector},..}
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> *readRequestArrivalArray;   //{{arrival,sector},..}

    void updateSortedArray(int currTime);
    void addRequestToSorted(int reqSector, Request *req);
    bool pushToDispatcher(Request *req);

    /*
    Assume to move diskPointer by one we need one clock-time
    */
    void runDispatcher(int &time);

   public:
    queue<Request *> *writeQueue, *readQueue, *dispatcher;
    vector<Request *> *sortedArray;
    int totalRequests;

    Deadline_IO_Schedular() : writeQueue(NULL), readQueue(NULL), writeRequestArrivalArray(NULL), readRequestArrivalArray(NULL), dispatcher(NULL), sortedArray(NULL), totalRequests(0), diskSeekPointer(0), readPriorityCount(0){};

    bool startScheduling();

    void setWriteRequestArrivalArray(vector<pair<int, int>> paraWriteRequestArrivalArray) {
        writeRequestArrivalArray = new priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>>();
        for (auto p : paraWriteRequestArrivalArray) {
            writeRequestArrivalArray->push(p);
        }
        cout << "writeRequestArrivalArray->size(): " << writeRequestArrivalArray->size() << " " << writeRequestArrivalArray->top().second << endl;
    }

    void setReadRequestArrivalArray(vector<pair<int, int>> paraReadRequestArrivalArray) {
        readRequestArrivalArray = new priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>>();
        for (auto p : paraReadRequestArrivalArray) {
            readRequestArrivalArray->push(p);
        }
        cout << "readRequestArrivalArray->size(): " << readRequestArrivalArray->size() << " " << readRequestArrivalArray->top().second << endl;
    }
    void displaySortedArray() {
        for (auto req : *sortedArray) {
            cout << req->reqID << " " << req->sector << "   ";
        }
        cout << endl;
    }
};

bool Deadline_IO_Schedular::startScheduling() {
    int time = 0;
    cout << "entered" << endl;
    writeQueue = new queue<Request *>();
    readQueue = new queue<Request *>();
    dispatcher = new queue<Request *>();
    sortedArray = new vector<Request *>();
    do {
        cout << "time: " << time << endl;

        updateSortedArray(time);

        //-------------------
        if (dispatcher->size() < fifoBatch) {
            cout << "yes dispatcher is not full" << endl;
            // fill the dispatcher to capacity or till no more request are left

            if ((writeQueue->front() && (time > writeQueue->front()->expireAt)) ||
                (readQueue->front() && (time > readQueue->front()->expireAt))) {
                // fill requests from fifo-queues(readQueue/writeQueue)
                cout << "fill requests from fifo-queues(readQueue/writeQueue)" << endl;
                while (dispatcher->size() < fifoBatch && ((!writeQueue->empty()) || (!readQueue->empty()))) {
                    if (writeQueue->empty()) {
                        if (pushToDispatcher(readQueue->front())) {
                            readPriorityCount++;
                        }
                        readQueue->pop();
                    } else if (readQueue->empty()) {
                        if (pushToDispatcher(writeQueue->front())) {
                            readPriorityCount = 0;
                        }
                        writeQueue->pop();
                    } else if (!writeQueue->empty() && readPriorityCount >= writeStarved) {
                        if (pushToDispatcher(writeQueue->front())) {
                            readPriorityCount = 0;
                        }
                        writeQueue->pop();
                    } else {
                        if (pushToDispatcher(readQueue->front())) {
                            readPriorityCount++;
                        }
                        readQueue->pop();
                    }
                }
            } else if (!sortedArray->empty()) {
                // fill requests from sorted array
                cout << "fill requests from sorted array" << endl;
                while ((!sortedArray->empty()) && dispatcher->size() < fifoBatch) {
                    pushToDispatcher(sortedArray->at(0));
                    sortedArray->erase(sortedArray->begin());
                }
            } else {
                cout << "ideal state" << endl;
                // ideal state
                time++;
                continue;
            }
        }

        if (dispatcher->size() >= fifoBatch) {
            runDispatcher(time);
        } else {
            time++;
            sleep(1);
        }

        //----------------
    } while ((!sortedArray->empty()) || (!writeRequestArrivalArray->empty()) || (!readRequestArrivalArray->empty()));
    return true;
}

void Deadline_IO_Schedular::updateSortedArray(int currTime) {
    cout << "updateSortedArray" << endl;

    while ((!readRequestArrivalArray->empty()) && (currTime >= readRequestArrivalArray->top().first)) {
        int arrival = readRequestArrivalArray->top().first;
        int sector = readRequestArrivalArray->top().second;
        Request *req = new Request(++totalRequests, arrival + readDeadline, sector);
        readRequestArrivalArray->pop();
        addRequestToSorted(sector, req);
        readQueue->push(req);
    }
    while ((!writeRequestArrivalArray->empty()) && currTime >= writeRequestArrivalArray->top().first) {
        int arrival = writeRequestArrivalArray->top().first;
        int sector = writeRequestArrivalArray->top().second;
        Request *req = new Request(++totalRequests, arrival + readDeadline, sector);
        writeRequestArrivalArray->pop();
        addRequestToSorted(sector, req);
        writeQueue->push(req);
    }
    displaySortedArray();
}

void Deadline_IO_Schedular::addRequestToSorted(int reqSector, Request *req) {
    cout << "addRequestToSorted with sector:" << req->sector << endl;
    bool SimpleSorting = true;
    if (SimpleSorting) {
        sortedArray->push_back(req);
        int start = 0, end = sortedArray->size() - 1;

        for (int i = start; i < (end - start + 1); i++) {
            int j = i - 1;
            Request *temp = sortedArray->at(i);
            while (j >= start && sortedArray->at(j)->sector > temp->sector) {
                sortedArray->at(j + 1) = sortedArray->at(j);
                j--;
            }
            sortedArray->at(j + 1) = temp;
        }
    } else {
        int secondArrStart = sortedArray->size();
        int sortedArrSize = sortedArray->size();
        for (int i = 0; i < (sortedArrSize - 1); i++) {
            if (sortedArray->at(i)->sector > sortedArray->at(i + 1)->sector) {
                secondArrStart = i + 1;
                break;
            }
        }

        if (diskSeekPointer < reqSector) {
            // first array
            sortedArray->emplace(sortedArray->begin() + secondArrStart, req);

            int start = 0, end = secondArrStart;
            int j = end - 1;
            Request *temp = sortedArray->at(end);
            while (j >= start && sortedArray->at(j)->sector > temp->sector) {
                sortedArray->at(j + 1) = sortedArray->at(j);
                j--;
            }
            sortedArray->at(j + 1) = temp;
        } else {
            // second array
            sortedArray->push_back(req);
            int start = secondArrStart, end = sortedArray->size() - 1;
            int j = end - 1;
            Request *temp = sortedArray->at(end);
            while (j >= start && sortedArray->at(j)->sector > temp->sector) {
                sortedArray->at(j + 1) = sortedArray->at(j);
                j--;
            }
            sortedArray->at(j + 1) = temp;
        }
    }

    displaySortedArray();
}

bool Deadline_IO_Schedular::pushToDispatcher(Request *req) {
    if (req->isPending) {
        cout << "pushToDispatcher: Success" << endl;
        dispatcher->push(req);
        dispatcher->back()->isPending = false;

        return true;
    }
    cout << "pushToDispatcher: Failed" << endl;
    return false;
}

void Deadline_IO_Schedular::runDispatcher(int &time) {
    cout << "runDispatcher" << endl;
    int tracks = 0;
    while (!dispatcher->empty()) {
        Request *req = dispatcher->front();
        dispatcher->pop();
        int diff = req->sector - diskSeekPointer;
        diskSeekPointer = req->sector;
        time += (diff < 0) ? -diff : diff;
        tracks += time;
        cout << req->reqID << ", " << req->sector << endl;
    }
    cout << "TOTAL TRACKS: " << tracks << endl;
}

int main(int argc, char const *argv[]) {
    Deadline_IO_Schedular dl;

    vector<pair<int, int>> write = vector<pair<int, int>>();
    write.push_back({0, 56});
    write.push_back({1, 200});
    write.push_back({0, 284});

    vector<pair<int, int>> read = vector<pair<int, int>>();
    read.push_back({0, 146});
    read.push_back({0, 232});
    read.push_back({1, 296});
    read.push_back({1, 10});
    read.push_back({1, 160});

    dl.setReadRequestArrivalArray(read);
    dl.setWriteRequestArrivalArray(write);
    dl.startScheduling();
    return 0;
}
