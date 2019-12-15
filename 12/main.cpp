/*
Задача 12 (5 баллов)
Мосты.
Ребро неориентированного графа называется мостом, если удаление этого ребра из графа увеличивает число компонент связности.
Дан неориентированный граф, требуется найти в нем все мосты.

Ввод: Первая строка входного файла содержит два целых числа n и m — количество вершин и ребер графа соответственно 
(1 ≤ n ≤ 20000, 1 ≤ m ≤ 200000). Следующие m строк содержат описание ребер по одному на строке. Ребро номер i описывается 
двумя натуральными числами bi, ei — номерами концов ребра (1 ≤ bi, ei ≤ n).
Вывод: Первая строка выходного файла должна содержать одно натуральное число b — количество мостов в заданном графе. 
На следующей строке выведите b целых чисел — номера ребер, которые являются мостами, в возрастающем порядке. 
Ребра нумеруются с единицы в том порядке, в котором они заданы во входном файле.
*/

#include <iostream>
#include <algorithm>
#include <map>
#include <set>
#include <assert.h>
#include <vector>
#include <fstream>
#define NIL -1

using std::vector;
using std::cout;
using std::map;
using std::pair;
using std::set;


class Graph {
public:
    Graph(int V): V(V) { adj.assign(V, {}); };
    void add_edge( int v, int w );
    vector<int> get_bridges();
    
private:
    const int V;
    int cnt = 0;
    vector< vector<int> > adj;
    map< pair<int,int>, int > edges_map;
    set< pair<int,int> > multiple_edges;
    void get_bridges_util( int v, vector<bool>& visited, vector<int>& disc, vector<int>& low, vector<int>& parent, vector<int>& bridges );
};

void Graph::add_edge(int v, int w) {
    if ( edges_map.count({v, w}) >= 1 ) {
        multiple_edges.insert({v, w});
        multiple_edges.insert({w, v});
        ++cnt;
        return;
    }
    adj[v].push_back(w);
    adj[w].push_back(v);
    edges_map[{v,w}] = cnt;
    edges_map[{w,v}] = cnt;
    ++cnt;
}

void Graph::get_bridges_util(int u, vector<bool>& visited, vector<int>& disc,
                                vector<int>& low, vector<int>& parent, vector<int>& bridges ) {
    static int time = 0;
    visited[u] = true;
    disc[u] = low[u] = ++time;
    for ( auto v: adj[u] ) {
        if (!visited[v]) {
            parent[v] = u;
            get_bridges_util( v, visited, disc, low, parent, bridges );
            low[u] = std::min(low[u], low[v]);
            if (low[v] > disc[u]) {
                if ( multiple_edges.count( {u, v} ) == 0 ) {
                    int idx = edges_map[{u, v}];
                    bridges.push_back(idx);
                }
            }
        }

        else if (v != parent[u])
            low[u] = std::min(low[u], disc[v]);
    }
}

vector<int> Graph::get_bridges() {
    vector<bool> visited(V, false);
    vector<int> disc(V);
    vector<int> low(V);
    vector<int> parent(V, NIL);
    vector<int> bridges;

    for ( int i = 0; i < V; ++i ) {
        if (visited[i] == false)
            get_bridges_util(i, visited, disc, low, parent, bridges);
    }
    sort( bridges.begin(), bridges.end() );
    return bridges;
}

int main() {
    std::ofstream fout;
    fout.open("bridges.out", std::ios::out);

    std::ifstream fin;
    fin.open("bridges.in", std::ios::in);

    int n, m;
    fin >> n >> m;
    Graph g(n);
    for ( int i = 0; i < m; ++i ) {
        int v, w;
        fin >> v >> w;
        g.add_edge(v-1, w-1);
    }
    vector<int>&& bridges = g.get_bridges();
    fout << bridges.size() << std::endl;
    for ( auto& it: bridges ) {
        fout << it + 1 << ' ';
    }
    fout << std::endl;
    fout.close();
    fin.close();

    return 0;
}


