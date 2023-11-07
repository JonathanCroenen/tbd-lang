#include "lexer.h"
#include "parser.h"
#include "evaluator.h"
#include <iostream>
#include <fstream>

int Repl() {
    Evaluator evaluator;
    EnvironmentPtr environment = std::make_shared<Environment>();

    while (true) {
        std::string input;
        std::cout << ">> ";
        std::getline(std::cin, input);

        if (input == "exit") {
            break;
        }

        Lexer lexer = Lexer(input);
        Parser parser = Parser(lexer);

        Program program = parser.Parse();

        if (!parser.GetErrors().empty()) {
            for (const ParseError& error : parser.GetErrors()) {
                std::cout << "SYNTAX ERROR: " << error.what() << std::endl;
            }
            continue;
        }

        ObjectPtr result = evaluator.Evaluate(program, environment);
        std::cout << *result << std::endl;
    }

    return EXIT_SUCCESS;
}

int RunFile(std::string source) {
    Evaluator evaluator;
    EnvironmentPtr environment = std::make_shared<Environment>();

    Lexer lexer = Lexer(source);
    Parser parser = Parser(lexer);

    Program program = parser.Parse();

    if (!parser.GetErrors().empty()) {
        for (const ParseError& error : parser.GetErrors()) {
            std::cerr << "SYNTAX ERROR: " << error.what() << std::endl;
        }
        return 1;
    }

    ObjectPtr result = evaluator.Evaluate(program, environment);
    if (result->type == Object::Type::ERROR) {
        std::cerr << "RUNTIME ERROR: " << *result << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int main(int argc, char** argv) {
    if (argc == 1) {
        return Repl();
    }

    if (argc == 2) {
        std::ifstream file(argv[1]);
        if (!file.is_open()) {
            std::cout << "Could not open file: " << argv[1] << std::endl;
            return 1;
        }

        std::string source((std::istreambuf_iterator<char>(file)),
                           std::istreambuf_iterator<char>());

        return RunFile(source);
    }

    return EXIT_SUCCESS;
}
