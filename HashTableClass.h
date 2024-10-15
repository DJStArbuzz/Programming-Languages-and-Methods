#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "Token.h"
#include <fstream>

using namespace std;

class HashTable {
public:
    // constructor and destructor
    HashTable() : tokenCount(0) {
        for (int i = 0; i < 100; ++i)
            tokenSequence[i] = nullptr;
    };
    ~HashTable() {
        for (long long i = 0; i < 100; i++)
            delete tokenSequence[i];
    };

    // adding an element with a hash
    void addToken(const Token& token);
    int hashFunction(const string& lexeme) const;

    // output arrays to the console and text files
    void printTokensToFiles(ofstream& outputFile, ofstream& errorFile);
    void printTokensToConsole();

    int getIndex(int i) {
        return tokenSequence[i]->getIndex();
    }

private:
    static const int HASH_SIZE = 10000;  // size max
    Token* tokenSequence[HASH_SIZE];     // an array of tokens
    int tokenCount;                      // for indexing
};

#endif