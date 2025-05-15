#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <stack>
#include <algorithm>
#include <limits>

using namespace std;

// City District Tree Structure
struct DistrictNode {
    char symbol;
    list<DistrictNode*> subdistricts;
    explicit DistrictNode(char symbol) : symbol(symbol) {}
};

struct DistrictTree {
    DistrictNode* root;
    explicit DistrictTree(DistrictNode* root = nullptr) : root(root) {}

    list<char> findUppercaseDFS() {
        list<char> result;
        if (!root) return result;

        stack<DistrictNode*> nodeStack;
        nodeStack.push(root);

        while (!nodeStack.empty()) {
            DistrictNode* current = nodeStack.top();
            nodeStack.pop();

            if (isupper(current->symbol)) {
                result.push_back(current->symbol);
            }

            for (auto it = current->subdistricts.rbegin(); it != current->subdistricts.rend(); ++it) {
                nodeStack.push(*it);
            }
        }

        return result;
    }

    list<char> findUppercaseBFS() {
        list<char> result;
        if (!root) return result;

        queue<DistrictNode*> nodeQueue;
        nodeQueue.push(root);

        while (!nodeQueue.empty()) {
            DistrictNode* current = nodeQueue.front();
            nodeQueue.pop();

            if (isupper(current->symbol)) {
                result.push_back(current->symbol);
            }

            for (DistrictNode* child : current->subdistricts) {
                nodeQueue.push(child);
            }
        }

        return result;
    }
};


// City Road Network Graph

struct RoadConnection;
struct City;

struct SearchInfo {
    bool discovered;
    unsigned distance;
    const City* previous;
    bool finalized;

    void reset() {
        discovered = false;
        distance = numeric_limits<unsigned>::max();
        previous = nullptr;
        finalized = false;
    }
};

struct City {
    string name;
    list<RoadConnection> connections;
    SearchInfo searchInfo;

    explicit City(string name) : name(move(name)) { resetSearchInfo(); }
    void resetSearchInfo() { searchInfo.reset(); }
};

struct RoadConnection {
    City* destination;
    unsigned length;

    RoadConnection(City* destination, unsigned length)
            : destination(destination), length(length) {}
};

class CityNetwork {
private:
    list<City> cities;

public:
    void addCity(const string& name) {
        cities.emplace_back(name);
    }

    void addRoad(const string& from, const string& to, unsigned length) {
        City* sourceCity = findCity(from);
        City* destCity = findCity(to);

        if (sourceCity && destCity) {
            sourceCity->connections.emplace_back(destCity, length);
            destCity->connections.emplace_back(sourceCity, length);
        }
    }

    City* findCity(const string& name) {
        for (auto& city : cities) {
            if (city.name == name) return &city;
        }
        return nullptr;
    }

    void resetSearchData() {
        for (City& city : cities) {
            city.resetSearchInfo();
        }
    }

    list<string> findReachableCities(const string& startCity) {
        resetSearchData();
        list<string> result;

        City* start = findCity(startCity);
        if (!start) return result;

        queue<City*> q;
        start->searchInfo.discovered = true;
        q.push(start);

        while (!q.empty()) {
            City* current = q.front();
            q.pop();

            result.push_back(current->name);

            for (const RoadConnection& road : current->connections) {
                if (!road.destination->searchInfo.discovered) {
                    road.destination->searchInfo.discovered = true;
                    q.push(road.destination);
                }
            }
        }

        return result;
    }

    map<string, unsigned> calculateShortestPaths(const string& startCity) {
        resetSearchData();
        map<string, unsigned> result;

        City* start = findCity(startCity);
        if (!start) return result;

        auto cmp = [](const City* a, const City* b) {
            return a->searchInfo.distance > b->searchInfo.distance;
        };
        priority_queue<City*, vector<City*>, decltype(cmp)> pq(cmp);

        start->searchInfo.distance = 0;
        pq.push(start);

        while (!pq.empty()) {
            City* current = pq.top();
            pq.pop();

            if (current->searchInfo.finalized) continue;
            current->searchInfo.finalized = true;

            for (const RoadConnection& road : current->connections) {
                unsigned newDist = current->searchInfo.distance + road.length;
                if (newDist < road.destination->searchInfo.distance) {
                    road.destination->searchInfo.distance = newDist;
                    road.destination->searchInfo.previous = current;
                    pq.push(road.destination);
                }
            }
        }

        for (const City& city : cities) {
            if (city.searchInfo.distance != numeric_limits<unsigned>::max()) {
                result[city.name] = city.searchInfo.distance;
            }
        }

        return result;
    }
};

// Demonstration
void displayHeader(const string& title) {
    cout << "\n=== " << title << " ===" << endl;
}

DistrictTree createCityDistrictTree() {
    /*
        City District Structure:
            C (City Center)
           / \
          N   S (North and South Districts)
         / \   \
        A   B   W (Areas A, B, and West)
    */
    DistrictNode* C = new DistrictNode('C');
    DistrictNode* N = new DistrictNode('N');
    DistrictNode* S = new DistrictNode('S');
    DistrictNode* A = new DistrictNode('A');
    DistrictNode* B = new DistrictNode('B');
    DistrictNode* W = new DistrictNode('W');

    C->subdistricts = {N, S};
    N->subdistricts = {A, B};
    S->subdistricts = {W};

    return DistrictTree(C);
}

void demonstrateDistrictAnalysis() {
    displayHeader("City District Hierarchy Analysis");

    DistrictTree districtTree = createCityDistrictTree();

    cout << "Administrative districts (DFS search): ";
    list<char> dfsImportant = districtTree.findUppercaseDFS();
    for (char c : dfsImportant) cout << c << " ";
    cout << endl;

    cout << "Administrative districts (BFS search): ";
    list<char> bfsImportant = districtTree.findUppercaseBFS();
    for (char c : bfsImportant) cout << c << " ";
    cout << endl;
}

void demonstrateRoadNetworkAnalysis() {
    displayHeader("City Road Network Analysis");

    CityNetwork network;

    network.addCity("Metropolis");
    network.addCity("Gotham");
    network.addCity("Star City");
    network.addCity("Central City");
    network.addCity("Coast City");

    network.addRoad("Metropolis", "Gotham", 50);
    network.addRoad("Metropolis", "Star City", 120);
    network.addRoad("Gotham", "Central City", 80);
    network.addRoad("Star City", "Central City", 60);
    network.addRoad("Central City", "Coast City", 40);

    cout << "Connected cities from Metropolis:\n";
    list<string> reachable = network.findReachableCities("Metropolis");
    for (const string& city : reachable) {
        cout << "- " << city << endl;
    }

    cout << "\nShortest travel distances from Metropolis:\n";
    map<string, unsigned> distances = network.calculateShortestPaths("Metropolis");
    for (const auto& [city, distance] : distances) {
        cout << city << ": " << distance << " km\n";
    }
}

int main() {
    cout << "CITY INFRASTRUCTURE ANALYZER\n";
    cout << "===========================\n";

    demonstrateDistrictAnalysis();
    demonstrateRoadNetworkAnalysis();

    cout << "\nAnalysis complete.\n";
    return 0;
}