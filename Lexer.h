#pragma once
// Lexer.h 
#ifndef LEXER_H 
#define LEXER_H 

#include <string> 
#include <vector> 
#include "Token.h" 
#include "HashTable.h"


using namespace std;

// Класс Lexer выполняет лексический анализ входного текста. 
class Lexer {
public:

    Lexer(const std::string& input); // Конструктор, принимает строку с исходным текстом. 
    Token getNextToken(); // Возвращает следующий токен из потока. 
    const vector<Token>& getTokens() const;  // Возвращает все токены, полученные из текста. 

private:
    string input; // Входной текст 
    size_t position; // Текущая позиция в тексте 
    int line; // Текущая строка 
    int column;  // Текущий столбец 
    
    vector<Token> tokens; // Список токенов 
    HashTable tokenList;

    char peek() const; // Возвращает текущий символ, не изменяя позицию. 
    void advance(); // Переходит к следующему символу. 
    void skipWhitespaceAndComments(); // Пропускает пробелы и комментарии. 
    Token identifier(); // Выделяет идентификатор (слова, переменные). 
    Token number(); // Выделяет числовую константу. 
    Token symbol(); // Выделяет символ (оператор или разделитель). 
    void tokenize(); // Основной метод для обработки входного текста. 
    int currIndex;

    bool isOperator(char elem) const;                       // Checking whether the operator is
    bool isRelationOperator(char elem) const;               // Checking whether a character is a comparison operator
    bool isDelimiter(const string& elem) const;                      // Checking whether the character is a separator
    bool isDigit(const string& lexeme);                     // checking for exceptional numbers
    bool isValidIdentifier(const string& lexeme) const;     // checking for the exceptional presence of letters
    Token isKeyWord(const string &lexeme, int ind, char c, int index, int num); // checking for int, while, return
};

#endif // LEXER_H
