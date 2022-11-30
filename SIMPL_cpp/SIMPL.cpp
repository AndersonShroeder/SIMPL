#include <iostream>
#include <vector>
#include <string>
#include <tuple>

using std::string;

const string DIGITS = "0123456789";
const string TT_INT = "TT_INT";
const string TT_FLOAT = "FLOAT";
const string TT_PLUS = "PLUS";
const string TT_MINUS = "MINUS";
const string TT_MUL = "MUL";
const string TT_DIV = "DIV";
const string TT_LPAREN = "LPAREN";
const string TT_RPAREN = "RPAREN";
const string TT_EOF = "EOF";

class Error{
    string error_name;
    string error_details;

    public:
    Error() {}

    Error(string error_name_, string error_details_){
        error_name = error_name_;
        error_details = error_details_;
    }

    string as_string(){
        string result = error_name + ": " + error_details;
        return result;
    }
};

class IllegalCharError : public Error{
    string str = "Illegal Character";
    public:
    IllegalCharError(string details){
        Error(str, details);
    }
};

class Token{
    string type;
    int value;

    public:
    Token(){}

    Token(string type_, int value_=0){
        type = type_;
        value = value_;
    }

    void print(){
        std::cout << type << ": " << value << '\n';
    }
};

struct Tokens_Error{
    std::vector<Token> tokens;
    Error error;
};

class Lexer{
    string text;
    int pos = -1;
    char current_char = 0;



    public:
    Lexer(){};

    Lexer(string text_){
        text = text_;
        advance();
    }

    //Moves Lexer along input string -> only updates current_char if havent reached end of text
    void advance(){
        pos += 1;
        if (pos < text.length()){
            current_char = text.at(pos);
        }
        else{
            current_char = 0;
        }
    }

    Token make_number(){
        string num_str = "";
        int dot_count = 0;

        //continue iteration as long as current_char exists and the current_char is eitehr a num or a decimal
        while (current_char && ((DIGITS+'.').find(current_char) != string::npos)){
            if (current_char == '.'){

                //cant have two decimals in a number -> invalid char
                if (dot_count == 1) {break;}
                dot_count += 1;
                num_str += '.';
            }
            
            else {
                num_str += current_char;
            }

            advance();
        }
        if (dot_count == 0){
            return Token(TT_INT, stoi(num_str));
        }

        else {
            return Token(TT_FLOAT, stof(num_str));
        }
    }


    Tokens_Error make_tokens(){
        Tokens_Error tokens;
        while (current_char!=0){
            if (current_char == '\t'){
                advance();
            }

            else if (current_char == '+'){
                tokens.tokens.push_back(Token(TT_PLUS));
                advance();
            }

            else if (current_char == '-'){
                tokens.tokens.push_back(Token(TT_MINUS));
                advance();
            }

            else if (current_char == '*'){
                tokens.tokens.push_back(Token(TT_MUL));
                advance();
            }

            else if (current_char == '/'){
                tokens.tokens.push_back(Token(TT_DIV));
                advance();
            }

            else if (current_char == '('){
                tokens.tokens.push_back(Token(TT_LPAREN));
                advance();
            }

            else if (current_char == ')'){
                tokens.tokens.push_back(Token(TT_RPAREN));
                advance();
            }

            else if (DIGITS.find(current_char) != string::npos){
                tokens.tokens.push_back(make_number());
                advance();
            }
            
            else {
                char character = current_char;
                advance();
                tokens.tokens = {};
                tokens.error = IllegalCharError("meow meow");
                return tokens;
            }
        }
        

        return tokens;
    }


};


Tokens_Error run(string text){
    Lexer lexer = Lexer(text);
    Tokens_Error structure = lexer.make_tokens();

    return structure;
};

int main(){
    bool end = false;
    string x;
    while (!end){
        std::cin >> x;
        if (x == "stop"){
            end = true;
        };

        Tokens_Error structure = run(x);

        auto tokens = structure.tokens;
        auto error = structure.error;

        for(Token token: tokens){
            token.print();
        };
    }
}
