11_2. Количество различных путей
Дан невзвешенный неориентированный граф. В графе может быть несколько кратчайших путей между какими-то вершинами. 
Найдите количество различных кратчайших путей между заданными вершинами. Требуемая сложность O(V+E).
Ввод: v: кол-во вершин (макс. 50000), n: кол-во ребер (макс. 200000), n пар реберных вершин, пара вершин v, w для запроса.
Вывод: количество кратчайших путей от v к w.

ссылка на Яндекс.Контест https://contest.yandex.ru/contest/15957/run-report/27592154/

#include <iostream>
#include <algorithm>
#include <queue>
#include <unordered_map>
#include <assert.h>
#include <list>
#include <vector>
#define INT_MAX 2147483647

using std::list;
using std::vector;
using std::queue;
using std::cout;

class Graph {
public:
    Graph(int v): v(v) { adj.assign(v, {}); };
    void BFS( vector<vector <int> > adj, int src, int dist[], int* paths, int n );
    int find_shortest_paths( int s, int n, int w );
    void add_edge( int u, int v );
private:
    const int v;
    vector<vector <int> > adj;
};

void Graph::BFS( vector<vector <int> > adj, int src, int dist[], int* paths, int n ) {
    bool* visited = new bool[n];
    for (int i = 0; i < n; i++)
        visited[i] = false;
    dist[src] = 0;
    paths[src] = 1;
  
    queue <int> q;
    q.push(src);
    visited[src] = true;
    while (!q.empty()) {
        int curr = q.front();
        q.pop();
  
        for (auto x : adj[curr]) {
            if (visited[x] == false) {
                q.push(x);
                visited[x] = true;
            }
            if (dist[x] > dist[curr] + 1) {
                dist[x] = dist[curr] + 1;
                paths[x] = paths[curr];
            }
            else if (dist[x] == dist[curr] + 1)
                paths[x] += paths[curr];
        }
    }
    delete[] visited;
}
  
int Graph::find_shortest_paths( int s, int n, int w ) {
    int dist[n];
    int* paths = new int[n];
    for (int i = 0; i < n; i++)
        dist[i] = INT_MAX;
    for (int i = 0; i < n; i++)
        paths[i] = 0;
    BFS(adj, s, dist, paths, n);
    int result = paths[w];
    delete[] paths;
    return result;
}
  
void Graph::add_edge( int u, int v ) {
    adj[u].push_back(v);
    adj[v].push_back(u);
}

int main() {
    int v_, n;
    std::cin >> v_ >> n;
    Graph g(v_);
    for ( int i = 0; i < n; ++i ) {
        int u, v;
        std::cin >> u >> v;
        g.add_edge( u, v );
    }
    int v, w;
    std::cin >> v >> w;
    int result = g.find_shortest_paths( v, v_, w );
      cout << result << std::endl;
  
    return 0;
}



