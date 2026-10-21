package structures

// Узел стека
type SNode struct {
	data string
	next *SNode
}

// Стек на односвязном списке, хранится только вершина (LIFO).
// Пустой стек создаётся просто объявлением: var stack Stack
type Stack struct {
	top *SNode
}

// Добавление элемента на вершину
func (stack *Stack) Push(data string) {
	stack.top = &SNode{data: data, next: stack.top}
}

// Чтение и удаление элемента с вершины
func (stack *Stack) Pop() (string, bool) {
	if stack.top == nil {
		return "", false
	}
	removed := stack.top.data
	stack.top = stack.top.next
	return removed, true
}

// Вывод от вершины ко дну
func (stack *Stack) ToString() string {
	result := ""
	for cur := stack.top; cur != nil; cur = cur.next {
		if cur != stack.top {
			result += " | "
		}
		result += cur.data
	}
	return result
}

// Строка для файла от дна к вершине,
// чтобы при загрузке Push восстановил тот же порядок
func (stack *Stack) ToFile() string {
	return bottomUp(stack.top)
}

// Рекурсия: сначала элементы под узлом, потом сам узел
func bottomUp(node *SNode) string {
	if node == nil {
		return ""
	}
	below := bottomUp(node.next)
	if below == "" {
		return node.data
	}
	return below + " " + node.data
}
