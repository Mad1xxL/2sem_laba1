#include "../headers/array.h"

using namespace std;

// Инициализация пустого массива
void InitArray(Array& arr) {
    arr.data = nullptr;
    arr.size = 0;
    arr.capacity = 0;
}

// Увеличение ёмкости в 2 раза с переносом элементов в новый блок
void ArrayGrow(Array& arr) {
    int newCapacity = (arr.capacity == 0) ? 4 : arr.capacity * 2;
    string* newData = new string[newCapacity];

    for (int i = 0; i < arr.size; i++) {
        newData[i] = arr.data[i];
    }

    delete[] arr.data;
    arr.data = newData;
    arr.capacity = newCapacity;
}

// Добавление элемента в конец
void ArrayPush(Array& arr, const string& value) {
    if (arr.size == arr.capacity) {
        ArrayGrow(arr);
    }
    arr.data[arr.size] = value;
    arr.size++;
}

// Добавление элемента по индексу, элементы правее сдвигаются вправо
bool ArrayInsert(Array& arr, int index, const string& value) {
    if (index < 0 || index > arr.size) {
        return false;
    }
    if (arr.size == arr.capacity) {
        ArrayGrow(arr);
    }

    for (int i = arr.size; i > index; i--) {
        arr.data[i] = arr.data[i - 1];
    }
    arr.data[index] = value;
    arr.size++;
    return true;
}

// Получение элемента по индексу
bool ArrayGet(const Array& arr, int index, string& result) {
    if (index < 0 || index >= arr.size) {
        return false;
    }
    result = arr.data[index];
    return true;
}

// Длина массива
int ArrayLength(const Array& arr) {
    return arr.size;
}

// Все элементы одной строкой через разделитель sep
string ArrayToString(const Array& arr, const string& sep) {
    string result;
    for (int i = 0; i < arr.size; i++) {
        if (i > 0) {
            result += sep;
        }
        result += arr.data[i];
    }
    return result;
}

// Замена элемента по индексу
bool ArraySet(Array& arr, int index, const string& value) {
    if (index < 0 || index >= arr.size) {
        return false;
    }
    arr.data[index] = value;
    return true;
}

// Удаление элемента по индексу, элементы правее сдвигаются влево
bool ArrayDelete(Array& arr, int index, string& removed) {
    if (index < 0 || index >= arr.size) {
        return false;
    }
    removed = arr.data[index];

    for (int i = index; i < arr.size - 1; i++) {
        arr.data[i] = arr.data[i + 1];
    }
    arr.size--;
    return true;
}

// Освобождение памяти
void FreeArray(Array& arr) {
    delete[] arr.data;
    InitArray(arr);
}
