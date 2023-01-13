#include "Constants.h"

struct Position
{
    bool end_file = false;
    int line = 1;
    int column = -1;
    char current_char;
    string filename;
    string text;
    string line_text;

    Position() {}

    Position(string txt, string fn)
    {
        // text = txt;
        filename = fn;
        line_text = txt;
        ++(*this);
    }

    void operator++()
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
        return "<" + filename + "> " + "Line: " + std::to_string(line) + ", Column: " + std::to_string(column) + '\n';
    }

    char *get_current_char()
    {
        return &current_char;
    }

    bool &stop()
    {
        return end_file;
    }
};

class Error
{
    string error_details;
    string error_name;
    Position pos;

public:
    bool error = false;

    Error() {}

    Error(string error_name, string error_details, Position pos)
    {
        this->error_name = error_name;
        this->error_details = error_details;
        this->pos = pos;
    }

    string str()
    {
        return pos.str() + error_name + ": " + error_details + '\n';
    }
};

class InvalidChar : public Error
{
public:
    InvalidChar(string character, Position pos) : Error("Invalid Character", character, pos) {}
};

class InvalidNum : public Error
{
public:
    InvalidNum(string num, Position pos) : Error("Invalid Number Character", num, pos) {}
};

class MissingCharacter : public Error
{
public:
    MissingCharacter(string character, Position pos) : Error("Missing Character", character, pos) {}
};

class Expected : public Error
{
public:
    Expected(string character, Position pos) : Error("Expected", character, pos) {}
};




class Result
{
    std::shared_ptr<Error> error = NULL;

public:

    Result() {}

    void make_error(std::shared_ptr<Error> error)
    {
        this->error = error;
    }

    bool check_error()
    {
        return error != NULL;
    }

    void print_error()
    {
        std::cout << error->str() << '\n';
    }
};