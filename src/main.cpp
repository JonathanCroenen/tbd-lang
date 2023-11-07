#include "lexer.h"
#include "parser.h"
#include "evaluator.h"
#include <iostream>

int main() {
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

    return 0;
}
