#include "../headers/dlist.h"

using namespace std;

// Инициализация пустого списка
void InitList(DList& list) {
    list.head = nullptr;
    list.tail = nullptr;
}

// Вывод от головы к хвосту через разделитель sep
string ListToString(const DList& list, const string& sep) {
    string result;
    for (DNode* cur = list.head; cur != nullptr; cur = cur->next) {
        if (cur != list.head) {
            result += sep;
        }
        result += cur->data;
    }
    return result;
}

// Вывод от хвоста к голове по ссылкам prev, без рекурсии
string ListToStringReverse(const DList& list) {
    string result;
    for (DNode* cur = list.tail; cur != nullptr; cur = cur->prev) {
        if (cur != list.tail) {
            result += " <-> ";
        }
        result += cur->data;
    }
    return result;
}

// Поиск узла по значению, nullptr если не найден
DNode* FindNode(const DList& list, const string& value) {
    for (DNode* cur = list.head; cur != nullptr; cur = cur->next) {
        if (cur->data == value) {
            return cur;
        }
    }
    return nullptr;
}

// Получение элемента по индексу
bool GetByIndex(const DList& list, int index, string& result) {
    if (index < 0) {
        return false;
    }
    DNode* cur = list.head;
    for (int i = 0; i < index && cur != nullptr; i++) {
        cur = cur->next;
    }
    if (cur == nullptr) {
        return false;
    }
    result = cur->data;
    return true;
}

// Вставка нового узла между left и right (любой из них может быть nullptr)
void InsertBetween(DList& list, DNode* left, DNode* right, const string& data) {
    DNode* newNode = new DNode{data, left, right};

    if (left == nullptr) {
        list.head = newNode;
    } else {
        left->next = newNode;
    }
    if (right == nullptr) {
        list.tail = newNode;
    } else {
        right->prev = newNode;
    }
}

// Отвязка и удаление узла node за O(1): соседи известны через prev и next
string RemoveNode(DList& list, DNode* node) {
    if (node->prev == nullptr) {
        list.head = node->next;
    } else {
        node->prev->next = node->next;
    }
    if (node->next == nullptr) {
        list.tail = node->prev;
    } else {
        node->next->prev = node->prev;
    }

    string data = node->data;
    delete node;
    return data;
}

// Добавление в голову
void AddHead(DList& list, const string& data) {
    InsertBetween(list, nullptr, list.head, data);
}

// Добавление в хвост
void AddTail(DList& list, const string& data) {
    InsertBetween(list, list.tail, nullptr, data);
}

// Добавление перед элементом target
bool AddBefore(DList& list, const string& target, const string& data) {
    DNode* node = FindNode(list, target);
    if (node == nullptr) {
        return false;
    }
    InsertBetween(list, node->prev, node, data);
    return true;
}

// Добавление после элемента target
bool AddAfter(DList& list, const string& target, const string& data) {
    DNode* node = FindNode(list, target);
    if (node == nullptr) {
        return false;
    }
    InsertBetween(list, node, node->next, data);
    return true;
}

// Удаление головы
bool DeleteHead(DList& list, string& removed) {
    if (list.head == nullptr) {
        return false;
    }
    removed = RemoveNode(list, list.head);
    return true;
}

// Удаление хвоста
bool DeleteTail(DList& list, string& removed) {
    if (list.tail == nullptr) {
        return false;
    }
    removed = RemoveNode(list, list.tail);
    return true;
}

// Удаление элемента перед target
bool DeleteBefore(DList& list, const string& target, string& removed) {
    DNode* node = FindNode(list, target);
    if (node == nullptr || node->prev == nullptr) {
        return false;
    }
    removed = RemoveNode(list, node->prev);
    return true;
}

// Удаление элемента после target
bool DeleteAfter(DList& list, const string& target, string& removed) {
    DNode* node = FindNode(list, target);
    if (node == nullptr || node->next == nullptr) {
        return false;
    }
    removed = RemoveNode(list, node->next);
    return true;
}

// Удаление элемента по значению
bool DeleteByValue(DList& list, const string& value) {
    DNode* node = FindNode(list, value);
    if (node == nullptr) {
        return false;
    }
    RemoveNode(list, node);
    return true;
}

// Освобождение памяти
void FreeList(DList& list) {
    while (list.head != nullptr) {
        DNode* tmp = list.head;
        list.head = list.head->next;
        delete tmp;
    }
    list.tail = nullptr;
}
