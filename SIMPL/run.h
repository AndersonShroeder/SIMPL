//#include "RecDescentParse.h"
#include "Interpreter.h"

FileData run(string text, string filename){
    Lexer lexer = Lexer(text, filename);
    FileData data = lexer.make_tokens();

    return data;
};