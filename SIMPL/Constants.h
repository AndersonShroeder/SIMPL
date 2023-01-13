#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>
#include <unordered_map>
#include <set>
#include <functional>
#include <tuple>
#include <memory>
#include <vector>
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
const string TT_ID = "ID";

// const std::unordered_map<string, string> TOKEN_TYPES = {
//     {"+", "PLUS"},
//     {"-", "MINUS"},
//     {"*", "MUL"},
//     {"/", "DIV"},
//     {"^", "EXP"},
//     {"%", "MOD"},
//     {"(", "LPAREN"},
//     {")", "RPAREN"},
// };

const std::unordered_map<string, string> SINGLE_SYMBOLS = {
    {"(", "LPAREN"},
    {")", "RPAREN"},
    {"{", "SOB"}, // start of block
    {"}", "EOB"}, // end of block
    {";", "EOL"},
    {",", "SEP"},
};

const std::unordered_map<string, string> SYMBOLS = {
    {"+", "PLUS"},
    {"-", "MINUS"},
    {"*", "MUL"},
    {"/", "DIV"},
    {"^", "EXP"},
    {"%", "MOD"},
    {"(", "LPAREN"},
    {")", "RPAREN"},
    {"=", "EQ"},
    {"<", "LT"},
    {">", "GT"},
    {"<=", "LTE"},
    {">=", "GTE"},
    {"==", "EQUIV"},
    {"!=", "NOTEQ"},
    {"-=", "MINEQ"},
    {"+=", "PLUSEQ"},
    {"++", "INCREMENT"},
    {"--", "DECREMENT"},
    {":", "COLON"},
    {"{", "SOB"}, // start of block
    {"}", "EOB"}, // end of block
    {";", "EOL"},
    {",", "SEP"},
};

const std::unordered_map<string, string> KEYWORDS = {
    {"and", "AND"},
    {"or", "OR"},
    {"not", "NOT"},
    {"fn", "FUNCTION"},
    {"if", "IF"},
    {"elif", "ELIF"},
    {"else", "ELSE"},
    {"while", "WHILE"},
    {"for", "FOR"},
    {"foreach", "FOREACH"},
    {"class", "CLASS"},
    {"let", "VARIABLE"}};

const std::unordered_map<string, string> VARIABLE_TYPES = {
    {"int", "FUNCTION"},
    {"float", "IF"},
    {"string", "WHILE"},
    {"bool", "FOR"},
    {"void", "FOREACH"},
};