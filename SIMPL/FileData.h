#include <vector>
#include "Errors.h"



//Token class -> stores a tokenized value and the type associated
class Token{
    string type;
    string value;

    public:
    Token(){}

    //constructor
    Token(string type, string value="0")
    {
        this->type = type;
        this->value = value;
    }

    //prints type and value of token
    void print()
    {
        std::cout << type << ": " << value << '\n';
    }

    string& str()
    {
        return type;
    }

    string& get_type()
    {
        return type;
    }

    string get_value()
    {
        return value;
    }
};

struct Position
{
    bool end_file = false;
    int line = 1;
    int column = -1;
    char current_char;
    string filename;
    string text;
    string line_text;

    Position(){}

    Position(string& txt, string fn)
    {
        // text = txt;
        filename = fn;
        line_text = txt;
    }

    void operator ++()
    {
        column += 1;
        if (column < line_text.size())
        {
            current_char = line_text.at(column);
        }

        // else if (line < text.size() - 1){
        //     line += 1;
        //     line_text = text.at(line);
           
        //     column = -1;
        //     ++(*this);
        // }

        else
        {
            end_file = true;
            current_char = 0;
        }

    }

    string str()
    {
        return filename + " >> Line: " + std::to_string(line) + ", Column: " +  std::to_string(column);
    }


    char* get_current_char()
    {
        return &current_char;
    }

    bool& stop()
    {
        return end_file;
    }
};



class FileData
{

    //contains tokens
    std::vector<Token> tokens;

    Error error;
    Position pos;
    
    public:
    FileData(){};

    FileData(string& text, string filename)
    {
        this->pos = Position(text, filename);
        // this->error = Error();
        ++(*this);
    }

    void operator ++()
    {
        ++pos;
    }

    const std::vector<Token>& get_tokens()
    {
        return tokens;
    }

    void add_token(Token token_to_add)
    {
        tokens.push_back(token_to_add);
    }

    void make_error(Error input_error)
    {
        error = input_error;
        tokens = {};
    }

    string get_error()
    {
        return error.str();
    }

    Position& get_position()
    {
        return pos;
    }

    bool& check_error()
    {
        return error.error;
    }

    bool& stop()
    {
        return pos.stop();
    }
};
