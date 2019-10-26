
#include <cassert>
#include <iostream>
#include <algorithm>
#include <vector>
#include <chrono>

using namespace std::chrono;

using namespace std;

class MaxHeap {
public:
    MaxHeap() = default;
    MaxHeap(const MaxHeap&) = delete;
    MaxHeap(const MaxHeap&&) = delete;
    MaxHeap& operator= (const MaxHeap&) = delete;
    MaxHeap& operator= (const MaxHeap&&) = delete;
    ~MaxHeap();
 
    int top() const;
    void push(int key);
    int pop();
    bool is_empty() { return !size; };
    int len() const { return size; };
    void heapify(int* new_arr, int n);
    
 
     void PrintArr() const {
         for (int i = 0; i < memory_size; ++i) {
             cout << arr[i] << ' ';
         }
         cout << endl;
         cout << "size of heap is " << size << endl;
         cout << "memory_size is " << memory_size << endl;
     }
     
    
    void self_test() {
        MaxHeap test_heap;
        assert(test_heap.len() == 0);
        test_heap.push(1);
        assert(test_heap.pop() == 1);
        test_heap.push(3);
        test_heap.push(2);
        test_heap.push(4);
        test_heap.push(-4);
        test_heap.push(-3);
        assert(test_heap.len() == 5);
        assert(test_heap.top() == 4);
        assert(test_heap.len() == 5);
        assert(test_heap.pop() == 4);
        assert(test_heap.pop() == 3);
        assert(test_heap.pop() == 2);
        assert(test_heap.pop() == -3);
        assert(test_heap.pop() == -4);
        assert(test_heap.len() == 0);
        vector<int> numbers = {96, 25, 58, 63, 41, 86, 49, 65, 29, 92, 96, 88, 85, 13, 26, 3, 56,
                               62, 32, 13,  1, 88, 55, 70, 61, 63, 90, 18, 92, 57,  1, 32, 16, 39,
                               84, 32, 82, 48, 12, 45, 88, 83, 44, 80,  9, 32, 35, 92, 75, 15,  6,
                               77, 42, 71, 90,  8, 90, 18, 96, 11, 19, 23, 97, 38, 51, 50, 85, 93,
                               13, 34, 14, 69, 58, 93, 75, 78, 99, 45, 73, 90, 17,  5, 32,  6, 98,
                               40, 14, 47, 86, 58, 45, 42, 78, 12, 29, 35, 17, 15, 59, 12};
        for (auto item : numbers) {
            test_heap.push(item);
        }
        sort(numbers.begin(), numbers.end(), greater<int>());
        for (auto item : numbers) {
            assert(item == test_heap.pop());
        }
    };
 
private:
    int size = 0;
    int memory_size = 1;
    int* arr = new int[memory_size];
    
    void copy_arr_into_new_arr(int* new_arr) {
        for (int idx = 0; idx < memory_size; ++idx) {
            new_arr[idx] = arr[idx];
        }
        return;
    }
    
    void add_memory() {
        int* new_arr = new int[memory_size*2];
        copy_arr_into_new_arr(new_arr);
        delete[] arr;
        arr = new_arr;
        memory_size *= 2;
        return;
    }
    void cut_memory() {
        int* new_arr = new int[memory_size/2];
        copy_arr_into_new_arr(new_arr);
        delete[] arr;
        arr = new_arr;
        memory_size /= 2;
        return;
    }
    
    void sift_up(int pos) {
        int startpos = 0;
        int new_item = arr[pos];
        while (pos > startpos) {
            int parentpos = (pos-1) / 2;
            int parent = arr[parentpos];
            if (new_item > parent) {
                arr[pos] = parent;
                pos = parentpos;
                continue;
            }
            break;
        }
        arr[pos] = new_item;
        return;
    }
    void sift_down(int pos) {
        int endpos = size-1;
        // int pos = 0;
        int childpos = 2*pos + 1;
        int element = arr[pos];
        while (childpos <= endpos) {
            int rightpos = childpos + 1;
            if (rightpos <= endpos && arr[childpos] < arr[rightpos]) {
                childpos = rightpos;
            }
            arr[pos] = arr[childpos];
            pos = childpos;
            childpos = 2*pos + 1;
        }
        arr[pos] = element;
        sift_up(pos);
        return;
    }
};
 


void MaxHeap::push(int key)
{
    if (memory_size == size) { add_memory(); }
    arr[size] = key;
    sift_up(size);
    ++size;

    return;
}
 
 int MaxHeap::pop()
 {
    if (memory_size == size/4) { cut_memory(); }
    int key = top();
    arr[0] = arr[size-1];
    sift_down(0);
    --size;
    return key;
 }

int MaxHeap::top() const {
    return arr[0];
}

MaxHeap::~MaxHeap() {
    delete[] arr;
    return;
}

void execute() {
    int n;
    cin >> n;
    MaxHeap my_heap;
    
     for (int i = 0; i < n; ++i) {
         int key;
         cin >> key;
         my_heap.push(key);
     }
    
    int K;
    cin >> K;
    int cnt = 0;
    while (!my_heap.is_empty()) {
        int acc = K;
        int temp_arr[K];
        int temp_arr_size = 0;
        while (my_heap.top() <= acc && !my_heap.is_empty()) {
            int fruit = my_heap.pop();
            temp_arr[temp_arr_size] = fruit;
            temp_arr_size++;
            acc -= fruit;
        }
        for (int idx=0; idx < temp_arr_size; idx++) {
            if (temp_arr[idx] <= 1) { break; }
            my_heap.push(temp_arr[idx]/2);
        }
        ++cnt;
    }

    cout << cnt << endl;
}

 
int main() {
    MaxHeap test_heap;
    test_heap.self_test();
    
    execute();
    return 0;
};




