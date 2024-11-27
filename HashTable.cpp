#include "HashTable.h"
#include <iostream>
#include <iomanip>

using namespace std;

int HashTable::hashFunction(const string& lexeme) const {
    int hash = 0;
    for (char elem : lexeme)
        hash = (hash * 31 + elem) % HASH_SIZE;
    return hash;
}


void HashTable::addToken(const Token& token) {
    int hash = hashFunction(token.value);

    if (tokenSequence[hash] == nullptr) {
        tokenSequence[hash] = new Token(token.type, token.value, token.line, token.column, tokenCount, hash);
        tokenCount++;
    }
    else {
        if (tokenSequence[hash]->value == token.value)
            return;

        int newHash = (hash + 1) % HASH_SIZE;
        while (tokenSequence[newHash] != nullptr) {
            newHash = (newHash + 1) % HASH_SIZE;
        }
        tokenSequence[newHash] = new Token(token.type, token.value, token.line, token.line, tokenCount, newHash);
        tokenCount++;
    }

}

void HashTable::printTokensToFiles() {
    ofstream outputFile("resultLenAn.txt");
    ofstream errorFile("errorLenAn.txt");

    for (int i = 0; i < HASH_SIZE; i++) {
        Token* token = new Token();
        for (int j = 0; j < HASH_SIZE; j++) {
            if (tokenSequence[j] == nullptr)
                continue;

            if (i == getIndex(j)) {
                token = tokenSequence[j];
                break;
            }
        }

        if (token->type != TokenType::UNKNOWN && token->type != TokenType::ERROR) {
            string tokenType = token->getType();
            outputFile << setw(4) << token->index <<
                " | " << setw(10) << tokenType <<
                setw(10) << " | " << setw(10) << token->value <<
                setw(10) << " | " << setw(10) << token->hash << '\n';
        }
    }

    for (int i = 0; i < HASH_SIZE;i++) {
        Token* token = tokenSequence[i];
        if (tokenSequence[i] != nullptr && token->type == TokenType::ERROR) {
            errorFile << setw(4) << token->index <<
                " | " << setw(10) << "ERROR" <<
                setw(10) << " | " << setw(10) << token->value <<
                setw(10) << " | " << setw(10) << token->hash << '\n';
        }
    }
}


void HashTable::printTokensToConsole() {
    for (int i = 0; i < HASH_SIZE; i++) {
        Token* token = new Token();
        for (int j = 0; j < HASH_SIZE; j++) {
            if (tokenSequence[j] == nullptr) {
                continue;
            }

            if (i == getIndex(j)) {
                token = tokenSequence[j];
                break;
            }
        }
        if (token->type != TokenType::UNKNOWN && token->type != TokenType::ERROR) {
            // Проверяем, был ли токен уже выведен
            string tokenType = token->getType();

            cout << setw(4) << token->index <<
                " | " << setw(10) << tokenType <<
                setw(10) << " | " << setw(10) << token->value <<
                setw(10) << " | " << setw(10) << token->hash << '\n';
        }
    }

    cout << '\n';
    for (int i = 0; i < HASH_SIZE; i++) {
        Token* token = tokenSequence[i];
        if (tokenSequence[i] != nullptr && token->type == TokenType::ERROR) {
            cout << setw(4) << token->index <<
                " | " << setw(10) << "ERROR" <<
                setw(10) << " | " << setw(10) << token->value <<
                setw(10) << " | " << setw(10) << token->hash << '\n';
        }
    }
}

