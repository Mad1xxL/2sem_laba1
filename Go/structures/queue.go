package structures

// Узел очереди
type QNode struct {
	data string
	next *QNode
}

// Очередь: добавление в хвост, извлечение из головы (FIFO), обе операции за O(1).
// Пустая очередь создаётся просто объявлением: var queue Queue
type Queue struct {
	head *QNode
	tail *QNode
}

// Добавление элемента в хвост
func (queue *Queue) Push(data string) {
	newNode := &QNode{data: data}

	if queue.tail == nil {
		queue.head = newNode
	} else {
		queue.tail.next = newNode
	}
	queue.tail = newNode
}

// Чтение и удаление элемента из головы
func (queue *Queue) Pop() (string, bool) {
	if queue.head == nil {
		return "", false
	}
	removed := queue.head.data
	queue.head = queue.head.next
	if queue.head == nil {
		queue.tail = nil // очередь опустела
	}
	return removed, true
}

// Вывод от головы к хвосту через разделитель sep
func (queue *Queue) ToString(sep string) string {
	result := ""
	for cur := queue.head; cur != nil; cur = cur.next {
		if cur != queue.head {
			result += sep
		}
		result += cur.data
	}
	return result
}
