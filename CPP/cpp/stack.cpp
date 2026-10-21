#include "../headers/stack.h"

using namespace std;

// Инициализация пустого стека
void InitStack(Stack& stack) {
    stack.top = nullptr;
}

// Добавление элемента на вершину
void Push(Stack& stack, const string& data) {
    SNode* newNode = new SNode{data, stack.top};
    stack.top = newNode;
}

// Чтение и удаление элемента с вершины
bool Pop(Stack& stack, string& removed) {
    if (stack.top == nullptr) {
        return false;
    }

    SNode* tmp = stack.top;
    removed = tmp->data;
    stack.top = tmp->next;
    delete tmp;
    return true;
}

// Вывод от вершины ко дну
string StackToString(const Stack& stack) {
    string result;
    for (SNode* cur = stack.top; cur != nullptr; cur = cur->next) {
        if (cur != stack.top) {
            result += " | ";
        }
        result += cur->data;
    }
    return result;
}

// Строка для файла от дна к вершине,
// чтобы при загрузке Push восстановил тот же порядок
string StackToFile(const SNode* node) {
    if (node == nullptr) {
        return "";
    }
    string below = StackToFile(node->next);
    if (below.empty()) {
        return node->data;
    }
    return below + " " + node->data;
}

// Освобождение памяти
void FreeStack(Stack& stack) {
    string tmp;
    while (Pop(stack, tmp)) {
    }
}
