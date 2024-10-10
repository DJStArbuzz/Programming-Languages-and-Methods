#include "Token.h"

using namespace std;

// Recognition of the type of token for information output
string Token::getTypeStr() {
        string tokenType = "UNKNOWN";

        switch (this->type) {
        case TokenType::FUNCTION:
            tokenType = "FUNCTION";
            break;
        case TokenType::BEGIN:
            tokenType = "BEGIN";
            break;
        case TokenType::END:
            tokenType = "END";
            break;
        case TokenType::FN:
            tokenType = "FN";
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
        case TokenType::EXPR:
            tokenType = "EXPR";
            break;
        case TokenType::CONDITION:
            tokenType = "CONDITION";
            break;
        case TokenType::ROP:
            tokenType = "ROP";
            break;
        case TokenType::ID:
            tokenType = "ID";
            break;
        case TokenType::CONST:
            tokenType = "CONST";
            break;
        case TokenType::DELIMITER:
            tokenType = "DELIMITER";
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