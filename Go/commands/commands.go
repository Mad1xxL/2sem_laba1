// Модуль обработки запросов: разбор запроса, чтение и запись файла,
// вызов операций нужной структуры данных
//
// Файл данных: одна строка на структуру — «<тип> <имя> <элементы через пробел>»
// Типы: M — массив, F — односвязный список, L — двусвязный список,
//
//	S — стек, Q — очередь, T — дерево (реализовано только на C++)
package commands

import (
	"errors"
	"os"
	"strconv"
	"strings"

	"dbms/structures"
)

// Перевод строки в число, если это не число — ошибка
func toInt(s string) (int, error) {
	value, err := strconv.Atoi(s)
	if err != nil {
		return 0, errors.New("'" + s + "' is not a number")
	}
	return value, nil
}

// Проверка количества слов в запросе
func checkArgs(q []string, count int, usage string) error {
	if len(q) != count {
		return errors.New("usage: " + usage)
	}
	return nil
}

// Команды массива
func runArray(cmd string, q, items []string) (string, bool, string, error) {
	var arr structures.Array
	for _, item := range items {
		arr.Push(item)
	}

	reply := ""
	changed := false
	ok := true

	switch cmd {
	case "MCREATE":
		if err := checkArgs(q, 2, "MCREATE name"); err != nil {
			return "", false, "", err
		}
		reply = "created"
		changed = true
	case "MPUSH":
		if err := checkArgs(q, 3, "MPUSH name value"); err != nil {
			return "", false, "", err
		}
		arr.Push(q[2])
		reply = q[2]
		changed = true
	case "MINSERT":
		if err := checkArgs(q, 4, "MINSERT name index value"); err != nil {
			return "", false, "", err
		}
		index, err := toInt(q[2])
		if err != nil {
			return "", false, "", err
		}
		ok = arr.Insert(index, q[3])
		reply = q[3]
		changed = true
	case "MGET":
		if err := checkArgs(q, 3, "MGET name index"); err != nil {
			return "", false, "", err
		}
		index, err := toInt(q[2])
		if err != nil {
			return "", false, "", err
		}
		reply, ok = arr.Get(index)
	case "MSET":
		if err := checkArgs(q, 4, "MSET name index value"); err != nil {
			return "", false, "", err
		}
		index, err := toInt(q[2])
		if err != nil {
			return "", false, "", err
		}
		ok = arr.Set(index, q[3])
		reply = q[3]
		changed = true
	case "MDEL":
		if err := checkArgs(q, 3, "MDEL name index"); err != nil {
			return "", false, "", err
		}
		index, err := toInt(q[2])
		if err != nil {
			return "", false, "", err
		}
		reply, ok = arr.Delete(index)
		changed = true
	case "MLEN":
		reply = strconv.Itoa(arr.Length())
	case "PRINT":
		reply = "[" + arr.ToString(", ") + "]"
	default:
		return "", false, "", errors.New("unknown command " + cmd)
	}

	if !ok {
		return "", false, "", errors.New("index out of range")
	}
	return reply, changed, arr.ToString(" "), nil
}

// Набор операций, общий для односвязного и двусвязного списков.
// FList и DList имеют одинаковые методы, поэтому один обработчик подходит обоим
type list interface {
	AddHead(data string)
	AddTail(data string)
	AddBefore(target, data string) bool
	AddAfter(target, data string) bool
	DeleteHead() (string, bool)
	DeleteTail() (string, bool)
	DeleteBefore(target string) (string, bool)
	DeleteAfter(target string) (string, bool)
	DeleteByValue(value string) bool
	GetByIndex(index int) (string, bool)
	Contains(value string) bool
	ToString(sep string) string
	ToStringReverse() string
}

// Команды списков. letter — первая буква команды (F или L),
// arrow — разделитель при выводе
func runList(lst list, letter, arrow, cmd string, q, items []string) (string, bool, string, error) {

	for _, item := range items {
		lst.AddTail(item)
	}

	op := strings.TrimPrefix(cmd, letter) // команда без первой буквы
	reply := ""
	changed := true // большинство команд меняют список, для команд чтения сбросим ниже
	ok := true

	// сколько слов должно быть в запросе для каждой команды
	argsCount := map[string]int{
		"PUSH": 3, "PUSHTAIL": 3, "PUSHHEAD": 3, "PUSHBEFORE": 4, "PUSHAFTER": 4,
		"DELHEAD": 2, "DELTAIL": 2, "DELBEFORE": 3, "DELAFTER": 3, "DEL": 3,
		"GET": 3, "FIND": 3, "PRINTREV": 2,
	}
	if count, known := argsCount[op]; known && len(q) != count {
		return "", false, "", errors.New("wrong number of arguments for " + cmd)
	}

	switch op {
	case "PUSH", "PUSHTAIL":
		lst.AddTail(q[2])
		reply = q[2]
	case "PUSHHEAD":
		lst.AddHead(q[2])
		reply = q[2]
	case "PUSHBEFORE":
		ok = lst.AddBefore(q[2], q[3])
		reply = q[3]
	case "PUSHAFTER":
		ok = lst.AddAfter(q[2], q[3])
		reply = q[3]
	case "DELHEAD":
		reply, ok = lst.DeleteHead()
	case "DELTAIL":
		reply, ok = lst.DeleteTail()
	case "DELBEFORE":
		reply, ok = lst.DeleteBefore(q[2])
	case "DELAFTER":
		reply, ok = lst.DeleteAfter(q[2])
	case "DEL":
		ok = lst.DeleteByValue(q[2])
		reply = q[2]
	default:
		changed = false // дальше только команды чтения
		switch {
		case op == "GET":
			index, err := toInt(q[2])
			if err != nil {
				return "", false, "", err
			}
			reply, ok = lst.GetByIndex(index)
		case op == "FIND":
			reply = "FALSE"
			if lst.Contains(q[2]) {
				reply = "TRUE"
			}
		case op == "PRINTREV":
			reply = lst.ToStringReverse()
		case cmd == "PRINT":
			reply = lst.ToString(arrow)
		default:
			return "", false, "", errors.New("unknown command " + cmd)
		}
	}

	if !ok {
		return "", false, "", errors.New("element not found or list is empty")
	}
	return reply, changed, lst.ToString(" "), nil
}

// Команды стека
func runStack(cmd string, q, items []string) (string, bool, string, error) {
	var stack structures.Stack
	for _, item := range items {
		stack.Push(item) // в файле элементы записаны от дна к вершине
	}

	reply := ""
	changed := false
	ok := true

	switch cmd {
	case "SPUSH":
		if err := checkArgs(q, 3, "SPUSH name value"); err != nil {
			return "", false, "", err
		}
		stack.Push(q[2])
		reply = q[2]
		changed = true
	case "SPOP":
		reply, ok = stack.Pop()
		changed = true
	case "PRINT":
		reply = "top: " + stack.ToString()
	default:
		return "", false, "", errors.New("unknown command " + cmd)
	}

	if !ok {
		return "", false, "", errors.New("stack is empty")
	}
	return reply, changed, stack.ToFile(), nil
}

// Команды очереди
func runQueue(cmd string, q, items []string) (string, bool, string, error) {
	var queue structures.Queue
	for _, item := range items {
		queue.Push(item)
	}

	reply := ""
	changed := false
	ok := true

	switch cmd {
	case "QPUSH":
		if err := checkArgs(q, 3, "QPUSH name value"); err != nil {
			return "", false, "", err
		}
		queue.Push(q[2])
		reply = q[2]
		changed = true
	case "QPOP":
		reply, ok = queue.Pop()
		changed = true
	case "PRINT":
		reply = "head: " + queue.ToString(" <- ") + " :tail"
	default:
		return "", false, "", errors.New("unknown command " + cmd)
	}

	if !ok {
		return "", false, "", errors.New("queue is empty")
	}
	return reply, changed, queue.ToString(" "), nil
}

// ---------------------------------------------------------------------
// Работа с файлом
// ---------------------------------------------------------------------

// Чтение всех непустых строк файла (если файла нет — пустой список строк)
func readFile(fileName string) []string {
	content, err := os.ReadFile(fileName)
	if err != nil {
		return nil
	}
	var lines []string
	for _, line := range strings.Split(string(content), "\n") {
		line = strings.TrimRight(line, "\r")
		if line != "" {
			lines = append(lines, line)
		}
	}
	return lines
}

// Запись всех строк в файл
func writeFile(fileName string, lines []string) error {
	return os.WriteFile(fileName, []byte(strings.Join(lines, "\n")+"\n"), 0644)
}

// Поиск строки структуры по имени.
// Возвращает номер строки (или -1), тип структуры и её элементы
func findStructure(lines []string, name string) (int, byte, []string) {
	for i, line := range lines {
		words := strings.Fields(line)
		if len(words) >= 2 && words[1] == name {
			return i, words[0][0], words[2:]
		}
	}
	return -1, 0, nil
}

// Выполнение одного запроса
func ExecuteQuery(fileName, query string) (string, error) {
	// 1. Разбиваем запрос на слова
	q := strings.Fields(query)
	if len(q) < 2 {
		return "", errors.New("query must be: COMMAND name [args]")
	}
	cmd := strings.ToUpper(q[0])
	name := q[1]

	// 2. Читаем файл и ищем строку нужной структуры
	lines := readFile(fileName)
	lineNumber, fileType, items := findStructure(lines, name)

	// 3. Тип структуры — первая буква команды
	//    (исключения: PRINT берёт тип из файла, ISMEMBER относится к дереву)
	structType := cmd[0]
	if cmd == "ISMEMBER" {
		structType = 'T'
	}
	if cmd == "PRINT" {
		structType = fileType
	}
	if structType == 'T' {
		return "", errors.New("trees are implemented only in the C++ version")
	}

	createsNew := strings.Contains(cmd, "PUSH") || cmd == "MCREATE" || cmd == "MINSERT"
	if lineNumber == -1 && !createsNew {
		return "", errors.New("structure '" + name + "' not found")
	}
	if lineNumber != -1 && fileType != structType {
		return "", errors.New("'" + name + "' has type " + string(fileType) +
			", command " + cmd + " is for another type")
	}

	// 4. Выполняем команду
	var reply, saved string
	var changed bool
	var err error
	switch structType {
	case 'M':
		reply, changed, saved, err = runArray(cmd, q, items)
	case 'F':
		reply, changed, saved, err = runList(&structures.FList{}, "F", " -> ", cmd, q, items)
	case 'L':
		reply, changed, saved, err = runList(&structures.DList{}, "L", " <-> ", cmd, q, items)
	case 'S':
		reply, changed, saved, err = runStack(cmd, q, items)
	case 'Q':
		reply, changed, saved, err = runQueue(cmd, q, items)
	default:
		return "", errors.New("unknown command " + cmd)
	}
	if err != nil {
		return "", err
	}

	// 5. Если состав элементов изменился — перезаписываем файл
	if changed {
		newLine := string(structType) + " " + name
		if saved != "" {
			newLine += " " + saved
		}
		if lineNumber == -1 {
			lines = append(lines, newLine)
		} else {
			lines[lineNumber] = newLine
		}
		if err := writeFile(fileName, lines); err != nil {
			return "", errors.New("cannot write file " + fileName)
		}
	}
	return reply, nil
}
