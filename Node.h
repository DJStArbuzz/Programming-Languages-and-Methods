#pragma once
// ������������ ���� ��� �������� ����� ������ �������. 
#ifndef NODE_H 
#define NODE_H 

#include <string> 
#include <vector> 
#include <memory> 
#include "Token.h" 

using namespace std;
// ����� Node ������������ ���� � ������ �������. 
class Node {
public:
    Node(const string& name, const Token& token = Token()); // ����������� 
    void addChild(shared_ptr<Node> child); // �������� �������� ���� 
    const string& getName() const; // �������� ��� ���� 
    const Token& getToken() const; // �������� �����, ��������� � ����� 
    const vector<shared_ptr<Node>>& getChildren() const; // �������� �������� ���� 


private:
    string name; // ��� ���� 
    Token token; // ��������� ����� 
    vector<shared_ptr<Node>> children; // �������� ���� 
};

// ��������������� ������� ��� ������ ������ � �������. 
void printTree(const shared_ptr<Node>& node, int indent = 0);

#endif // NODE_H
