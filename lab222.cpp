#include <iostream> 
#include <fstream> 
#include <sstream> 
#include "Lexer.h" 
#include "Parser.h" 
#include "Node.h" 

using namespace std;

int main() {
    // Открытие входного файла 
    ifstream inputFile("input.txt");

    if (!inputFile) {
        cerr << "Failed to open the input file: input.txt\n";
        cout << "\nPress Enter to close the program...";
        cin.get();
        return 1;
    }

    // Чтение содержимого файла в строку 
    string inputText((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());

    // Создание лексера 
    Lexer lexer(inputText);

    // Создание парсера 
    Parser parser(lexer);

    try {
        // Выполнение синтаксического анализа 
        auto parseTree = parser.parse();

        // Проверка на наличие ошибок 
        const auto& errors = parser.getErrors();
        if (!errors.empty()) {
            for (const auto& error : errors) {
                cerr << error << '\n';
            }
            cout << "\nPress Enter to close the program...";
            cin.get();
            return 1;
        }

        // Запись дерева разбора в выходной файл 
        ofstream outputFile("output.txt");
        if (!outputFile) {
            cerr << "Failed to open the output file: output.txt\n";
            cout << "\nPress Enter to close the program...";
            cin.get();
            return 1;
        }

        streambuf* coutBuf = cout.rdbuf();
        cout.rdbuf(outputFile.rdbuf());

            printTree(parseTree);
            cout << errors.size();

        // Восстановление std::cout 
        cout.rdbuf(coutBuf);
        
        cout << "Syntax analysis completed successfully. Parse tree saved to output.txt\n";

    }
    catch (const exception& ex) {
        cerr << "Error: " << ex.what() << '\n';
        std::ofstream ofs;
        ofs.open("output.txt", ofstream::out | ofstream::trunc);
        ofs.close();
        // Ожидание ввода, чтобы оставить консоль открытой 
        cout << "\nPress Enter to close the program...";
        cin.get();
        return 1;
    }

    // Ожидание ввода перед закрытием программы 
    cout << "\nPress Enter to close the program...";
    cin.get();

    return 0;
}
