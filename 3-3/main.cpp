/*
3_3. Количество инверсий.

Дана последовательность целых чисел из диапазона (-10^9 .. 10^9). Длина последовательности не больше 10^6. 
Числа записаны по одному в строке. Количество чисел не указано.  
Пусть количество элементов n, и числа записаны в массиве a = a[i]: i из [0..n-1].
Требуется напечатать количество таких пар индексов (i,j) из [0..n-1], что (i < j и a[i] > a[j]).
Указание: количество инверсий может быть больше 4*10^9 - используйте int64_t.
*/


#include <cassert>
#include <iostream>
#include <string>
#include <memory.h>
#include <stdint.h>

using namespace std;


template<class T, class TLess>
int64_t Merge(T* a, int firstLen, int secondLen, T* c, int64_t inversions, TLess less) {
    int idx_first = 0;
    int idx_second = firstLen;
    for (int idx_c = 0; idx_c < (firstLen+secondLen); idx_c++) {
        if ( (less(a[idx_first], a[idx_second]) || idx_second > firstLen+secondLen-1) && idx_first < firstLen ) {
            c[idx_c] = a[idx_first++];
        }
        else {
            c[idx_c] = a[idx_second++];
            if (idx_first < firstLen) {
                inversions += firstLen - idx_first;
            }
        }
    }
    return inversions;
}
template<class T, class TLess>
int64_t merge_sort(T* a, int aLen, int64_t inversions, TLess less) {
    if ( aLen <= 1 ) { return inversions; }
    int firstLen = aLen / 2 + aLen % 2;
    int secondLen = aLen - firstLen;
    inversions = merge_sort( a, firstLen , inversions, less );
    inversions = merge_sort( a + firstLen, secondLen, inversions, less );
    T* c = new T[aLen];
    inversions = Merge( a, firstLen, secondLen, c, inversions, less);
    memcpy( a, c, sizeof( int ) * aLen );
    delete[] c;
    return inversions;
}

bool compare_arrays(int* a, int* b, int len) {
    for (int i = 0; i < len; i++) {
        if (a[i] != b[i]) { delete[] b; return false; }
    }
    delete[] b;
    
    return true;
}

class MyArray {
    int size = 0;
    int memory_size = 2;
public:
    int* array = new int[memory_size];
    void add_element(int element) {
        array[size++] = element;
        if (size == memory_size) { add_memory(); }
    }
    int get_size() const { return size; }
private:
    void copy_arr_into_new_arr(int* new_arr) {
        for (int i = 0; i < size; i++) { new_arr[i] = array[i]; }
    }
    void add_memory() {
        int* new_arr = new int[memory_size*2];
        copy_arr_into_new_arr(new_arr);
        delete[] array;
        array = new_arr;
        memory_size *= 2;
        return;
    }
};
 
int main() {
    MyArray array_for_sort;
    string line;
    getline(cin, line);
    while (!line.empty()) {
        array_for_sort.add_element(stoi(line));
        getline(cin, line);
    }
    int64_t inversions = merge_sort(array_for_sort.array,
                                    array_for_sort.get_size(),
                                    0,
                                    [](int a, int b) { return a <= b; });
    cout << inversions << endl;

    return 0;
}
