#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <fstream>

using namespace std;

enum class TokenType {
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

class Token {
public:
    TokenType type;
    string lexeme;
    int index = -1;
    int hash;
    Token() : type(TokenType::UNKNOWN), lexeme(""), index(-1), hash(0) {}

    Token(TokenType t, const string& lex, int idx, int hsh) : type(t), lexeme(lex), index(idx), hash(hsh) {}

    int getIndex();   // print index
    string getType(); // Recognition of the type of token for information output
};


#endif 