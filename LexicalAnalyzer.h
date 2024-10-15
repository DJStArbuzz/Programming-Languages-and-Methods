#ifndef LEXICAL_ANALYZER_H
#define LEXICAL_ANALYZER_H

#include <iostream>
#include <fstream>
#include <string>
#include "HashTableClass.h"
#include "Token.h"

using namespace std;
class LexicalAnalyzer {
public:
    LexicalAnalyzer(const string& input, const string& output, const string& error) {
        testTxtFile.open(input);
        resultTxtFile.open(output);
        errorTxtFile.open(error);
    };
    ~LexicalAnalyzer() {
        if (testTxtFile.is_open())
            testTxtFile.close();
        if (resultTxtFile.is_open())
            resultTxtFile.close();
        if (errorTxtFile.is_open())
            errorTxtFile.close();
    };
    void lexAnBeg();

private:
    ifstream testTxtFile;                                   // cin
    ofstream resultTxtFile;                                 // out
    ofstream errorTxtFile;                                  // out with error
    
    HashTable tokenList;                                    // Hash-table

    bool isOperator(char elem) const;                       // Checking whether the operator is
    bool isRelationOperator(char elem) const;               // Checking whether a character is a comparison operator
    bool isDelimiter(char elem) const;                      // Checking whether the character is a separator
    bool isDigit(const string& lexeme);                     // checking for exceptional numbers
    bool isValidIdentifier(const string& lexeme) const;     // checking for the exceptional presence of letters
    
    // checking the next token
    Token getNextLexeme();
    // checking for int, while, return
    Token isKeyWord(ifstream& testTxtFile, int index, int num, char c);
  };

#endif