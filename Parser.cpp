#include "Parser.h"
#include <stdexcept>
#include <iostream>

using namespace std;

// Конструктор парсера. Инициализирует парсер лексером и вызывает метод advance для получения первого токена.
Parser::Parser(Lexer& lexer)
    : lexer(lexer) {
    advance();
}

// Переход к следующему токену из лексера.
void Parser::advance() {
    currentToken = lexer.getNextToken();
}

// Обработка ошибок синтаксиса. Формирует сообщение об ошибке и выбрасывает исключение.
void Parser::error(const string& message) {
    string errorMsg = "Syntax error at line " + to_string(currentToken.line) +
        ", column " + to_string(currentToken.column) + ": " + message;
    errors.push_back(errorMsg);
    throw std::runtime_error(errorMsg);
}

// Возвращает список найденных ошибок.
const vector<string>& Parser::getErrors() const {
    return errors;
}

// Главный метод парсера для синтаксического анализа.
shared_ptr<Node> Parser::parse() {
    return parse_Function();
}

// Парсинг функции на основе грамматики.
// Function → Begin Descriptions Operators End
std::shared_ptr<Node> Parser::parse_Function() {
    auto beginNode = parse_Begin();
    auto descriptionsNode = parse_Descriptions();
    auto operatorsNode = parse_Operators();
    auto endNode = parse_End();
    if (currentToken.value == "}") {
        endNode->addChild(make_shared<Node>(currentToken.value, currentToken));
        auto functionNode = make_shared<Node>("Function");
        functionNode->addChild(beginNode);
        functionNode->addChild(descriptionsNode);
        functionNode->addChild(operatorsNode);
        functionNode->addChild(endNode);
        return functionNode;
    }
    else {
        error("Expected '}' at the end of the function");
        return nullptr;
    }
}

// Begin → Type FunctionName ( ) {
shared_ptr<Node> Parser::parse_Begin() {
    auto typeNode = parse_Type();
    auto functionNameNode = parse_FunctionName();
    if (currentToken.value == "(") {
        auto openParenToken = currentToken;
        advance();
        if (currentToken.value == ")") {
            auto closeParenToken = currentToken;
            advance();
            if (currentToken.value == "{") {
                auto openBraceToken = currentToken;
                advance();
                auto beginNode = make_shared<Node>("Begin");
                beginNode->addChild(typeNode);
                beginNode->addChild(functionNameNode);
                beginNode->addChild(make_shared<Node>("{", openBraceToken)); // Add the '{' node here
                return beginNode;
            }
            else {
                error("Expected '{' after ')'");
            }
        }
        else {
            error("Expected ')' after '('");
        }
    }
    else {
        error("Expected '(' after the function name");
    }
    return nullptr;
}

// End → return Id ; }
shared_ptr<Node> Parser::parse_End() {
    if (currentToken.value != "return") {
        error("Operator 'return' not found");
        return nullptr;
    }
    auto returnToken = currentToken;
    advance();
    if (currentToken.type == ID) {
        auto idToken = currentToken;
        advance();
        if (currentToken.value == ";") {
            auto semicolonToken = currentToken;
            advance();
            auto endNode = make_shared<Node>("End");
            endNode->addChild(make_shared<Node>(returnToken.value, returnToken));
            endNode->addChild(make_shared<Node>("Id", idToken));
            endNode->addChild(make_shared<Node>(";", semicolonToken));

            return endNode;
        }
        else {
            error("Expected ';' after 'return'");
            return nullptr;
        }
    }
    else {
        error("Expected identifier after 'return'");
        return nullptr;
    }
    return nullptr;
}
// FunctionName → Id
shared_ptr<Node> Parser::parse_FunctionName() {
    if (currentToken.type == ID) {
        auto idToken = currentToken;
        advance();
        return make_shared<Node>("FunctionName", idToken);
    }
    else {
        error("Expected a function name");
        return nullptr;
    }
}

// Descriptions → Descr Descriptions | ε
shared_ptr<Node> Parser::parse_Descriptions() {
    auto descriptionsNode = make_shared<Node>("Descriptions");
    while (currentToken.value == "int") {
        auto descrNode = parse_Descr();
        descriptionsNode->addChild(descrNode);
    }
    return descriptionsNode;
}

// Descr → Type VarList ;
shared_ptr<Node> Parser::parse_Descr() {
    auto typeNode = parse_Type();
    auto varListNode = parse_VarList();
    if (currentToken.value == ";") {
        auto semicolonToken = currentToken;
        advance();
        auto descrNode = std::make_shared<Node>("Descr");
        descrNode->addChild(typeNode);
        descrNode->addChild(varListNode);
        descrNode->addChild(std::make_shared<Node>(";", semicolonToken));   
        return descrNode;
    }
    else {
        error("Expected ';' after variable list");
        return nullptr;
    }
}

// VarList → Id VarList'
shared_ptr<Node> Parser::parse_VarList() {
    auto varListNode = make_shared<Node>("VarList");
    if (currentToken.type == ID) {
        auto idToken = currentToken;
        advance();
        varListNode->addChild(make_shared<Node>("Id", idToken));
        while (currentToken.value == ",") {
            auto commaToken = currentToken;
            advance();
            if (currentToken.type == ID) {
                auto nextIdToken = currentToken;
                advance();
                varListNode->addChild(make_shared<Node>(",", commaToken));
                varListNode->addChild(make_shared<Node>("VarList"));
                varListNode->getChildren().back()->addChild(make_shared<Node>("Id", nextIdToken));
            }
            else {
                error("Expected identifier after ','");
                return nullptr;
            }
        }
        return varListNode;
    }
    else {
        error("Expected an identifier in the variable list");
        return nullptr;
    }
}

// Type → int
shared_ptr<Node> Parser::parse_Type() {
    if (currentToken.value == "int") {
        auto intToken = currentToken;
        advance();
        auto typeNode = make_shared<Node>("Type");
        typeNode->addChild(std::make_shared<Node>(intToken.value, intToken));
        return typeNode;
    }
    else {
        error("Expected type 'int'");
        return nullptr;
    }
}

// Operators → Op Operators | ε
shared_ptr<Node> Parser::parse_Operators() {
    auto operatorsNode = make_shared<Node>("Operators");
    while (currentToken.type != END_OF_FILE && currentToken.value != "}") {
        auto opNode = parse_Op();
        if (opNode == nullptr) break;
        operatorsNode->addChild(opNode);
    }
    return operatorsNode;
}

// Op → Id = Expr ; | for ( Init; Condition ; Expr ) { Operators }
shared_ptr<Node> Parser::parse_Op() {
    if (currentToken.type == ID) {
        // Id = Expr ;
        auto idToken = currentToken;
        advance();
        if (currentToken.value == "=") {
            auto equalToken = currentToken;
            advance();
            auto exprNode = parse_Expr();
            if (currentToken.value == ";") {
                auto semicolonToken = currentToken;
                advance();
                auto opNode = make_shared<Node>("Op");
                opNode->addChild(make_shared<Node>("Id", idToken));
                opNode->addChild(make_shared<Node>("=", equalToken));
                opNode->addChild(exprNode);
                opNode->addChild(make_shared<Node>(";", semicolonToken));
                return opNode;
            }
            else {
                error("Expected ';' after expression");
                return nullptr;
            }
        }
        else {
            error("Expected '=' after identifier");
            return nullptr;
        }
    }
    else if (currentToken.value == "while") {
        auto forToken = currentToken;
        advance();
        if (currentToken.value == "(") {
            auto openParenToken = currentToken;
            advance();  
            auto conditionNode = parse_Condition();
            if (currentToken.value == ")") {
                auto closeParenToken = currentToken;
                advance();
                if (currentToken.value == "{") {
                    auto openBraceToken = currentToken;
                    advance();
                    auto operatorsNode = parse_Operators();
                    if (currentToken.value == "}") {
                        auto closeBraceToken = currentToken;
                        advance();
                        auto opNode = std::make_shared<Node>("Op");
                        opNode->addChild(std::make_shared<Node>(forToken.value, forToken));
                        opNode->addChild(std::make_shared<Node>("(", openParenToken));

                        // Добавляем условие цикла
                        opNode->addChild(conditionNode);
                        opNode->addChild(std::make_shared<Node>(")", closeParenToken));
                        opNode->addChild(std::make_shared<Node>("{", openBraceToken));
                        opNode->addChild(operatorsNode);
                        opNode->addChild(std::make_shared<Node>("}", closeBraceToken));
                        return opNode;
                    }
                    else {
                        error("Expected '}' after the body of the loop");  // Ошибка: ожидался "}"
                        return nullptr;
                    }
                }
                else {
                    error("Expected '{' after ')'"); // Ошибка: ожидался "{"
                    return nullptr;
                }
            }
            else {
                error("Expected ')' after expression"); // Ошибка: ожидался ")"
                return nullptr;
            }

          
        }
        else {
            error("Expected '(' after 'for'"); // Ошибка: ожидался "(" после "for"
            return nullptr;
        }
    }
    else if (currentToken.value == "return") {
        return nullptr;

    }
    else {
        error("Unknown operation"); // Ошибка: неизвестная операция
        return nullptr;
    }
}

// SimpleExpr → Id | Const | ( Expr )
shared_ptr<Node> Parser::parse_SimpleExpr() {
    if (currentToken.type == ID) {
        auto idToken = currentToken;
        advance();
        return make_shared<Node>("SimpleExpr", idToken);
    }
    else if (currentToken.type == CONST) {
        auto constToken = currentToken;
        advance();
        return make_shared<Node>("SimpleExpr", constToken);
    }
    else if (currentToken.value == "(") {
        auto openParenToken = currentToken;
        advance();
        auto exprNode = parse_Expr();
        if (currentToken.value == ")") {
            auto closeParenToken = currentToken;
            advance();
            auto simpleExprNode = make_shared<Node>("SimpleExpr");
            simpleExprNode->addChild(make_shared<Node>("(", openParenToken));
            simpleExprNode->addChild(exprNode);
            simpleExprNode->addChild(make_shared<Node>(")", closeParenToken));

            return simpleExprNode;
        }
        else {
            error("Expected ')' after '('");
            return nullptr;
        }
    }
    else {
        error("Expected a simple expression");
        return nullptr;
    }
}

// Expr → SimpleExpr Expr'
shared_ptr<Node> Parser::parse_Expr() {
    auto leftNode = parse_SimpleExpr();
    while (currentToken.value == "+" || currentToken.value == "-") {
        auto opToken = currentToken;
        advance();
        auto rightNode = parse_SimpleExpr();
        auto exprNode = std::make_shared<Node>("Expr");
        exprNode->addChild(leftNode);
        exprNode->addChild(std::make_shared<Node>(opToken.value, opToken));
        exprNode->addChild(rightNode);
        leftNode = exprNode;

    }
    return leftNode;
}

// Condition → Expr RelationOperators Expr
shared_ptr<Node> Parser::parse_Condition() {
    auto leftExpr = parse_Expr();
    if (isRelationOperator(currentToken.value)) {
        auto relOpToken = currentToken;
        advance();
        auto rightExpr = parse_Expr();
        auto conditionNode = make_shared<Node>("Condition");
        conditionNode->addChild(leftExpr);
        conditionNode->addChild(make_shared<Node>("RelationsOperators", relOpToken));
        conditionNode->addChild(rightExpr);
        conditionNode->addChild(make_shared<Node>(";", currentToken));
        return conditionNode;
    }
    else {
        error("Expected a relational operator in condition");
        return nullptr;
    }
}

// RelationOperators → == | != | < | > | <= | >=
bool Parser::isRelationOperator(const string& op) {
    return op == "==" || op == "!=" || op == "<" || op == ">" || op == "<=" || op == ">=";
}