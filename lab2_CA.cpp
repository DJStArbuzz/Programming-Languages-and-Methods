#include <iostream>
#include "LexicalAnalyzer.h"
#include "Token.h"


int main() {
    LexicalAnalyzer lexer("input.txt", "output.txt", "error.txt");
    lexer.lexAnBeg();
    return 0;
}