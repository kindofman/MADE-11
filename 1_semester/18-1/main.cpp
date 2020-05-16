/*
Задача 18. Построение выпуклой оболочки (5 баллов)
Дано множество точек на плоскости (x, y). Постройте выпуклую оболочку этого множества и вычислите ее периметр.
Вариант 1. С помощью алгоритма Грэхема.
Обратите внимание: три и более точки из множества могут лежать на одной прямой.
*/

#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>
#include <math.h>
#define INT_MAX 2147483647

using std::endl;
using std::cout;
using std::vector;
using std::cin;
using std::sort;

const double EPSILON = 0.000001;
const double PI = 3.14159265359;

struct Point {
    double x;
    double y;
    double angle;
};

void print_points( const vector<Point> vec ) {
    cout << "start printing points" << endl;
    for ( auto& element: vec ) {
        cout << element.x << ' ' << element.y << ' ' << element.angle << endl;
    }
}


struct Less {
    bool operator() ( const Point& left, const Point& right ) {
        return left.angle <= right.angle;
    }
};

Point extract_min( vector<Point>& dots ) {
    Point min_point({0, INT_MAX});
    int idx = -1;
    for ( int i = 0; i < dots.size(); ++i ) {
        if ( dots[i].y < min_point.y ) {
            min_point = dots[i];
            idx = i;
        } else if ( dots[i].y == min_point.y && dots[i].x < min_point.x ) {
            min_point = dots[i];
            idx = i;
        }
    }
    dots.erase( dots.begin() + idx );
    return min_point;
}

void calc_angles( vector<Point>& dots, Point min_point ) {
    for ( auto& p: dots ) {
        p.angle = atan2( p.y - min_point.y, p.x - min_point.x );
    }
}

bool condition( Point dot, Point p ) {
    if ( dot.angle < -2*EPSILON ) { dot.angle += 2*PI; }
    if ( p.angle < -2*EPSILON ) { p.angle += 2*PI; }
    if ( (dot.angle < p.angle + EPSILON) && ( dot.angle > p.angle - EPSILON ) ) {
        if ( dot.y < p.y ) {
            return true;
        }
    
    } else if ( abs( dot.angle - p.angle ) < PI + EPSILON && ( abs( dot.angle - p.angle ) > PI - EPSILON  ) ) {
        if ( dot.y > p.y ) {
            return true;
        }
    } else if ( dot.angle < p.angle ) {
        return true;
    }
    return false;
};

bool not_collinear( Point dot, Point p ) {
    if ( dot.angle < 0 ) { dot.angle += 2*PI; }
    if ( p.angle < 0 ) { p.angle += 2*PI; }
    if ( ( dot.angle < p.angle + EPSILON ) && ( dot.angle > p.angle - EPSILON ) ) {
        if ( dot.y < p.y ) {
            return false;
        }
    } if ( abs( dot.angle - p.angle ) < PI + EPSILON && ( abs( dot.angle - p.angle ) > PI - EPSILON  ) ) {
        if ( dot.y < p.y ) {
            return false;
        }
    }
    return true;
}

vector<Point> get_convex_hull( vector<Point>& dots ) {
    vector<Point> stack;
    stack.push_back( extract_min( dots ) );
    calc_angles( dots, stack[0] );
    stack[0].angle = -2*EPSILON;
    sort( dots.begin(), dots.end(), Less() );
    for ( int i = 0; i < dots.size(); ++i ) {
        dots[i].angle = atan2( dots[i].y - stack.back().y, dots[i].x - stack.back().x );
        while( condition( dots[i], stack.back() ) ) {
            stack.pop_back();
            dots[i].angle = atan2( dots[i].y - stack.back().y, dots[i].x - stack.back().x );
        }
        if ( not_collinear( dots[i], stack.back() ) ) {
            stack.push_back( dots[i] );
        }
    }
    return stack;
}

double get_perimeter( const vector<Point>& convex_points ) {
    double result = 0;
    Point prev = convex_points.back();
    for ( const auto& p: convex_points ) {
        result += sqrt( (p.x - prev.x) * (p.x - prev.x) + (p.y - prev.y) * (p.y - prev.y) );
        prev = p;
    }
    return result;
}


int main() {
    int n;
    cin >> n;
    vector<Point> dots;
    for ( int i = 0; i < n; ++i ) {
        double x, y;
        cin >> x >> y;
        dots.push_back( {x, y} );
    }
    vector<Point> convex_points = get_convex_hull( dots );
    cout.precision(9);
    cout << get_perimeter( convex_points ) << endl;
    return 0;
}

