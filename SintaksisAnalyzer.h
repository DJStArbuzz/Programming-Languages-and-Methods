#ifndef SINTAKSIS_ANALYZER_H
#define SINTAKSIS_ANALYZER_H

#include "Token.h"
#include "HashTable.h"
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iomanip> 

using namespace std;

class SyntaxAnalyzer {
public:
    SyntaxAnalyzer() {
        outputFile.open("parsing_tree.txt");
        if (!outputFile.is_open()) {
            throw ios_base::failure("Failed to open the file.");
        }
    }

    ~SyntaxAnalyzer() {
        if (outputFile.is_open()) {
            outputFile.close();
        }
    }
    
    bool maybe_error1 = false; // ���������� ����� �������� ������
    int maybe_error2 = 0;
    int dist = 2;

    void error(const int count_line, const string line, const string type_error);                    // ����� ��� ������ ������ 
    void building_tree(const int count_line, const string line);            // ����� ��� ���������� ������ �������
    string* split(const string line, int& numWords);

    bool isValidIdentifier(const string word); 
    bool isValidExpression(const string expr);
    bool isValidOperator  (const string line, const int count_line);
    bool is_cycle         (const string line, const int count_line);
    bool is_start_program (const string line, const int count_line);  
    bool is_end_program   (const string line, const int count_line); 
    bool is_descriptions  (const string line, const int count_line); 
    bool is_condition     (const string line, const int count_line); 

    void draw_cycle          (const string line, int num); 
    void draw_start_program  (const string line); 
    void draw_end_program    (const string line); 
    void draw_descriptions   (const string line, int num); 
    void draw_operators      (const string line, int num);
    void draw_operators_cycle(const string line, int num); 
    void draw_varlist        (const string line, int num); 
    void draw_expression     (const string& expr, const string& indent, int num);
    void draw_condition      (const string line, int num);
    bool tmp_print(string*& tokens, int count_words, int num);

    void printWhiteSpace(const int n);

    void trim(std::string& str) {
        // ������� ������� � ������ ������
        str.erase(str.begin(), find_if(str.begin(), str.end(), [](unsigned char ch) {
            return !isspace(ch);
            }));

        // ������� ������� � ����� ������
        str.erase(find_if(str.rbegin(), str.rend(), [](unsigned char ch) {
            return !isspace(ch);
            }).base(), str.end());
    }

private:
    bool is_error_flag = false;
    

    ofstream outputFile;  // ����� ��� ������ � ����
};

#endif
