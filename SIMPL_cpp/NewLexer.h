#include "Errors.h"
#include <vector>
#include <unordered_map>

//Use a map to generate tokens -> index into a map constant with current_char to find the associated value
std::unordered_map<string, string> SYMBOLS = {
    {"+", "PLUS"},
    {"-", "MINUS"},
    {"*", "MUL"},
    {"/", "DIV"},
    {"(", "LPAREN"},
    {")", "RPAREN"},
};

//Token class -> stores a tokenized value and the type associated
class Token{
    string type;
    int value;

    public:
    //default constructor
    Token(){}

    //constructor
    Token(string type_, int value_=0){
        type = type_;
        value = value_;
    }

    //prints type and value of token
    void print(){
        std::cout << type << ": " << value << '\n';
    }

    //getters
    string get_type(){
        return type;
    }
};

struct Tokens_Error{
    std::vector<Token> tokens;
    Error error;
    bool error_eval = false;

    void make_error(Error input_error){
        error = input_error;
        error_eval = true;
    }
};

//Lexer Class -> generates tokens/errors from input text
class Lexer{
    string text;
    Position pos;
    char current_char = 0;

    public:

    //default constructor
    Lexer(){};


    //constructor -> sets text to be lexed and creates a default positon tracker object
    Lexer(string text_, string filename){
        text = text_;
        pos = Position(-1, 0 ,-1, text_, filename);
        advance();
    }


    //Moves Lexer along input string -> only updates current_char if havent reached end of text 
    void advance(){
        pos.advance(current_char);

        if (pos.get_idx() < text.length()){
            current_char = text.at(pos.get_idx());
        }

        else{
            current_char = 0;
        }
    }


    //Makes a number token
    Token make_number(){
        string num_str = "";
        int dot_count = 0;

        //continue iteration as long as current_char exists and the current_char is either a num or a decimal
        while (current_char && ((DIGITS+'.').find(current_char) != string::npos)){
            if (current_char == '.'){

                //cant have two decimals in a number -> invalid char
                if (dot_count == 1) {
                    break;
                    }
                
                dot_count += 1;
                num_str += '.';
            }
            
            else {
                num_str += current_char;
            }

            advance();
        }


        //if there are no decimals -> int
        if (dot_count == 0){
            return Token(TT_INT, stoi(num_str));
        }


        //if there are decimals -> flaot
        else {
            return Token(TT_FLOAT, stof(num_str));
        }
    }

    
    //Produces a Tokens_Error structure which contains a vector of errors and a vector of tokens
    Tokens_Error make_tokens(){
        Tokens_Error tokens;

        //checks every character until reached the end
        while (current_char!=0){
            if (DIGITS.find(current_char) != string::npos){
                tokens.tokens.push_back(make_number());
            }
            
            else {
                string str_char;
                str_char = current_char;
                try{
                    SYMBOLS.at(str_char);
                }
                catch (Tokens_Error tokens){
                    char character = current_char;
                    string details;
                    details += character;

                    Position pos_start = pos;
                    tokens.tokens = {};
                    tokens.make_error(IllegalCharError(details, pos_start, pos));
                    advance();
                    return tokens;
                }
                tokens.tokens.push_back(Token(SYMBOLS.at(str_char)));
                advance();
        }
        }

        return tokens;
    }


};