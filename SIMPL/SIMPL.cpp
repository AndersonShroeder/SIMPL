#include "run.h"

int main(){
    string input;
    string filename = "<stdout>";
    while (true){
        std::getline(std::cin, input);

        FileData structure = run(input, filename);

        auto tokens = structure.get_tokens();
        auto error = structure.get_error();
        Parser parser = Parser(tokens);
        auto ast = parser.parse();

        if (structure.check_error()){
            std::cout << error << '\n';
        }

        else{
            std::cout << (*ast).str() << '\n';
        }
        Interpreter interp = Interpreter(ast);
        float out = interp.visit();
        std::cout << out << '\n';
    }

    return 0;
}