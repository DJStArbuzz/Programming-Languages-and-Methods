#pragma once
// Заголовочный файл для описания узлов дерева разбора. 
#ifndef NODE_H 
#define NODE_H 

#include <string> 
#include <vector> 
#include <memory> 
#include "Token.h" 

using namespace std;
// Класс Node представляет узел в дереве разбора. 
class Node {
public:
    Node(const string& name, const Token& token = Token()); // Конструктор 
    void addChild(shared_ptr<Node> child); // Добавить дочерний узел 
    const string& getName() const; // Получить имя узла 
    const Token& getToken() const; // Получить токен, связанный с узлом 
    const vector<shared_ptr<Node>>& getChildren() const; // Получить дочерние узлы 


private:
    string name; // Имя узла 
    Token token; // Связанный токен 
    vector<shared_ptr<Node>> children; // Дочерние узлы 
};

// Вспомогательная функция для печати дерева в консоль. 
void printTree(const shared_ptr<Node>& node, int indent = 0);

#endif // NODE_H
