#pragma once
#include <iostream>
#include <algorithm>
#include <cassert>
#include <vector>
#include <iterator>
#include "AVLTrree.Class.h"

using namespace std;

// Функция для тестирования стандартных алгоритмов
void testcheck() {

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

    // Использование copy_if для копирования четных чисел
    vector<int> evenKeys;
    copy_if(keys.begin(), keys.end(), back_inserter(evenKeys), [](int x) { return x % 2 == 0; });
    assert(evenKeys == vector<int>({ 10, 20, 30, 40, 50, 60, 70, 80 }));

    // Использование for_each для увеличения всех элементов на 1
    for_each(keys.begin(), keys.end(), [](int& x) { x += 1; });
    assert(keys == vector<int>({ 11, 21, 31, 41, 51, 61, 71, 81 }));

    // Использование any_of для проверки наличия числа больше 50
    bool anyGreaterThan50 = any_of(keys.begin(), keys.end(), [](int x) { return x > 50; });
    assert(anyGreaterThan50 == true);

    // Использование all_of для проверки, что все числа больше 10
    bool allGreaterThan10 = all_of(keys.begin(), keys.end(), [](int x) { return x > 10; });
    assert(allGreaterThan10 == true);

    // Использование none_of для проверки, что ни одно число не меньше 10
    bool noneLessThan10 = none_of(keys.begin(), keys.end(), [](int x) { return x < 10; });
    assert(noneLessThan10 == true);

    // Использование transform для умножения всех элементов на 2
    vector<int> doubledKeys(keys.size());
    transform(keys.begin(), keys.end(), doubledKeys.begin(), [](int x) { return x * 2; });
    assert(doubledKeys == vector<int>({ 22, 42, 62, 82, 102, 122, 142, 162 }));

    cout << "All standard algorithm tests passed!" << endl;
}