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

int64_t inversions = 0;

template<class T>
void Merge(T* a, int firstLen, int secondLen, T* c) {
    int idx_first = 0;
    int idx_second = firstLen;
    for (int idx_c = 0; idx_c < (firstLen+secondLen); idx_c++) {
        if ( (a[idx_first] <= a[idx_second] || idx_second > firstLen+secondLen-1) && idx_first < firstLen ) {
            c[idx_c] = a[idx_first++];
        }
        else {
            c[idx_c] = a[idx_second++];
            if (idx_first < firstLen) {
                inversions += firstLen - idx_first;
            }
        }
    }
}
template<class T>
void merge_sort(T* a, int aLen) {
    if ( aLen <= 1 ) { return; }
    int firstLen = aLen / 2 + aLen % 2;
    int secondLen = aLen - firstLen;
    merge_sort( a, firstLen );
    merge_sort( a + firstLen, secondLen );
    T* c = new T[aLen];
    Merge( a, firstLen, secondLen, c );
    memcpy( a, c, sizeof( int ) * aLen );
    delete[] c;
}

bool compare_arrays(int* a, int* b, int len) {
    for (int i = 0; i < len; i++) {
        if (a[i] != b[i]) { delete[] b; return false; }
    }
    delete[] b;
    
    return true;
}

void test_merge() {
    int* a = new int[2]{1, 2};
    int* c = new int[2];
    Merge(a, 1, 1, c);
    assert( compare_arrays(c, new int[2]{1, 2}, 2) );
    delete[] a, c;
    
    a = new int[2]{2, 1};
    c = new int[2];
    Merge(a, 1, 1, c);
    assert( compare_arrays(c, new int[2]{1, 2}, 2) );
    delete[] a, c;
    
    a = new int[3]{1,2,3};
    c = new int[3];
    Merge(a, 2, 1, c);
    assert( compare_arrays(c, new int[3]{1, 2, 3}, 3) );
    delete[] a, c;
    
    a = new int[3]{1,3,2};
    c = new int[3];
    Merge(a, 2, 1, c);
    assert( compare_arrays(c, new int[3]{1, 2, 3}, 3) );
    delete[] a, c;
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

//Ниже тот же самый класс для проверки сортировки со значениями типа string

//class MyArray {
//    int size = 0;
//    int memory_size = 2;
//public:
//    string* array = new string[memory_size];
//    void add_element(string element) {
//        array[size++] = element;
//        if (size == memory_size) { add_memory(); }
//    }
//    int get_size() const { return size; }
//private:
//    void copy_arr_into_new_arr(string* new_arr) {
//        for (int i = 0; i < size; i++) { new_arr[i] = array[i]; }
//    }
//    void add_memory() {
//        string* new_arr = new string[memory_size*2];
//        copy_arr_into_new_arr(new_arr);
//        delete[] array;
//        array = new_arr;
//        memory_size *= 2;
//        return;
//    }
//};

void test_sort() {
    test_merge();
}
 
int main() {
    MyArray array_for_sort;
    string line;
    getline(cin, line);
    while (!line.empty()) {
        array_for_sort.add_element(stoi(line));
//        array_for_sort.add_element(line); заменить эту строчку на ту, что выше для проверки string
        getline(cin, line);
    }
    merge_sort(array_for_sort.array, array_for_sort.get_size());
    cout << inversions << endl;

    return 0;
}
