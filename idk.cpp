#include <string>
#include <iostream>
#include <sstream>
#include<fstream>
#include <cmath>
#include <unordered_map>
#include <set>
#include <functional>
#include <tuple>
using std::string;

struct Value
{
    union
    {
        int val;
        string val;
        bool val;
        float val;
    };

    Value(auto value)
    {
        val = value;
    }
};

//Token class -> stores a tokenized value and the type associated
class Token{
    string type;
    Value value;

    public:
    Token(){}

    //constructor
    Token(type, string value="0")
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
