#include "SintaksisAnalyzer.h"
#include "HashTable.h"
#include <iostream>
#include <string>
#include <cstring> // для memset
#include <vector>

using namespace std;

void SyntaxAnalyzer::printWhiteSpace(const int n) {
    for (int i = 0; i < n; i++) {
        outputFile << " ";
    }
}
bool SyntaxAnalyzer::isValidIdentifier(const string word) {
    for (char c : word) {
        if (!isalpha(c)) {
            return false;
        }
    }
    return true;
}

bool SyntaxAnalyzer::isValidExpression(const string expr) {
    return !expr.empty() && std::all_of(expr.begin(), expr.end(), ::isdigit);
}

string* SyntaxAnalyzer::split(const std::string line, int& numWords) {
    static const int MAX_WORDS = 30; 
    static string words[MAX_WORDS];
    for (int i = 0; i < MAX_WORDS; i++) {
        words[i].clear();
    }

    size_t start = 0;
    size_t end = line.find(' '); 
    numWords = 0;                

    // Разделение строки на слова
    while (end != string::npos && numWords < MAX_WORDS) {
        string word = line.substr(start, end - start);
        if (!word.empty()) {
            words[numWords++] = word;
        }
        start = end + 1;
        end = line.find(' ', start); 
    }

    if (start < line.length()) {
        string word = line.substr(start);
        if (!word.empty() && numWords < MAX_WORDS) {
            words[numWords++] = word;
        }
    }
    return words; 
}


// Проверка, является ли строка корректным оператором (пока простая проверка)
bool SyntaxAnalyzer::isValidOperator(const string opLine, const int count_line) {
    string line2 = opLine, line = opLine;
    line = line2;
    if (line2.find("=") != string::npos) {
        line = line2.substr(0, line.find("=")) + " = " + line2.substr(line2.find("=") + 1);
    }

    bool flag = maybe_error1;
    if ((line.front() == '{') && (maybe_error1)) {
        maybe_error1 = false;
        line = line.substr(1);
    }
    else if ((line.front() != '{') && (!maybe_error1)) {
        maybe_error1 = false;
    }
    else {
        return false;
    }

    if (line.back() == '}') {
        maybe_error2--;
        line.pop_back();
    }

    int lS = 0, rS = 0;
    for (int i = 0; i < line.size(); i++) {
        if (line[i] == '(') {
            line[i] = ' ';
            lS++;
            continue;
        }
        if (line[i] == ')') {
            rS++;
            line[i] = ' ';
            continue;
        }
    }

    if (lS != rS) {
        is_error_flag = true;
        error(count_line, opLine, "Неправмльное  написание оператора ().");
        return false;
    }

    string error_;
    int count_words = 0;
    // Разделяем строку на токены
    string* tokens = split(line, count_words);

    // Проверяем, что строка не пустая и содержит минимум "id = что-то"
    if (count_words < 3) {
        maybe_error1 = flag;
        return false;
    }

    // Проверяем второй токен как оператор присваивания
    if (tokens[1] != "=") {
        maybe_error1 = flag;
        return false;
    }

    // Проверяем первый токен как идентификатор
    if (!isValidIdentifier(tokens[0])) {
        error_ = "Id не соответствует правилам, встречен посторонний символ -> " + tokens[0];
        is_error_flag = true;
        error(count_line, line, error_);
        maybe_error1 = flag;
        return false;
    }

    int countT = (tokens[count_words - 1].size() == 1);

    // Проверяем оставшиеся токены после '='
    bool expectingOperand = true; // Указывает, ожидаем ли операнд или оператор
    for (int i = 2; i < count_words - countT; ++i) {
        if (expectingOperand) {
            // Ожидаем операнд: идентификатор или выражение
            if (tokens[i].back() == ';') {
                tokens[i].pop_back();
            }

            if (!isValidExpression(tokens[i]) && !isValidIdentifier(tokens[i])) {
                error_ = "Ожидается идентификатор или выражение, встречено -> " + tokens[i];
                is_error_flag = true;
                error(count_line, opLine, error_);
                maybe_error1 = flag;
                return false;
            }
            expectingOperand = false; // Следующий токен должен быть оператором
        }
        else {
            // Ожидаем оператор: '+' или '-'
            if (tokens[i] != "+" && tokens[i] != "-") {
                error_ = "Ожидается оператор '+' или '-', встречено -> " + tokens[i];
                is_error_flag = true;
                error(count_line, opLine, error_);
                maybe_error1 = flag;
                return false;
            }
            expectingOperand = true; // Следующий токен должен быть операндом
        }
    }

    if (expectingOperand) {
        error_ = "Строка заканчивается на оператор без операнда.";
        is_error_flag = true;
        error(count_line, opLine, error_);
        return false;
    }
    if ((count(opLine.begin(), opLine.end(), ';') != 1)) {
        maybe_error1 = flag;
        return false;
    }
    return true;
}


bool SyntaxAnalyzer::isCondition(const string opLine, const int count_line) {
    string error_;
    int count_words = 0, count_words1 = 0, count_words2 = 0;
    // Разделяем строку на 


    size_t pos = string::npos;
    vector<string> list = { "==", "!=", ">", "<", "<=", ">=" };
    for (int i = 0; i < list.size(); i++) {
        if (pos == string::npos)
            pos = opLine.find(list[i]);
    }

    if (pos == string::npos) {
        return false;
    }

    string cond1 = opLine.substr(0, pos);
    string cond2 = opLine.substr(pos + 2);

    string* tokens1 = split(cond1, count_words1);
    string* tokens2 = split(cond2, count_words2);
    bool expectingOperand = true; 
    bool expectingOperand2 = true; 

    for (int i = 0; i < count_words1; i++) {
        if (expectingOperand) {

            if (!isValidExpression(tokens1[i]) && !isValidIdentifier(tokens1[i])) {
                error_ = "Ожидается идентификатор или выражение, встречено -> " + tokens1[i];
                is_error_flag = true;
                error(count_line, opLine, error_);
                return false;
            }
            expectingOperand = false; 
        }
        else {
            // Ожидаем оператор: '+' или '-'
            if (tokens1[i] != "+" && tokens1[i] != "-") {
                error_ = "Ожидается оператор '+' или '-', встречено -> " + tokens1[i];
                is_error_flag = true;
                error(count_line, opLine, error_);
                return false;
            }
            expectingOperand = true; 
        }
    }


    for (int i = 0; i < count_words2; i++) {
        if (expectingOperand2) {

            if (!isValidExpression(tokens2[i]) && !isValidIdentifier(tokens2[i])) {
                error_ = "Ожидается идентификатор или выражение, встречено -> " + tokens2[i];
                is_error_flag = true;
                error(count_line, opLine, error_);
                return false;
            }
            expectingOperand2 = false;
        }
        else {
            if (tokens2[i] != "+" && tokens2[i] != "-") {
                error_ = "Ожидается оператор '+' или '-', встречено -> " + tokens2[i];
                is_error_flag = true;
                error(count_line, opLine, error_);
                return false;
            }
            expectingOperand2 = true; 
        }
    }


    if (expectingOperand || expectingOperand2) {
        error_ = "Строка заканчивается на оператор без операнда.";
        is_error_flag = true;
        error(count_line, opLine, error_);
        return false;
    }
    return true;
}


void SyntaxAnalyzer::building_tree(const int count_line, const string line) {
    if (is_error_flag) {
        outputFile.close();
        ofstream treeFile("parsing_tree.txt", ios::trunc);
        if (!treeFile.is_open()) {
            cerr << "Ошибка: не удалось открыть файл errors.txt для очистки.\n";
        }
        treeFile.close();
        cout << "An error has been detected, take a look at the file <errors.txt> to get acquainted.\n";
    }
    if (isWhile(line, count_line)) {
        printWhile(line, 0);
    }
    else if (isBegin(line, count_line)) {
        printBegin(line);
    }
    else if (isEnd(line, count_line)) {
        printEnd(line);
    }
    else if (isDescriptions(line, count_line)) {
        printDescriptions(line, 0);
    }
    else if (isValidOperator(line, count_line)) {
        printOperators(line, 0);
    }

    else {
        if (!is_error_flag) {
            int count_words = 0;
            string* splitting_line = split(line, count_words);
            if (count_words == 1) {
                is_error_flag = true;
                error(count_line, splitting_line[0], "Недопускается писать 1 переменную/число");
            }
            else {
                is_error_flag = true;
                error(count_line, line, "Нераспознанные индентификатор, проверьте его корректность.");
            }
        }
    }
}

void SyntaxAnalyzer::error(const int count_line, const string line, const string type_error) {
    ofstream errorFile("errors.txt", ios::app);

    if (!errorFile.is_open()) {
        cerr << "Ошибка: не удалось открыть файл errors.txt для записи." << std::endl;
        return;
    }

    errorFile << "Ошибка в строке #" << count_line << " - " << type_error << ": " << line << '\n';

    errorFile.close();
}


bool SyntaxAnalyzer::isWhile(const string line, const int count_line) {
    size_t posSc1 = line.find('(');
    size_t posSc2 = line.rfind(')');
    size_t posSc3 = line.find('{');
    size_t posSc4 = line.find('}');
    size_t posWhi = line.find("while");

    if (posWhi == string::npos || posSc1 == string::npos || posSc2 == string::npos)
        return false;

    if (posSc4 == string::npos)
        maybe_error2++;

    string whileS = line.substr(posWhi, 5);
    int dist = posSc2 - posSc1 - 1;
    string cond = line.substr(posSc1 + 1, dist);
    bool flag2 = isCondition(cond, count_line);

    if (posSc3 == string::npos)
        maybe_error1 = true;
    return flag2;
}


bool SyntaxAnalyzer::isBegin(const string line, const int count_line) {
    int count_words = 0;
    string* words = split(line, count_words);

    if (count_words > 5) 
        return false;

    if (words[0] == "int") {
        string tmp = "";
        for (int i = 1; i < count_words; i++)
            tmp += words[i];

        if ((tmp.back() != '{'))
            maybe_error1 = true;
        else
            tmp.pop_back();

        if (!(tmp.back() == ')')) {
            maybe_error1 = false;
            return false;
        }
        tmp.pop_back();
        if (!(tmp.back() == '(')) {
            maybe_error1 = false;
            return false;
        }
        tmp.pop_back();

        return isValidIdentifier(tmp);
    }

    return false;
}

bool isDigit(const string& lexeme) {
    for (char c : lexeme)
        if (!isdigit(c))
            return false;
    return true;
}

bool SyntaxAnalyzer::isEnd(const std::string line, const int count_line) {
    int count_words = 0;
    string* words = split(line, count_words);

    int a = 0, b = 0;
    cout << "a";
    
    if (words[0] == "return") {
        string tmp = "";
        for (int i = 1; i < count_words; i++) {
            tmp += words[i];
        }
        if (!(tmp.back() == '}')) {
            return false;
        }

        tmp.pop_back();
        if (!(tmp.back() == ';'))
            return false;
        tmp.pop_back();

        bool res = isDigit(tmp);
        return isDigit(tmp);
    }

    return false;
}

bool SyntaxAnalyzer::isDescriptions(const string lines, const int count_line) {
    int count_words = 0;

    string line = lines;

    bool flag = (maybe_error1);

    if ((line.front() == '{') && (maybe_error1)) {
        maybe_error1 = false;
        line = line.substr(1);
    }
    else if ((line.front() != '{') && (!maybe_error1)) {
        maybe_error1 = false;
    }
    else {
        return false;
    }

    if (line.back() == '}') {
        maybe_error2--;
        line.pop_back();
    }


    string* words = split(line, count_words);
    if (count_words < 2 || words[0] != "int") {
        if (words[0] == "int") {
            is_error_flag = true;
            error(count_line, line, "Нет VarList после int");
            is_error_flag = true;
        }
        maybe_error1 = flag;
        return false;
    }
    int flagZ = 0, flagT = 0;

    int countT = (words[count_words - 1].size() == 1);
    for (int i = 1; i < count_words - countT; i++) {

        if (words[i].back() == ',') {
            words[i].erase(words[i].size() - 1);
            flagZ++;
        }

        if (words[i].back() == ';') {
            words[i].erase(words[i].size() - 1); 
            flagT++;
        }

        if (!isValidIdentifier(words[i])) {
            is_error_flag = true;
            error(count_line, line, "Недопустимый индентификатор: " + words[i]);
            maybe_error1 = flag;
            return false;
        }
    }
    if ((count(line.begin(), line.end(), ';') != 1)) {
        maybe_error1 = flag;
        return false;
    }
    if (!(flagZ == (count_words - 2 - countT))) {
        maybe_error1 = flag;
        return false;
    }
    return true;
}


void SyntaxAnalyzer::printOperators(const string cline, int num) {
    dist = 2;
    printWhiteSpace(dist);
    outputFile << "Operators\n";
    printWhiteSpace(dist + 2);
    outputFile << "Op\n";
    string line = cline;
    int count_words = 0;
    bool flag = (line.back() == '}');
    if (flag) {
        line.pop_back();
    }
    line = cline.substr(0, line.find("=")) + " = " + cline.substr(line.find("=") + 1);
    for (int i = 0; i < line.size(); i++) {
        if (line[i] == '(' || line[i] == ')') {
            line[i] = ' ';
        }
    }

    string* tokens = split(line, count_words);

    string left_var = tokens[0];
    string equal_sign = tokens[1];

    printWhiteSpace(dist + 5);
    outputFile << left_var << "  [Id]\n";
    printWhiteSpace(dist + 5);
    outputFile << " =  [AssignmentOperator]\n";

    int tmp = 2;
    bool expectOperator = false;
    for (int i = 2; i < count_words; i++) {
        string token = tokens[i];

        if (expectOperator) {
            if (token == "+" || token == "-") {
                printWhiteSpace(dist + 5 + tmp);
                outputFile << token << "  [MathematicalOperation]\n";
                expectOperator = false;
                tmp += 1;
            }
        }
        else {
            printWhiteSpace(dist + tmp + 5);
            outputFile << "Expr\n";
            for (int i = 0; i < tmp + 1; i++) {
                outputFile << "  ";
            }
            printWhiteSpace(dist + 5 + tmp);
            outputFile << "SimpleExpr\n";
            for (int i = 0; i < tmp + 1; i++) {
                outputFile << "  ";
            }
            if (token.back() == ';') {
                token.pop_back();
            }
            printWhiteSpace(dist + 5 + tmp);
            if (isValidIdentifier(token))
                outputFile << token << "  [Id]\n";
            else
                outputFile << token << "  [Const]\n";
            expectOperator = true; 
        }
    }
    printWhiteSpace(dist + 5);
    outputFile << ";  [Semicolon]\n";
    if (flag) {
        printWhiteSpace(dist + 2);
        outputFile << "}  [Separator]\n";
    }
}


bool SyntaxAnalyzer::printExpr(string*& tokens, int count_words, int num) {
    int tmp = 2;
    bool expectOperator = false;
    for (int i = 0; i < count_words; ++i) {
        string token = tokens[i];

        if (expectOperator) {

            if (token == "+" || token == "-") {
                printWhiteSpace(dist + tmp + 3);
                outputFile << token << "  [MathematicalOperation]\n";
                expectOperator = false;
                tmp += 2;
            }
        }
        else {
            for (int i = 0; i < tmp; i++) {
                outputFile << "  ";
            }

            printWhiteSpace(dist + tmp);
            outputFile << "Expr\n";
            printWhiteSpace(dist + tmp + 3);
            outputFile << "  SimpleExpr\n";
            for (int i = 0; i < tmp + 1; i++) {
                outputFile << "  ";
            }
            printWhiteSpace(dist + tmp +  5);
            if (isValidIdentifier(token))
                outputFile << token << "  [Id]\n";
            else
                outputFile << token << "  [Const]\n";
            expectOperator = true;
        }
    }

    return expectOperator;
}

void SyntaxAnalyzer::printOpearatorsWhile(const string line, int num) {
    printWhiteSpace(dist + num - 7);
    outputFile << "Conditon\n";

    vector<string> comb = { "==", "<=", ">=", "!=", "<", ">" };
    int i = 0;
    size_t equal_pos = string::npos;
    while (equal_pos == string::npos) {
        equal_pos = line.find(comb[i]);
        i++;
        if (i == comb.size()) {
            return;
        }
    }
    string left_var = line.substr(0, equal_pos); 
    string right_expr = line.substr(equal_pos + comb[i].size()); 


    int first = 0, second = 0;
    string* tokens1 = split(left_var, first);
    string* tokens2 = split(right_expr, second);

    bool first2 = printExpr(tokens1, first, num + 2);
    printWhiteSpace(dist + num + 5);
    outputFile << comb[i] << " [RelationOperator]\n";
    bool second2 = printExpr(tokens2, second, num + 2);

}

void SyntaxAnalyzer::printExpression(const string& expr, const string& indent, int num) {
    string trimmed_expr = expr;
    trim(trimmed_expr);
    printWhiteSpace(dist + num);
    outputFile << "SimpleExpr\n";
    size_t operator_pos = trimmed_expr.find_first_of("+-");
    if (operator_pos == string::npos) {
        printWhiteSpace(dist + num);
        if (isDigit(trimmed_expr))
            outputFile << trimmed_expr << "  [Const]\n";
        else
            outputFile << trimmed_expr << "  [Id]\n";
    }
    else {
        string left = trimmed_expr.substr(0, operator_pos);
        string op = trimmed_expr.substr(operator_pos, 1);
        string right = trimmed_expr.substr(operator_pos + 1);

        trim(left);
        trim(right);

        printWhiteSpace(dist + num + 7);
        if (isDigit(left))
            outputFile << left << "  [Const]\n";
        else
            outputFile << left << "  [Id]\n";
        printWhiteSpace(dist + num + 5);
        outputFile << op << "  [MathematicalOperation]\n";
        printExpression(right, indent + " ", num + 2);
    }
}

void SyntaxAnalyzer::printWhile(const string curr, int num) {
    dist = 2;
    printWhiteSpace(dist);
    outputFile << "Operators\n";
    printWhiteSpace(dist + 2);
    outputFile << "Op\n";

    size_t for_pos = curr.find("while");
    size_t sc1 = curr.find("(");
    size_t sc2 = curr.rfind(")");

    printWhiteSpace(dist + 5);
    outputFile << "while [KeyWord]\n";
    printWhiteSpace(dist + 5);
    outputFile << "(  [StaplesL]\n";

    int dist = sc2 - sc1 - 1;
    string cond = curr.substr(sc1 + 1, dist);
    printOpearatorsWhile(cond, 12);
    dist = 2;
    printWhiteSpace(dist + 5);
    outputFile << ")  [StaplesR]\n";
    printWhiteSpace(dist + 5);
    outputFile << "{  [CurlyBraceL]\n";
    if (curr.back() == '}') {
        printWhiteSpace(dist + 5);
        outputFile << "  }  [CurlyBraceR]\n";
    }
}


void SyntaxAnalyzer::printBegin(const string line) {
    outputFile << "Function\n";
    printWhiteSpace(dist);
    dist += 2;
    outputFile << "Begin\n";
    printWhiteSpace(dist);

    size_t prog_pos = line.find("int");
    string programName = line.substr(prog_pos + 4);

    while (!isValidIdentifier(programName)) {
        programName.pop_back();
    }

    outputFile << "Type\n";
    printWhiteSpace(dist + 3);
    outputFile << "int  [KeyWord]\n";
    printWhiteSpace(dist);
    outputFile << "FunctionName\n";
    printWhiteSpace(dist + 3);
    outputFile << "" << programName << "  [Id]\n";
    printWhiteSpace(dist);
    outputFile << "(  [StaplesL]\n";
    printWhiteSpace(dist);
    outputFile << ")  [StaplesR]\n";
    printWhiteSpace(dist);
    outputFile << "{  [CurlyBraceL]\n";

}

void SyntaxAnalyzer::printEnd(const string line) {
    dist = 2;
    printWhiteSpace(dist);
    outputFile << "End\n";

    size_t endPos = line.find("return");
    string programRes = line.substr(endPos + 7);

    while (!isDigit(programRes)) {
        programRes.pop_back();
    }
    printWhiteSpace(dist);
    outputFile << "  return  [KeyWord]\n";
    printWhiteSpace(dist);
    outputFile << "  " << programRes << "  [Const]\n";
    printWhiteSpace(dist);
    outputFile << "  ;  [Semicolon]\n";
    printWhiteSpace(dist);
    outputFile << "  }  [CurlyBraceR]\n";

    if (maybe_error2 > 0) {
        error(0, line, "Отсутсиве фигруных скобок.");
        outputFile.open("parsing_tree.txt", ofstream::out | ofstream::trunc);
        is_error_flag = true;
    }

}

void SyntaxAnalyzer::printVarList(const string curr, int num) {
    size_t pos = 0;
    size_t comma_pos;
    string line = curr;
    line.pop_back();

    while ((comma_pos = line.find(',', pos)) != string::npos) {
        num += 3;
        printWhiteSpace(dist + num);
        outputFile << line.substr(pos, comma_pos - pos) << "  [Id]\n";
        printWhiteSpace(dist + num);
        outputFile << ",  [Comma]\n";
        printWhiteSpace(dist + num);
        outputFile << "VarList\n";
        num += 3;
        pos = comma_pos + 1;
    }

    bool flag = (line.back() == '}');
    if (flag) {
        line.pop_back();
    }
    printWhiteSpace(dist + num);
    outputFile << line.substr(pos) << "  [Id]\n";
    printWhiteSpace(dist + 2);
    outputFile << ";" << "  [Semicolon]\n";
    if (flag) {
        printWhiteSpace(dist + 2);
        outputFile << "}" << "  [CurlyBraceR]\n";
    }
}

void SyntaxAnalyzer::printDescriptions(const string curr, int num) {
    dist = 2;
    printWhiteSpace(dist);
    outputFile << "Descriptions\n";
    printWhiteSpace(dist + 2);
    outputFile << "Descr\n";

    size_t type_pos = curr.find("int");
    string vars = curr.substr(type_pos + 3); 

    printWhiteSpace(dist + 5);
    outputFile << "Type\n";
    printWhiteSpace(dist + 8);
    outputFile << "int  [KeyWord]\n";

    // Вложенные Varlist
    printWhiteSpace(dist + 5);
    outputFile << "Varlist\n";
    printVarList(vars, 5);
}