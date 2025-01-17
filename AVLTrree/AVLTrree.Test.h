﻿//Сапожников Юрий ИВТ-22

#pragma once

#include <iostream>
#include <algorithm>
#include <cassert>
#include <vector>
#include <iterator>
#include "AVLTrree.Class.h"

using namespace std;

// Функция для тестирования работы AVL дерева
void testcheck() {

    AVLTree<int> tree;

    tree.insert(10);
    tree.preOrder(); cout << "\n";
    tree.insert(20);
    tree.preOrder(); cout << "\n";
    tree.insert(30);
    tree.preOrder(); cout << "\n";
    tree.insert(40);
    tree.preOrder(); cout << "\n";
    tree.insert(50);
    tree.preOrder(); cout << "\n";
    tree.insert(60);
    tree.preOrder(); cout << "\n";
    tree.insert(70);
    tree.preOrder(); cout << "\n";
    tree.insert(80);
    tree.preOrder(); cout << "\n";

    vector<int> keys = tree.getKeys();// Метод возвращает вектор всех ключей в дереве в отсортированном порядке
    vector<int> expectedKeys = { 10, 20, 30, 40, 50, 60, 70, 80 };// Вектор хранит ожидаемые значения ключей
    assert(keys == expectedKeys); // Проверяет

    tree.remove(20);
    vector<int> keys1 = tree.getKeys();
    vector<int> expectedKeys1 = { 10, 30, 40, 50, 60, 70, 80 };
    assert(keys1 == expectedKeys1);

    assert(tree.search(50) != nullptr);
    assert(tree.search(100) == nullptr);

    tree.update(10, 90);
    assert(tree.search(10) == nullptr);

    cout << "Тестирование работы AVL дерева выполненно" << endl;
}

// Функция для тестирования стандартных алгоритмов
void stdcheck() {

    AVLTree<int> tree;

    tree.insert(10);
    tree.insert(20);
    tree.insert(30);
    tree.insert(40);
    tree.insert(50);
    tree.insert(60);
    tree.insert(70);
    tree.insert(80);

    // Сбор всех ключей дерева в вектор
    vector<int> keys = tree.getKeys();

    // Использование copy_if для копирования диапозона чисел с условием (четных чисел)
    vector<int> evenKeys;
    //keys.begin() keys.end() - откуда надо копировать и до
    //back_inserter(evenKeys) - представляет итератор на начало диапазона, в который надо вставить скопированные значения 
    //условие
    copy_if(keys.begin(), keys.end(), back_inserter(evenKeys), [](int x) { return x % 2 == 0; });
    assert(evenKeys == vector<int>({ 10, 20, 30, 40, 50, 60, 70, 80 }));

    // Использование for_each для выполенния обхода и обработки элементов (увеличения всех элементов на 1)
    for_each(keys.begin(), keys.end(), [](int& x) { x += 1; });
    assert(keys == vector<int>({ 11, 21, 31, 41, 51, 61, 71, 81 }));

    // Использование any_of для проверки, что хотя бы одно число больше 50
    bool anyGreaterThan50 = any_of(keys.begin(), keys.end(), [](int x) { return x > 50; });
    assert(anyGreaterThan50 == true);

    // Использование all_of для проверки, что все числа больше 10
    bool allGreaterThan10 = all_of(keys.begin(), keys.end(), [](int x) { return x > 10; });
    assert(allGreaterThan10 == true);

    // Использование none_of для проверки, что ни одно число не меньше 10
    bool noneLessThan10 = none_of(keys.begin(), keys.end(), [](int x) { return x < 10; });
    assert(noneLessThan10 == true);

    // Использование transform для умножения всех элементов на 2 ()
    // Вернёт итератор doubledKeys.begin() увеличенный на количество элементов записанных через него. 
    vector<int> doubledKeys(keys.size());
    transform(keys.begin(), keys.end(), doubledKeys.begin(), [](int x) { return x * 2; });
    assert(doubledKeys == vector<int>({ 22, 42, 62, 82, 102, 122, 142, 162 }));

    cout << "Тестирование стандартных алгоритмов выполненно" << endl;
}