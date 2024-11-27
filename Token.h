#pragma once
// Заголовочный файл описывает структуру токенов, которые генерирует лексер. 
#ifndef TOKEN_H 
#define TOKEN_H 

#include <string> 

using namespace std;

// Типы токенов, соответствующие различным элементам языка. 
enum TokenType {
    WORDS_KEY, // Ключевые слова 
    ID, // Идентификаторы 
    SYMBOLS_OF_OPERATION, // Операторы 
    SYMBOLS_OF_SEPARATING, // Разделители 
    END_OF_FILE, // Конец файла 


    PROGRAM,        // Program -> Begin Descriptions Operators End
    BEGIN,          // Type FunctionName() {
    END,            // return Id ; }
    DESCRIPTIONS,   // Descriptions -> Descr | Descr Descriptions
    DESCR,          // Descr -> Type VarList
    TYPE,           // Type -> INTEGER
    VARLIST,        // VarList -> Id | Id , VarList
    OP,             // Id = Expr ; | while ( Condition ) { Operators }
    OPERATORS,      // Op | Op Operators
    ID_NAME,        // a-z,A-Z
    CONST,          // 0-9
    OPERATOR,       // Op | Op Operators
    DELIMITER,      // ( ) [ ] { } 
    ERROR,          // Ошибки
    THEN,           // Then statement       
    SIMPLEEXPR,     // Id | Const | ( Expr ) Expr → SimpleExpr | SimpleExpr + Expr | SimpleExpr - Expr
    ROP,            // == | != | < | > | <= | >=
    UNKNOWN,
    END_LINE,
};

// Структура Token содержит информацию о типе, значении и местоположении токена. 
struct Token {
    TokenType type; // Тип токена 
    std::string value; // Текстовое значение токена 
    int line; // Номер строки 
    int column; // Номер столбца 
    //string tokenLexeme;
    int index;
    int hash;

    Token(TokenType type = UNKNOWN, const string& value = "", int line = 0, int column = 0, int index=0, int hash = 0)
        : type(type), value(value), line(line), column(column), index(index), hash(hash) {
        
    }

    int getIndex();  
    string getType();
};

#endif // TOKEN_H
