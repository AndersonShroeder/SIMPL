#include "NewLexer.h"

Tokens_Error run(string text, string filename){
    Lexer lexer = Lexer(text, filename);
    Tokens_Error structure = lexer.make_tokens();

    return structure;
};