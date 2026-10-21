#pragma once

#include <string>

using namespace std;

// Узел стека
struct SNode {
    string data;
    SNode* next;
};

// Стек на односвязном списке, хранится только вершина (LIFO)
struct Stack {
    SNode* top;
};

void InitStack(Stack& stack);

// Добавление и удаление
void Push(Stack& stack, const string& data);
bool Pop(Stack& stack, string& removed);

// Чтение
string StackToString(const Stack& stack);
string StackToFile(const SNode* node);

// Освобождение памяти
void FreeStack(Stack& stack);
