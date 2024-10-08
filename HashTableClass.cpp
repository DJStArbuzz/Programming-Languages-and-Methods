#include "HashTableClass.h"
#include <iostream>
#include <iomanip>

using namespace std;

// get key
int HashTableClass::hashFunction(const string& lexeme) const {
    int hash = 0;
    for (char c : lexeme)
        hash += c;
    return hash % HASH_TABLE_SIZE;
}

void HashTableClass::addToken(const Token& token) {
    if (tokenMaxCount < MAX_TOKENS) {
        tokenSequence[tokenMaxCount] = new Token(token.type, token.lexeme, token.index);
        tokenMaxCount++;
    }
    /*
    A key is supplied to the input of the hash function, and the output is an array index, 
    according to which this key should be located in the table. 

    Then a new object with a key and value is created in memory, 
    and the address of this object is stored in the corresponding table element.
    */
    int index = hashFunction(token.lexeme);
    while (hashTable[index] != nullptr && hashTable[index]->lexeme != token.lexeme)
        index = (index + 1) % HASH_TABLE_SIZE; // Пробирование при коллизии

    // adding a new token when the cell is empty
    if (hashTable[index] == nullptr)
        hashTable[index] = new Token(token.type, token.lexeme, token.index);
}

void HashTableClass::printTokensFile(ofstream& resultTxtFile) {
    resultTxtFile << "Синтаксический анализ кода на C++.\n";
    for (int i = 0; i < tokenMaxCount; i++) {
        Token* token = tokenSequence[i];
        string tokenType = token->getTypeStr();
   
        if (tokenSequence[i] != nullptr && token->type != TokenType::ERROR && token->type != TokenType::DESCR)
            resultTxtFile << setw(10) <<  tokenType <<
                setw(10) << " | " << setw(10) << token->lexeme <<
                setw(10) << " | " << setw(10) << i<< '\n';
        }
    
    resultTxtFile << "Лексемы, которые не удалось распознать:\n";
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        Token* token = tokenSequence[i];

        if (tokenSequence[i] != nullptr && token->type == TokenType::ERROR) {
            resultTxtFile << setw(10) << "ERROR" <<
                setw(10) << " | " << setw(10) << token->lexeme <<
                setw(10) << " | " << setw(10) << i << '\n';
        }
    }

    resultTxtFile << "Комментарии\n";
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        Token* token = tokenSequence[i];

        if (tokenSequence[i] != nullptr && token->type == TokenType::DESCR) {
            resultTxtFile << setw(10) << "DESCR" <<
                setw(10) << " | " << setw(10) << 
                token->lexeme << setw(10) << " | "
                << setw(10) << i << '\n';
        }
    }
}


void HashTableClass::printTokensConsole() {
    cout << "Синтаксический анализ кода на C++.\n";
    for (int i = 0; i < tokenMaxCount; i++) {
        Token* token = tokenSequence[i];
        string tokenType = token->getTypeStr();

        if (tokenSequence[i] != nullptr && token->type != TokenType::ERROR && token->type != TokenType::DESCR)
            cout << setw(10) << tokenType <<
            setw(10) << " | " << setw(10) << token->lexeme <<
            setw(10) << " | " << setw(10) << i << '\n';
    }

    cout << "Лексемы, которые не удалось распознать:\n";
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        Token* token = tokenSequence[i];

        if (tokenSequence[i] != nullptr && token->type == TokenType::ERROR) {
            cout << setw(10) << "ERROR" <<
                setw(10) << " | " << setw(10) << token->lexeme <<
                setw(10) << " | " << setw(10) << i << '\n';
        }
    }

    cout << "Комментарии\n";
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        Token* token = tokenSequence[i];

        if (tokenSequence[i] != nullptr && token->type == TokenType::DESCR) {
            cout << setw(10) << "DESCR" <<
                setw(10) << " | " << setw(10) <<
                token->lexeme << setw(10) << " | "
                << setw(10) << i << '\n';
        }
    }
}