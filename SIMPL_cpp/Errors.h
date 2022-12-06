#include "Constants.h"

//Position class to track position of errors
class Position{
    int idx;
    int ln;
    int col;
    string ftxt;
    string fn;

    public:
    //default constructor
    Position(){}

    //constructor
    Position(int index, int line, int column, string filetxt, string filename){
        idx = index;
        ln = line;
        col = column;
        ftxt = filetxt;
        fn = filename;
    }

    //moves the position tracker over -> if end of line the line counter is updated
    void advance(char current_char = 0){
        idx += 1;
        col += 1;

        if (current_char == '\n'){
            ln += 1;
            col = 0;
        }
    }

    //getters
    int get_idx(){
        return idx;
    }

    int get_col(){
        return col;
    }

    int get_ln(){
        return ln;
    }

    string get_fn(){
        return fn;
    }
};

//Parent Error class -> stores an error name and the details
class Error{
    string error_name;
    string error_details;
    Position pos_start;
    Position pos_end;
    
    public:
    //default constructor
    Error(){}

    //constructor -> positions by reference because they must be modified
    Error(string error_name_, string error_details_, Position& pos_start_, Position& pos_end_){
        error_name = error_name_;
        error_details = error_details_;
        pos_start = pos_start_;
        pos_end = pos_end_;
    }

    //string representation method
    std::stringstream as_string(){
        std::stringstream fmt;
        fmt << error_name << ": " << error_details;
        fmt << "\nFile " << pos_start.get_fn() << ", line " << pos_start.get_ln() + 1;
        return fmt;
    }

    void print(){
        auto result = as_string();
        std::cout << result.str() << '\n';
    }
};


//Illegal Character Error -> inherits Error/represents unparsable values
class IllegalCharError : public Error{
    public:

    IllegalCharError(string details, Position& pos_start, Position& pos_end): 
    Error("Illegal Character", details, pos_start, pos_end){}
};

class InvalidSyntaxError: public Error{
    public:

    InvalidSyntaxError(string details, Position& pos_start, Position& pos_end):
    Error("Invalid Syntax", details, pos_start, pos_end){}
};
