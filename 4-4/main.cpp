/*
Задача № 4 (5 баллов)

Даны неотрицательные целые числа n,k и массив целых чисел из [0..10^9] размера n. 
Требуется найти k-ю порядковую статистику. т.е. напечатать число, которое бы стояло на позиции с индексом k (0..n-1) 
в отсортированном массиве. Напишите нерекурсивный алгоритм.
Требования к дополнительной памяти: O(n). Требуемое среднее время работы: O(n).
Функцию Partition следует реализовывать методом прохода двумя итераторами в одном направлении. Описание для случая прохода 
от начала массива к концу:
Выбирается опорный элемент. Опорный элемент меняется с последним элементом массива.
Во время работы Partition в начале массива содержатся элементы, не бОльшие опорного. Затем располагаются элементы, строго 
бОльшие опорного. В конце массива лежат нерассмотренные элементы. Последним элементом лежит опорный.
Итератор (индекс) i указывает на начало группы элементов, строго бОльших опорного.
Итератор j больше i, итератор j указывает на первый нерассмотренный элемент.
Шаг алгоритма. Рассматривается элемент, на который указывает j. Если он больше опорного, то сдвигаем j.
Если он не больше опорного, то меняем a[i] и a[j] местами, сдвигаем i и сдвигаем j.
В конце работы алгоритма меняем опорный и элемент, на который указывает итератор i.

4_4. Реализуйте стратегию выбора опорного элемента “случайный элемент”. Функцию Partition реализуйте методом прохода двумя 
итераторами от конца массива к началу.
*/

#include <cassert>
#include <iostream>
#include <string>
#include <memory.h>
#include <stdint.h>

using namespace std;

int partition(unsigned int* a,unsigned int n ) {
    if( n <= 1 ) { return 0; }
    int idx = rand() % n;
    swap( a[idx], a[0] );
    int pivot = a[0];

    int i = n - 1, j = n - 1;
    while (i > 0) {
        if (a[i] >= pivot) {
            swap(a[i], a[j--]);
        }
        i--;
    }
    swap( a[j], a[0] );
    return j;
}

void kth_largest(unsigned int* arr, unsigned int n, int k) {
    int acc = 0;
    int part = 0;
    part = partition( arr, n );
    while (part + acc != k) {
        if (part + acc > k) {
            part = partition( arr, part );
        }
        else {
            acc += part + 1;
            n = n - part - 1;
            arr += part + 1;
            part = partition( arr, n );
        }
    }
    return;
}

int main() {
    int size, k;
    cin >> size >> k;
    unsigned int* arr = new unsigned int[size];
    for (unsigned int* i = arr; i < arr + size; ++i) {
        cin >> *i;
    }
    kth_largest(arr, size, k);
    cout << arr[k] << endl;
    
    return 0;
}
