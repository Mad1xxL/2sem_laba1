#pragma once

#include <string>

using namespace std;

// Выполнение одного запроса к файлу данных. Возвращает ответ программы
string ExecuteQuery(const string& fileName, const string& query);
