?//Сапожников Юрий ИВТ-22

#pragma once

#include <iostream>
#include <algorithm>
#include <cassert>
#include <vector>
#include <iterator>

using namespace std;

template <typename T>
class AVLTree {
private:
    struct Node {
        T key; // Ключ узла
        int height; // Высота узла
        Node* left; // Левый потомок
        Node* right; // Правый потомок

        Node(T k) : key(k), height(1), left(nullptr), right(nullptr) {} // Конструктор узла
    };

    Node* root; // Корень дерева

    int height(Node* N) {
        return N ? N->height : 0; // Возвращает высоту узла или 0, если узел пустой
    }

    int getBalance(Node* N) {
        return N ? height(N->left) - height(N->right) : 0; // Возвращает баланс узла или 0, если узел пустой
    }

    Node* rightRotate(Node* y) {
        Node* x = y->left; // Левый потомок узла y
        Node* T2 = x->right; // Правый потомок узла x

        // Выполняем правый поворот
        x->right = y;
        y->left = T2;

        // Обновляем высоты
        y->height = max(height(y->left), height(y->right)) + 1;
        x->height = max(height(x->left), height(x->right)) + 1;

        // Возвращаем новый корень
        return x;
    }

    Node* leftRotate(Node* x) {
        Node* y = x->right; // Правый потомок узла x
        Node* T2 = y->left; // Левый потомок узла y

        // Выполняем левый поворот
        y->left = x;
        x->right = T2;

        // Обновляем высоты
        x->height = max(height(x->left), height(x->right)) + 1;
        y->height = max(height(y->left), height(y->right)) + 1;

        // Возвращаем новый корень
        return y;
    }

    Node* insert(Node* node, T key) {
        // 1. Выполняем стандартную вставку в BST
        if (!node) return new Node(key); // Если узел пустой, создаем новый узел

        if (key < node->key)
            node->left = insert(node->left, key); // Вставляем в левое поддерево
        else if (key > node->key)
            node->right = insert(node->right, key); // Вставляем в правое поддерево
        else
            return node; // Дублирующиеся ключи не допускаются

        // 2. Обновляем высоту текущего узла
        node->height = 1 + max(height(node->left), height(node->right));

        // 3. Получаем фактор баланса текущего узла, чтобы проверить, стал ли он несбалансированным
        int balance = getBalance(node);

        // Если узел стал несбалансированным, то есть 4 случая

        // Левый-Левый случай
        if (balance > 1 && key < node->left->key)
            return rightRotate(node);

        // Правый-Правый случай
        if (balance < -1 && key > node->right->key)
            return leftRotate(node);

        // Левый-Правый случай
        if (balance > 1 && key > node->left->key) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // Правый-Левый случай
        if (balance < -1 && key < node->right->key) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        // Возвращаем неизменный указатель на узел
        return node;
    }

    Node* minValueNode(Node* node) {
        Node* current = node; // Начинаем с текущего узла
        while (current->left != nullptr)
            current = current->left; // Идем влево до конца
        return current; // Возвращаем узел с минимальным значением
    }

    Node* deleteNode(Node* root, T key) {
        // 1. Выполняем стандартное удаление в BST
        if (!root) return root; // Если узел пустой, возвращаем его

        if (key < root->key)
            root->left = deleteNode(root->left, key); // Удаляем из левого поддерева
        else if (key > root->key)
            root->right = deleteNode(root->right, key); // Удаляем из правого поддерева
        else {
            // Узел с одним потомком или без потомков
            if ((root->left == nullptr) || (root->right == nullptr)) {
                Node* temp = root->left ? root->left : root->right; // Получаем потомка

                // Случай без потомков
                if (temp == nullptr) {
                    temp = root;
                    root = nullptr;
                }
                else // Случай с одним потомком
                    *root = *temp; // Копируем содержимое непустого потомка

                delete temp; // Удаляем старый узел
            }
            else {
                // Узел с двумя потомками: получаем преемника (минимальный в правом поддереве)
                Node* temp = minValueNode(root->right);

                // Копируем данные преемника в текущий узел
                root->key = temp->key;

                // Удаляем преемника
                root->right = deleteNode(root->right, temp->key);
            }
        }

        // Если дерево имело только один узел, возвращаем его
        if (!root) return root;

        // 2. Обновляем высоту текущего узла
        root->height = 1 + max(height(root->left), height(root->right));

        // 3. Получаем фактор баланса текущего узла (чтобы проверить, стал ли он несбалансированным)
        int balance = getBalance(root);

        // Если узел стал несбалансированным, то есть 4 случая

        // Левый-Левый случай
        if (balance > 1 && getBalance(root->left) >= 0)
            return rightRotate(root);

        // Левый-Правый случай
        if (balance > 1 && getBalance(root->left) < 0) {
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }

        // Правый-Правый случай
        if (balance < -1 && getBalance(root->right) <= 0)
            return leftRotate(root);

        // Правый-Левый случай
        if (balance < -1 && getBalance(root->right) > 0) {
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }

        return root;
    }

    Node* updateNode(Node* node, T oldKey, T newKey) {
        if (!node) return node; // Если узел пустой, возвращаем его

        if (oldKey < node->key)
            node->left = updateNode(node->left, oldKey, newKey); // Обновляем в левом поддереве
        else if (oldKey > node->key)
            node->right = updateNode(node->right, oldKey, newKey); // Обновляем в правом поддереве
        else {
            node->key = newKey; // Обновляем ключ
            return node;
        }

        // Обновляем высоту текущего узла
        node->height = 1 + max(height(node->left), height(node->right));

        // Получаем фактор баланса текущего узла, чтобы проверить, стал ли он несбалансированным
        int balance = getBalance(node);

        // Если узел стал несбалансированным, то есть 4 случая

        // Левый-Левый случай
        if (balance > 1 && newKey < node->left->key)
            return rightRotate(node);

        // Правый-Правый случай
        if (balance < -1 && newKey > node->right->key)
            return leftRotate(node);

        // Левый-Правый случай
        if (balance > 1 && newKey > node->left->key) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // Правый-Левый случай
        if (balance < -1 && newKey < node->right->key) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        // Возвращаем неизменный указатель на узел
        return node;
    }

    Node* searchNode(Node* node, T key) {
        if (!node || node->key == key)
            return node; // Если узел пустой или ключ найден, возвращаем узел

        if (key < node->key)
            return searchNode(node->left, key); // Ищем в левом поддереве

        return searchNode(node->right, key); // Ищем в правом поддереве
    }

    void preOrder(Node* node) {
        if (!node) return; // Если узел пустой, возвращаемся
        cout << node->key << " "; // Выводим ключ узла
        preOrder(node->left); // Рекурсивно выводим левое поддерево
        preOrder(node->right); // Рекурсивно выводим правое поддерево
    }

    // Функция для сбора всех ключей дерева в вектор
    void collectKeys(Node* node, vector<T>& keys) {
        if (!node) return; // Если узел пустой, возвращаемся
        collectKeys(node->left, keys); // Рекурсивно собираем ключи из левого поддерева
        keys.push_back(node->key); // Добавляем ключ текущего узла в вектор
        collectKeys(node->right, keys); // Рекурсивно собираем ключи из правого поддерева
    }

public:
    AVLTree() : root(nullptr) {} // Конструктор дерева

    void insert(T key) {
        root = insert(root, key); // Вставляем ключ в дерево
    }

    void remove(T key) {
        root = deleteNode(root, key); // Удаляем ключ из дерева
    }

    void update(T oldKey, T newKey) {
        root = updateNode(root, oldKey, newKey); // Обновляем ключ в дереве
    }

    Node* search(T key) {
        return searchNode(root, key); // Ищем ключ в дереве
    }

    void preOrder() {
        preOrder(root); // Выводим дерево в порядке префиксного обхода
    }

    // Функция для сбора всех ключей дерева в вектор
    vector<T> getKeys() {
        vector<T> keys;
        collectKeys(root, keys);
        return keys;
    }


};
/*

AVL повороты
Малый поворот - левый, правый
Двойной поворот (большой поворот) - лево-правый, право-левый, лево-левый, право-правый

*/