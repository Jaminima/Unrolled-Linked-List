#include "Header.h"

class Object {
	unsigned int* i = 0;

public:
	Object(unsigned int s) {
		i = new unsigned int(s);
	}
};

class Chunk {
private:
	Object* Values[chunkSize];

public:
	unsigned int Items = 0;

	Chunk* Next = 0x0;
	Chunk* Prev = 0x0;

	bool IsFull() {
		return Items == chunkSize;
	}

	bool IsEmpty() {
		return Items == 0;
	}

	void Add(Object* Item) {
		Values[Items] = Item;
		Items++;
	}

	void SetAt(unsigned int Pos, Object* Item) {
		Values[Pos] = Item;
	}

	Object* ObjectAt(unsigned int Pos) {
		return Values[Pos];
	}

	bool ContainsObject(Object* Item) {
		return FindObject(Item) != -1;
	}

	unsigned int FindObject(Object* Item) {
		for (int i = 0; i < chunkSize; i++) {
			if (Values[i] == Item) return i;
		}
		return -1;
	}

	void RemoveAt(unsigned int Pos) {
		Values[Pos] = NULL;
		Items--;
	}
};