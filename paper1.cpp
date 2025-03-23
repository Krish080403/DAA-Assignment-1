#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <thread>
#include <future>
#include <mutex>
#include <chrono>

#ifdef _WIN32
    #include <windows.h>
    #include <psapi.h>
#elif defined(_unix) || defined(APPLE_)
    #include <sys/resource.h>
#endif

using namespace std;

vector<vector<int>> adj;
vector<unordered_set<int>> adj_sets;
mutex print_mutex;
ofstream outfile("cliques_output.txt");
int n, max_threads = thread::hardware_concurrency();

int intersection_size(const unordered_set<int> &a, const unordered_set<int> &b) {
    int count = 0;
    if (a.size() < b.size()) {
        for (auto &x : a)
            if (b.find(x) != b.end())
                count++;
    } else {
        for (auto &x : b)
            if (a.find(x) != a.end())
                count++;
    }
    return count;
}

void print_clique(const vector<int> &clique) {
    lock_guard<mutex> lock(print_mutex);
    for (int v : clique)
        outfile << v << " ";
    outfile << "\n";
}

void EXPAND(unordered_set<int> SUBG, unordered_set<int> CAND, vector<int> localQ, int depth = 0);

void expand_worker(int q, vector<int> localQ, unordered_set<int> SUBG, unordered_set<int> CAND, int depth) {
    localQ.push_back(q);

    unordered_set<int> new_SUBG, new_CAND;
    auto &nq = adj_sets[q];

    for (auto &x : SUBG)
        if (nq.find(x) != nq.end())
            new_SUBG.insert(x);
    for (auto &x : CAND)
        if (nq.find(x) != nq.end())
            new_CAND.insert(x);

    EXPAND(new_SUBG, new_CAND, localQ, depth + 1);
}

void EXPAND(unordered_set<int> SUBG, unordered_set<int> CAND, vector<int> localQ, int depth) {
    if (SUBG.empty()) {
        if (!localQ.empty())
            print_clique(localQ);
        return;
    }

    int u = -1, max_inter = -1;
    for (auto &v : SUBG) {
        int val = intersection_size(CAND, adj_sets[v]);
        if (val > max_inter) {
            max_inter = val;
            u = v;
        }
    }

    unordered_set<int> ext;
    auto &nu = adj_sets[u];
    for (auto &x : CAND)
        if (nu.find(x) == nu.end())
            ext.insert(x);

    vector<future<void>> futures;
    for (auto q : ext) {
        CAND.erase(q);

        if (depth < 1 && (int)futures.size() < max_threads) {
            futures.push_back(async(launch::async, expand_worker, q, localQ, SUBG, CAND, depth));
        } else {
            expand_worker(q, localQ, SUBG, CAND, depth);
        }
    }

    for (auto &f : futures)
        f.get();
}

double getMemoryUsageMB() {
#ifdef _WIN32
    PROCESS_MEMORY_COUNTERS info;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &info, sizeof(info))) {
        return info.PeakWorkingSetSize / (1024.0 * 1024.0);
    }
    return 0.0;
#elif defined(_unix) || defined(APPLE_)
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    #ifdef _APPLE_
        // On macOS, ru_maxrss is in bytes.
        return usage.ru_maxrss / (1024.0 * 1024.0);
    #else
        // On Linux, ru_maxrss is in kilobytes.
        return usage.ru_maxrss / 1024.0;
    #endif
#else
    return 0.0;
#endif
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <input_file>" << endl;
        return 1;
    }

    ifstream infile(argv[1]);
    if (!infile) {
        cerr << "Error opening file." << endl;
        return 1;
    }

    string line;
    while (getline(infile, line)) {
        if (line.rfind("# Nodes:", 0) == 0) {
            stringstream ss(line);
            string temp;
            ss >> temp >> temp >> n;
            adj.resize(n);
        } else if (!line.empty() && line[0] != '#') {
            stringstream ss(line);
            int u, v;
            ss >> u >> v;
            if (u < v && u >= 0 && v >= 0 && u < n && v < n) {
                adj[u].push_back(v);
                adj[v].push_back(u);
            }
        }
    }
    infile.close();

    adj_sets.resize(n);
    for (int i = 0; i < n; i++) {
        adj_sets[i] = unordered_set<int>(adj[i].begin(), adj[i].end());
    }

    unordered_set<int> V;
    for (int i = 0; i < n; i++)
        V.insert(i);

    unordered_set<int> CAND = V;
    vector<int> localQ;

    auto start = chrono::high_resolution_clock::now();
    EXPAND(V, CAND, localQ);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;

    outfile << "Total Time Taken: " << elapsed.count() << " seconds" << endl;
    outfile << "Peak Memory Usage: " << getMemoryUsageMB() << " MB" << endl;
    outfile.close();

    cout << "Output written to cliques_output.txt" << endl;
    return 0;
}