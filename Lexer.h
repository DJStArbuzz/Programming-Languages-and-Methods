#pragma once
// Lexer.h 
#ifndef LEXER_H 
#define LEXER_H 

#include <string> 
#include <vector> 
#include "Token.h" 
#include "HashTable.h"


using namespace std;

// ����� Lexer ��������� ����������� ������ �������� ������. 
class Lexer {
public:

    Lexer(const std::string& input); // �����������, ��������� ������ � �������� �������. 
    Token getNextToken(); // ���������� ��������� ����� �� ������. 
    const vector<Token>& getTokens() const;  // ���������� ��� ������, ���������� �� ������. 

private:
    string input; // ������� ����� 
    size_t position; // ������� ������� � ������ 
    int line; // ������� ������ 
    int column;  // ������� ������� 
    
    vector<Token> tokens; // ������ ������� 
    HashTable tokenList;

    char peek() const; // ���������� ������� ������, �� ������� �������. 
    void advance(); // ��������� � ���������� �������. 
    void skipWhitespaceAndComments(); // ���������� ������� � �����������. 
    Token identifier(); // �������� ������������� (�����, ����������). 
    Token number(); // �������� �������� ���������. 
    Token symbol(); // �������� ������ (�������� ��� �����������). 
    void tokenize(); // �������� ����� ��� ��������� �������� ������. 
    int currIndex;

    bool isOperator(char elem) const;                       // Checking whether the operator is
    bool isRelationOperator(char elem) const;               // Checking whether a character is a comparison operator
    bool isDelimiter(const string& elem) const;                      // Checking whether the character is a separator
    bool isDigit(const string& lexeme);                     // checking for exceptional numbers
    bool isValidIdentifier(const string& lexeme) const;     // checking for the exceptional presence of letters
    Token isKeyWord(const string &lexeme, int ind, char c, int index, int num); // checking for int, while, return
};

#endif // LEXER_H
