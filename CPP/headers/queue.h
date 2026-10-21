#pragma once

#include <string>

using namespace std;

// Узел очереди
struct QNode {
    string data;
    QNode* next;
};

// Очередь: добавление в хвост, извлечение из головы (FIFO), обе операции за O(1)
struct Queue {
    QNode* head;
    QNode* tail;
};

void InitQueue(Queue& queue);

// Добавление и удаление
void Push(Queue& queue, const string& data);
bool Pop(Queue& queue, string& removed);

// Чтение
string QueueToString(const Queue& queue, const string& sep);

// Освобождение памяти
void FreeQueue(Queue& queue);
