#ifndef LEXICAL_ANALYZER_H
#define LEXICAL_ANALYZER_H

#include <iostream>
#include <fstream>
#include <string>
#include "HashTableClass.h"
#include "Token.h"

class LexicalAnalyzer {
public:
    LexicalAnalyzer(const string& input, const string& output) {
        testTxtFile.open(input);
        resultTxtFile.open(output);
    };
    ~LexicalAnalyzer() {
        if (testTxtFile.is_open())
            testTxtFile.close();
        if (resultTxtFile.is_open())
            resultTxtFile.close();
    };
    void lexAnBeg();
    string s;

private:
    ifstream testTxtFile;                             // cin
    ofstream resultTxtFile;                           // out
    HashTableClass tokenList;                         // Hash-table
    TokenType getNumber(const std::string& lexeme);
    TokenType unknownType(const string& lexeme);      // a special method for special cases
    bool isOperator(const string& str) const;         // Checking whether the operator is
    bool isRelationOperator(const string& str) const; // Checking whether a character is a comparison operator
    bool isRelationOperator(char str) const;          // Checking whether a character is a comparison operator
    bool isDelimiter(char c) const;                   // Checking whether the character is a separator
    bool isError(Token* tmp) const;            // Checking for an incorrect entry
    Token getNextLexeme();      
    // The method for obtaining a new token
    bool isValidIdentifier(const std::string& word) const; // Объявление функции
    bool isDigit(const string& word);
};

#endif