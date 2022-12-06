#include "run.h"

int main(){
    bool end = false;
    string input;
    string filename = "<stdout>";
    while (!end){
        std::cin >> input;
        if (input == "stop"){
            end = true;
        };

        Tokens_Error structure = run(input, filename);

        auto tokens = structure.tokens;
        auto error = structure.error;

        if (structure.error_eval){
            error.print();
        }

        else{
            for(Token token: tokens){
                token.print();
            };
        }
    }

    return 0;
}