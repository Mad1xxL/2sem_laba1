#pragma once

#include <string>

using namespace std;

// Узел двусвязного списка, хранит ссылки на соседей с обеих сторон
struct DNode {
    string data;
    DNode* prev;
    DNode* next;
};

// Двусвязный список, ссылка prev даёт удаление хвоста за O(1)
struct DList {
    DNode* head;
    DNode* tail;
};

void InitList(DList& list);

// Чтение и поиск
string ListToString(const DList& list, const string& sep);
string ListToStringReverse(const DList& list);
DNode* FindNode(const DList& list, const string& value);
bool GetByIndex(const DList& list, int index, string& result);

// Добавление
void AddHead(DList& list, const string& data);
void AddTail(DList& list, const string& data);
bool AddBefore(DList& list, const string& target, const string& data);
bool AddAfter(DList& list, const string& target, const string& data);

// Удаление
bool DeleteHead(DList& list, string& removed);
bool DeleteTail(DList& list, string& removed);
bool DeleteBefore(DList& list, const string& target, string& removed);
bool DeleteAfter(DList& list, const string& target, string& removed);
bool DeleteByValue(DList& list, const string& value);

// Освобождение памяти
void FreeList(DList& list);
