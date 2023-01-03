#include "FileData.h"

class Lexer{
    bool keyword = false;
    char* current_char;
    FileData file;

    public:
    
    Lexer(string text, string filename){
        file = FileData(text, filename);
        current_char = &(file.get_current_char());
    }

    bool is_valid_keyword_char(char character){
        if (!file.stop() && (isalpha(character) || character == '_' || isdigit(character))){
            return true;
        }
        //std::cout << "MAKE ERROR - Invalid characters for keyword" << '\n';
        return false;
    }

    bool is_valid_number_char(char character){
        if (!file.stop() && (DIGITS+'.').find(file.get_current_char()) != string::npos){
            return true;
        }
        //std::cout << "MAKE ERROR - Invalid characters for number" << '\n';
        return false;
    }

    Token make_symbol(){
        string symbol;
        Token tok;

        while (!is_valid_keyword_char(*current_char) && !is_valid_number_char(*current_char) && *current_char != ' ' && !file.stop()){
            symbol += *current_char;
            ++file;
            if (symbol == ")" || *current_char == '{'){
                break;
            }
        }

        try{
            tok = Token(SYMBOLS.at(symbol));
        }

        catch (const std::out_of_range &e){
            file.make_error("Invalid Character: " + symbol + '\n' + "Line 0, Column " + std::to_string(file.get_position() + 1));
        }

        return tok;
    }

    Token make_keyword(){
        string keyword;
        Token tok;

        while (is_valid_keyword_char(*current_char)){
            keyword += *current_char;
            ++file;
        }

        // if generated word isnt in keywords -> it is an Identifier
        try{
            tok = Token(KEYWORDS.at(keyword));
        }

        catch (const std::out_of_range &e){
            tok = Token(TT_ID, keyword);
        }
        
        return tok;
    }

    //Makes a number token
    Token make_number(){
        string num_str;
        int dot_count = 0;

        //continue iteration as long as current_char exists and the current_char is either a num or a decimal
        while (is_valid_number_char(*current_char)){
            
            if (isdigit(*current_char)){
                num_str += *current_char;
            }
            
            else {         
                //cant have two decimals in a number -> invalid char
                if (dot_count != 0) {break;}
                dot_count += 1;
                num_str += '.';
            }

            ++file;
        }

        //if there are no decimals -> int / else float
        switch (dot_count){
            case 1:
                return Token(TT_FLOAT, num_str);
            default:
                return Token(TT_INT, num_str);
        }
    }

    FileData make_tokens(){

        while (*current_char!=0){
            if (*current_char == '\t' || *current_char == ' '){
                ++file;
            }
            
            // create number token
            else if (isdigit(*current_char)){
                file.add_token(make_number());
            }

            // create a keyword/ID token
            else if (isalpha(*current_char)){
                file.add_token(make_keyword());
            }
            
            //create symbol token
            else {
                file.add_token(make_symbol());
        }
        }

        return file;
    }
};

//function that trys returned value from generate keyword


void try_keyword(string keyword){
    
}