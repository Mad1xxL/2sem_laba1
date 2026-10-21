#include <iostream>
#include <string>

#include "commands/commands.h"

using namespace std;

int main(int argc, char* argv[]) {
    // Разбор параметров --file и --query
    string fileName;
    string query;
    for (int i = 1; i + 1 < argc; i += 2) {
        string key = argv[i];
        if (key == "--file") {
            fileName = argv[i + 1];
        } else if (key == "--query") {
            query = argv[i + 1];
        }
    }
    if (fileName.empty() || query.empty()) {
        cout << "usage: ./dbms --file <file> --query '<COMMAND name [args]>'" << endl;
        return 1;
    }

    try {
        string reply = ExecuteQuery(fileName, query);
        cout << "-> " << reply << endl;
    } catch (const string& error) {
        cout << "-> ERROR: " << error << endl;
        return 2;
    }
    return 0;
}
