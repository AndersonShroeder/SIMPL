#include <vector>
//Token class -> stores a tokenized value and the type associated
class Token{
    string type;
    string value;

    public:
    Token(){}

    //constructor
    Token(string type, string value="0"){
        this->type = type;
        this->value = value;
    }

    //prints type and value of token
    void print(){
        std::cout << type << ": " << value << '\n';
    }
    string& str(){
        return type;
    }

    //getters
    string& get_type(){
        return type;
    }

    string get_value(){
        return value;
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

    FileData(string text, string filename){
        this->text = text;
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

    std::vector<Token>& get_tokens(){
        return tokens;
    }

    void add_token(Token token_to_add){
        tokens.push_back(token_to_add);
    }

    void make_error(string input_error){
        tokens = {};
        error_details = input_error;
        error = true;
    }

    string& get_error(){
        return error_details;
    }

    string& get_text(){
        return text;
    }

    int& get_position(){
        return pos;
    }

    char& get_current_char(){
        return current_char;
    }

    bool& check_error(){
        return error;
    }

    bool& stop(){
        return end_file;
    }
};
