#include "NewLexer.h"

FileData run(string text, string filename){
    Lexer lexer = Lexer(text, filename);
    FileData data = lexer.make_tokens();

    return data;
};