package structures

// Узел двусвязного списка, хранит ссылки на соседей с обеих сторон
type DNode struct {
	data string
	prev *DNode
	next *DNode
}

// Двусвязный список, ссылка prev даёт удаление хвоста за O(1).
// Пустой список создаётся просто объявлением: var list DList
type DList struct {
	head *DNode
	tail *DNode
}

// Вывод от головы к хвосту через разделитель sep
func (list *DList) ToString(sep string) string {
	result := ""
	for cur := list.head; cur != nil; cur = cur.next {
		if cur != list.head {
			result += sep
		}
		result += cur.data
	}
	return result
}

// Вывод от хвоста к голове по ссылкам prev, без рекурсии
func (list *DList) ToStringReverse() string {
	result := ""
	for cur := list.tail; cur != nil; cur = cur.prev {
		if cur != list.tail {
			result += " <-> "
		}
		result += cur.data
	}
	return result
}

// Поиск узла по значению, nil если не найден
func (list *DList) FindNode(value string) *DNode {
	for cur := list.head; cur != nil; cur = cur.next {
		if cur.data == value {
			return cur
		}
	}
	return nil
}

// Проверка, есть ли элемент в списке
func (list *DList) Contains(value string) bool {
	return list.FindNode(value) != nil
}

// Получение элемента по индексу
func (list *DList) GetByIndex(index int) (string, bool) {
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

// Вставка нового узла между left и right (любой из них может быть nil)
func (list *DList) insertBetween(left, right *DNode, data string) {
	newNode := &DNode{data: data, prev: left, next: right}

	if left == nil {
		list.head = newNode
	} else {
		left.next = newNode
	}
	if right == nil {
		list.tail = newNode
	} else {
		right.prev = newNode
	}
}

// Отвязка узла node за O(1): соседи известны через prev и next
func (list *DList) removeNode(node *DNode) string {
	if node.prev == nil {
		list.head = node.next
	} else {
		node.prev.next = node.next
	}
	if node.next == nil {
		list.tail = node.prev
	} else {
		node.next.prev = node.prev
	}
	return node.data
}

// Добавление в голову
func (list *DList) AddHead(data string) {
	list.insertBetween(nil, list.head, data)
}

// Добавление в хвост
func (list *DList) AddTail(data string) {
	list.insertBetween(list.tail, nil, data)
}

// Добавление перед элементом target
func (list *DList) AddBefore(target, data string) bool {
	node := list.FindNode(target)
	if node == nil {
		return false
	}
	list.insertBetween(node.prev, node, data)
	return true
}

// Добавление после элемента target
func (list *DList) AddAfter(target, data string) bool {
	node := list.FindNode(target)
	if node == nil {
		return false
	}
	list.insertBetween(node, node.next, data)
	return true
}

// Удаление головы
func (list *DList) DeleteHead() (string, bool) {
	if list.head == nil {
		return "", false
	}
	return list.removeNode(list.head), true
}

// Удаление хвоста
func (list *DList) DeleteTail() (string, bool) {
	if list.tail == nil {
		return "", false
	}
	return list.removeNode(list.tail), true
}

// Удаление элемента перед target
func (list *DList) DeleteBefore(target string) (string, bool) {
	node := list.FindNode(target)
	if node == nil || node.prev == nil {
		return "", false
	}
	return list.removeNode(node.prev), true
}

// Удаление элемента после target
func (list *DList) DeleteAfter(target string) (string, bool) {
	node := list.FindNode(target)
	if node == nil || node.next == nil {
		return "", false
	}
	return list.removeNode(node.next), true
}

// Удаление элемента по значению
func (list *DList) DeleteByValue(value string) bool {
	node := list.FindNode(value)
	if node == nil {
		return false
	}
	list.removeNode(node)
	return true
}
