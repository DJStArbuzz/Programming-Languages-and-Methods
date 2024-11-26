#include "SintaksisAnalyzer.h"
#include "HashTable.h"
#include <iostream>
#include <string>
#include <cstring> // для memset
#include <vector>

using namespace std;

void SyntaxAnalyzer::printWhiteSpace(const int n){
    for (int i = 0; i < n; i++) {
        outputFile << " ";
    }
}
bool SyntaxAnalyzer::isValidIdentifier(const string word) {
    for (char c : word) {
        if (!isalpha(c)) { // Если символ не является буквой
            return false;
        }
    }
    return true;
}

// Проверка, является ли строка валидным выражением (пока допускаем только числа)
bool SyntaxAnalyzer::isValidExpression(const string expr) {
    return !expr.empty() && std::all_of(expr.begin(), expr.end(), ::isdigit);
}

string* SyntaxAnalyzer::split(const std::string line, int& numWords) {
    static const int MAX_WORDS = 30;  // Максимальное количество слов
    static string words[MAX_WORDS];  // Статический массив слов

    // Очистка массива перед использованием
    for (int i = 0; i < MAX_WORDS; i++) {
        words[i].clear();
    }

    size_t start = 0;
    size_t end = line.find(' '); // Находим первый пробел
    numWords = 0;                // Счётчик слов

    // Разделение строки на слова
    while (end != string::npos && numWords < MAX_WORDS) {
        string word = line.substr(start, end - start);
        if (!word.empty()) { // Добавляем слово, только если оно не пустое
            words[numWords++] = word;
        }
        start = end + 1;
        end = line.find(' ', start); // Находим следующий пробел
    }

    // Добавляем последнее слово, если оно есть
    if (start < line.length()) {
        string word = line.substr(start);
        if (!word.empty() && numWords < MAX_WORDS) {
            words[numWords++] = word;
        }
    }

    return words;  // Возвращаем указатель на массив слов
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
    cout << rS << " " << lS << endl;
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

    // Если цикл завершился, но последним токеном был оператор, это ошибка
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
    cout << "correct\n";
    // Если дошли до сюда, строка корректна
    return true;
}


bool SyntaxAnalyzer::is_condition(const string opLine, const int count_line) {
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
    bool expectingOperand = true; // Указывает, ожидаем ли операнд или оператор
    bool expectingOperand2 = true; // Указывает, ожидаем ли операнд или оператор

    for (int i = 0; i < count_words1; i++) {
        if (expectingOperand) {

            if (!isValidExpression(tokens1[i]) && !isValidIdentifier(tokens1[i])) {
                error_ = "Ожидается идентификатор или выражение, встречено -> " + tokens1[i];
                is_error_flag = true;
                error(count_line, opLine, error_);
                return false;
            }
            expectingOperand = false; // Следующий токен должен быть оператором
        }
        else {
            // Ожидаем оператор: '+' или '-'
            if (tokens1[i] != "+" && tokens1[i] != "-") {
                error_ = "Ожидается оператор '+' или '-', встречено -> " + tokens1[i];
                is_error_flag = true;
                error(count_line, opLine, error_);
                return false;
            }
            expectingOperand = true; // Следующий токен должен быть операндом
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
            expectingOperand2 = false; // Следующий токен должен быть оператором
        }
        else {
            // Ожидаем оператор: '+' или '-'
            if (tokens2[i] != "+" && tokens2[i] != "-") {
                error_ = "Ожидается оператор '+' или '-', встречено -> " + tokens2[i];
                is_error_flag = true;
                error(count_line, opLine, error_);
                return false;
            }
            expectingOperand2 = true; // Следующий токен должен быть операндом
        }
    }


    // Если цикл завершился, но последним токеном был оператор, это ошибка
    if (expectingOperand || expectingOperand2) {
        error_ = "Строка заканчивается на оператор без операнда.";
        is_error_flag = true;
        error(count_line, opLine, error_);
        return false;
    }
    // Если дошли до сюда, строка корректна
    return true;
}


void SyntaxAnalyzer::building_tree(const int count_line, const string line) {
    if (is_error_flag) {
        outputFile.close();
        ofstream treeFile("parsing_tree.txt", ios::trunc);
        if (!treeFile.is_open()) {
            cerr << "Ошибка: не удалось открыть файл errors.txt для очистки.\n";
        }
        treeFile.close(); // Закрываем файл после очистки
        cout << "An error has been detected, take a look at the file <errors.txt> to get acquainted.\n";
    }
    if (is_cycle(line, count_line)) {
        draw_cycle(line, 0);
    }
    else if (is_start_program(line, count_line)) {
            draw_start_program(line);
    }
    else if (is_end_program(line, count_line)) {
        draw_end_program(line);
    }
    else if (is_descriptions(line, count_line)) {
        draw_descriptions(line, 0);
    }
    else if (isValidOperator(line, count_line)) {
        draw_operators(line, 0);
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
    // Режим добавления
   ofstream errorFile("errors.txt", ios::app);

    if (!errorFile.is_open()) {
        cerr << "Ошибка: не удалось открыть файл errors.txt для записи." << std::endl;
        return;
    }

    // Формируем строку с ошибкой
    errorFile << "Ошибка в строке #" << count_line << " - " << type_error << ": " << line << std::endl;

    errorFile.close();
}


bool SyntaxAnalyzer::is_cycle(const string line, const int count_line) {
    //std::string* lines = split(line, count_words); // Разбиваем строку на слова

    size_t posSc = line.find('(');
    size_t posSc2 = line.rfind(')');
    size_t posSc3 = line.find('{');
    size_t posSc4 = line.find('}');
    size_t posW = line.find("while");

    if (posW == string::npos || posSc == string::npos || posSc2 == string::npos) {
        return false;
    }
    if (posSc4 == string::npos) {
        maybe_error2++;
    }
    string whileS = line.substr(posW, 5);
    int dist = posSc2 - posSc - 1;
    string cond = line.substr(posSc + 1, dist);
    bool flag2 = is_condition(cond, count_line);

    if (posSc3 == string::npos) {
        maybe_error1 = true;
    }
    return flag2;
}


// Проверяет, соответствует ли строка началу программы (PROGRAM <идентификатор>)
bool SyntaxAnalyzer::is_start_program(const string line, const int count_line) {
    int count_words = 0;
    string* words = split(line, count_words);

    // Проверка: строка должна состоять ровно из двух слов
    if (count_words > 4) {
        return false;
    }

    // Первое слово должно быть "PROGRAM", второе - валидным идентификатором
    if (words[0] == "int"){
        string tmp = "";
        for (int i = 1; i < count_words; i++) {
            tmp += words[i];
        }

        if ((tmp.back() != '{')) {
            maybe_error1 = true;
        }
        else {
            tmp.pop_back();
        }

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

// Проверяет, соответствует ли строка завершению программы (END PROGRAM <идентификатор>)
bool SyntaxAnalyzer::is_end_program(const std::string line, const int count_line) {
    int count_words = 0;
    string* words = split(line, count_words);

    // Проверка: строка должна состоять ровно из трех слов
    if (count_words > 4) {
        return false;
    }

    int a = 0, b = 0;
    if (words[0] == "return") {
        string tmp = "";
        for (int i = 1; i < count_words; i++) {
            tmp += words[i];
        }

        if (!(tmp.back() == '}')) {
            return false;
        }
        
        tmp.pop_back();
        if (!(tmp.back() == ';')) {
            return false;
        }
        tmp.pop_back();
        
        bool res = isDigit(tmp);
        if (res) {
            is_error_flag = !(maybe_error2 > 0);
            return is_error_flag;
        }
        else { return false; }
    }

    return false;
}

bool SyntaxAnalyzer::is_descriptions(const string lines, const int count_line) {
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
    else  {
        return false;}
     
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
    // Проверяем каждое слово после "INTEGER"
    for (int i = 1; i < count_words - countT; ++i) {
        // Удаляем запятую, если она есть в конце слова

        if (words[i].back() == ',') {
            words[i].erase(words[i].size() - 1); // Убираем последнюю запятую
            flagZ++;
        }

        if (words[i].back() == ';') {
            words[i].erase(words[i].size() - 1); // Убираем последнюю запятую
            flagT++;
        }

        // Проверяем валидность идентификатора
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


void SyntaxAnalyzer::draw_operators(const string cline, int num) {
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
            expectOperator = true; // Следующий токен должен быть оператором
        }
    }
    printWhiteSpace(dist + 5);
    outputFile << ";  [Semicolon]\n";
    if (flag) {
        printWhiteSpace(dist + 2);
        outputFile << "}  [Separator]\n";
    }
}


bool SyntaxAnalyzer::tmp_print(string* & tokens, int count_words, int num) {
    int tmp = 2;
    bool expectOperator = false; 
    for (int i = 0; i < count_words; ++i) {
        string token = tokens[i];

        if (expectOperator) {
            
            if (token == "+" || token == "-") {
                for (int i = 0; i < tmp; i++) {
                    outputFile << "  ";
                }
                printWhiteSpace(dist + tmp - 2);
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
            for (int i = 0; i < tmp + 1; i++) {
                outputFile << "  ";
            }
            printWhiteSpace(dist + tmp + 2);
            outputFile << "SimpleExpr\n";
            for (int i = 0; i < tmp + 1; i++) {
                outputFile << "  ";
            }
            printWhiteSpace(dist + tmp + 4);
            if (isValidIdentifier(token))
                outputFile << token << "  [Id]\n";
            else
                outputFile << token << "  [Const]\n";
            expectOperator = true; 
        }
    }

    return expectOperator;
}

void SyntaxAnalyzer::draw_operators_cycle(const string line, int num) {
    printWhiteSpace(dist + num + 2);
    outputFile << "Conditon\n";

    // Разделяем строку на левый и правый операнд
    vector<string> comb = { "==", "<=", ">=", "!=", "<", ">"};
    int i = 0;
    size_t equal_pos = string::npos;
    while (equal_pos == string::npos) {
        equal_pos = line.find(comb[i]);
        i++;
        if (i == comb.size()) {
            return;
        }
    }
    string left_var = line.substr(0, equal_pos); // Левая часть
    string right_expr = line.substr(equal_pos + comb[i].size()); // Правая часть


    int first = 0, second = 0;
    string* tokens1 = split(left_var, first);
    string* tokens2 = split(right_expr, second);

    bool first2 = tmp_print(tokens1, first, num);
    printWhiteSpace(dist + num + 5);
    outputFile << comb[i] << " [RelationOperator]\n";
    bool second2 = tmp_print(tokens2, second, num);
    
}

void SyntaxAnalyzer::draw_expression(const string& expr, const string& indent, int num) {
    string trimmed_expr = expr;
    trim(trimmed_expr); 
    outputFile << indent << "SimpleExpr\n";
    size_t operator_pos = trimmed_expr.find_first_of("+-");
    if (operator_pos == string::npos) {
        // Если нет операторов, это простое выражение
        if (isDigit(trimmed_expr))
            outputFile << indent << " " << trimmed_expr << "  [Const]\n";
        else
            outputFile << indent << " " << trimmed_expr << "  [Id]\n";
    }
    else {
        string left = trimmed_expr.substr(0, operator_pos);
        string op = trimmed_expr.substr(operator_pos, 1);
        string right = trimmed_expr.substr(operator_pos + 1);

        trim(left);
        trim(right);
        
        if (isDigit(left)) 
            outputFile << indent << " " << left << "  [Const]\n";
        else
            outputFile << indent << " " << left << "  [Id]\n";

        outputFile << indent << " " << op << "  [MathematicalOperation]\n";
        draw_expression(right, indent + " ", 0);
    }
}

void SyntaxAnalyzer::draw_cycle(const string line, int num) {
    dist = 2;
    printWhiteSpace(dist);
    outputFile << "Operators\n";
    printWhiteSpace(dist + 2);
    outputFile << "Op\n";

    size_t for_pos = line.find("while");
    size_t sc1 = line.find("(");
    size_t sc2 = line.rfind(")");

    printWhiteSpace(dist + 5);
    outputFile << "while [KeyWord]\n";
    printWhiteSpace(dist + 5);
    outputFile << "(  [StaplesL]\n";
    
    int dist = sc2 - sc1 - 1;
    string cond = line.substr(sc1 + 1, dist);
    draw_operators_cycle(cond, 5);
    dist = 2;
    printWhiteSpace(dist + 5);
    outputFile << ")  [StaplesR]\n";
    printWhiteSpace(dist + 5);
    outputFile << "{  [CurlyBraceL]\n";
    if (line.back() == '}') {
        printWhiteSpace(dist + 5);
        outputFile << "  }  [CurlyBraceR]\n";
    }
}


void SyntaxAnalyzer::draw_start_program(const string line) {
    outputFile << "Function\n";
    printWhiteSpace(dist);
    dist += 2;
    outputFile << "Begin\n";
    printWhiteSpace(dist);

    size_t prog_pos = line.find("int");
    string program_res = line.substr(prog_pos + 4); // Имя программы

    while (!isValidIdentifier(program_res)) {
        program_res.pop_back();
    }
    
    outputFile << "Type\n";
    printWhiteSpace(dist + 3);
    outputFile << "int  [KeyWord]\n";
    printWhiteSpace(dist);
    outputFile << "FunctionName\n";
    printWhiteSpace(dist + 3);
    outputFile << "" << program_res << "  [Id]\n";
    printWhiteSpace(dist);
    outputFile << "(  [StaplesL]\n";
    printWhiteSpace(dist);
    outputFile << ")  [StaplesR]\n";
    printWhiteSpace(dist
    );
    outputFile << "{  [CurlyBraceL]\n";

}

void SyntaxAnalyzer::draw_end_program(const string line) {
    dist = 2;
    printWhiteSpace(dist);
    outputFile << "End\n";

    size_t end_pos = line.find("return");
    string program_res = line.substr(end_pos + 7); // Имя программы

    while (!isDigit(program_res)) {
        program_res.pop_back();
    }
    printWhiteSpace(dist);
    outputFile << "  return  [KeyWord]\n";
    printWhiteSpace(dist);
    outputFile << "  " << program_res << "  [Const]\n";
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

void SyntaxAnalyzer::draw_varlist(const string lin, int num) {
    size_t pos = 0;
    size_t comma_pos;
    string line = lin;
    line.pop_back();
    // Разделение на переменные и вывод с учетом вложенности
    while ((comma_pos = line.find(',', pos)) != std::string::npos) {
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

    // Последняя переменная, после последней запятой
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

void SyntaxAnalyzer::draw_descriptions(const string line, int num) {
    dist = 2;
    printWhiteSpace(dist);
    outputFile << "Descriptions\n";
    printWhiteSpace(dist + 2);
    outputFile << "Descr\n";

    size_t type_pos = line.find("int");
    string vars = line.substr(type_pos + 3); // Переменные после типа

    printWhiteSpace(dist + 5);
    outputFile << "Type\n";
    printWhiteSpace(dist + 8);
    outputFile << "int  [KeyWord]\n";

    // Вложенные Varlist
    printWhiteSpace(dist + 5);
    outputFile << "Varlist\n";
    draw_varlist(vars, 5);
}
