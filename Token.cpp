#include "Token.h"

using namespace std;

int Token::getIndex() {
    return index;
}

// Recognition of the type of token for information output
string Token::getType() {
    string tokenType = "UNKNOWN";

    switch (type) {
    case TokenType::BEGIN:
        tokenType = "BEGIN";
        break;
    case TokenType::END:
        tokenType = "END";
        break;
    case TokenType::DESCRIPTIONS:
        tokenType = "DESCRIPTIONS";
        break;
    case TokenType::OPERATORS:
        tokenType = "OPERATORS";
        break;
    case TokenType::DESCR:
        tokenType = "DESCR";
        break;
    case TokenType::VARLIST:
        tokenType = "VARLIST";
        break;
    case TokenType::TYPE:
        tokenType = "TYPE";
        break;
    case TokenType::OP:
        tokenType = "OP";
        break;
    case TokenType::SIMPLEEXPR:
        tokenType = "SIMPLEEXPR";
        break;
    case TokenType::ROP:
        tokenType = "ROP";
        break;
    case TokenType::CONST:
        tokenType = "CONST";
        break;
    case TokenType::DELIMITER:
        tokenType = "DELIMITER";
        break;
    case TokenType::ID_NAME:
        tokenType = "ID_NAME";
        break;
    case TokenType::ERROR:
        tokenType = "ERROR";
        break;
    default:
        tokenType = "UNKNOWN";
        break;
    }

    return tokenType;
}