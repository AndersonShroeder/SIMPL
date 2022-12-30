#include <string>
#include <iostream>
#include <sstream>
#include <unordered_map>
using std::string;

const string DIGITS = "0123456789";
const string TT_INT = "TT_INT";
const string TT_FLOAT = "FLOAT";
const string TT_DIV = "/";
const string TT_MUL = "*";
const string TT_PLUS = "+";
const string TT_MINUS = "-";

//Use a map to generate tokens -> index into a map constant with current_char to find the associated value
std::unordered_map<string, string> SYMBOLS = {
    {"+", "PLUS"},
    {"-", "MINUS"},
    {"*", "MUL"},
    {"/", "DIV"},
    {"(", "LPAREN"},
    {")", "RPAREN"},
    {"fn", "FUNCTION"},
    {"if", "IF"},
    {"while", "WHILE"},
    {"for", "FOR"},
    {"foreach", "FOREACH"},
    {"class","CLASS"},
    {"let", "VARIABLE"}
};