// 1_2. Дек с динамическим буфером

// Ограничение времени	1 секунда
// Ограничение памяти	5Mb
// Ввод	стандартный ввод или input.txt
// Вывод	стандартный вывод или output.txt
// Реализовать дек с динамическим зацикленным буфером.
// Обрабатывать команды push * и pop *.
// Формат ввода

// В первой строке количество команд n. n ≤ 1000000.
// Каждая команда задаётся как 2 целых числа: a b.
// a = 1 - push front
// a = 2 - pop front
// a = 3 - push back
// a = 4 - pop back
// Если дана команда pop *, то число b - ожидаемое значение. Если команда pop * вызвана для пустой структуры данных, 
// то ожидается “-1”.




#include <cassert>
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;


class Dequeue {
public:
    Dequeue() = default;
    Dequeue(const Dequeue&) = delete;
    Dequeue(const Dequeue&&) = delete;
    Dequeue& operator= (const Dequeue&) = delete;
    Dequeue& operator= (const Dequeue&&) = delete;
    ~Dequeue();
 
    void PushBack(int key);
    void PushFront(int key);
    int PopBack();
    int PopFront();
 
    bool IsEmpty() const { return head_ == tail_; }
    bool IsFull() const {
        if (head_ - tail_ == 1 || tail_ - head_ == size - 1) {
            return true;
        }
        return false;
    }

    int size = 0;
    int arr_size = 2;
    void PrintArr() const {
        for (int i = 0; i < arr_size; ++i) {
            cout << arr[i] << ' ';
        }
        cout << endl;
    }
 
private:
    int* arr = new int[arr_size];
    int head_ = 0;
    int tail_ = 0;
    void copy_arr_into_new_arr(int* new_arr) {
        int idx = 0;
        if (tail_ < head_) {
            for (int i = head_; i < arr_size; ++i) {
                new_arr[idx] = arr[i];
                // cout << "new_arr[idx]: " << new_arr[idx] << " idx: " << idx << " arr[i]: " << arr[i] << " i: " << i << endl;
                ++idx;
            }
            for (int i = 0; i < tail_; ++i) {
                new_arr[idx] = arr[i];
                // cout << "idx: " << idx << " arr[i]: " << arr[i] << " i: " << i << endl;
                ++idx;
            }
        }
        else {
            for (int i = head_; i < tail_; ++i) {
                new_arr[idx] = arr[i];
                ++idx;
            }
        }
        return;
    }
    void add_memory() {
        int* new_arr = new int[arr_size*2];
        copy_arr_into_new_arr(new_arr);
        arr_size *= 2;
        delete[] arr;
        arr = new_arr;
        head_ = 0;
        tail_ = size;
        // cout << tail_ << endl;
        
    }
    void cut_memory() {
        int* new_arr = new int[arr_size/2];
        copy_arr_into_new_arr(new_arr);
        arr_size /= 2;
        delete[] arr;
        arr = new_arr;
        head_ = 0;
        tail_ = size;
    }
};

Dequeue::~Dequeue()
{
    delete[] arr;
}
 
void Dequeue::PushBack(int key)
{
//    Node* new_node = new Node(key);
    if (arr_size-1 == size) { add_memory(); }
//    cout << "current size after PushBack: " << size << endl;
    arr[tail_] = key;
    tail_ = (tail_ < arr_size-1) ? tail_+1 : 0;
    ++size;

    return;
}

void Dequeue::PushFront(int key) {
    if (arr_size-1 == size) { add_memory(); }
    head_ = (head_ != 0)? head_-1 : arr_size-1;
    arr[head_] = key;
    ++size;
    return;
}
 
int Dequeue::PopBack()
{
//    cout << "current size before PopBack: " << size << endl;
    if (IsEmpty()) {
        return -1;
    }
    int key;
    if (tail_ != 0) {
        key = arr[tail_-1];
        --tail_;
    }
    else {
        key = arr[arr_size-1];
        tail_ = arr_size-1;
        
    }
    --size;
 
    return key;
}

int Dequeue::PopFront()
{
    if (IsEmpty()) {
        return -1;
    }
    int key = arr[head_];
    (head_ != arr_size-1) ? head_++ : head_ = 0;
    --size;
    if (size < arr_size/4) {
        cut_memory();
    }
 
    return key;
}

void execute(int command, int key, Dequeue& my_dequeue) {
    if (command == 1) {
        my_dequeue.PushFront(key);
    }
    else if (command == 2) {
        int response = my_dequeue.PopFront();
        if (response == key) {
            cout << "YES" << endl;
        }
        else {
            cout << "NO" << endl;
        }
    }
    else if (command == 3) {
        my_dequeue.PushBack(key);
    }
    else {
        int response = my_dequeue.PopBack();
        if (response == key) {
            cout << "YES" << endl;
        }
        else {
            cout << "NO" << endl;
        }
    }
}

int main() {
    Dequeue my_dequeue;
    int n;

    cin >> n;
    int command, key;
    for (int i = 0; i < n; ++i) {
        cin >> command >> key;
        execute(command, key, my_dequeue);
    }

    return 0;
};

