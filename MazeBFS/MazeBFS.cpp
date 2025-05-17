#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <SFML/Graphics.hpp>

const int WIDTH = 900;
const int HEIGHT = 900;
const int GRIDISZE = 50;

void bfsMazeDraw(sf::RenderWindow& window, const std::vector<std::vector<int>>& maze, std::unordered_map<int, bool>& visited)
{
    const int rows = maze.size();
    const int cols = maze[0].size();
    const int padding = 20;

    window.clear();

    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            sf::RectangleShape rect(sf::Vector2f(GRIDISZE - 2, GRIDISZE - 2));
            rect.setPosition(sf::Vector2f(c * GRIDISZE + padding, r * GRIDISZE + padding));

            // Colors
            if (r == 0 && c == 0)
                rect.setFillColor(sf::Color::Green);  // Start
            else if (r == rows - 1 && c == cols - 1)
                rect.setFillColor(sf::Color::Red);    // End
            else
                rect.setFillColor(maze[r][c] ? sf::Color(245, 245, 245) : sf::Color(30, 30, 30));

            int node = r * cols + c;

            if (visited[node])
                rect.setFillColor(sf::Color::Blue);

            // Outline for visual grid
            rect.setOutlineThickness(1);
            rect.setOutlineColor(sf::Color(80, 80, 80));

            window.draw(rect);
        }
    }

    window.display();

}

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
    std::vector<int> bfs(sf::RenderWindow& window, std::vector<std::vector<int>> maze, int start, int end, int rows, int cols, std::unordered_map<int, bool>& visited)
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
                    bfsMazeDraw(window, maze, visited);
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

        return path;
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

void drawMazeInitial(sf::RenderWindow& window, const std::vector<std::vector<int>>& maze)
{
    const int rows = maze.size();
    const int cols = maze[0].size();
    const int padding = 20;

    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            sf::RectangleShape rect(sf::Vector2f(GRIDISZE - 2, GRIDISZE - 2));
            rect.setPosition(sf::Vector2f(c * GRIDISZE + padding, r * GRIDISZE + padding));

            // Colors
            if (r == 0 && c == 0)
                rect.setFillColor(sf::Color::Green);  // Start
            else if (r == rows - 1 && c == cols - 1)
                rect.setFillColor(sf::Color::Red);    // End
            else
                rect.setFillColor(maze[r][c] ? sf::Color(245, 245, 245) : sf::Color(30, 30, 30));

            // Outline for visual grid
            rect.setOutlineThickness(1);
            rect.setOutlineColor(sf::Color(80, 80, 80));

            window.draw(rect);
        }
    }
}

void drawMazeFinal(sf::RenderWindow& window, const std::vector<std::vector<int>>& maze, const std::vector<int>& path, std::unordered_map<int, bool>& visited)
{
    const int rows = maze.size();
    const int cols = maze[0].size();
    const int padding = 20;

    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            sf::RectangleShape rect(sf::Vector2f(GRIDISZE - 2, GRIDISZE - 2));
            rect.setPosition(sf::Vector2f(c * GRIDISZE + padding, r * GRIDISZE + padding));

            // Colors
            if (r == 0 && c == 0)
                rect.setFillColor(sf::Color::Green);  // Start
            else if (r == rows - 1 && c == cols - 1)
                rect.setFillColor(sf::Color::Red);    // End
            else
                rect.setFillColor(maze[r][c] ? sf::Color(245, 245, 245) : sf::Color(30, 30, 30));

            int node = r * cols + c;

            if (visited[node])
                rect.setFillColor(sf::Color::Blue);

            bool found = false;

            for (int i = 0; i < path.size() && !found; i++)
                if (path[i] == node)
                    found = true;

            if (found)
                rect.setFillColor(sf::Color::Green);

            // Outline for visual grid
            rect.setOutlineThickness(1);
            rect.setOutlineColor(sf::Color(80, 80, 80));

            window.draw(rect);
        }
    }
}

int main()
{
    std::vector<std::vector<int>> maze = {
    {1,1,1,0,1,0,1,1,1,1,0,1,1,1,0,1,1,1,1,1},
    {0,0,1,0,1,0,1,0,0,1,0,0,0,1,0,1,0,0,0,1},
    {1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,0,1},
    {1,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,1,0,1},
    {1,1,1,1,1,0,1,1,1,0,1,1,1,1,1,1,0,1,1,1},
    {1,0,0,0,1,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0},
    {1,1,1,0,1,1,1,0,1,1,1,1,1,1,0,1,1,1,1,1},
    {0,0,1,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,1},
    {1,1,1,1,1,0,1,1,1,1,1,1,0,1,1,1,1,1,0,1},
    {1,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,1,0,1},
    {1,0,1,1,1,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1},
    {1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1},
    {1,1,1,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,0,1},
    {1,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,1},
    {1,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,0,1,1,1},
    {1,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,1},
    {1,0,1,1,0,1,1,1,1,1,0,1,1,1,0,1,1,1,0,1},
    {1,0,1,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,1},
    {1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,0,1},
    {0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1}
    };


    int rows = maze.size();
    int cols = maze[0].size();

    Graph g;

    mazeToGraph(maze, g);

    std::unordered_map<int, bool> visited;
    std::vector<int> path;

    bool started = false;
    
    sf::RenderWindow window(sf::VideoMode(WIDTH + 200, HEIGHT + 200), "BFS MAZE SOLVER");
    window.setFramerateLimit(30);

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && !started)
        {
            path = g.bfs(window, maze, 0, rows * cols - 1, rows, cols, visited);
            started = true;
        }

        window.clear();
        if (!started)
            drawMazeInitial(window, maze);
        else
            drawMazeFinal(window, maze, path, visited);
        window.display();
    }
    


    return 0;
}