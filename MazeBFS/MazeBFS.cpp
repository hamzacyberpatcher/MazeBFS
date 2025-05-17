#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <SFML/Graphics.hpp>

class Graph
{
    std::unordered_map<int, std::vector<int>> adjList;
public:
    void addEdge(int u, int v, bool bidirection = false)
    {
        adjList[u].push_back(v);

        if (bidirection)
            adjList[v].push_back(u);
    }
    void displayList() const
    {
        for (auto i : adjList)
        {
            std::cout << i.first << " -> ";
            for (auto j : i.second)
                std::cout << j << ", ";
            std::cout << std::endl;
        }
    }
    std::vector<std::pair<int, int>> bfs(int start, int end, int rows, int cols, std::unordered_map<int, bool> visited)
    {
        std::unordered_map<int, int> parent;

        std::queue<int> q;
        q.push(start);
        visited[start] = true;
        parent[start] = -1;

        while (!q.empty())
        {
            int node = q.front();
            q.pop();

            if (node == end)
                break;

            for (auto neighbour : adjList[node])
            {
                if (!visited[neighbour])
                {
                    visited[neighbour] = true;
                    parent[neighbour] = node;
                    q.push(neighbour);
                }
            }
        }

        std::vector<int> path;
        int current = end;
        while (current != -1)
        {
            path.push_back(current);
            current = parent[current];
        }

        reverse(path.begin(), path.end());

        std::vector<std::pair<int, int>> pathCoords;

        for (auto node : path)
        {
            int r = node / cols;
            int c = node % cols;

            pathCoords.push_back({ r, c });
        }

        return pathCoords;
    }
};

void mazeToGraph(std::vector<std::vector<int>>& maze, Graph& g)
{
    int rows = maze.size();
    int cols = maze[0].size();

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {

            if (maze[i][j])
            {
                std::vector<std::pair<int, int>> dirs = { {-1,0},{1,0},{0,-1},{0,1} };

                int node = i * cols + j;

                for (auto pair : dirs)
                {
                    int nr = i + pair.first;
                    int nc = j + pair.second;

                    if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && maze[nr][nc])
                    {
                        int neighbour = nr * cols + nc;
                        g.addEdge(node, neighbour);
                    }
                }
            }
        }
    }
}


int main()
{
    std::vector<std::vector<int>> maze = {
    {1, 0, 1, 1, 1, 0, 1, 1, 1, 1},
    {1, 0, 1, 0, 1, 1, 1, 0, 0, 1},
    {1, 1, 1, 0, 0, 0, 1, 0, 1, 1},
    {0, 0, 1, 1, 1, 0, 1, 0, 1, 0},
    {1, 1, 0, 0, 1, 0, 1, 1, 1, 1},
    {1, 0, 0, 1, 1, 1, 0, 0, 0, 1},
    {1, 0, 1, 1, 0, 0, 0, 1, 0, 1},
    {1, 1, 1, 0, 0, 1, 1, 1, 0, 1},
    {0, 0, 1, 1, 1, 0, 0, 1, 1, 1},
    {1, 1, 1, 0, 1, 1, 1, 0, 0, 1}
    };

    int rows = maze.size();
    int cols = maze[0].size();

    Graph g;

    mazeToGraph(maze, g);

    std::unordered_map<int, bool> visited;
    std::vector<std::pair<int, int>> pathCoords;
    pathCoords = g.bfs(0, rows * cols - 1, rows, cols, visited);

    
    sf::RenderWindow window(sf::VideoMode(1600, 900), "BFS MAZE SOLVER");

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        window.display();
    }
    


    return 0;
}