#include "Chunk.cpp"

class List {
private:
	unsigned int Chunks = 1;

	void AddChunk() {
		Chunk * newChunk = new Chunk();

		Feet->Next = newChunk;
		newChunk->Prev = Feet;
		Feet = newChunk;

		Chunks++;
	}

	void DeleteChunk() {
		Feet = Feet->Prev;
		delete Feet->Next;
		Feet->Next = 0x0;
		Chunks--;
	}

public:
	Chunk* Head = 0x0;
	Chunk* Feet = 0x0;

	void Add(Object* Item) {
		if (Feet->IsFull()) AddChunk();
		Feet->Add(Item);
	}

	void SetAt(unsigned int Chunk, unsigned int Pos, Object* Value) {
		ChunkAt(Chunk)->SetAt(Pos, Value);
	}

	Object* ObjectAt(unsigned int Chunk, unsigned int Pos) {
		return ChunkAt(Chunk)->ObjectAt(Pos);
	}

	void RemoveAt(unsigned int Chunk, unsigned int Pos) {
		ChunkAt(Chunk)->RemoveAt(Pos);
	}

	void RemoveAll(Object* Item) {
		unsigned int P;
		Chunk* C = Head;
		for (int i = 0; i < Chunks; i++) {
			P = Head->FindObject(Item);
			if (P != -1) C->RemoveAt(P);
		}
	}

	void RemoveFirst(Object* Item) {
		unsigned int P;
		Chunk* C = Head;
		for (int i = 0; i < Chunks; i++) {
			P = Head->FindObject(Item);
			if (P != -1) { C->RemoveAt(P); return; }
		}
	}

	bool Contains(Object* Value) {
		return ChunkContaining(Value) != NULL;
	}

	Chunk* ChunkContaining(Object* Value) {
		Chunk* C = Head;
		while (C != NULL) {
			if (C->ContainsObject(Value)) return C;
			C = C->Next;
		}
		return NULL;
	}

	Chunk* ChunkAt(unsigned int Pos) {
		Chunk* C;
		if (Chunks / 2 > Pos) {
			C = Head;
			for (unsigned int i = 0; i < Pos; i++) { C = C->Next; }
		}
		else {
			C = Feet;
			for (unsigned int i = Chunks-1; i > Pos; i--) { C = C->Prev; }
		}
		return C;
	}

	List() {
		Chunk* newChunk = new Chunk();
		Head = newChunk;
		Feet = newChunk;
	}

	void Optimise() {
		unsigned int nulls = 0;
		Chunk* chunk = Head;
		Object* Temp;

		for (unsigned int C = 0, P = 0; C < Chunks;) {
			Temp = ObjectAt(C, P);

			chunk->SetAt(P, NULL);
			chunk->Items--;

			if (Temp == NULL) { nulls++; chunk->Items++; }
			else if (nulls<=P) {
				chunk->SetAt(P - nulls, Temp);
				chunk->Items++;
			}
			else if (nulls+P<10) {
				chunk->Prev->SetAt(chunkSize - nulls + P, Temp);
				chunk->Prev->Items++;
			}
			else {
				SetAt(C - ((P + nulls) / chunkSize), chunkSize - nulls + P, Temp);
				ChunkAt(C - ((P + nulls) / chunkSize))->Items++;
			}

			P++;
			if (P == chunkSize) {
				C++;
				P = 0;
				chunk = ChunkAt(C);
			}
		}

		while (chunk->IsEmpty()) DeleteChunk();
	}
};

int main()
{
	List* L = new List();
	for (int i = 0; i < 25; i++) {
		L->Add(new Object(i));
	}

	L->RemoveAt(0, 1);
	L->RemoveAt(0, 5);
	L->RemoveAt(0, 7);
	L->RemoveAt(1, 2);
	L->RemoveAt(1, 3);
	L->RemoveAt(1, 8);
	L->RemoveAt(2, 0);
	L->RemoveAt(2, 2);
	L->RemoveAt(2, 4);

	L->Optimise();
}
