#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H

#include <iostream>
#include <fstream>
#include <string>
#include "HashTable.h"
#include "SintaksisAnalyzer.h"

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

    SyntaxAnalyzer sintaksis_analyzer;
    string lexeme;
    int count_line = 0;
    int count = 0;
    void lexAnBeg();

    TokenList& getTokenList() {
        return tokenList;
    }

private:
    ifstream testTxtFile;                                   // cin
    ofstream resultTxtFile;                                 // out
    ofstream errorTxtFile;
    TokenList tokenList;
    string sent = "";
    const string tree = "parsing_tree.txt";

    Token getNextLexeme();
    Token isKeyWord(ifstream& testTxtFile, int index, int num, char c); // checking for int, while, return
    bool isOperator(char elem) const;                       // Checking whether the operator is
    bool isRelationOperator(char elem) const;               // Checking whether a character is a comparison operator
    bool isDelimiter(char elem) const;                      // Checking whether the character is a separator
    bool isDigit(const string& lexeme);                     // checking for exceptional numbers
    bool isValidIdentifier(const string& lexeme) const;     // checking for the exceptional presence of letters
};

#endif // LEXICALANALYZER_H
