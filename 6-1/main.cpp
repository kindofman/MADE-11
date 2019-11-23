/*
Задача 6. Порядок обхода (3 балла)

Дано число N < 106 и последовательность целых чисел из [-231..231] длиной N.
Требуется построить бинарное дерево, заданное наивным порядком вставки.
Т.е., при добавлении очередного числа K в дерево с корнем root, если root→Key ≤ K, 
то узел K добавляется в правое поддерево root; иначе в левое поддерево root.
Рекурсия запрещена.

6_1. Выведите элементы в порядке pre-order (сверху вниз).
*/



#include <cassert>
#include <iostream>
#include <string>
#include <memory.h>
#include <stdint.h>
#include <vector>
#include <queue>
#include <ctime>

using namespace std;

struct Print {
    void operator() (int key) { cout << key << ' '; }
};

struct Node {
    Node(const Node&) = delete;
    Node(const Node&&) = delete;
    Node& operator= (const Node&) = delete;
    Node& operator= (const Node&&) = delete;
    ~Node();
    
    int key;
    Node* left = nullptr;
    Node* right = nullptr;
   
    Node(int key_) : key(key_) {}
    void add(int new_key);
    void pre_order(Print print);
    
};

void Node::add(int new_key) {
    Node* current = this;
    Node* next = this;
    while (next) {
        if (new_key < next->key) {
            current = next;
            next = current->left;
        }
        else {
            current = next;
            next = current->right;
        }
    }
    new_key < current->key ? current->left = new Node(new_key) : current->right = new Node(new_key);
}

void Node::pre_order(Print print) {
    vector<Node *> stack;
    Node* current = this;
    while (current || !stack.empty()) {
        if (current) {
            print(current->key);
//            cout << current->key << ' ';
            stack.push_back(current);
            current = current->left;
            }
        else {
            current = stack.back()->right;
            stack.pop_back();
        }
    }
}

Node::~Node() {
    queue<Node* > q;
    q.push(this);
    while (!q.empty())
    {
        Node *node = q.front();
        q.pop();
        if (node->left != nullptr) q.push(node->left);
        if (node->right != nullptr) q.push(node->right);
        delete node;
    }
}



int main() {
    int size, key;
    cin >> size >> key;
    Node* root = new Node(key);
    for (int i = 1; i < size; ++i) {
        cin >> key;
        root->add(key);
    }
    Print print;
    root->pre_order(print);
    cout << endl;
    return 0;
}


