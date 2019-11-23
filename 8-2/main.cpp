/*
Задача 8. Хеш-таблица (10 баллов)
Реализуйте структуру данных типа “множество строк” на основе динамической хеш-таблицы с открытой адресацией. 
Хранимые строки непустые и состоят из строчных латинских букв.
Хеш-функция строки должна быть реализована с помощью вычисления значения многочлена методом Горнера.
Начальный размер таблицы должен быть равным 8-ми. Перехеширование выполняйте при добавлении элементов в случае, 
когда коэффициент заполнения таблицы достигает 3/4.
Структура данных должна поддерживать операции добавления строки в множество, удаления строки из множества и проверки 
принадлежности данной строки множеству.

1_2. Для разрешения коллизий используйте двойное хеширование.

Формат входных данных
Каждая строка входных данных задает одну операцию над множеством. Запись операции состоит из типа операции и следующей за 
ним через пробел строки, над которой проводится операция.
Тип операции  – один из трех символов:
    +  означает добавление данной строки в множество; 
    -  означает удаление  строки из множества;  
    ?  означает проверку принадлежности данной строки множеству. 
При добавлении элемента в множество НЕ ГАРАНТИРУЕТСЯ, что он отсутствует в этом множестве. При удалении элемента из множества 
НЕ ГАРАНТИРУЕТСЯ, что он присутствует в этом множестве.
Формат выходных данных
Программа должна вывести для каждой операции одну из двух строк OK или FAIL, в зависимости от того, встречается ли данное 
слово в нашем множестве.

*/

#include <cassert>
#include <iostream>
#include <string>
#include <memory.h>
#include <stdint.h>
#include <vector>
#include <queue>
#include <ctime>

#include <assert.h>

using std::cin;
using std::cout;
using std::vector;
using std::string;
using std::overflow_error;
using std::endl;

struct GetHash {
    size_t operator() ( const string& key, size_t m ) const {
        int a = 3571;
        size_t hash_ = 0;
        for (int i = 0; i < key.length(); i++) {
            hash_ = ( hash_ * a + key[i] ) % m;
        }
        return hash_;
    }
};

template<class T, class H>
class HashTable {
public:
    explicit HashTable(size_t initial_size, H get_hash);
    bool has(const T& key) const;
    bool add(const T& key);
    bool remove(const T& key);

private:
    const T DELETED_KEY = "Deleted";
    const T EMPTY = "";
    H get_hash;
    bool is_Nil( const T& key ) { return key != DELETED_KEY ? key.empty() : true; }
    size_t probe( size_t hash, int i, const T& key ) const;
    vector<T> table;
    size_t limit;
    void rehash();
};

template<class T, class H>
HashTable<T, H>::HashTable(size_t initial_size, H get_hash) : table(initial_size, EMPTY), limit(initial_size) {}

template<class T, class H>
void HashTable<T, H>::rehash() {
    vector<T> temp_table( table.size()*2, EMPTY );
    table.swap( temp_table );
    limit = table.size() * 3 / 4;
    for( int i = 0; i < temp_table.size(); i++ ) {
        if ( !is_Nil( temp_table[i] ) ) {
            add( temp_table[i] );
        }
    }
}

template<class T, class H>
size_t HashTable<T, H>::probe(size_t hash, int i, const T& key ) const {
    int sum_of_letters = 0;
    if ( i == 0 ) { return hash; } //если i==0, то не будет лишний раз высчитывать h2
    for ( int i = 0; i < key.length(); i++ ) {
        sum_of_letters += key[i];
    }
    return (hash + i * ( 2*sum_of_letters + 1 ) ) % table.size();
}

template<class T, class H>
bool HashTable<T, H>::has(const T& key) const {
    assert(!key.empty());
    const size_t hash = get_hash(key, table.size());
    for ( int i = 0; i < table.size(); ++i ) {
        size_t idx = probe( hash, i, key );
        if ( table[idx].empty() ) {
            return false;
        }
        if ( table[idx] == key ) {
            return true;
        }
    }
    throw overflow_error( " no more space for a key " );
}

// Вставка ключа в хеш-таблицу (без учета удаленных элементов).
template<class T, class H>
bool HashTable<T, H>::add(const T& key) {
    assert(!key.empty());
    const size_t hash = get_hash(key, table.size());
    for ( int i = 0; i < table.size(); ++i ) {
        size_t idx = probe( hash, i, key );
        if ( table[idx] == key ) { return false; }
        if ( is_Nil( table[idx] ) ) {
            if ( table[idx] != DELETED_KEY ) limit--;
            table[idx] = key;
            if ( limit == 0 ) rehash();
            return true;
        }
    }
    throw overflow_error( " no more space for a key " );
}

template<class T, class H>
bool HashTable<T, H>::remove(const T& key) {
    assert(!key.empty());
    const size_t hash = get_hash(key, table.size());
    for ( int i = 0; i < table.size(); ++i ) {
        size_t idx = probe( hash, i, key );
        if ( table[idx] == key ) {
            table[idx] = DELETED_KEY;
            return true;
        }
        if ( table[idx].empty() ) {
            return false;
        }
    }
    throw overflow_error( " no more space for a key " );
}

int main() {
    GetHash get_hash;
    HashTable<string, GetHash> table( 8, get_hash );
    char command = ' ';
    string key;

    while (cin >> command >> key) {
        switch (command) {
          case '?':
            cout << (table.has(key) ? "OK" : "FAIL") << endl;
            break;
          case '+':
            cout << (table.add(key) ? "OK" : "FAIL") << endl;
            break;
          case '-':
            cout << (table.remove(key) ? "OK" : "FAIL") << endl;
            break;
        }
    }
    return 0;
}

/*
 
 + hello
 + bye
 ? bye
 + bye
 - bye
 ? bye
 ? hello
*/
