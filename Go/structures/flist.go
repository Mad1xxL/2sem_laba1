package structures

// Узел односвязного списка
type Node struct {
	data string
	next *Node
}

// Односвязный список, указатель на хвост даёт добавление в конец за O(1).
// Пустой список создаётся просто объявлением: var list FList
type FList struct {
	head *Node
	tail *Node
}

// Вывод от головы к хвосту через разделитель sep
func (list *FList) ToString(sep string) string {
	result := ""
	for cur := list.head; cur != nil; cur = cur.next {
		if cur != list.head {
			result += sep
		}
		result += cur.data
	}
	return result
}

// Вывод от хвоста к голове
func (list *FList) ToStringReverse() string {
	return reverseFrom(list.head)
}

// Рекурсия: сначала выводим остаток списка, потом сам узел
func reverseFrom(node *Node) string {
	if node == nil {
		return ""
	}
	rest := reverseFrom(node.next)
	if rest == "" {
		return node.data
	}
	return rest + " -> " + node.data
}

// Поиск узла по значению, nil если не найден
func (list *FList) FindNode(value string) *Node {
	for cur := list.head; cur != nil; cur = cur.next {
		if cur.data == value {
			return cur
		}
	}
	return nil
}

// Проверка, есть ли элемент в списке
func (list *FList) Contains(value string) bool {
	return list.FindNode(value) != nil
}

// Получение элемента по индексу
func (list *FList) GetByIndex(index int) (string, bool) {
	if index < 0 {
		return "", false
	}
	cur := list.head
	for i := 0; i < index && cur != nil; i++ {
		cur = cur.next
	}
	if cur == nil {
		return "", false
	}
	return cur.data, true
}

// Поиск узла, стоящего перед node (ссылки назад нет, поэтому идём с головы)
func (list *FList) findPrev(node *Node) *Node {
	for cur := list.head; cur != nil; cur = cur.next {
		if cur.next == node {
			return cur
		}
	}
	return nil
}

// Отвязка узла node от списка, возвращает его значение.
// Память освобождает сборщик мусора
func (list *FList) removeNode(node *Node) string {
	prev := list.findPrev(node)

	if prev == nil {
		list.head = node.next // удаляется голова
	} else {
		prev.next = node.next
	}
	if node == list.tail {
		list.tail = prev // удаляется хвост
	}
	return node.data
}

// Добавление в голову
func (list *FList) AddHead(data string) {
	newNode := &Node{data: data, next: list.head}
	list.head = newNode

	if list.tail == nil {
		list.tail = newNode
	}
}

// Добавление в хвост
func (list *FList) AddTail(data string) {
	newNode := &Node{data: data}

	if list.tail == nil {
		list.head = newNode
	} else {
		list.tail.next = newNode
	}
	list.tail = newNode
}

// Добавление перед элементом target
func (list *FList) AddBefore(target, data string) bool {
	node := list.FindNode(target)
	if node == nil {
		return false
	}

	if node == list.head {
		list.AddHead(data)
	} else {
		prev := list.findPrev(node)
		prev.next = &Node{data: data, next: node}
	}
	return true
}

// Добавление после элемента target
func (list *FList) AddAfter(target, data string) bool {
	node := list.FindNode(target)
	if node == nil {
		return false
	}

	node.next = &Node{data: data, next: node.next}
	if node == list.tail {
		list.tail = node.next
	}
	return true
}

// Удаление головы
func (list *FList) DeleteHead() (string, bool) {
	if list.head == nil {
		return "", false
	}
	return list.removeNode(list.head), true
}

// Удаление хвоста (нужен предпоследний узел — проход по списку)
func (list *FList) DeleteTail() (string, bool) {
	if list.tail == nil {
		return "", false
	}
	return list.removeNode(list.tail), true
}

// Удаление элемента перед target
func (list *FList) DeleteBefore(target string) (string, bool) {
	node := list.FindNode(target)
	if node == nil || node == list.head {
		return "", false
	}
	return list.removeNode(list.findPrev(node)), true
}

// Удаление элемента после target
func (list *FList) DeleteAfter(target string) (string, bool) {
	node := list.FindNode(target)
	if node == nil || node.next == nil {
		return "", false
	}
	return list.removeNode(node.next), true
}

// Удаление элемента по значению
func (list *FList) DeleteByValue(value string) bool {
	node := list.FindNode(value)
	if node == nil {
		return false
	}
	list.removeNode(node)
	return true
}
