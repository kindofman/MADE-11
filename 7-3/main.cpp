/*
Порядковые статистики. Дано число N и N строк. Каждая строка содержит команду добавления или удаления натуральных чисел, 
а также запрос на получение k-ой порядковой статистики. Команда добавления числа A задается положительным числом A, 
команда удаления числа A задается отрицательным числом “-A”. Запрос на получение k-ой порядковой статистики задается числом k.

Вариант 7_3.  Требуемая скорость выполнения запроса - O(log n) в среднем. В реализации используйте декартово дерево.
*/

#include <cassert>
#include <iostream>
#include <string>
#include <memory.h>
#include <stdint.h>
#include <vector>
#include <queue>
#include <ctime>
#include <math.h>

using namespace std;

struct Node {
    int value;
    int priority;
    int count;
    Node* left = nullptr;
    Node* right = nullptr;
    Node(int value_, int priority_) : value(value_), priority(priority_), count(1) {}
};

class Treap {
public:
    Treap() = default;
    Treap(const Treap&) = delete;
    Treap(const Treap&&) = delete;
    Treap& operator= (const Treap&) = delete;
    Treap& operator= (const Treap&&) = delete;
    ~Treap();
    
    void pre_order_print() const;
    void insert(int key, int priority);
    void erase(int key);
    int kth_largest(int idx);
    
private:
    Node* root = nullptr;
    void split(Node* node, int key, Node*& left, Node*& right);
    Node* merge(Node* left, Node* right);
    Node* kth_largest_(int idx, Node* node);
    int get_count(Node* node) {return (node) ? node->count : 0; }
    void update_count(Node* node) {
        if (node) {
            node->count = get_count(node->left) + get_count(node->right) + 1;
        }
    }
};

void Treap::pre_order_print() const {
    Node* current = root;
    vector<Node *> stack;
    while (current || !stack.empty()) {
        if (current) {
            cout << current->value << ' ' << current->count << endl;
            stack.push_back(current);
            current = current->left;
            }
        else {
            current = stack.back()->right;
            stack.pop_back();
        }
    }
    cout << endl;
}

void Treap::split(Node* node, int key, Node*& left, Node*& right) {
    if (!node) {
        left = right = nullptr;
    } else if (node->value <= key){
        split(node->right, key, node->right, right);
        left = node;
    } else {
        split(node->left, key, left, node->left);
        right = node;
    }
    update_count(node);
}
Node* Treap::merge(Node *left, Node *right) {
    if (!left || !right) { return left == 0 ? right : left; }
    if (left->priority > right->priority) {
        left->right = merge(left->right, right);
        update_count(left);
        return left;
    } else {
        right->left = merge(left, right->left);
        update_count(right);
        return right;
    }
}

//Реализация вставки номер 2
void Treap::insert(int key, int priority) {
    if (!root) {
        root = new Node(key, priority);
        return;
    }
    
    Node* new_node = new Node(key, priority);
    Node* current = root;
    Node* parent = nullptr;
    while (current && current->priority >= priority) {
        parent = current;
        current = (current->value <= key) ? current->right : current->left;
        parent->count += 1;
    }
    split(current, key, new_node->left, new_node->right);
    new_node->count += get_count(new_node->left) + get_count(new_node->right);
    if ( !parent )  {
        root = new_node;
        return;
    }
    (parent->value <= key) ? parent->right = new_node : parent->left = new_node;
}
//Реализация удаления номер 1
void Treap::erase(int key) {
    Node* node_to_delete = nullptr;
    Node* left = nullptr;
    Node* right = nullptr;
    split(root, key, left, right);
    split(left, key-0.5, left ,node_to_delete);
    root = merge(left, right);
    delete node_to_delete;
}

Node* Treap::kth_largest_(int idx, Node* node) {
    if ( get_count(node->left) < idx ) {
        return kth_largest_( idx-get_count(node->left)-1, node->right );
    } else if ( get_count(node->left) > idx ) {
        return kth_largest_( idx, node->left );
    } else return node;
}

int Treap::kth_largest(int idx) {
    Node* node = kth_largest_(idx, root);
    return node->value;
}

Treap::~Treap() {
    if (!root) return;
    queue<Node* > q;
    q.push(root);
    while (!q.empty()) {
        Node *node = q.front();
        q.pop();
        if (node->left != nullptr) q.push(node->left);
        if (node->right != nullptr) q.push(node->right);
        delete node;
    }
}

int main() {
    int N;
    Treap treap;
    cin >> N;
    for (int i = 0; i < N; ++i) {
        int A, k;
        cin >> A >> k;
        int priority = rand() % 1000000;
        (A > 0) ? treap.insert(abs(A), priority) : treap.erase(abs(A));
        cout << treap.kth_largest(k) << endl;
    }
    cout << endl;
}

