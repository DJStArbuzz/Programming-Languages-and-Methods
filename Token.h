#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <fstream>

using namespace std;

/*
    Function → Begin Descriptions Operators End
    Begin → Type FunctionName() {
    End → return Id ; }
    FunctionName → Id
    Descriptions → Descr | Descr Descriptions
    Operators → Op | Op Operators
    Descr → Type VarList ;
    VarList → Id | Id , VarList
    Type → int
    Op → Id = Expr ; | while ( Condition ) { Operators }
    SimpleExpr → Id | Const | ( Expr ) Expr → SimpleExpr | SimpleExpr + Expr | SimpleExpr - Expr
    Condition → Expr RelationOperators Expr
    RelationOperators → == | != | < | > | <= | >=
    Id → id_name
    Const → int_num
*/

enum class TokenType {
    FUNCTION,
    BEGIN,
    END,
    FN,
    DESCRIPTIONS,
    OPERATORS,
    DESCR,
    VARLIST,
    TYPE,
    OP,
    SIMPLEEXPR,
    EXPR,
    CONDITION,
    ROP,
    ID,
    CONST,
    DELIMITER,
    ERROR,
    UNKNOWN
};

class Token {
public:
    TokenType type;
    string lexeme;
    int index;

    Token() : type(TokenType::UNKNOWN), lexeme(""), index(-1) {}
    Token(TokenType t, const string& lex, int idx) : type(t), lexeme(lex), index(idx) {}

    string getTypeStr(); // Recognition of the type of token for information output
};

#endif 