#pragma once
// ������������ ���� ��� �������. ���������� ��������� ������ Parser. 
#ifndef PARSER_H 
#define PARSER_H 

#include "Lexer.h" 
#include "Node.h" 
#include <vector> 
#include <memory> 

using namespace std;

// ����� Parser ������������ �������������� ������ �� ������ ������� �� Lexer. 
class Parser {
public:
    Parser(Lexer& lexer); // ����������� 
    
    shared_ptr<Node> parse(); // �������� ����� ������� 
    const vector<string>& getErrors() const; // �������� ������ ������ 

private:
    Token currentToken; // ������� ����� 
    Lexer& lexer; // ������ 
    vector<string> errors; // ������ ������ 

    // ��������������� ������ 
    void advance();
    void error(const string& message);

    // ������ ��� ��������� ��������� ���������� 
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
    bool isRelationOperator(const std::string& op);  // �������� �� �������� ��������� 
};

#endif // PARSER_H
