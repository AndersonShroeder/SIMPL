#include "Lexer.h"
#include "Constants.h"

class Parser{
    FileData data;
    int pos = 0;
    Token current_token;
    std::vector<Token> tokens;
    bool end = false;

    public:
    Parser(){}

    Parser(FileData input_data){
        data = input_data;
        current_token = data.get_tokens().at(pos);
        tokens = data.get_tokens();
    }

    void operator ++(){
        pos += 1;
        if (pos < tokens.size()){
            current_token = tokens.at(pos);
        }

        else{
            end = true;
        }
    }   

    //If the current token type matches the expected token type -> move the parser along
    void pop_token(string token_type){
        if (current_token.get_type() == token_type){
            ++(*this);
        }

        else{
            end = true;
        }
    }

    //Lowest level of arithmetic expressions -> just a number
    int factor(){
        int value = current_token.get_value();
        pop_token(TT_INT);
        return value;
    }

    //the result of multiply/div 1 or more factors
    void term(){

    }

    //the result of multiply/div 1 or more terms
    void expression(){

    }
};