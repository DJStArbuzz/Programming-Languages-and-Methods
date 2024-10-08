#include <iostream>
#include "LexicalAnalyzer.h"
#include "Token.h"

/*  Лабораторная работа №1, вариант 7
    Лексический анализатор
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

int main() {
    setlocale(LC_ALL, "RUS");
    // Лексический анализ
    LexicalAnalyzer firstStage("input.txt", "output.txt");
    firstStage.lexAnBeg();
    
    // Синтаксический анализ

    // Семантический анализ
    return 0;
}