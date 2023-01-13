#include "Errors.h"

// Token class -> stores a tokenized value and the type associated
class Token
{
    string type;
    string value;
    Position pos;

public:  

    Token() {}

    // constructor
    Token(Position pos, string type, string value = "0")
    {
        this->type = type;
        this->value = value;
        this->pos = pos;
    }

    // prints type and value of token
    void print()
    {
        std::cout << type << ": " << value << '\n';
    }

    string &str()
    {
        return type;
    }

    string &get_type()
    {
        return type;
    }

    string &get_value()
    {
        return value;
    }

    Position get_position()
    {
        return pos;
    }
};

class Lexer
{
    bool keyword = false;
    char *current_char;
    std::vector<Token> tokens;
    Position pos;
    Result* res;

public:
    Lexer(string text, string filename, Result* res)
    {
        this->pos = Position(text, filename);
        this->current_char = (pos.get_current_char());
        this->res = res;
    }

    void add_token(Token tok)
    {
        tokens.push_back(tok);
    }

    bool is_valid_keyword_char(char character)
    {
        if (!pos.stop() && (isalpha(character) || character == '_' || isdigit(character)))
        {
            return true;
        }
        return false;
    }

    bool is_valid_number_char(char character)
    {
        if (!pos.stop() && (DIGITS + '.').find(character) != string::npos)
        {
            return true;
        }
        return false;
    }

    bool is_valid_symbol_char(char character)
    {
        return (!is_valid_keyword_char(*current_char) && !is_valid_number_char(*current_char) && *current_char != ' ' && !pos.stop());
    }

    bool is_single_char_symbol(string symbol)
    {
        return SINGLE_SYMBOLS.count(symbol);
    }

    void make_symbol()
    {
        string symbol;
        Token tok;

        // optimize this
        while (is_valid_symbol_char(*current_char) && !is_single_char_symbol(symbol))
        {
            symbol += *current_char;
            ++pos;
        }

        if (SYMBOLS.count(symbol))
        {
            tok = Token(pos, SYMBOLS.at(symbol));
        }

        else
        {
            res->make_error(std::make_unique<InvalidChar>(symbol, pos));
            return;
        }

        add_token(tok);
    }

    void make_keyword()
    {
        string keyword;
        Token tok;

        while (is_valid_keyword_char(*current_char))
        {
            keyword += *current_char;
            ++pos;
        }

        // if generated word isnt in keywords -> it is an Identifier
        try
        {
            tok = Token(pos, KEYWORDS.at(keyword));
        }

        catch (const std::out_of_range &e)
        {
            tok = Token(pos, TT_ID, keyword);
        }

        add_token(tok);
    }

    // Makes a number token
    void make_number()
    {
        string num_str;
        int dot_count = 0;

        // continue iteration as long as current_char exists and the current_char is either a num or a decimal
        while (is_valid_number_char(*current_char))
        {

            if (isdigit(*current_char))
            {
                num_str += *current_char;
            }

            else
            {
                dot_count += 1;
                num_str += '.';
                // cant have two decimals in a number -> invalid char
                if (dot_count != 1)
                {
                    res->make_error(std::make_unique<InvalidNum>(num_str, pos));
                    return;
                }
            }

            ++pos;
        }

        // if there are no decimals -> int / else float
        switch (dot_count)
        {
        case 1:
            add_token(Token(pos, TT_FLOAT, num_str));
        default:
            add_token(Token(pos, TT_INT, num_str));
        }
    }

    std::vector<Token> make_tokens()
    {
        while (*current_char != 0)
        {
            if (res->check_error())
            {
                return tokens;
            }

            if (*current_char == '\t' || *current_char == ' ')
            {
                ++pos;
            }

            // create number token
            else if (isdigit(*current_char))
            {
                make_number();
            }

            // create a keyword/ID token
            else if (isalpha(*current_char))
            {
                make_keyword();
            }

            // create symbol token
            else
            {
                make_symbol();
            }
        }

        return tokens;
    }
};
