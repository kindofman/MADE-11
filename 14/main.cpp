/*
14. «MST». Дан неориентированный связный граф. Требуется найти вес минимального остовного дерева в этом графе.
Вариант 1. С помощью алгоритма Прима.
*/

#include <iostream>
#include <algorithm>
#include <queue>
#include <unordered_map>
#include <assert.h>
#include <list>
#include <vector>
#define INT_MAX 2147483647

using std::vector;
using std::queue;
using std::cout;
using std::pair;
class Graph {
public:
    Graph(int v): v(v) { adj.assign(v, {}); };
    void add_edge( int u, int v, int w );
    int MST() const;
private:
    const int v;
    vector<vector <pair<int, int> > > adj;
};
void Graph::add_edge( int u, int v, int w ) {
    adj[u-1].push_back( {v-1, w} );
    adj[v-1].push_back( {u-1, w} );
}
struct Less {
    const bool operator()( pair <int, int>& left, pair <int, int>& right ) const {
        return left.second > right.second;
    };
};
int Graph::MST () const {
    if ( adj.size() == 0 ) { return 0; }
    int result = 0;
    std::priority_queue< pair <int, int>, vector< pair<int,int> >, Less> positions_queue;
    vector<bool> visited( adj.size(), false );
//Начинаем обход с вершины 0
    positions_queue.push( { 0, 0 } );
    while (!positions_queue.empty()) {
        pair<int, int> current = positions_queue.top();
        positions_queue.pop();
        if ( visited[current.first] ) { continue; }
        visited[current.first] = true;
        result += current.second;
        for (auto sibling : adj[current.first]) {
            if ( !visited[sibling.first] ) {
                positions_queue.push( sibling );
            }
        }
    }
    return result;
}

int main() {
    int v_, n;
    std::cin >> v_ >> n;
    Graph g(v_);
    for ( int i = 0; i < n; ++i ) {
        int u, v, w;
        std::cin >> u >> v >> w;
        g.add_edge( u, v, w );
    }
    cout << g.MST() << std::endl;
    return 0;
}

