/*
Задача 16. Поиск подстроки (5 баллов)
Найдите все вхождения шаблона в строку. Длина шаблона – p, длина строки ­– n. Время O(n + p), доп. память – O(p).
Вариант 1. С помощью префикс-функции (алгоритм Кнута-Морриса-Пратта).
*/

#include <string>
#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;

void print_indices( const vector<int>& indices ) {
    for ( auto i: indices ) {
        cout << i << ' ';
    }
    cout << endl;
}

vector<int> prefix_function ( const string& s ) {
    int n = (int) s.length();
    vector<int> pi (n);
    pi[0] = 0;
    for ( int i = 1; i < n; ++i ) {
        int j = pi[i-1];
        while ( j > 0 && s[i] != s[j] ) {
            j = pi[j-1];
        }
        if ( s[i] == s[j] ) { ++j; }
        pi[i] = j;
    }
    return pi;
}

vector<int> prefix_function_mod( const string& templ, const string& s, vector<int> pi_templ ) {
    int n = (int) s.length();
    vector<int> pi (n+1);
    int prev = 0;
    for ( int i = 0; i < n; ++i ) {
        int j = prev;
        while ( j > 0 && s[i] != templ[j] ) {
            j = pi_templ[j-1];
        }
        if ( s[i] == templ[j] ) { ++j; }
        pi[i] = j;
        prev = j;
    }
    return pi;
}


vector<int> count_substring( const string& templ, const string& str ) {
    vector<int> result;
    vector<int> prefixes_templ = prefix_function( templ );
    vector<int> prefixes = prefix_function_mod( templ, str, prefixes_templ );
    int size_templ = (int) templ.size();
    for ( int i = 0; i < prefixes.size(); ++i ) {
        if ( prefixes[i] == size_templ ) { result.push_back( i - size_templ + 1 ); }
    }
    return result;
}


int main() {
    string templ;
    string str;
    std::getline( cin, templ );
    std::getline( cin, str );
    vector<int> indices = count_substring( templ, str );
    print_indices( indices );
    return 0;
}


