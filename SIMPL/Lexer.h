#include "FileData.h"

class Lexer{
    public:

    FileData file;
    
    Lexer(string text, string filename){
        file = FileData(text, filename);
    }

    string make_keyword(){
        string keyword;

        //Iterate as long as havent reached the end of line and it is a letter. If it is not a letter and varflag is active -> variable.
        while (!file.stop() && (isalpha(file.get_current_char()))){
            keyword += file.get_current_char();
            ++file;
        }
        return keyword;
    }

    //Makes a number token
    Token make_number(){
        string num_str;
        int dot_count = 0;

        //continue iteration as long as current_char exists and the current_char is either a num or a decimal
        while (!file.stop() && ((DIGITS+'.').find(file.get_current_char()) != string::npos)){
            
            if (isdigit(file.get_current_char())){
                num_str += file.get_current_char();
            }
            
            else {         
                //cant have two decimals in a number -> invalid char
                if (dot_count != 0) {break;}
                
                dot_count += 1;
                num_str += '.';
            }

            ++file;
        }

        //if there are no decimals -> int / else float
        switch (dot_count){
            case 1:
                return Token(TT_FLOAT, stof(num_str));
            default:
                return Token(TT_INT, stoi(num_str));
        }
    }

    //Produces a Tokens_Error structure which contains a vector of errors and a vector of tokens 
    FileData make_tokens(){

        //checks every character until reached the end
        while (file.get_current_char()!=0){

            //if current character is a digit -> create number token
            if (isdigit(file.get_current_char())){
                file.add_token(make_number());
            }

            //if current character is alpha -> create a keyword token
            else if (isalpha(file.get_current_char())){
                string keyword = make_keyword();
                file.add_token(Token(SYMBOLS.at(keyword)));
            }
            
            else {
                string str_char;
                str_char = file.get_current_char();

                try{ 
                    if (SYMBOLS.count(str_char) != 0){
                        file.add_token(Token(SYMBOLS.at(str_char)));
                        ++file;
                    }

                    else if (file.get_current_char() == '\t' || file.get_current_char() == ' '){
                        ++file;
                    }

                    else {
                        throw (str_char);
                    }
                }

                catch (string str_char){
                    file.make_error("Invalid Character: " + str_char + '\n' + "Line 0, Column " + std::to_string(file.get_position() + 1));
                    return file;
                } 
        }
        }

        return file;
    }
};

//function that trys returned value from generate keyword


void try_keyword(string keyword){
    
}