#include <iostream>
#include <chrono>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <random>

#define endl '\n'

using namespace std;

const int MAX_NODES = 5000;

mt19937 generator(chrono::steady_clock::now().time_since_epoch().count());

struct DSU
{
    int nodesCounter;
    vector<int> parent;
    vector<int> rank;

    DSU(int _nodesCounter) : nodesCounter(_nodesCounter)
    {
        parent.resize(nodesCounter);
        for (int i = 0; i < nodesCounter; ++i)
        {
            parent[i] = i;
        }

        rank.resize(nodesCounter, 0);
    }

    int find_parent(int currv)
    {
        if (currv == parent[currv])
        {
            return currv;
        }

        return parent[currv] = find_parent(parent[currv]);
    }

    bool union_nodes(int x, int y)
    {
        int root_x = find_parent(x);
        int root_y = find_parent(y);

        if (root_x == root_y)
        {
            return false;
        }

        if (rank[root_x] < rank[root_y])
        {
            swap(root_x, root_y);
        }

        parent[root_y] = root_x;
        rank[root_x] += rank[root_y];

        return true;
    }
};

int availableNodeId;
unordered_map<string, int> nodeId;

int allNodes;
vector<pair<int, int>> graph[MAX_NODES];
bool visited[MAX_NODES];

int allEdges;
vector<pair<int, int>> edges;

vector<string> split_string(string &s, char delimiter)
{
    istringstream stringStream(s);
    string currentString;

    vector<string> result;

    while (getline(stringStream, currentString, delimiter))
    {
        result.push_back(currentString);
    }

    return result;
}

int assign_id(string &s)
{
    if (nodeId.count(s) == 0)
    {
        nodeId[s] = availableNodeId++;
    }

    return nodeId[s];
}

vector<int> karger()
{
    DSU dsu(allNodes);

    int leftNodes = allNodes;
    while (leftNodes > 2)
    {
        vector<int> availableEdges;

        for (int i = 0; i < allEdges; ++i)
        {
            if (dsu.find_parent(edges[i].first) != dsu.find_parent(edges[i].second))
            {
                availableEdges.push_back(i);
            }
        }

        int targetEdge = uniform_int_distribution(0, (int)availableEdges.size() - 1)(generator);
        auto [from, to] = edges[availableEdges[targetEdge]];

        dsu.union_nodes(from, to);
        --leftNodes;
    }

    vector<int> result;
    for (int i = 0; i < allEdges; ++i)
    {
        if (dsu.find_parent(edges[i].first) != dsu.find_parent(edges[i].second))
        {
            result.push_back(i);
        }
    }

    return result;
}

vector<int> find_min_cut()
{
    vector<int> result;

    do
    {
        vector<int> currentNodes = karger();

        if (result.empty() or (int) currentNodes.size() < (int)result.size())
        {
            result = currentNodes;
        }
    } while ((int)result.size() > 3);

    return result;
}

int dfs(int currentNode, vector<bool> &isEdgeRemoved)
{
    visited[currentNode] = true;

    int counter = 1;
    for (auto [nextNode, edgeIndex] : graph[currentNode])
    {
        if (!isEdgeRemoved[edgeIndex] and !visited[nextNode])
        {
            counter += dfs(nextNode, isEdgeRemoved);
        }
    }

    return counter;
}

int part1()
{
    vector<int> removedEdges = find_min_cut();

    vector<bool> isEdgeRemoved(allEdges, false);
    for (int i = 0; i < 3; ++i)
    {
        isEdgeRemoved[removedEdges[i]] = true;
    }

    int result = 1;
    for (int i = 0; i < allNodes; ++i)
    {
        if (!visited[i])
        {
            result *= dfs(i, isEdgeRemoved);
        }
    }

    return result;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ifstream fin("input.txt");

    string row;
    while (getline(fin, row))
    {
        stringstream sin(row);

        string currentNode;
        sin >> currentNode;

        currentNode.pop_back();
        int currentNodeId = assign_id(currentNode);

        string nextNode;
        while (sin >> nextNode)
        {
            int edgeIndex = (int)edges.size();
            int nextNodeId = assign_id(nextNode);

            graph[currentNodeId].push_back({nextNodeId, edgeIndex});
            graph[nextNodeId].push_back({currentNodeId, edgeIndex});

            edges.push_back({currentNodeId, nextNodeId});
        }
    }

    allNodes = availableNodeId;
    allEdges = (int)edges.size() - 1;

    fin.close();

    auto t_begin = chrono::high_resolution_clock::now();
    cout << "Result, part 1: " << part1() << endl;
    auto t_end = chrono::high_resolution_clock::now();

    auto part1Time = chrono::duration<double, std::milli>(t_end - t_begin).count();

    cout << "Completed part 1 in: " << part1Time << " ms" << endl;

    return 0;
}