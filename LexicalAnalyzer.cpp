#include "LexicalAnalyzer.h"
#include "Token.h"
#include <vector>

using namespace std;

vector<char> forbiden = { '@', '#', '$', '%', '!', '?' };

bool LexicalAnalyzer::isForbid(char c) const {
    for (char t : forbiden) {
        if (t == c) {
            return true;
        }
    }
    return false;

}

bool LexicalAnalyzer::isError(const string& (str)) const {
    if (('0' <= str[0] <= '9') && (str.size() != 1))
        if ('z' < tolower(str[1]) || tolower(str[1]) < 'a')
            return true;

    for (char c : forbiden)
        if (count(str.begin(), str.end(), c) != 0)
            return true;
    return false;
}

bool LexicalAnalyzer::isRelationOperator(const std::string& str) const {
    return (str == "<" || str == ">" || str == "<=" || str == ">=" || str == "!=");
}

TokenType LexicalAnalyzer::unknownType(const string& lexeme) {
    if (lexeme == "return") return TokenType::OP;
    if (lexeme == "while") return TokenType::OP;
    if (lexeme == "int") return TokenType::TYPE;
    if (isRelationOperator(lexeme)) return TokenType::ROP;
    if (isError(lexeme)) {
        return TokenType::ERROR;
    }
    return TokenType::ID;
}

bool LexicalAnalyzer::isOperator(const string& str) const {
    return (str == "+" || str == "-" || str == "→" || str == "=");
}


bool LexicalAnalyzer::isRelationOperator(char str) const {
    return (str == '<' || str == '>');
}

bool LexicalAnalyzer::isDelimiter(char c) const {
    return (c == '(' || c == ')' || c == '[' || c == ']' || c == ',' || c == ';' || c == '{' || c == '}');
}


Token LexicalAnalyzer::getNextLexeme() {
    string word;
    char c;
    int index = 1; // Переменная для отслеживания индекса

    while (testTxtFile.get(c)) {
        if (isForbid(c)) 
        {
            word += c;
            while (testTxtFile.get(c) && (isalnum(c) || c == '_'))
                word += c;
            return Token(TokenType::ERROR, word, index);

        }
        if (isspace(c)) {
            index++; // Увеличиваем индекс для пробелов
            continue; // Игнорируем пробелы
        }

        // Проверка на одиночные операторы
        if (isOperator(std::string(1, c)))
            return Token(TokenType::OP, string(1, c), index);

        // Добавляем проверку на комментарий
        if (c == '/') {
            char nextChar;
            if (testTxtFile.get(nextChar) && nextChar == '/') { // Нашли начало комментария "//"
                while (testTxtFile.get(c) && c != '\n')
                    word += c; // Читаем комментарий до конца строки
                return Token(TokenType::DESCR, word, index);
            }
            else
                testTxtFile.unget(); // Если не комментарий, возвращаем символ обратно
        }

        if (isalpha(c) || c == '_') { // Идентификатор или ключевое слово
            word.clear();
            word += c;
            while (testTxtFile.get(c) && (isalnum(c) || c == '_'))
                word += c;
            testTxtFile.unget(); // Возвращаем последний символ в поток
            TokenType type = unknownType(word);
            return Token(type, word, index);
        }
        else if (isdigit(c)) { // Константа целого числа
            word.clear();
            word += c;
            while (testTxtFile.get(c) && isdigit(c)) {
                word += c;
            }
            testTxtFile.unget();
            return Token(TokenType::CONST, word, index);
        }
        else if (isDelimiter(c))
            return Token(TokenType::DELIMITER, string(1, c), index);
        else if (isRelationOperator(c)) {
            char nextChar;
            if (testTxtFile.get(nextChar) && nextChar == '=') {
                string tmp = string(1, c);
                tmp += nextChar;
                return Token(TokenType::ROP, tmp, index);
            }
            else
                return Token(TokenType::ROP, string(1, c), index);
        }
        else
            return Token(TokenType::ERROR, string(1, c), index);
    }
    return Token(TokenType::UNKNOWN, "", index);
}

void LexicalAnalyzer::lexAnBeg() {
    Token token;
    while ((token = getNextLexeme()).type != TokenType::UNKNOWN) {
        tokenList.addToken(token);
    }
    tokenList.printTokensFile(resultTxtFile);
    tokenList.printTokensConsole();
}