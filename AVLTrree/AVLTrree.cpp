#include <iostream>
#include <algorithm>
#include <cassert>
#include <vector>
#include <iterator>
#include "AVLTrree.Class.h"
#include "AVLTrree.Test.h"

using namespace std;

int main() {

    setlocale(LC_ALL, "Russian");

    testcheck();

    AVLTree<int> tree;

    tree.testavl(); // Тестируем дерево с 8 элементамиv

    return 0;
}