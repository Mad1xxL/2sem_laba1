package structures

// Динамический массив: элементы лежат подряд в одном блоке памяти,

type Array struct {
	data     []string 
	size     int      
	capacity int      
}

// Увеличение ёмкости в 2 раза с переносом элементов в новый блок
func (arr *Array) grow() {
	newCapacity := 4
	if arr.capacity > 0 {
		newCapacity = arr.capacity * 2
	}
	newData := make([]string, newCapacity)

	for i := 0; i < arr.size; i++ {
		newData[i] = arr.data[i]
	}

	arr.data = newData
	arr.capacity = newCapacity
}

// Добавление элемента в конец
func (arr *Array) Push(value string) {
	if arr.size == arr.capacity {
		arr.grow()
	}
	arr.data[arr.size] = value
	arr.size++
}

// Добавление элемента по индексу, элементы правее сдвигаются вправо
func (arr *Array) Insert(index int, value string) bool {
	if index < 0 || index > arr.size {
		return false
	}
	if arr.size == arr.capacity {
		arr.grow()
	}

	for i := arr.size; i > index; i-- {
		arr.data[i] = arr.data[i-1]
	}
	arr.data[index] = value
	arr.size++
	return true
}

// Получение элемента по индексу
func (arr *Array) Get(index int) (string, bool) {
	if index < 0 || index >= arr.size {
		return "", false
	}
	return arr.data[index], true
}

// Замена элемента по индексу
func (arr *Array) Set(index int, value string) bool {
	if index < 0 || index >= arr.size {
		return false
	}
	arr.data[index] = value
	return true
}

// Удаление элемента по индексу, элементы правее сдвигаются влево
func (arr *Array) Delete(index int) (string, bool) {
	if index < 0 || index >= arr.size {
		return "", false
	}
	removed := arr.data[index]

	for i := index; i < arr.size-1; i++ {
		arr.data[i] = arr.data[i+1]
	}
	arr.size--
	return removed, true
}

// Длина массива
func (arr *Array) Length() int {
	return arr.size
}

// Все элементы одной строкой через разделитель sep
func (arr *Array) ToString(sep string) string {
	result := ""
	for i := 0; i < arr.size; i++ {
		if i > 0 {
			result += sep
		}
		result += arr.data[i]
	}
	return result
}
