#ifndef TOKENLIST_H
#define TOKENLIST_H

#include "Token.h"
#include <fstream>

using namespace std;

class HashTableClass {
private:
    int tokenMaxCount;                            // Counter for the number of tokens added
    static const long long MAX_TOKENS = 1000;           // maximum number of tokens
    static const long long HASH_TABLE_SIZE = 1000;

    Token* hashTable[HASH_TABLE_SIZE];            // arrays of tokens
    Token* tokenSequence[MAX_TOKENS];
public:
    HashTableClass() {                            // constructor
        for (long long i = 0; i < HASH_TABLE_SIZE; i++)
            hashTable[i] = nullptr;
        for (long long i = 0; i < MAX_TOKENS; i++)
            tokenSequence[i] = nullptr;
    };                          
    ~HashTableClass() {                           // destructor
        for (long long i = 0; i < HASH_TABLE_SIZE; i++)
            delete hashTable[i];
        for (long long i = 0; i < MAX_TOKENS; i++)
            delete tokenSequence[i];
    };                            

    void addToken(const Token& token);            // adding a token
    int hashFunction(const string& lexeme) const; // hash function
    void printTokensFile(ofstream& outputFile);   // output an array of tokens to a file
    void printTokensConsole();                    // output an array of tokens to a console

    void resize() {                               // size correction
        long long newSz = this->HASH_TABLE_SIZE * 2 + 1;
        Token** newArr = new Token * [newSz];
        for (long long i = 0; i < newSz; i++)
            newArr[i] = nullptr; 
        for (long long i = 0; i < HASH_TABLE_SIZE; i++) {
            if (hashTable[i] != nullptr) {
                int newIndex = hashFunction(hashTable[i]->lexeme) % newSz;
                while (newArr[newIndex] != nullptr)
                    newIndex = (newIndex + 1) % newSz; 
                newArr[newIndex] = hashTable[i];
            }
        }
        delete[] hashTable;
    };
};

#endif // TOKENLIST_H