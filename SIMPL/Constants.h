#include <string>
#include <iostream>
#include <sstream>
#include <cmath>
#include <unordered_map>
#include <set>
#include <functional>
using std::string;

const string DIGITS = "0123456789";
const string TT_INT = "TT_INT";
const string TT_FLOAT = "FLOAT";
const string TT_DIV = "/";
const string TT_MUL = "*";
const string TT_PLUS = "+";
const string TT_MINUS = "-";
const string TT_LPAREN = "(";
const string TT_RPAREN = ")";

//Use a map to generate tokens -> index into a map constant with current_char to find the associated value
std::unordered_map<string, string> SYMBOLS = {
    {"+", "PLUS"},
    {"-", "MINUS"},
    {"*", "MUL"},
    {"/", "DIV"},
    {"^", "EXP"},
    {"%", "MOD"},
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