#pragma once

#include "constants.h"

#include <vector>
#include <memory.h>

class TranspositionTable {
public:

	TranspositionTable(unsigned int maxSize) : size(maxSize) {
		table = (HashScore*)malloc(size*sizeof(HashScore));
		memset(table, 0, size*sizeof(HashScore));
	}

	~TranspositionTable() {
		free(table);
	}
	
	int* Find(int hashing) {
		if (hashing == 0) return NULL; //Reserved value, can't be used as a hash
		HashScore& hs = table[hashing%size];
		if (hs.hash != 0) return &hs.score;
		else return NULL;
	}

	void Insert(int hash, int score) {
		table[hash%size].set(hash,score);
	}

	void Clear() {
		memset(table, 0, size*sizeof(HashScore));
	}
private:
	struct HashScore {
		int hash;
		int score;
		void set(int h, int s) { hash = h; score = s; }
	};

	unsigned int size;
	HashScore* table;
	
};
