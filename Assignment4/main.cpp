#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

int charToInt(char c) 
{
    if (c >= 'A' && c <= 'Z') 
    {
        return c - 'A';
    } 
    else if (c >= 'a' && c <= 'z') 
    {
        return c - 'a' + 26;
    }
    return -1;
}

// Structure to represent an edge with weight
struct Edge 
{
    int u, v, weight;
};

// Function to compare edges based on weight (for sorting)
bool compareEdges(const Edge& e1, const Edge& e2) 
{
    return e1.weight < e2.weight;
}

// Function to find the parent of a vertex in a disjoint-set data structure
int findParent(int vertex, vector<int>& parent) 
{
    if (parent[vertex] == vertex) return vertex;
    return parent[vertex] = findParent(parent[vertex], parent);
}

// Function to apply Kruskal's algorithm to find the MST
int kruskalMST(const vector<vector<int>>& adjacencyMatrix) 
{
    int n = adjacencyMatrix.size();
    vector<Edge> edges;

    // Initialize edges based on adjacency matrix
    for (int i = 0; i < n; i++) 
    {
        for (int j = i + 1; j < n; j++) 
        {
            edges.push_back({i, j, adjacencyMatrix[i][j]});
        }
    }

    // Sort the edges by weight
    sort(edges.begin(), edges.end(), compareEdges);

    // Initialize disjoint-set data structure
    vector<int> parent(n);
    for (int i = 0; i < n; i++) 
    {
        parent[i] = i;
    }

    vector<Edge> mst;  // Minimum Spanning Tree

    // Iterate through sorted edges and add to MST if it doesn't create a cycle
    for (const Edge& edge : edges) 
    {
        int parentU = findParent(edge.u, parent);
        int parentV = findParent(edge.v, parent);

        if (parentU != parentV) 
        {
            mst.push_back(edge);
            parent[parentU] = parentV;  // Union the sets
        }

        if (mst.size() == n - 1) 
        {
            break;  // MST is complete
        }
    }
    int cost = 0;

    for (int i = 0; i < n; i++) 
    {
        for (int j = i + 1; j < n; j++) 
        {
            int weight = adjacencyMatrix[i][j];
            bool edgeInMST = false;

            // Check if the edge (i, j) is in the MST
            for (const Edge& edge : mst) 
            {
                if ((edge.u == i && edge.v == j) || (edge.u == j && edge.v == i)) 
                {
                    edgeInMST = true;
                    break;
                }
            }

            if (edgeInMST) 
            {
                cost += weight;  // Add positive weights of MST edges
            }
            else 
            {
                cost -= weight;  // Subtract negative weights of non-MST edges
            }
        }
    }
    return cost;
}

int main() 
{
    string input;

    getline(cin, input);  // Read the entire line as input

    // Tokenize the input string by spaces
    istringstream iss(input);
    vector<string> tokens;

    string token;
    while (iss >> token) 
    {
        tokens.push_back(token);
    }
    
    // Create 2D arrays for the first token with individual character conversion
    vector<vector<int>> arrays[3];

    istringstream strStream(tokens[0]);
    vector<int> row;
    string cell;

    while (getline(strStream, cell, ',')) 
    {
        for (char c : cell) 
        {
            int num = c - '0';  // Convert each character to an integer
            row.push_back(num);
        }
        arrays[0].push_back(row);
        row.clear();
    }

    // Create 2D arrays for the second and third tokens using charToInt
    for (int i = 1; i < 3; i++) 
    {
        istringstream strStream(tokens[i]);
        vector<int> row;
        string cell;

        while (getline(strStream, cell, ',')) 
        {
            for (char c : cell) 
            {
                int num = charToInt(c);  // Convert characters to integers using charToInt function
                row.push_back(num);
            }
            arrays[i].push_back(row);
            row.clear();
        }
    }

    // Create an adjacency matrix
    int n = arrays[0].size();
    vector<vector<int>> adjacencyMatrix(n, vector<int>(n, 0));

    // Populate the adjacency matrix
    for (int i = 0; i < n; i++) 
    {
        for (int j = 0; j < n; j++) 
        {
            if (arrays[0][i][j] == 0) 
            {
                // Value is 0 in arrays[0], so use arrays[1] 
                adjacencyMatrix[i][j] = arrays[1][i][j];
            } 
            else 
            {
                // Value is not 0 in arrays[0], so use arrays[2] with a negative sign
                adjacencyMatrix[i][j] = -arrays[2][i][j];
            }
        }
    }
    int cost = kruskalMST(adjacencyMatrix);

    // Print the cost
    cout << cost << endl;

    return 0;
}