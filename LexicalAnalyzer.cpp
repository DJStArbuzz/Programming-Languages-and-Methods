#include "LexicalAnalyzer.h"
#include "Token.h"
#include <iostream>
#include <vector>
#include "SintaksisAnalyzer.h"

using namespace std;

vector<string> words = { "int", "while", "return" };

bool LexicalAnalyzer::isDigit(const string& lexeme) {
    for (char c : lexeme)
        if (!isdigit(c))
            return false;
    return true;
}


bool LexicalAnalyzer::isValidIdentifier(const string& lexeme) const {
    for (char elem : lexeme)
        if (!isalpha(elem))
            return false;

    return true;
}


bool LexicalAnalyzer::isOperator(char elem) const {
    return (elem == '+' || elem == '-' || elem == '=');
}


bool LexicalAnalyzer::isRelationOperator(char elem) const {
    return (elem == '<' || elem == '>');
}

bool LexicalAnalyzer::isDelimiter(char elem) const {
    return (elem == '(' || elem == ')' || elem == ',' || elem == ';' || elem == '{' || elem == '}');
}


Token LexicalAnalyzer::isKeyWord(ifstream& testTxtFile, int index, int num, char elem) {
    string lexeme;
    int count = 1;
    int indexKey = 1;
    bool flag = false;

    lexeme += elem;
    while (testTxtFile.get(elem) && !isspace(elem)) {
        if (isDelimiter(elem) || isspace(elem)) {
            testTxtFile.unget();
            break;
        }
        lexeme += elem;

        if (indexKey < words[num].size())
            if (elem == words[num][indexKey])
                count++;
            else
                flag = true;
        else
            flag = true;
        indexKey++;
    }

    if (count == indexKey && (!flag)) {
        if (num == 0)
            return Token(TokenType::TYPE, lexeme, index, 0);
        else if (num == 1)
            return Token(TokenType::OP, lexeme, index, 0);
        else
            return Token(TokenType::END, lexeme, index, 0);
    }
    else {
        if (!isValidIdentifier(lexeme))
            return Token(TokenType::ERROR, lexeme, index, 0);
        else
            return Token(TokenType::ID_NAME, lexeme, index, 0);
    }
}

Token LexicalAnalyzer::getNextLexeme() {
    char elem;
    int index = 1;
    bool lastWasOperator = false;

    while (testTxtFile.get(elem)) {
        if (elem == '\n' || elem == '\r' || elem == '\n\r') {
            cout << sent << '\n';
            count_line++;
            if (!sent.empty() && sent != "\n") {
                sintaksis_analyzer.trim(sent);
                sintaksis_analyzer.building_tree(count_line, sent);
            }
            sent = "";
        }
        if (isspace(elem)) {
            index++;
            sent += " ";
            continue;
        }
        // int
        if (elem == 'i') {
            Token result = isKeyWord(testTxtFile, index, 0, elem);
            sent += (result.lexeme + " ");
            return result;
        }
        // while
        else if (elem == 'w') {
            Token result = isKeyWord(testTxtFile, index, 1, elem);
            sent += (result.lexeme + " ");
            return result;
        }
        // return
        else if (elem == 'r') {
            Token result = isKeyWord(testTxtFile, index, 2, elem);
            sent += (result.lexeme + " ");
            return result;
        }
        if (isalpha(elem)) {
            lexeme.clear();
            lexeme += elem;
            while (testTxtFile.get(elem) && (isalnum(elem)))
                lexeme += elem;
            testTxtFile.unget();
            if (isValidIdentifier(lexeme)) {// a - z, A-Z
                sent += (lexeme );
                return Token(TokenType::ID_NAME, lexeme, index, 0);
            }
            else {
                sent += (lexeme );
                return Token(TokenType::ERROR, lexeme, index, 0);
            }
            
        }

        else if (isdigit(elem)) { // 0 - 9
            lexeme.clear();
            lexeme += elem;
            while (testTxtFile.get(elem) && isdigit(elem))
                lexeme += elem;
            testTxtFile.unget();
            sent += (lexeme + " ");
            return Token(TokenType::CONST, lexeme, index, 0);
        }

        else if (isRelationOperator(elem)) { // < >
            if (testTxtFile.peek() == '=') {
                string result = "";
                result += elem;
                testTxtFile.get(elem);
                sent += (result + "=");
                return Token(TokenType::ROP, result + "=", index, 0);
            }
            else {
                sent += (string(1, elem) + " ");
                return Token(TokenType::ROP, string(1, elem), index, 0);
            }
        }

        else if (isOperator(elem)) { // + - =
            sent += (" " + string(1, elem) + " ");
            return Token(TokenType::OP, string(1, elem), index, 0);
        }

        else if (isDelimiter(elem)){ // ( ) [ ] { }
            sent += (string(1, elem));
        return Token(TokenType::DELIMITER, string(1, elem), index, 0);
    }

        else {
            // Additional verification
            lexeme.clear();
            lexeme += elem;
            while (testTxtFile.get(elem) && !isspace(elem) && !isDelimiter(elem))
                lexeme += elem;
            testTxtFile.unget();
            sent += (lexeme + " ");
            return Token(TokenType::ERROR, lexeme, index, 0);

        }
    }

    return Token(TokenType::UNKNOWN, "", index, 0); // Plug
}


void LexicalAnalyzer::lexAnBeg() {
    Token token;
    string tmp = "";
    while ((token = getNextLexeme()).type != TokenType::UNKNOWN)
        tokenList.addToken(token);

    tokenList.printTokensToFiles(resultTxtFile, errorTxtFile);
    tokenList.printTokensToConsole();
}