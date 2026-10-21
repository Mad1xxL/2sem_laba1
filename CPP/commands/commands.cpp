// Модуль обработки запросов: разбор запроса, чтение и запись файла, вызов операций нужной структуры данных
// Файл данных: одна строка на структуру — «<тип> <имя> <элементы через пробел>»
// Типы: M — массив, F — односвязный список, L — двусвязный список,
//       S — стек, Q — очередь, T — дерево (Complete Binary Tree)

#include <fstream>
#include <string>

#include "commands.h"
#include "../headers/array.h"
#include "../headers/cbt.h"
#include "../headers/dlist.h"
#include "../headers/flist.h"
#include "../headers/queue.h"
#include "../headers/stack.h"

using namespace std;

// Разбиение строки на слова по пробелам
void Split(const string& line, Array& words) {
    string word;
    for (char c : line) {
        if (c == ' ' || c == '\t' || c == '\r') {
            if (!word.empty()) {
                ArrayPush(words, word);
                word.clear();
            }
        } else {
            word += c;
        }
    }
    if (!word.empty()) {
        ArrayPush(words, word);
    }
}

// Перевод строки в число, если это не число — ошибка
int ToInt(const string& s) {
    size_t pos = 0;
    int value = 0;
    try {
        value = stoi(s, &pos);
    } catch (...) {
        pos = 0;
    }
    if (pos == 0 || pos != s.size()) {
        throw string("'" + s + "' is not a number");
    }
    return value;
}

// Проверка количества слов в запросе
void CheckArgs(const Array& q, int count, const string& usage) {
    if (q.size != count) {
        throw string("usage: " + usage);
    }
}

// Команды массива
string RunArray(const string& cmd, const Array& q, const Array& items, bool& changed, string& saved) {
    Array arr;
    InitArray(arr);
    for (int i = 0; i < items.size; i++) {
        ArrayPush(arr, items.data[i]);
    }

    string reply;
    bool ok = true;

    if (cmd == "MCREATE") {
        CheckArgs(q, 2, "MCREATE name");
        reply = "created";
        changed = true;
    } else if (cmd == "MPUSH") {
        CheckArgs(q, 3, "MPUSH name value");
        ArrayPush(arr, q.data[2]);
        reply = q.data[2];
        changed = true;
    } else if (cmd == "MINSERT") {
        CheckArgs(q, 4, "MINSERT name index value");
        ok = ArrayInsert(arr, ToInt(q.data[2]), q.data[3]);
        reply = q.data[3];
        changed = true;
    } else if (cmd == "MGET") {
        CheckArgs(q, 3, "MGET name index");
        ok = ArrayGet(arr, ToInt(q.data[2]), reply);
    } else if (cmd == "MSET") {
        CheckArgs(q, 4, "MSET name index value");
        ok = ArraySet(arr, ToInt(q.data[2]), q.data[3]);
        reply = q.data[3];
        changed = true;
    } else if (cmd == "MDEL") {
        CheckArgs(q, 3, "MDEL name index");
        ok = ArrayDelete(arr, ToInt(q.data[2]), reply);
        changed = true;
    } else if (cmd == "MLEN") {
        reply = to_string(ArrayLength(arr));
    } else if (cmd == "PRINT") {
        reply = "[" + ArrayToString(arr, ", ") + "]";
    } else {
        FreeArray(arr);
        throw string("unknown command " + cmd);
    }

    saved = ArrayToString(arr, " ");
    FreeArray(arr);
    if (!ok) {
        throw string("index out of range");
    }
    return reply;
}

// Команды односвязного списка
string RunFList(const string& cmd, const Array& q, const Array& items, bool& changed, string& saved) {
    FList list;
    InitList(list);
    for (int i = 0; i < items.size; i++) {
        AddTail(list, items.data[i]);
    }

    string reply;
    bool ok = true;
    changed = true;  // большинство команд меняют список, для команд чтения сбросим ниже

    if (cmd == "FPUSH" || cmd == "FPUSHTAIL") {
        CheckArgs(q, 3, cmd + " name value");
        AddTail(list, q.data[2]);
        reply = q.data[2];
    } else if (cmd == "FPUSHHEAD") {
        CheckArgs(q, 3, "FPUSHHEAD name value");
        AddHead(list, q.data[2]);
        reply = q.data[2];
    } else if (cmd == "FPUSHBEFORE") {
        CheckArgs(q, 4, "FPUSHBEFORE name target value");
        ok = AddBefore(list, q.data[2], q.data[3]);
        reply = q.data[3];
    } else if (cmd == "FPUSHAFTER") {
        CheckArgs(q, 4, "FPUSHAFTER name target value");
        ok = AddAfter(list, q.data[2], q.data[3]);
        reply = q.data[3];
    } else if (cmd == "FDELHEAD") {
        ok = DeleteHead(list, reply);
    } else if (cmd == "FDELTAIL") {
        ok = DeleteTail(list, reply);
    } else if (cmd == "FDELBEFORE") {
        CheckArgs(q, 3, "FDELBEFORE name target");
        ok = DeleteBefore(list, q.data[2], reply);
    } else if (cmd == "FDELAFTER") {
        CheckArgs(q, 3, "FDELAFTER name target");
        ok = DeleteAfter(list, q.data[2], reply);
    } else if (cmd == "FDEL") {
        CheckArgs(q, 3, "FDEL name value");
        ok = DeleteByValue(list, q.data[2]);
        reply = q.data[2];
    } else {
        changed = false;  // дальше только команды чтения
        if (cmd == "FGET") {
            CheckArgs(q, 3, "FGET name index");
            ok = GetByIndex(list, ToInt(q.data[2]), reply);
        } else if (cmd == "FFIND") {
            CheckArgs(q, 3, "FFIND name value");
            reply = (FindNode(list, q.data[2]) != nullptr) ? "TRUE" : "FALSE";
        } else if (cmd == "FPRINTREV") {
            reply = ListToStringReverse(list.head);
        } else if (cmd == "PRINT") {
            reply = ListToString(list, " -> ");
        } else {
            FreeList(list);
            throw string("unknown command " + cmd);
        }
    }

    saved = ListToString(list, " ");
    FreeList(list);
    if (!ok) {
        throw string("element not found or list is empty");
    }
    return reply;
}

// Команды двусвязного списка
string RunDList(const string& cmd, const Array& q, const Array& items, bool& changed, string& saved) {
    DList list;
    InitList(list);
    for (int i = 0; i < items.size; i++) {
        AddTail(list, items.data[i]);
    }

    string reply;
    bool ok = true;
    changed = true;  // большинство команд меняют список, для команд чтения сбросим ниже

    if (cmd == "LPUSH" || cmd == "LPUSHTAIL") {
        CheckArgs(q, 3, cmd + " name value");
        AddTail(list, q.data[2]);
        reply = q.data[2];
    } else if (cmd == "LPUSHHEAD") {
        CheckArgs(q, 3, "LPUSHHEAD name value");
        AddHead(list, q.data[2]);
        reply = q.data[2];
    } else if (cmd == "LPUSHBEFORE") {
        CheckArgs(q, 4, "LPUSHBEFORE name target value");
        ok = AddBefore(list, q.data[2], q.data[3]);
        reply = q.data[3];
    } else if (cmd == "LPUSHAFTER") {
        CheckArgs(q, 4, "LPUSHAFTER name target value");
        ok = AddAfter(list, q.data[2], q.data[3]);
        reply = q.data[3];
    } else if (cmd == "LDELHEAD") {
        ok = DeleteHead(list, reply);
    } else if (cmd == "LDELTAIL") {
        ok = DeleteTail(list, reply);
    } else if (cmd == "LDELBEFORE") {
        CheckArgs(q, 3, "LDELBEFORE name target");
        ok = DeleteBefore(list, q.data[2], reply);
    } else if (cmd == "LDELAFTER") {
        CheckArgs(q, 3, "LDELAFTER name target");
        ok = DeleteAfter(list, q.data[2], reply);
    } else if (cmd == "LDEL") {
        CheckArgs(q, 3, "LDEL name value");
        ok = DeleteByValue(list, q.data[2]);
        reply = q.data[2];
    } else {
        changed = false;  // дальше только команды чтения
        if (cmd == "LGET") {
            CheckArgs(q, 3, "LGET name index");
            ok = GetByIndex(list, ToInt(q.data[2]), reply);
        } else if (cmd == "LFIND") {
            CheckArgs(q, 3, "LFIND name value");
            reply = (FindNode(list, q.data[2]) != nullptr) ? "TRUE" : "FALSE";
        } else if (cmd == "LPRINTREV") {
            reply = ListToStringReverse(list);
        } else if (cmd == "PRINT") {
            reply = ListToString(list, " <-> ");
        } else {
            FreeList(list);
            throw string("unknown command " + cmd);
        }
    }

    saved = ListToString(list, " ");
    FreeList(list);
    if (!ok) {
        throw string("element not found or list is empty");
    }
    return reply;
}

// Команды стека
string RunStack(const string& cmd, const Array& q, const Array& items, bool& changed, string& saved) {
    Stack stack;
    InitStack(stack);
    for (int i = 0; i < items.size; i++) {
        Push(stack, items.data[i]);  // в файле элементы записаны от дна к вершине
    }

    string reply;
    bool ok = true;

    if (cmd == "SPUSH") {
        CheckArgs(q, 3, "SPUSH name value");
        Push(stack, q.data[2]);
        reply = q.data[2];
        changed = true;
    } else if (cmd == "SPOP") {
        ok = Pop(stack, reply);
        changed = true;
    } else if (cmd == "PRINT") {
        reply = "top: " + StackToString(stack);
    } else {
        FreeStack(stack);
        throw string("unknown command " + cmd);
    }

    saved = StackToFile(stack.top);
    FreeStack(stack);
    if (!ok) {
        throw string("stack is empty");
    }
    return reply;
}

// Команды очереди
string RunQueue(const string& cmd, const Array& q, const Array& items, bool& changed, string& saved) {
    Queue queue;
    InitQueue(queue);
    for (int i = 0; i < items.size; i++) {
        Push(queue, items.data[i]);
    }

    string reply;
    bool ok = true;

    if (cmd == "QPUSH") {
        CheckArgs(q, 3, "QPUSH name value");
        Push(queue, q.data[2]);
        reply = q.data[2];
        changed = true;
    } else if (cmd == "QPOP") {
        ok = Pop(queue, reply);
        changed = true;
    } else if (cmd == "PRINT") {
        reply = "head: " + QueueToString(queue, " <- ") + " :tail";
    } else {
        FreeQueue(queue);
        throw string("unknown command " + cmd);
    }

    saved = QueueToString(queue, " ");
    FreeQueue(queue);
    if (!ok) {
        throw string("queue is empty");
    }
    return reply;
}

// Команды дерева
string RunTree(const string& cmd, const Array& q, const Array& items, bool& changed, string& saved) {
    // проверяем, что в файле у дерева только числа и '#'
    for (int i = 0; i < items.size; i++) {
        if (items.data[i] != "#") {
            ToInt(items.data[i]);
        }
    }
    Tree tree;
    InitTree(tree);
    TreeFromFile(tree, items.data, items.size);

    string reply;
    try {
        if (cmd == "TINSERT") {
            CheckArgs(q, 3, "TINSERT name number");
            Insert(tree, ToInt(q.data[2]));
            reply = q.data[2];
            changed = true;
        } else if (cmd == "TINSERTAT") {
            CheckArgs(q, 5, "TINSERTAT name parent L|R number");
            char side = toupper(q.data[3][0]);
            if (q.data[3].size() != 1 || !InsertAt(tree, ToInt(q.data[2]), side, ToInt(q.data[4]))) {
                throw string("cannot insert: no such parent, wrong side or place is taken");
            }
            reply = q.data[4];
            changed = true;
        } else if (cmd == "TGET" || cmd == "ISMEMBER") {
            CheckArgs(q, 3, cmd + " name number");
            reply = Contains(tree, ToInt(q.data[2])) ? "TRUE" : "FALSE";
        } else if (cmd == "TISCOMPLETE") {
            reply = IsComplete(tree) ? "TRUE" : "FALSE";
        } else if (cmd == "TBFS") {
            reply = LevelOrder(tree);
        } else if (cmd == "TINORDER") {
            InOrder(tree.root, reply);
        } else if (cmd == "TPREORDER") {
            PreOrder(tree.root, reply);
        } else if (cmd == "TPOSTORDER") {
            PostOrder(tree.root, reply);
        } else if (cmd == "PRINT") {
            reply = "\n" + DrawTree(tree);
        } else {
            throw string("unknown command " + cmd);
        }
    } catch (const string&) {
        FreeTree(tree);  // перед выходом с ошибкой освобождаем память
        throw;
    }

    saved = TreeToFile(tree);
    FreeTree(tree);
    return reply;
}

// ---------------------------------------------------------------------
// Работа с файлом
// ---------------------------------------------------------------------

// Чтение всех непустых строк файла
void ReadFile(const string& fileName, Array& lines) {
    ifstream in(fileName);
    string line;
    while (getline(in, line)) {
        if (!line.empty()) {
            ArrayPush(lines, line);
        }
    }
}

// Запись всех строк в файл
void WriteFile(const string& fileName, const Array& lines) {
    ofstream out(fileName);
    for (int i = 0; i < lines.size; i++) {
        out << lines.data[i] << "\n";
    }
}

// Поиск строки структуры по имени.
// Возвращает номер строки (или -1), а также тип структуры и её элементы
int FindStructure(const Array& lines, const string& name, char& type, Array& items) {
    for (int i = 0; i < lines.size; i++) {
        Array words;
        InitArray(words);
        Split(lines.data[i], words);

        if (words.size >= 2 && words.data[1] == name) {
            type = words.data[0][0];
            for (int k = 2; k < words.size; k++) {
                ArrayPush(items, words.data[k]);
            }
            FreeArray(words);
            return i;
        }
        FreeArray(words);
    }
    return -1;
}

// Выполнение одного запроса
string ExecuteQuery(const string& fileName, const string& query) {
    // 1. Разбиваем запрос на слова
    Array q;
    InitArray(q);
    Split(query, q);
    if (q.size < 2) {
        throw string("query must be: COMMAND name [args]");
    }
    string cmd = q.data[0];
    for (char& c : cmd) {
        c = toupper(c);
    }
    string name = q.data[1];

    // 2. Читаем файл и ищем строку нужной структуры
    Array lines;
    InitArray(lines);
    ReadFile(fileName, lines);

    Array items;
    InitArray(items);
    char fileType = 0;
    int lineNumber = FindStructure(lines, name, fileType, items);

    // 3. Тип структуры — первая буква команды
    //    (исключения: PRINT берёт тип из файла, ISMEMBER относится к дереву)
    char type = cmd[0];
    if (cmd == "ISMEMBER") {
        type = 'T';
    }
    if (cmd == "PRINT") {
        type = fileType;
    }

    bool createsNew = cmd.find("PUSH") != string::npos || cmd == "MCREATE" ||
                      cmd == "MINSERT" || cmd == "TINSERT";
    if (lineNumber == -1 && !createsNew) {
        throw string("structure '" + name + "' not found");
    }
    if (lineNumber != -1 && fileType != type) {
        throw string("'" + name + "' has type " + fileType + ", command " + cmd + " is for another type");
    }

    // 4. Выполняем команду
    bool changed = false;
    string saved;
    string reply;
    switch (type) {
        case 'M': reply = RunArray(cmd, q, items, changed, saved); break;
        case 'F': reply = RunFList(cmd, q, items, changed, saved); break;
        case 'L': reply = RunDList(cmd, q, items, changed, saved); break;
        case 'S': reply = RunStack(cmd, q, items, changed, saved); break;
        case 'Q': reply = RunQueue(cmd, q, items, changed, saved); break;
        case 'T': reply = RunTree(cmd, q, items, changed, saved); break;
        default: throw string("unknown command " + cmd);
    }

    // 5. Если состав элементов изменился — перезаписываем файл
    if (changed) {
        string newLine = string(1, type) + " " + name;
        if (!saved.empty()) {
            newLine += " " + saved;
        }
        if (lineNumber == -1) {
            ArrayPush(lines, newLine);
        } else {
            ArraySet(lines, lineNumber, newLine);
        }
        WriteFile(fileName, lines);
    }

    FreeArray(q);
    FreeArray(lines);
    FreeArray(items);
    return reply;
}
