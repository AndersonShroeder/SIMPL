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
    float value;

    public:
    Token(){}

    //constructor
    Token(string type_, float value_=0){
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


//Lexer Class -> generates tokens/errors from input text
class FileData{

    //contains tokens
    std::vector<Token> tokens;

    //keeps track of errors in lexing
    string error_details;
    bool error = false;

    //information to traverse input text
    string text;
    bool end_file = false;
    int pos = -1;
    char current_char;
    
    public:
    FileData(){};

    FileData(string text_, string filename){
        text = text_;
        ++(*this);
    }

    //Moves Lexer along input string -> only updates current_char if havent reached end of text
    void operator ++(){
        pos += 1;
        if (pos < text.length()){
            current_char = text.at(pos);
        }

        else{
            end_file = true;
            current_char = 0;
        }
    }

    std::vector<Token> get_tokens(){
        return tokens;
    }

    void add_token(Token token_to_add){
        tokens.push_back(token_to_add);
    }

    void clear_tokens(){
        tokens = {};
    }

    void make_error(string input_error){
        error_details = input_error;
        error = true;
    }

    string get_error(){
        return error_details;
    }

    string get_text(){
        return text;
    }

    int get_position(){
        return pos;
    }

    char get_current_char(){
        return current_char;
    }

    bool check_error(){
        return error;
    }

    bool stop(){
        return end_file;
    }
};

class Lexer{
    public:

    FileData file;
    
    Lexer(string text, string filename){
        file = FileData(text, filename);
    }

    void make_keyword(){
    }

    //Makes a number token
    Token make_number(){
        string num_str;
        int dot_count = 0;

        //continue iteration as long as current_char exists and the current_char is either a num or a decimal
        while (!file.stop() && ((DIGITS+'.').find(file.get_current_char()) != string::npos)){
            
            if (isdigit(file.get_current_char())){
                num_str += file.get_current_char();
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
                return Token(TT_FLOAT, stof(num_str));
            default:
                return Token(TT_INT, stoi(num_str));
        }
    }

    //Produces a Tokens_Error structure which contains a vector of errors and a vector of tokens 
    FileData make_tokens(){

        //checks every character until reached the end
        while (file.get_current_char()!=0){
            if (isdigit(file.get_current_char())){
                file.add_token(make_number());
            }
            
            else {
                string str_char;
                str_char = file.get_current_char();

                try{ 
                    if (SYMBOLS.count(str_char) != 0){
                        file.add_token(Token(SYMBOLS.at(str_char)));
                        ++file;
                    }

                    else if (file.get_current_char() == '\t' || file.get_current_char() == ' '){
                        ++file;
                    }

                    else {
                        throw (str_char);
                    }
                }

                catch (string str_char){
                    file.clear_tokens();
                    file.make_error("Invalid Character: " + str_char + '\n' + "Line 0, Column " + std::to_string(file.get_position() + 1));
                    return file;
                } 
        }
        }

        return file;
    }
};