#include "../headers/queue.h"

using namespace std;

// Инициализация пустой очереди
void InitQueue(Queue& queue) {
    queue.head = nullptr;
    queue.tail = nullptr;
}

// Добавление элемента в хвост
void Push(Queue& queue, const string& data) {
    QNode* newNode = new QNode{data, nullptr};

    if (queue.tail == nullptr) {
        queue.head = newNode;
    } else {
        queue.tail->next = newNode;
    }
    queue.tail = newNode;
}

// Чтение и удаление элемента из головы
bool Pop(Queue& queue, string& removed) {
    if (queue.head == nullptr) {
        return false;
    }

    QNode* tmp = queue.head;
    removed = tmp->data;
    queue.head = tmp->next;
    if (queue.head == nullptr) {
        queue.tail = nullptr;  // очередь опустела
    }
    delete tmp;
    return true;
}

// Вывод от головы к хвосту через разделитель sep
string QueueToString(const Queue& queue, const string& sep) {
    string result;
    for (QNode* cur = queue.head; cur != nullptr; cur = cur->next) {
        if (cur != queue.head) {
            result += sep;
        }
        result += cur->data;
    }
    return result;
}

// Освобождение памяти
void FreeQueue(Queue& queue) {
    string tmp;
    while (Pop(queue, tmp)) {
    }
}
