#include "Constants.h"

class Error
{
    string error_details;
    string error_name;

    public:

    bool error = false;

    Error(){}

    Error(string error_name, string error_details)
    {
        this->error_name = error_name;
        this->error_details = error_details;
        error = true;
    }

    void make_error(string type)
    {
        error_details = type;
        error = true;
    }

    string str()
    {
        return error_name + ": " + error_details;
    }

};

class InvalidChar: public Error
{
    public:
    InvalidChar(string character) : Error("Invalid Character", character){}
};

class InvalidNum: public Error
{
    public:
    InvalidNum(string num) : Error("Invalid Number Character", num){}
};

class MissingCharacter: public Error
{
    public:
    MissingCharacter(string character) : Error("Missing Character", character){}
};