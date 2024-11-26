#include <iostream>
#include <fstream>
#include "LexicalAnalyzer.h"
#include "SintaksisAnalyzer.h"
#include "Token.h"
#include "HashTable.h"

int main() {
    LexicalAnalyzer lexer("input.txt", "output.txt", "error.txt");;
    lexer.lexAnBeg();
    return 0;
}