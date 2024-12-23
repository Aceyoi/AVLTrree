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

    // Функция для тестирования дерева с 8 элементами
    void testavl() {
        AVLTree<int> tree8;

        // Вставка элементов в AVL-дерево
        tree8.insert(10); // Дерево: 10
        tree8.insert(20); // Дерево: 10 (левый) - 20 (корень)
        tree8.insert(30); // Дерево: 10 (левый) - 20 (корень) - 30 (правый)
        tree8.insert(40); // Дерево: 10 (левый) - 20 (левый) - 30 (корень) - 40 (правый)
        tree8.insert(50); // Дерево: 10 (левый) - 20 (левый) - 30 (левый) - 40 (корень) - 50 (правый)
        tree8.insert(60); // Дерево: 10 (левый) - 20 (левый) - 30 (левый) - 40 (корень) - 50 (правый) - 60 (правый)
        tree8.insert(70); // Дерево: 10 (левый) - 20 (левый) - 30 (левый) - 40 (корень) - 50 (правый) - 60 (правый) - 70 (правый)
        tree8.insert(80); // Дерево: 10 (левый) - 20 (левый) - 30 (левый) - 40 (корень) - 50 (правый) - 60 (правый) - 70 (правый) - 80 (правый)

        // Ожидаемые повороты:
        // После вставки 40: правый поворот вокруг 30
        // После вставки 50: левый поворот вокруг 40
        // После вставки 60: правый поворот вокруг 50
        // После вставки 70: левый поворот вокруг 60
        // После вставки 80: правый поворот вокруг 70

        assert(tree8.root->key == 40); // Проверяем, что корень дерева равен 40
        assert(tree8.root->height == 4); // Проверяем, что высота дерева равна 4
        assert(tree8.root->left->key == 20); // Проверяем, что левый потомок корня равен 20
        assert(tree8.root->right->key == 60); // Проверяем, что правый потомок корня равен 60
        assert(tree8.root->left->left->key == 10); // Проверяем, что левый потомок левого потомка корня равен 10
        assert(tree8.root->left->right->key == 30); // Проверяем, что правый потомок левого потомка корня равен 30
        assert(tree8.root->right->left->key == 50); // Проверяем, что левый потомок правого потомка корня равен 50
        assert(tree8.root->right->right->key == 70); // Проверяем, что правый потомок правого потомка корня равен 70
        assert(tree8.root->right->right->right->key == 80); // Проверяем, что правый потомок правого потомка правого потомка корня равен 80
        assert(tree8.search(10)->key == 10); // Проверяем, что элемент 10 найден
        assert(tree8.search(20)->key == 20); // Проверяем, что элемент 20 найден
        assert(tree8.search(30)->key == 30); // Проверяем, что элемент 30 найден
        assert(tree8.search(40)->key == 40); // Проверяем, что элемент 40 найден
        assert(tree8.search(50)->key == 50); // Проверяем, что элемент 50 найден
        assert(tree8.search(60)->key == 60); // Проверяем, что элемент 60 найден
        assert(tree8.search(70)->key == 70); // Проверяем, что элемент 70 найден
        assert(tree8.search(80)->key == 80); // Проверяем, что элемент 80 найден
        assert(tree8.search(90) == nullptr); // Проверяем, что элемент 90 не найден
        assert(tree8.search(0) == nullptr); // Проверяем, что элемент 0 не найден
        tree8.update(20, 25); // Обновляем элемент 20 на 25
        tree8.update(60, 65); // Обновляем элемент 60 на 65
        assert(tree8.root->key == 40); // Проверяем, что корень дерева равен 40
        assert(tree8.root->left->key == 25); // Проверяем, что левый потомок корня равен 25
        assert(tree8.root->right->key == 65); // Проверяем, что правый потомок корня равен 65
        tree8.remove(10); // Удаляем элемент 10
        tree8.remove(80); // Удаляем элемент 80
        assert(tree8.root->key == 40); // Проверяем, что корень дерева равен 40
        assert(tree8.root->left->key == 25); // Проверяем, что левый потомок корня равен 25
        assert(tree8.root->right->key == 65); // Проверяем, что правый потомок корня равен 65
        assert(tree8.root->left->left == nullptr); // Проверяем, что левый потомок левого потомка корня пустой
        assert(tree8.root->right->right->right == nullptr); // Проверяем, что правый потомок правого потомка правого потомка корня пустой
    }
};
