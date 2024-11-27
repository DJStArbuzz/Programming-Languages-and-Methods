#pragma once
// Заголовочный файл для парсера. Определяет интерфейс класса Parser. 
#ifndef PARSER_H 
#define PARSER_H 

#include "Lexer.h" 
#include "Node.h" 
#include <vector> 
#include <memory> 

using namespace std;

// Класс Parser осуществляет синтаксический разбор на основе токенов из Lexer. 
class Parser {
public:
    Parser(Lexer& lexer); // Конструктор 
    
    shared_ptr<Node> parse(); // Основной метод разбора 
    const vector<string>& getErrors() const; // Получить список ошибок 

private:
    Token currentToken; // Текущий токен 
    Lexer& lexer; // Лексер 
    vector<string> errors; // Список ошибок 

    // Вспомогательные методы 
    void advance();
    void error(const string& message);

    // Методы для различных элементов грамматики 
    shared_ptr<Node> parse_Function();
    shared_ptr<Node> parse_Begin();
    shared_ptr<Node> parse_FunctionName();
    shared_ptr<Node> parse_Descriptions();
    shared_ptr<Node> parse_Descr();
    shared_ptr<Node> parse_VarList();
    shared_ptr<Node> parse_Type();
    shared_ptr<Node> parse_Operators();
    shared_ptr<Node> parse_Op();
    shared_ptr<Node> parse_SimpleExpr();
    shared_ptr<Node> parse_Expr();
    shared_ptr<Node> parse_Condition();
    shared_ptr<Node> parse_End();
    bool isRelationOperator(const std::string& op);  // Проверка на оператор сравнения 
};

#endif // PARSER_H
