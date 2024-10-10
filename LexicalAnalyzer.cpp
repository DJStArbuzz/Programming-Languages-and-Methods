#include "LexicalAnalyzer.h"
#include "Token.h"
#include <vector>

using namespace std;


//vector<vector<char>> words() {
//    {'i', 'n', 't';}, 
//    {'r', 'e', 't', 'u', 'r', 'n'}, 
//    {'w', 'h', 'i', 'l', 'e'}
//}

bool LexicalAnalyzer::isDigit(const string& lexeme) {
    for (char c : lexeme) {
        if (!isdigit(c)) {
            return false;
        }
    }

    return true;
}

TokenType LexicalAnalyzer::getNumber(const std::string& lexeme) {
    // Проверяем, является ли строка целым числом
    for (char c : lexeme) {
        if (!isdigit(c)) {
            return TokenType::ERROR;
        }
    }
    return TokenType::CONST;
}

TokenType LexicalAnalyzer::unknownType(const std::string& lexeme) {
    if (lexeme == "int") return TokenType::TYPE;
    if (lexeme == "while") return TokenType::OP;
    if (lexeme == "return") return TokenType::END;
    return TokenType::ERROR;
}

bool LexicalAnalyzer::isValidIdentifier(const std::string& word) const {
    // Проверяем, состоит ли слово только из заглавных и строчных букв латинского алфавита
    for (char c : word) {
        if (!isalpha(c)) { // Если символ не является буквой
            return false;
        }
    }
    return true;
}


bool LexicalAnalyzer::isError(Token* tmp) const {
    /*
    if (('0' <= str[0] <= '9') && (str.size() != 1))
        if ('z' < tolower(str[1]) || tolower(str[1]) < 'a')
            return true;

    for (char c : forbiden)
        if (count(str.begin(), str.end(), c) != 0)
            return true;
    
    */
    if (tmp->type == TokenType::ID) {
        for (int i = 0; i < tmp->lexeme.size(); i++) {
            if (!isalpha(tmp->lexeme[i])) {
                return true;
            }
       }
    }

    if (tmp->type == TokenType::CONST) {
        for (int i = 0; i < tmp->lexeme.size(); i++) {
            if (!isdigit(tmp->lexeme[i])) {
                return true;
            }
        }
    }
    return false;
}

bool LexicalAnalyzer::isRelationOperator(const std::string& str) const {
    return (str == "<" || str == ">" || str == "<=" || str == ">=" || str == "!=");
}



bool LexicalAnalyzer::isOperator(const string& str) const {
    return (str == "+" || str == "-" || str == "→" || str == "=");
}


bool LexicalAnalyzer::isRelationOperator(char str) const {
    return (str == '<' || str == '>');
}

bool LexicalAnalyzer::isDelimiter(char c) const {
    return (c == '(' || c == ')' || c == ';' || c == '{' || c == '}');
}

void condWithOrNotWhile() {

}

Token LexicalAnalyzer::getNextLexeme() {
std::string word;
char c;
int index = 1;
while (testTxtFile.get(c)) {
    if (isspace(c)) {
        index++;
        continue;
    }

    if (isalpha(c)) { // Идентификатор или ключевое слово
        word.clear();
        word += c;
        while (testTxtFile.get(c) && (isalnum(c))) {
            word += c;
        }
        testTxtFile.unget(); // Возвращаем последний символ в поток
        TokenType type = unknownType(word);

        if (type == TokenType::TYPE) {
            std::string description = word;

            // Пропускаем пробелы после ключевого слова
            while (testTxtFile.get(c) && isspace(c)) {}

            // Проверяем наличие переменных после типа
            std::string varList;
            // Считываем список переменных
            while (true) {
                if (c == '\n' || c == ';' || c == '}') {
                    break;
                }

                if (isspace(c)) {
                    while (testTxtFile.get(c) && isspace(c)) {}
                    continue; // Пропускаем пробелы
                }

                if (isalpha(c)) {
                    if (!varList.empty()) {
                        varList += ','; // Добавляем запятую перед новой переменной, если varList не пуст
                    }
                    varList += c;

                    // Считываем дополнительные символы в идентификатор
                    while (testTxtFile.get(c) && (isalnum(c) || c == '_')) {
                        varList += c;
                    }
                    testTxtFile.unget(); // Возвращаем последний символ в поток
                }
                else if (c == ',') {

                    // продолжим считывать следующее
                }
                else if (isDelimiter(c)){

                    description += " " + varList;
                    description += c;
                    while (testTxtFile.get(c) && !isspace(c)) {
                        if (isDelimiter(c)) {
                            description += c;
                        }
                    }
                    return Token(TokenType::BEGIN, description, index);
                    break; // Выход из цикла, если символ не соответствует ожидаемому
                }

                // Переход к следующему символу
                if (!testTxtFile.get(c)) {
                    break; // Достигнут конец файла
                }
            }
            // Проверяем, не пуст ли varList
            if (!varList.empty()) {
                description += " " + varList;
                return Token(TokenType::DESCRIPTIONS, description, index);
            }
            
            else {
                return Token(TokenType::TYPE, word, index);
            }
        }

       
        // Читаем следующий символ после идентификатора
        if (testTxtFile.get(c)) { // Проверка на наличие следующего символа
            std::string variable = word; // Сохраняем переменную
            
            char nextChar;

            // Проверяем следующий символ
            if (testTxtFile.get(nextChar)) {
                // Пропускаем пробелы
                //if (isspace(nextChar)) {
                //    while (testTxtFile.get(nextChar) && isspace(nextChar));
                //}

                bool containsROP = false;
                if (isRelationOperator(nextChar)) {
                    std::string expression = variable;
                    string tmp;
                    tmp += nextChar;
                    expression += c;
                    expression += nextChar; // Начало выражения
                    while (testTxtFile.get(c) && c != '\n') 
                    {
                        tmp += c;

                        if (isRelationOperator(tmp) && tmp.size() == 2) {
                            containsROP = true;
                            expression += c;
                        }
                        else if (isRelationOperator(c)) {
                            containsROP = true;
                            expression += c;
                        }
                        else {
                            expression += c;
                        }

                        if (isDelimiter(c)) {
                            testTxtFile.unget();
                            expression.pop_back();
                            break;
                        }
                    }

                if (containsROP) {
                    cout << "";
                    std::string new_expression;
                    for (int i = 4; i < expression.size(); i++) {
                        new_expression += expression[i];
                    }

                    
                    return Token(TokenType::CONDITION, expression, index);
                }
                }



                // Проверяем на оператор присвоения
                if (nextChar == '=') {
                    std::string expression = variable + " ="; // Начало выражения
                    char c;
                    bool containsArithmetic = false; // Флаг для проверки на наличие + или -

                    string tmp = "";
                    // Собираем выражение до конца строки
                    while (testTxtFile.get(c) && c != '\n') {
                        tmp += c;
                        if (c == '+' || c == '-') {
                            containsArithmetic = true; // Нашли арифметический оператор
                            expression += std::string(1, c);
                        }
                        else {
                            expression += c;
                        }
                    }
                    // Проверяем, содержит ли выражение арифметические операторы
                    if (containsArithmetic) {
                        std::string new_expression;
                        for (int i = 4; i < expression.size(); i++) {
                            new_expression += expression[i];
                        }
                        return Token(TokenType::EXPR, new_expression, index);
                    }
                    
                    else {
                        return Token(TokenType::OP, expression, index); // Простое присвоение, возвращаем как OP
                    }
                }
                // Если не оператор присвоения, возвращаем символ обратно
                testTxtFile.unget();
            }

        }
        // Проверка на конструкцию end do
        if (word == "return") {
            std::string nextWord, tmp2;
            testTxtFile >> nextWord;
            cout << nextWord << " ";
            cout << " " << (isDigit(nextWord)) << "sd";

            int count = 0;
            if (!isDigit(nextWord)) {
                for (char c : nextWord) {
                    if (isdigit(c)) {

                    }
                    else if (isDelimiter(c)) {
                        count++;
                    }
                }
            }

            cout << c;

                while (testTxtFile.get(c)) {
                    cout << c;
                    if (isDelimiter(c)) {
                        tmp2 += c;
                        count++;
                    }
                    else if (isdigit(c)) {
                        tmp2 += c;

                    }
                    else if (c == '\n' || isspace(c)) {

                    }
                    else {
                        count = 100;
                        break;
                    }
                }
                cout << count;
                if (count == 2) {
                    return Token(TokenType::END, "return " + nextWord + tmp2, index);
                }
                else {
                    return Token(TokenType::ERROR, "return " + nextWord + tmp2, index);
                }
            
        }

        // Проверка на идентификатор
        if (isValidIdentifier(word)) {
            TokenType type = unknownType(word);
            // Добавить while
            return Token(type, word, index);
        }
        else {
            return Token(TokenType::ERROR, word, index);
        }
    }

    else if (isdigit(c)) { // Константа целого числа
        word.clear();
        word += c;
        while (testTxtFile.get(c) && isdigit(c)) {
            word += c;
        }
        testTxtFile.unget(); // Возвращаем последний символ в поток
        return Token(getNumber(word), word, index);
    }
    else if (isDelimiter(c)) { // Разделитель
        return Token(TokenType::DELIMITER, std::string(1, c), index);
    }
    else {
        // Если встретился ошибочный символ, начинаем собирать ошибочную лексему
        word.clear();
        word += c;
        while (testTxtFile.get(c) && !isspace(c) && !isDelimiter(c)) {
            word += c;
        }
        testTxtFile.unget(); // Вернуть последний корректный символ
        return Token(TokenType::ERROR, word, index);
    }
}
return Token(TokenType::UNKNOWN, "", index); // UNKNOWN токен в конце файла
}

void LexicalAnalyzer::lexAnBeg() {
    Token token;
    while ((token = getNextLexeme()).type != TokenType::UNKNOWN) {
        tokenList.addToken(token);
    }
    tokenList.printTokensFile(resultTxtFile);
    tokenList.printTokensConsole();
}