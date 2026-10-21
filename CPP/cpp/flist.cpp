#include "../headers/flist.h"

using namespace std;

// Инициализация пустого списка
void InitList(FList& list) {
    list.head = nullptr;
    list.tail = nullptr;
}

// Вывод от головы к хвосту через разделитель sep
string ListToString(const FList& list, const string& sep) {
    string result;
    for (Node* cur = list.head; cur != nullptr; cur = cur->next) {
        if (cur != list.head) {
            result += sep;
        }
        result += cur->data;
    }
    return result;
}

// Вывод от хвоста к голове: рекурсией сначала выводим остаток списка, потом сам узел
string ListToStringReverse(const Node* node) {
    if (node == nullptr) {
        return "";
    }
    string rest = ListToStringReverse(node->next);
    if (rest.empty()) {
        return node->data;
    }
    return rest + " -> " + node->data;
}

// Поиск узла по значению, nullptr если не найден
Node* FindNode(const FList& list, const string& value) {
    for (Node* cur = list.head; cur != nullptr; cur = cur->next) {
        if (cur->data == value) {
            return cur;
        }
    }
    return nullptr;
}

// Получение элемента по индексу
bool GetByIndex(const FList& list, int index, string& result) {
    if (index < 0) {
        return false;
    }
    Node* cur = list.head;
    for (int i = 0; i < index && cur != nullptr; i++) {
        cur = cur->next;
    }
    if (cur == nullptr) {
        return false;
    }
    result = cur->data;
    return true;
}

// Поиск узла, стоящего перед node (ссылки назад нет, поэтому идём с головы)
Node* FindPrev(const FList& list, const Node* node) {
    for (Node* cur = list.head; cur != nullptr; cur = cur->next) {
        if (cur->next == node) {
            return cur;
        }
    }
    return nullptr;
}

// Отвязка и удаление узла node, возвращает его значение
string RemoveNode(FList& list, Node* node) {
    Node* prev = FindPrev(list, node);

    if (prev == nullptr) {
        list.head = node->next;  // удаляется голова
    } else {
        prev->next = node->next;
    }
    if (node == list.tail) {
        list.tail = prev;        // удаляется хвост
    }

    string data = node->data;
    delete node;
    return data;
}

// Добавление в голову
void AddHead(FList& list, const string& data) {
    Node* newNode = new Node{data, list.head};
    list.head = newNode;

    if (list.tail == nullptr) {
        list.tail = newNode;
    }
}

// Добавление в хвост
void AddTail(FList& list, const string& data) {
    Node* newNode = new Node{data, nullptr};

    if (list.tail == nullptr) {
        list.head = newNode;
    } else {
        list.tail->next = newNode;
    }
    list.tail = newNode;
}

// Добавление перед элементом target
bool AddBefore(FList& list, const string& target, const string& data) {
    Node* node = FindNode(list, target);
    if (node == nullptr) {
        return false;
    }

    if (node == list.head) {
        AddHead(list, data);
    } else {
        Node* prev = FindPrev(list, node);
        prev->next = new Node{data, node};
    }
    return true;
}

// Добавление после элемента target
bool AddAfter(FList& list, const string& target, const string& data) {
    Node* node = FindNode(list, target);
    if (node == nullptr) {
        return false;
    }

    node->next = new Node{data, node->next};
    if (node == list.tail) {
        list.tail = node->next;
    }
    return true;
}

// Удаление головы
bool DeleteHead(FList& list, string& removed) {
    if (list.head == nullptr) {
        return false;
    }
    removed = RemoveNode(list, list.head);
    return true;
}

// Удаление хвоста (нужен предпоследний узел — проход по списку)
bool DeleteTail(FList& list, string& removed) {
    if (list.tail == nullptr) {
        return false;
    }
    removed = RemoveNode(list, list.tail);
    return true;
}

// Удаление элемента перед target
bool DeleteBefore(FList& list, const string& target, string& removed) {
    Node* node = FindNode(list, target);
    if (node == nullptr || node == list.head) {
        return false;
    }
    removed = RemoveNode(list, FindPrev(list, node));
    return true;
}

// Удаление элемента после target
bool DeleteAfter(FList& list, const string& target, string& removed) {
    Node* node = FindNode(list, target);
    if (node == nullptr || node->next == nullptr) {
        return false;
    }
    removed = RemoveNode(list, node->next);
    return true;
}

// Удаление элемента по значению
bool DeleteByValue(FList& list, const string& value) {
    Node* node = FindNode(list, value);
    if (node == nullptr) {
        return false;
    }
    RemoveNode(list, node);
    return true;
}

// Освобождение памяти
void FreeList(FList& list) {
    while (list.head != nullptr) {
        Node* tmp = list.head;
        list.head = list.head->next;
        delete tmp;
    }
    list.tail = nullptr;
}
