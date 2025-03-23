#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <chrono>

using namespace std;
using namespace chrono;

class MaximalClique {
public:
    MaximalClique(int n, vector<vector<int>> &edges) : n(n), adj(n), cliqueCount(0), maxCliqueSize(0) {
        for (auto &edge : edges) {
            adj[edge[0]].insert(edge[1]);
            adj[edge[1]].insert(edge[0]); 
        }
    }

    void findMaximalCliques(const string &outputFile) {
        ofstream outFile(outputFile);
        vector<int> order = degeneracyOrdering();
        unordered_set<int> processed;

        for (int v : order) {
            unordered_set<int> P, X;
            for (int w : adj[v]) {
                if (processed.find(w) == processed.end()) {
                    P.insert(w);
                } else {
                    X.insert(w);
                }
            }
            unordered_set<int> R = {v};
            BronKerboschPivot(P, R, X, outFile);
            processed.insert(v);
        }

        outFile.close();
        
        // Print summary results
        cout << "Total maximal cliques found: " << cliqueCount << endl;
        cout << "Size of largest clique: " << maxCliqueSize << endl;
                
        // Print clique size distribution
        cout << "Clique size distribution:\n";
        for (const auto &entry : cliqueSizeCount) {
            cout << "Size " << entry.first << ": " << entry.second << " cliques\n";
        }
    }

private:
    int n;
    vector<unordered_set<int>> adj;
    int cliqueCount;
    int maxCliqueSize;
    unordered_map<int, int> cliqueSizeCount;
    duration<double> elapsedTime;

    vector<int> degeneracyOrdering() {
        vector<int> order;
        vector<int> degree(n);
        vector<vector<int>> bucket(n);
        vector<int> pos(n);

        for (int i = 0; i < n; i++) {
            degree[i] = adj[i].size();
            bucket[degree[i]].push_back(i);
        }
        
        int d = 0;
        for (int i = 0; i < n; i++) {
            while (bucket[d].empty()) d++;
            int v = bucket[d].back();
            bucket[d].pop_back();
            order.push_back(v);
            pos[v] = order.size() - 1;

            for (int w : adj[v]) {
                if (degree[w] > d) {
                    bucket[degree[w]].erase(find(bucket[degree[w]].begin(), bucket[degree[w]].end(), w));
                    degree[w]--;
                    bucket[degree[w]].push_back(w);
                }
            }
        }
        return order;
    }

    void BronKerboschPivot(unordered_set<int> &P, unordered_set<int> &R, unordered_set<int> &X, ofstream &outFile) {
        if (P.empty() && X.empty()) {
            printClique(R, outFile);
            cliqueCount++;
            return;
        }

        int pivot = choosePivot(P, X);
        unordered_set<int> P_pivot_diff;

        for (int v : P) { 
            if (adj[pivot].find(v) == adj[pivot].end()) {
                P_pivot_diff.insert(v);
            }
        }

        for (int v : P_pivot_diff) {
            unordered_set<int> newP, newX;
            for (int w : P) { 
                if (adj[v].find(w) != adj[v].end()) {
                    newP.insert(w);
                }
            }
            for (int w : X) { 
                if (adj[v].find(w) != adj[v].end()) {
                    newX.insert(w);
                }
            }
            unordered_set<int> newR = R;
            newR.insert(v);
            BronKerboschPivot(newP, newR, newX, outFile);
            P.erase(v);
            X.insert(v);
        }
    }

    int choosePivot(const unordered_set<int> &P, const unordered_set<int> &X) {
        int pivot = -1, maxCount = -1;
        for (int v : P) {
            int count = 0;
            for (int w : P) {
                if (adj[v].find(w) != adj[v].end()) {
                    count++;
                }
            }
            if (count > maxCount) {
                maxCount = count;
                pivot = v;
            }
        }
        return pivot;
    }

    void printClique(unordered_set<int> &clique, ofstream &outFile) {
        int size = clique.size();
        cliqueSizeCount[size]++;
        maxCliqueSize = max(maxCliqueSize, size);

        outFile << "{ ";
        for (int v : clique) {
            outFile << v << " ";
        }
        outFile << "}\n";
    }
};

// Reads graph from file and ensures zero-based indexing
vector<vector<int>> readGraphFromFile(const string &filename, int &numNodes) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error: Cannot open file " << filename << endl;
        exit(1);
    }

    string line;
    vector<vector<int>> edges;
    unordered_map<int, int> vertexMap;
    int nextIndex = 0;

    while (getline(file, line)) {
        if (line.empty() || line[0] == '#') continue; 
        stringstream ss(line);
        int u, v;
        if (!(ss >> u >> v)) continue; 

        if (vertexMap.find(u) == vertexMap.end()) vertexMap[u] = nextIndex++;
        if (vertexMap.find(v) == vertexMap.end()) vertexMap[v] = nextIndex++;

        int newU = vertexMap[u];
        int newV = vertexMap[v];

        edges.push_back({newU, newV});
        edges.push_back({newV, newU}); 
    }

    numNodes = nextIndex; 
    return edges;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <input_file>" << endl;
        return 1;
    }

    string filename = argv[1];
    string outputFilename = "maximal_cliques.txt";

    cout << "Processing file: " << filename << endl;

    int n = 0;
    auto start = high_resolution_clock::now(); 

    vector<vector<int>> edges = readGraphFromFile(filename, n);
    MaximalClique mc(n, edges);
    mc.findMaximalCliques(outputFilename);

    auto end = high_resolution_clock::now(); 
    duration<double> elapsed = end - start;

    cout << "Execution Time: " << elapsed.count() << " seconds" << endl;
    cout << "Results saved in: " << outputFilename << endl;

    return 0;
}