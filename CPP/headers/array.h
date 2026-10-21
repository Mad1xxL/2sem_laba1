#pragma once

#include <string>

using namespace std;

// Динамический массив: элементы лежат подряд в одном блоке памяти,
struct Array {
    string* data;   // блок памяти
    int size;       // количество элементов
    int capacity;   // ёмкость блока
};

void InitArray(Array& arr);

// Добавление
void ArrayPush(Array& arr, const string& value);
bool ArrayInsert(Array& arr, int index, const string& value);

// Чтение
bool ArrayGet(const Array& arr, int index, string& result);
int ArrayLength(const Array& arr);
string ArrayToString(const Array& arr, const string& sep);

// Замена и удаление
bool ArraySet(Array& arr, int index, const string& value);
bool ArrayDelete(Array& arr, int index, string& removed);

// Освобождение памяти
void FreeArray(Array& arr);
