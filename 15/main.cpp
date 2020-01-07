/*
Задача 15 (5 баллов)
«Приближенное решение метрической неориентированной задачи коммивояжера». Найдите приближенное решение метрической 
неориентированной задачи коммивояжера в полном графе (на плоскости) с помощью минимального остовного дерева.
Оцените качество приближения на случайном наборе точек, нормально распределенном на плоскости с дисперсией 1. 
Нормально распределенный набор точек получайте с помощью преобразования Бокса-Мюллера.
При фиксированном N, количестве вершин графа, несколько раз запустите оценку качества приближения. Вычислите среднее значение
и среднеквадратичное отклонение качества приближения для данного N.
Запустите данный эксперимент для всех N в некотором диапазоне, например, [2, 10].
Автоматизируйте запуск экспериментов.
В решении требуется разумно разделить код на файлы. Каждому классу - свой заголовочный файл и файл с реализацией.
*/


#include <iostream>
#include <fstream>
#include <algorithm>
#include <queue>
#include <unordered_map>
#include <assert.h>
#include <list>
#include <vector>
#include <cmath>

using std::vector;
using std::queue;
using std::cout;
using std::pair;

struct Point {
    Point(double x, double y): x(x), y(y) {};
    const double x;
    const double y;
};

struct QuasiNode {
    int vertex;
    int parent;
    double dist;
};

class Graph {
public:
    Graph( vector<Point> vertices ): vertices( vertices ) { adj.assign(vertices.size(), {}); };
    void add_edge( int u, int v );
    double MST();
    void self_test() const;
    void __get_optimal_path( int num, vector<bool> visited, int start, double result, int prev );
    double get_optimal_path( int num );
    double main_DFS();

private:
    const vector<Point> vertices;
    vector< vector<int> > adj;
    double get_distance( int v1, int v2 ) const;
    void one_test( vector<Point> vertices, int dist ) const;
    void DFS( vector<bool>& visited, double& dist, int prev );
    double min_distance = INT_MAX;
    int last_vertex = -1;
};

double Graph::get_distance( int v1, int v2 ) const {
    Point point1 = vertices[v1];
    Point point2 = vertices[v2];
    int x = point1.x - point2.x;
    int y = point1.y - point2.y;
//    cout << "x: " << x << ", y: " << y << ", dist: " << sqrt( x*x + y*y ) << std::endl;
    return sqrt( x*x + y*y );
}

void Graph::add_edge( int u, int v ) {
    adj[u].push_back( v );
    adj[v].push_back( u );
}

struct Less {
    const bool operator()( QuasiNode& left, QuasiNode& right ) const {
        return left.dist > right.dist;
    };
};

double Graph::MST () {
    if ( vertices.size() == 0 ) { return 0; }
    double result = 0;
    std::priority_queue< QuasiNode, vector< QuasiNode >, Less> positions_queue;
    vector<bool> visited( vertices.size(), false );
//Начинаем обход с вершины 0
    positions_queue.push( { 0, 0 } );
    while (!positions_queue.empty()) {
        QuasiNode current = positions_queue.top();
        positions_queue.pop();
        if ( visited[current.vertex] ) { continue; }
        visited[current.vertex] = true;
        add_edge( current.vertex, current.parent );
        result += current.dist;
        for ( int sibling = 0; sibling < vertices.size(); ++sibling ) {
            if ( visited[sibling] ) { continue; }
            double dist = get_distance( current.vertex, sibling );
            positions_queue.push( {sibling, current.vertex, dist} );
        }
    }
    return result;
}

void Graph::one_test( vector<Point> vertices, int dist ) const {
    Graph g( vertices );
    assert( g.MST() == dist );
}

void Graph::self_test() const {
    vector<Point> vertices{{1,2}, {2,4}, {2,6}, {2,2}, {4,3}};
    one_test(vertices, 14);
    vertices = vector<Point>{ {-2, -3}, {-2, 3}, {2, 3}, {2, -3}, {-4, 0}, {7, 0} };
    one_test(vertices, 92);
    vertices = vector<Point> { {0,17}, {27,17}, {12,11}, {10,5}, {13,5}, {15,8}, {9,2}, {15,1}, {19,7}, {30,0} };
    one_test(vertices, 601);
}

// int cnt = 0;
void Graph::__get_optimal_path( int num, vector<bool> visited, int start, double result, int prev ) {
    bool indicator = true;
    for ( int i = 0; i < num; ++i ) {
        if ( visited[i] ) { continue; }
        int curr = i;
        indicator = false;
        visited[i] = true;
        __get_optimal_path( num, visited, start, result + get_distance( prev, curr ), curr );
        visited[i] = false;
    }
    if ( indicator ) {
//         cnt++;
        result += get_distance( start, prev );
        if ( result < min_distance ) { min_distance = result; }
        return;
    }
}

double Graph::get_optimal_path( int num ) {
    double result = 0;
    int start = 0;
    vector<bool> visited( num, false );
    visited[start] = true;
    __get_optimal_path( num, visited, start, result, 0 );
    return min_distance;
}

double Graph::main_DFS() {
    vector<bool> visited( adj.size(), false );
    int start = 0;
    last_vertex = start;
    double dist = 0;
    visited[start] = true;
    DFS( visited, dist, start );
    dist += get_distance( start, last_vertex );
    return dist;
}

void Graph::DFS( vector<bool>& visited, double& dist, int prev ) {
    for ( int curr: adj[prev] ) {
        if ( visited[curr] ) { continue; }
        visited[curr] = true;
        dist += get_distance( last_vertex, curr );
        last_vertex = curr;
        DFS( visited, dist, curr );
    }
}

double generate_gaussian_noise(double mu, double sigma) {

    static const double epsilon = std::numeric_limits<double>::min();
    static const double two_pi = 2.0*3.14159265358979323846;

    thread_local double z1;
    thread_local bool generate;
    generate = !generate;

    if (!generate) {
       return z1 * sigma + mu;
    }

    double u1, u2;
    do {
       u1 = rand() * (1.0 / RAND_MAX);
       u2 = rand() * (1.0 / RAND_MAX);
     }
    while ( u1 <= epsilon );

    double z0;
    z0 = sqrt(-2.0 * log(u1)) * cos(two_pi * u2);
    z1 = sqrt(-2.0 * log(u1)) * sin(two_pi * u2);
    return z0 * sigma + mu;
}

vector<Point> generate_points( double mu, double sigma, int size ) {
    vector<Point> result;
    for ( int i = 0; i < size; ++i ) {
        Point point( generate_gaussian_noise( mu, sigma ), generate_gaussian_noise( mu, sigma ) );
        result.push_back( point );
    }
    return result;
}

void make_experiment( int N_start, int N_end, int n_each ) {
    std::ofstream myfile;
    myfile.open ("/Users/User/Desktop/example.csv");
    myfile << "n,absolute opt,absolute,mean,min,max,median,percentile(98)\n";
    for ( int N = N_start; N <= N_end; ++ N ) {
        double sum = 0;
        double sum_opt = 0;
        double squares = 0;
        double squares_opt = 0;
        double min = 2;
        double max = -1;
        vector<double> values;
        for ( int i = 0; i < n_each; ++i ) {
            vector<Point> points = generate_points( 0 , 1, N );
            Graph g( points );
            g.MST();
            double current = g.main_DFS();
            sum += current;
            squares += ( current * current );
            auto start = std::chrono::high_resolution_clock::now();
            double current_opt = g.get_optimal_path( N );
            auto finish = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = finish - start;
//            std::cout << "Elapsed time: " << elapsed.count() << " s\n";
            sum_opt += current_opt;
            squares_opt += ( current_opt * current_opt );
            if ( (current_opt / current < min) && (current_opt / current > 0 ) ) { min = current_opt / current; }
            if ( current_opt / current > max ) { max = current_opt / current; }
            values.push_back( current_opt / current );


        }
        double mean = sum / n_each;
        //Вначале сигма будет дисперсией
        double sigma = ( squares - 2 * mean * sum + n_each * mean * mean ) / n_each;
        sigma = sqrt( sigma );
        cout << "        N = " << N << "         " << std::endl;
//        cout << "mean = " << mean << ", sigma = " << sigma << std::endl;
        //t квантиль для n_each = 100 будет 1.6602
//        cout << "Доверительный интервал для приближенного решения\n" << mean << " ± " << 1.6602 * sigma / sqrt(n_each) << std::endl;

        double mean_opt = sum_opt / n_each;
        double sigma_opt = ( squares_opt - 2 * mean_opt + n_each * mean_opt * mean_opt ) / n_each;
        sigma_opt = sqrt( sigma_opt );
//        cout << "mean_opt = " << mean_opt << ", sigma_opt = " << sigma_opt << std::endl;
//        cout << "Доверительный интервал для точного решения\n" << mean_opt << " ± " << 1.6602 * sigma_opt / sqrt(n_each) << std::endl;
        cout << std::endl;
        sort( values.begin(), values.end() );
        double median = values[values.size() / 2];
        double percentile = values[n_each * 0.98];
        myfile << N << ',' << mean_opt << ',' << mean << ',' << mean_opt/mean << ',' << min << ',' << max << ',' << median << ',' << percentile << '\n';

    }
    myfile.close();
}

int main() {
//    make_experiment( 2, 10, 100 );
    cout << "Вывод: \n1)В среднем результат приближенного решения удовлетворителен для всех рассматриваемых n. Меньше, чем 0.65 отношение оптимального и приближенного решения не опускалось. Однако есть тенденция. С увеличением n приближенное решение становится монотонно хуже. Необходимо дальнейшее исследование.\n\n2)Если в среднем приближенное решение показывает себя хорошо, то в худшем случае результат значительно хуже. Для n = 10 соотношение равно 0.2\n\n";
    return 0;
}

