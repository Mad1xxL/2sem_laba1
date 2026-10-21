#pragma once

#include <string>

using namespace std;

// Узел дерева
struct TreeNode {
    int data;
    TreeNode* left;
    TreeNode* right;
};

// Complete Binary Tree: все уровни, кроме последнего, заполнены полностью,
// последний уровень заполняется слева направо
struct Tree {
    TreeNode* root;
    int size;  
};

void InitTree(Tree& tree);

// Добавление
void Insert(Tree& tree, int data);
bool InsertAt(Tree& tree, int parent, char side, int data);

// Поиск и проверка
bool Contains(const Tree& tree, int data);
bool IsComplete(const Tree& tree);

// Чтение (обходы)
string LevelOrder(const Tree& tree);
void InOrder(const TreeNode* node, string& result);
void PreOrder(const TreeNode* node, string& result);
void PostOrder(const TreeNode* node, string& result);
string DrawTree(const Tree& tree);

// Работа с файлом
string TreeToFile(const Tree& tree);
void TreeFromFile(Tree& tree, const string* items, int count);

// Освобождение памяти
void FreeTree(Tree& tree);
