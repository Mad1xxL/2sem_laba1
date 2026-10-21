#include "../headers/cbt.h"

using namespace std;

// Инициализация пустого дерева
void InitTree(Tree& tree) {
    tree.root = nullptr;
    tree.size = 0;
}

// Обход в ширину: все узлы по уровням складываются в массив order.
// Массив работает как очередь: читаем с позиции head, дописываем в позицию tail.
// Возвращает количество узлов
int CollectLevels(const Tree& tree, TreeNode** order) {
    if (tree.root == nullptr) {
        return 0;
    }
    int head = 0;
    int tail = 0;
    order[tail++] = tree.root;

    while (head < tail) {
        TreeNode* cur = order[head++];
        if (cur->left != nullptr) {
            order[tail++] = cur->left;
        }
        if (cur->right != nullptr) {
            order[tail++] = cur->right;
        }
    }
    return tail;
}

// Поиск узла по значению (обход в ширину), nullptr если не найден
TreeNode* FindTreeNode(const Tree& tree, int data) {
    TreeNode** order = new TreeNode*[tree.size + 1];
    int count = CollectLevels(tree, order);

    TreeNode* found = nullptr;
    for (int i = 0; i < count; i++) {
        if (order[i]->data == data) {
            found = order[i];
            break;
        }
    }
    delete[] order;
    return found;
}

// Добавление в первое свободное место по уровням слева направо,
void Insert(Tree& tree, int data) {
    TreeNode* newNode = new TreeNode{data, nullptr, nullptr};

    if (tree.root == nullptr) {
        tree.root = newNode;
        tree.size = 1;
        return;
    }

    TreeNode** order = new TreeNode*[tree.size];
    int count = CollectLevels(tree, order);

    for (int i = 0; i < count; i++) {
        if (order[i]->left == nullptr) {
            order[i]->left = newNode;
            break;
        }
        if (order[i]->right == nullptr) {
            order[i]->right = newNode;
            break;
        }
    }
    delete[] order;
    tree.size++;
}

// Добавление сыном к узлу parent (side = 'L' или 'R').
// Так можно построить дерево, которое не является Complete
bool InsertAt(Tree& tree, int parent, char side, int data) {
    TreeNode* node = FindTreeNode(tree, parent);
    if (node == nullptr) {
        return false;
    }

    if (side == 'L' && node->left == nullptr) {
        node->left = new TreeNode{data, nullptr, nullptr};
    } else if (side == 'R' && node->right == nullptr) {
        node->right = new TreeNode{data, nullptr, nullptr};
    } else {
        return false;  // неверная сторона или место занято
    }
    tree.size++;
    return true;
}

// Поиск элемента
bool Contains(const Tree& tree, int data) {
    return FindTreeNode(tree, data) != nullptr;
}

// Проверка на Complete: идём по уровням, после первого пустого места
// ни у одного следующего узла сыновей быть не должно
bool IsComplete(const Tree& tree) {
    TreeNode** order = new TreeNode*[tree.size + 1];
    int count = CollectLevels(tree, order);

    bool wasEmpty = false;
    bool complete = true;

    for (int i = 0; i < count; i++) {
        TreeNode* children[2] = {order[i]->left, order[i]->right};
        for (TreeNode* child : children) {
            if (child == nullptr) {
                wasEmpty = true;
            } else if (wasEmpty) {
                complete = false;  // узел после пустого места
            }
        }
    }
    delete[] order;
    return complete;
}

// Добавление числа к строке через пробел
void AddToResult(string& result, int data) {
    if (!result.empty()) {
        result += " ";
    }
    result += to_string(data);
}

// Обход в ширину (по уровням)
string LevelOrder(const Tree& tree) {
    TreeNode** order = new TreeNode*[tree.size + 1];
    int count = CollectLevels(tree, order);

    string result;
    for (int i = 0; i < count; i++) {
        AddToResult(result, order[i]->data);
    }
    delete[] order;
    return result;
}

// Центрированный обход: левое поддерево, узел, правое поддерево
void InOrder(const TreeNode* node, string& result) {
    if (node == nullptr) {
        return;
    }
    InOrder(node->left, result);
    AddToResult(result, node->data);
    InOrder(node->right, result);
}

// Прямой обход: узел, левое поддерево, правое поддерево
void PreOrder(const TreeNode* node, string& result) {
    if (node == nullptr) {
        return;
    }
    AddToResult(result, node->data);
    PreOrder(node->left, result);
    PreOrder(node->right, result);
}

// Обратный обход: левое поддерево, правое поддерево, узел
void PostOrder(const TreeNode* node, string& result) {
    if (node == nullptr) {
        return;
    }
    PostOrder(node->left, result);
    PostOrder(node->right, result);
    AddToResult(result, node->data);
}

// Рисование поддерева: каждый сын с отступом под родителем
void DrawNode(const TreeNode* node, const string& prefix, bool isLast, const string& label, string& result) {
    if (node == nullptr) {
        return;
    }
    result += prefix + (isLast ? "`-- " : "|-- ") + label + to_string(node->data) + "\n";

    string nextPrefix = prefix + (isLast ? "    " : "|   ");
    DrawNode(node->left, nextPrefix, node->right == nullptr, "L: ", result);
    DrawNode(node->right, nextPrefix, true, "R: ", result);
}

// Рисунок всего дерева
string DrawTree(const Tree& tree) {
    if (tree.root == nullptr) {
        return "(empty)";
    }
    string result = to_string(tree.root->data) + "\n";
    DrawNode(tree.root->left, "", tree.root->right == nullptr, "L: ", result);
    DrawNode(tree.root->right, "", true, "R: ", result);
    result.pop_back();  // убираем последний перевод строки
    return result;
}

// Строка для файла: узлы по уровням, пустое место обозначается '#'.
// Так при загрузке восстанавливается точно такая же форма дерева
string TreeToFile(const Tree& tree) {
    if (tree.root == nullptr) {
        return "";
    }
    TreeNode** order = new TreeNode*[2 * tree.size + 1];  // в очереди бывают и nullptr
    int head = 0;
    int tail = 0;
    order[tail++] = tree.root;

    string result;
    while (head < tail) {
        TreeNode* cur = order[head++];
        if (!result.empty()) {
            result += " ";
        }
        if (cur == nullptr) {
            result += "#";
            continue;
        }
        result += to_string(cur->data);
        order[tail++] = cur->left;
        order[tail++] = cur->right;
    }
    delete[] order;

    // '#' в конце строки ничего не значат, убираем их
    while (result.size() >= 2 && result.substr(result.size() - 2) == " #") {
        result.erase(result.size() - 2);
    }
    return result;
}

// Загрузка дерева из элементов файла (формат как в TreeToFile)
void TreeFromFile(Tree& tree, const string* items, int count) {
    if (count == 0) {
        return;
    }
    TreeNode** order = new TreeNode*[count];
    int head = 0;
    int tail = 0;

    tree.root = new TreeNode{stoi(items[0]), nullptr, nullptr};
    tree.size = 1;
    order[tail++] = tree.root;

    int i = 1;
    while (head < tail && i < count) {
        TreeNode* cur = order[head++];

        // левый сын
        if (i < count && items[i] != "#") {
            cur->left = new TreeNode{stoi(items[i]), nullptr, nullptr};
            order[tail++] = cur->left;
            tree.size++;
        }
        i++;

        // правый сын
        if (i < count && items[i] != "#") {
            cur->right = new TreeNode{stoi(items[i]), nullptr, nullptr};
            order[tail++] = cur->right;
            tree.size++;
        }
        i++;
    }
    delete[] order;
}

// Удаление всех узлов поддерева (сначала сыновья, потом сам узел)
void FreeNode(TreeNode* node) {
    if (node == nullptr) {
        return;
    }
    FreeNode(node->left);
    FreeNode(node->right);
    delete node;
}

// Освобождение памяти
void FreeTree(Tree& tree) {
    FreeNode(tree.root);
    InitTree(tree);
}
