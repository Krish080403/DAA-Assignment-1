#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <chrono>

using namespace std;


string trim(const string &s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}


void readGraph(const string &filename, vector<unordered_set<int> > &graph) {
    ifstream infile(filename);
    if (!infile) {
        cerr << "Error opening file " << filename << endl;
        exit(1);
    }
    string line;
    vector<pair<int,int> > edges;
    unordered_set<int> vertexSet;
    while (getline(infile, line)) {
        line = trim(line);
        if (line.empty() || line[0] == '#') continue;
        istringstream iss(line);
        int u, v;
        if (!(iss >> u >> v))
            continue;
        // Use pair<int,int>(u, v) for C++03 compatibility.
        edges.push_back(pair<int,int>(u, v));
        vertexSet.insert(u);
        vertexSet.insert(v);
    }
    infile.close();

    
    vector<int> vertices(vertexSet.begin(), vertexSet.end());
    sort(vertices.begin(), vertices.end());
    unordered_map<int,int> mapping;
    for (int i = 0; i < vertices.size(); i++) {
        mapping[vertices[i]] = i;
    }
    int n = vertices.size();
    graph.clear();
    graph.resize(n);
    
    
    for (int i = 0; i < edges.size(); i++) {
        int u = mapping[edges[i].first];
        int v = mapping[edges[i].second];
        if (u == v) continue; // ignore self-loops
        graph[u].insert(v);
        graph[v].insert(u);
    }
    cout << "Graph loaded: " << n << " vertices, " << edges.size() 
         << " (original) edges." << endl;
}


void BronKerbosch(vector<int>& R, vector<int>& P, vector<int>& X, 
                    const vector<unordered_set<int> > &graph, vector<vector<int> > &cliques) {
    if (P.empty() && X.empty()) {
        cliques.push_back(R);
        return;
    }
    
    int pivot = -1;
    vector<int> unionPX = P;
    unionPX.insert(unionPX.end(), X.begin(), X.end());
    if (!unionPX.empty())
        pivot = unionPX[0];
    
   
    vector<int> candidates;
    if (pivot != -1) {
        for (int v : P) {
            if (graph[pivot].find(v) == graph[pivot].end())
                candidates.push_back(v);
        }
    } else {
        candidates = P;
    }
    
    for (int v : candidates) {
        vector<int> newR = R;
        newR.push_back(v);
        vector<int> newP, newX;
        
        for (int w : P)
            if (graph[v].find(w) != graph[v].end())
                newP.push_back(w);
        
        for (int w : X)
            if (graph[v].find(w) != graph[v].end())
                newX.push_back(w);
        BronKerbosch(newR, newP, newX, graph, cliques);
        
        auto it = find(P.begin(), P.end(), v);
        if (it != P.end())
            P.erase(it);
        X.push_back(v);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <graph_file>" << endl;
        return 1;
    }
    string filename = argv[1];
    vector<unordered_set<int> > graph;
    readGraph(filename, graph);
    int n = graph.size();
    
    
    vector<int> R;
    vector<int> P(n), X;
    for (int i = 0; i < n; i++) {
        P[i] = i;
    }
    
    vector<vector<int> > cliques;
    
    
    auto start = chrono::high_resolution_clock::now();
    BronKerbosch(R, P, X, graph, cliques);
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    
    
    ofstream outfile("cliques_output2.txt");
    if (!outfile) {
        cerr << "Error opening output file." << endl;
        return 1;
    }
    for (auto &clique : cliques) {
        for (int v : clique)
            outfile << v << " ";
        outfile << "\n";
    }
    outfile.close();
    
    cout << "Total maximal cliques found: " << cliques.size() << endl;
    cout << "Total execution time: " << duration.count() << " ms" << endl;
    
    return 0;
}