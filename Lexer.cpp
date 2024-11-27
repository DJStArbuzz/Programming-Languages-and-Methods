#include "Lexer.h" 
#include <cctype> 
#include <stdexcept> 

using namespace std;

Lexer::Lexer(const string& input)
    : input(input), position(0), line(1), column(1) {
    tokenize();
}

bool Lexer::isDigit(const string& lexeme) {
    for (char c : lexeme)
        if (!isdigit(c))
            return false;
    return true;
}


bool Lexer::isValidIdentifier(const string& lexeme) const {
    for (char elem : lexeme)
        if (!isalpha(elem))
            return false;

    return true;
}


bool Lexer::isOperator(char elem) const {
    return (elem == '+' || elem == '-');
}


bool Lexer::isRelationOperator(char elem) const {
    return (elem == '<' || elem == '>');
}

bool Lexer::isDelimiter(const string &elem) const {
    return (elem == ")" || elem == "(" || elem == "," 
        || elem == ";" || elem == "{" || elem == "}");
}

void Lexer::tokenize() {
    while (position < input.length()) {
        skipWhitespaceAndComments();
        if (position >= input.length()) break;

        char current_char = peek();
        if (isalpha(current_char) || current_char == '_') {
            Token tmp = identifier();
            tokens.push_back(tmp);
            tokenList.addToken(tmp);
        }
        else if (isdigit(current_char)) {
            Token tmp = number();
            tokens.push_back(tmp);
            tokenList.addToken(tmp);
        }
        else {
            Token tmp = symbol();
            tokenList.addToken(tmp);
            tokens.push_back(tmp);
        }
    }
    tokens.push_back(Token(END_OF_FILE, "", line, column, 0, 0));
    tokenList.printTokensToConsole();
    tokenList.printTokensToFiles();
}

// Возвращает текущий символ без изменения позиции. 
char Lexer::peek() const {
    return input[position];
}

// Переходит к следующему символу. 
void Lexer::advance() {
    if (peek() == '\n') {
        line++;
        column = 1;
    }
    else {
        column++;
    }
    position++;
}

// Пропускает пробельные символы и однострочные комментарии. 
void Lexer::skipWhitespaceAndComments() {
    while (position < input.length()) {
        char current_char = peek();
        if (isspace(current_char)) {
            advance();
        }
        else
            break;
    }
}


// Обрабатывает идентификаторы и ключевые слова. 
Token Lexer::identifier() {
    int start_line = line;
    int start_column = column;
    string result;
    while (position < input.length() && (isalnum(peek()))) {
        result += peek();
        advance();
    }
    if (result == "int") {
        return Token(TYPE, result, start_line, start_column, 0, 0);
    }
    else if (result == "while") {
        return Token(OP, result, start_line, start_column, 0, 0);
    }
    else if (result == "return") {
        return Token(OP, result, start_line, start_column, 0, 0);
    }
    return Token(ID, result, start_line, start_column, 0, 0);
}

// Обрабатывает числовые константы. 
Token Lexer::number() {
    int start_line = line;
    int start_column = column;
    string result;
    while (position < input.length() && isdigit(peek())) {
        result += peek();
        advance();
    }
    return Token(CONST, result, start_line, start_column);
}

// Обрабатывает символы (операторы и разделители). 
Token Lexer::symbol() {
    int start_line = line;
    int start_column = column;
    char currElemChar = peek();
    string value(1, currElemChar);
    advance();

    // Обработка операторов с двумя символами, например, '==', '!=', '<=', '>=' 
    if ((currElemChar == '=' || currElemChar == '!' 
        || (isRelationOperator(currElemChar))) &&
        position < input.length() && peek() == '=') {
        value += peek();
        advance();
    }
    else if (isOperator(currElemChar) &&
        position < input.length() && peek() == currElemChar) {
        value += peek();
        advance();
    }

    if (isDelimiter(value)) {
        return Token(DELIMITER, value, start_line, start_column);
    }
    if (value == "=") {
        return Token(OP, value, start_line, start_column);
    }
    else{
      return Token(ROP, value, start_line, start_column);
    }
}

// Возвращает следующий токен из списка. 
Token Lexer::getNextToken() {
    if (!tokens.empty()) {
        Token token = tokens.front();
        tokens.erase(tokens.begin());
        return token;
    }
    return Token(END_OF_FILE, "", line, column, 0, 0);
}

// Возвращает все токены. 
const vector<Token>& Lexer::getTokens() const {
    return tokens;
}
