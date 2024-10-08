#include "Token.h"

using namespace std;

// Recognition of the type of token for information output
string Token::getTypeStr() {
        string tokenType = "UNKNOWN";

        switch (this->type) {
        case TokenType::TYPE:
            tokenType = "TYPE";
            break;
        case TokenType::ID:
            tokenType = "ID";
            break;
        case TokenType::OP:
            tokenType = "OP";
            break;
        case TokenType::ROP:
            tokenType = "ROP";
            break;
        case TokenType::DELIMITER:
            tokenType = "DELIMITER";
            break;
        case TokenType::CONST:
            tokenType = "CONST";
            break;
        case TokenType::DESCR:
            tokenType = "DESCR";
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