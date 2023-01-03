#include "Nodes.h"

/////////////////////////////////////////////
// ADD
/////////////////////////////////////////////
//  1. Add seperate Evaluation methods instead of just preforming the evaluation in the eval() method
//  2. Add some form of context for error checking and variable assesment

//Parser class -> generates AST
class Parser{
    std::vector<Token> tokens;
    Token* current_tok;
    string current_type;
    int tok_idx = -1;

    public:
    //default constructor
    Parser(){}

    //constructor
    Parser(std::vector<Token> &tokens){
        this->tokens = tokens;
        ++(*this);
    }

    //advance method -> moves to next token to be parsed
    void operator ++(){
        tok_idx += 1;
        if (tok_idx < tokens.size()){
            current_tok = &tokens.at(tok_idx);
            current_type = (*current_tok).get_type();
        }
    }

    // evaluates if current_type is in s
    bool current_matches(std::set<string> s){
        return (s.find(current_type) != s.end());
    }


    Node* parse(){
        Node* res = expression();
        return res;
    }

    Node* if_expr(std::vector<std::tuple<Node*, Node*>>& cases){
        Node* else_case=NULL;
        ++(*this);

        if (!current_matches({"LPAREN"})){
            std::cout << "MAKE ERROR - paren must enclose conditional (EL/IF)" << '\n';
        }
        Node* condition = expression(); // get if condition

        if (!current_matches({"SOB"})){
            std::cout << "MAKE ERROR - invalid if statement (EL/IF)" << '\n';
        }

        ++(*this);
        Node* expr = expression(); // get expr to be executed

        if (!current_matches({"EOB"})){
            std::cout << "MAKE ERROR - missing enclosing braces (EL/IF)" << '\n';
        }

        ++(*this);
        cases.push_back(std::make_tuple(condition, expr)); //add condition/expr to data struct -> if cond is true expr is evaluated

        while (current_matches({"ELIF"})){
            if_expr(cases);
        }

        if (current_matches({"ELSE"})){
            ++(*this);
            if (!current_matches({"SOB"})){
                std::cout << "MAKE ERROR - invalid if statement (Else)" << '\n';
            }
            ++(*this);

            else_case = expression();
            ++(*this);
            if (!current_matches({"EOB"})){
                std::cout << "MAKE ERROR - missing enclosing braces (Else)" << '\n';
            }
            ++(*this);
        }

        return (new IfNode(cases, else_case));
    }


    Node* atom(){
        if (current_matches({"TT_INT", "TT_FLOAT"})){
            Node *node = new NumberNode((*current_tok));
            ++(*this);

            return node;
        }

        else if (current_matches({"ID"})){
            Node *node = new VarAccessNode((*current_tok));
            ++(*this);

            return node;
        }

        else if (current_matches({"LPAREN"})){
            ++(*this);
            Node *node = expression();

            if (!current_matches({"RPAREN"})){
                std::cout << "MAKE ERROR - No closing paranetheses" << '\n';
            }
            
            ++(*this); // accounts for right paren
            return node;
        }

        else if (current_matches({"IF"})){
            std::vector<std::tuple<Node*, Node*>> cases;
            return if_expr(cases);
        }

        // no appropriate values
        else{
            //std::cout << "MAKE ERROR - No appropriate values" << '\n';
            return (new NumberNode((*current_tok)));
        }
        
    }


    //Generates Factors
    Node* factor(){
        std::set<string> operands = {"EXP"};
        return binary_operation(operands, &Parser::atom, &Parser::factor);
    }


    //Generates Terms
    Node* term(){
        std::set<string> operands = {"MUL", "DIV", "MOD"};
        return binary_operation(operands, &Parser::factor);
    }


    Node* arith_expr(){
        std::set<string> operands = {"PLUS", "MINUS"};
        return binary_operation(operands, &Parser::term);
    }


    Node* comp_expr(){
        Token tok;
        if (current_matches({"NOT"})){
            tok = *current_tok;
            ++(*this);

            Node* node = comp_expr();
            return new UnaryOpNode(tok, node);
        }
        std::set<string> operands = {"EQUIV", "NOTEQ", "GT", "LT", "LTE", "GTE"};
        return binary_operation(operands, &Parser::arith_expr);
    }


    //Generates Expressions
    Node* expression(){
        if (current_matches({"VARIABLE"})){
            ++(*this);

            // no variable name after declaring variable
            if (!current_matches({"ID"})){
                std::cout << "MAKE ERROR - No variable name after decleration" << '\n';
            }
            Token identifier = *current_tok;
            ++(*this);

            // no equal sign after declaring varaible and name

            if (!current_matches({"EQ"})){
                std::cout << "MAKE ERROR - No assignment of variable" << '\n';
            }
            ++(*this);
            Node* node = expression();
            return (new VarAssignNode(identifier.get_value(), node));

        }
        std::set<string> operands = {"AND", "OR"};
        return binary_operation(operands, &Parser::comp_expr);
    }

    //Generalizes term/expr rules -> a set s is the accepted tokens to form either term/expr and func is term/expr function
    Node* binary_operation(std::set<string> s, Node* (Parser::*func)(), Node* (Parser::*func_b)() = NULL){

        //no func_b -> use func for both left/right
        if (func_b == NULL){
            func_b = func;
        }

        Node* left = (this->*func)();
        Node* right;
        Token op_tok;

        while (current_matches(s)){
            op_tok = (*current_tok);
            ++(*this);
            right = (this->*func_b)();
            Node* node = new BinOpNode(left, right, op_tok);
            left = node;
        }
        
        return left;
    }
};