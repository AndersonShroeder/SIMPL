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
    Parser(std::vector<Token>& tokens)
    {
        this->tokens = tokens;
        ++(*this);
    }


    //advance method -> moves to next token to be parsed
    void operator ++()
    {
        tok_idx += 1;
        if (tok_idx < tokens.size()){
            current_tok = &tokens.at(tok_idx);
            current_type = (*current_tok).get_type();
        }
    }


    // evaluates if current_type is in s
    bool current_matches(std::set<string> s)
    {
        return (s.find(current_type) != s.end());
    }


    std::shared_ptr<Node> parse()
    {
        std::shared_ptr<Node> res = expression();
        return res;
    }


    std::shared_ptr<Node> if_expr(std::vector<std::tuple<std::shared_ptr<Node>, std::shared_ptr<Node>>>& cases)
    {
        std::shared_ptr<Node> else_case=NULL;
        ++(*this);

        if (!current_matches({"LPAREN"})){
            std::cout << "MAKE ERROR - paren must enclose conditional (EL/IF)" << '\n';
        }
        std::shared_ptr<Node> condition = expression(); // get if condition

        if (!current_matches({"SOB"})){
            std::cout << "MAKE ERROR - invalid if statement (EL/IF)" << '\n';
        }

        ++(*this);
        std::shared_ptr<Node> expr = expression(); // get expr to be executed

        if (!current_matches({"EOB"})){
            std::cout << "MAKE ERROR - missing enclosing braces (EL/IF)" << '\n';
        }

        ++(*this);
        cases.push_back(std::make_tuple(condition, expr)); //add condition/expr to data struct -> if cond is true expr is evaluated

        while (current_matches({"ELIF"})){
            ++(*this);
            std::shared_ptr<Node> condition = expression();
            ++(*this);
            std::shared_ptr<Node> expr = expression();
            ++(*this);
            ++(*this);
            cases.push_back(std::make_tuple(condition, expr));
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

        return (std::make_unique<IfNode>(cases, else_case));
    }

    std::shared_ptr<Node> for_expr()
    {
        ++(*this);
        //check for LPAREN
        ++(*this);
        //check for VAR -> variable assignment in loop scope
        std::shared_ptr<Node> var = expression();
        //check for comma
        ++(*this);
        //end cond
        std::shared_ptr<Node> end_condition = expression();
        //check for comma
        ++(*this);
        std::shared_ptr<Node> step_condition = expression();
        //check for RPAREN
        ++(*this);
        //check for SOB
        ++(*this);
        std::shared_ptr<Node> execute = expression();
        //check for EOB

        return std::make_unique<ForNode>(end_condition, step_condition, execute, var);
    }

    void check_paren()
    {
        ++(*this);
        //check for LPAREN
        ++(*this);
        while (current_matches({"SEP"}))
        {

        }


    }

    std::shared_ptr<Node> while_expr()
    {
        ++(*this);
        //check for LPAREN
        ++(*this);
        std::shared_ptr<Node> condition = expression();
        //check for RPAREN
        ++(*this);
        //check for SOB
        ++(*this);
        std::shared_ptr<Node> execute = expression();
        //check for EOB
        return std::make_unique<WhileNode>(condition, execute);
    }

    std::shared_ptr<Node> func_def()
    {
        Token var_name_tok;
        std::vector<Token> arg_name_toks;
        ++(*this);
        if (current_matches({"ID"}))
        {
            var_name_tok = (*current_tok);
            ++(*this);
            //expect lParen
        }
        else
        {
            // set var_name_tok to null
            //expected lparen
        }

        ++(*this);

        if (current_matches({"ID"}))
        {
            arg_name_toks.push_back((*current_tok));
            ++(*this);

            while (current_matches({"SEP"}))
            {
                ++(*this);
                //check for id
                arg_name_toks.push_back((*current_tok));
                ++(*this);
            }
        }
        //check for rparen - expected rparen or comma
        else
        {
            //check for rparen -> expected id or parent
        }
        ++(*this);
        //check for curly braces
        ++(*this);
        std::shared_ptr<Node> node_to_return = expression();
        ++(*this);
        //check for curly brace
        return std::make_unique<FuncDefNode>(var_name_tok, arg_name_toks, node_to_return);
    }

    std::shared_ptr<Node> call()
    {
        std::shared_ptr<Node> node = atom();
        if (current_matches({"LPAREN"}))
        {
            ++(*this);
            std::vector<std::shared_ptr<Node>> args;
            if (current_matches({"RPAREN"}))
            {
                ++(*this);
            }
            else
            {
                args.push_back(expression());
                while (current_matches({"SEP"}))
                {
                    ++(*this);
                    args.push_back(expression());
                }
            }
            return std::make_unique<CallNode>(node, args);
        }
        return node;
    }

    std::shared_ptr<Node> atom(){
        if (current_matches({"TT_INT", "TT_FLOAT"})){
            std::shared_ptr<Node> node = std::make_unique<NumberNode>((*current_tok));
            ++(*this);

            return node;
        }

        else if (current_matches({"ID"})){
            std::shared_ptr<Node> node = std::make_unique<VarAccessNode>((*current_tok));
            ++(*this);

            return node;
        }

        else if (current_matches({"LPAREN"})){
            ++(*this);
            std::shared_ptr<Node> node = expression();

            if (!current_matches({"RPAREN"})){
                std::cout << "MAKE ERROR - No closing paranetheses" << '\n';
            }
            
            ++(*this); // accounts for right paren
            return node;
        }

        else if (current_matches({"IF"})){
            std::vector<std::tuple<std::shared_ptr<Node>, std::shared_ptr<Node>>> cases;
            return if_expr(cases);
        }

        else if (current_matches({"FOR"})){
            std::vector<std::tuple<std::shared_ptr<Node>, std::shared_ptr<Node>>> cases;
            return for_expr();
        }

        else if (current_matches({"WHILE"})){
            std::vector<std::tuple<std::shared_ptr<Node>, std::shared_ptr<Node>>> cases;
            return while_expr();
        }

        else if (current_matches({"FUNC"})){
            std::vector<std::tuple<std::shared_ptr<Node>, std::shared_ptr<Node>>> cases;
            return func_def();
        }

        // no appropriate values
        else{
            //std::cout << "MAKE ERROR - No appropriate values" << '\n';
            return (std::make_unique<NumberNode>((*current_tok)));
        }
        
    }

    std::shared_ptr<Node> power()
    {
        std::set<string> operands = {"EXP"};
        return binary_operation(operands, &Parser::call, &Parser::factor);
    }

    //Generates Factors
    std::shared_ptr<Node> factor(){
        std::set<string> operands = {"EXP"};
        return binary_operation(operands, &Parser::atom, &Parser::factor);
    }


    //Generates Terms
    std::shared_ptr<Node> term(){
        std::set<string> operands = {"MUL", "DIV", "MOD"};
        return binary_operation(operands, &Parser::factor);
    }


    std::shared_ptr<Node> arith_expr(){
        std::set<string> operands = {"PLUS", "MINUS"};
        return binary_operation(operands, &Parser::term);
    }


    std::shared_ptr<Node> comp_expr(){
        Token tok;
        if (current_matches({"NOT"})){
            tok = *current_tok;
            ++(*this);

            std::shared_ptr<Node> node = comp_expr();
            return std::make_unique<UnaryOpNode>(tok, node);
        }

        std::set<string> operands = {"EQUIV", "NOTEQ", "GT", "LT", "LTE", "GTE"};
        return binary_operation(operands, &Parser::arith_expr);
    }

    std::shared_ptr<Node> make_scope()
    {
        std::vector<std::shared_ptr<Node>> expressions;
        expressions.push_back(expression());

        // if the scope expression is correct -> this will always return
        while (current_matches({"EOL"}))
        {
            ++(*this);

            if (current_matches({"EOB"}))
            {
                ++(*this);
                return std::make_unique<Scope>(expressions);
            }

            expressions.push_back(expression());

            //No "EOL"

            if (!current_matches({"EOL"}))
            {
                std::cout << "Expected ;\n";
            }
        }

        

        if (current_matches({"EOB"}))
        {
            std::cout << "Expected }\n";
        }

        //Return -> if this statement is ever reached, there is an error
        return std::make_unique<Scope>(expressions);
    }

    //Generates Expressions
    std::shared_ptr<Node> expression(){
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
            std::shared_ptr<Node> node = expression();
            return (std::make_unique<VarAssignNode>(identifier.get_value(), node));

        }

        //If there is a scope we must create a scoped node
        if (current_matches({"SOB"}))
        {
            ++(*this);
            return make_scope();
        }

        std::set<string> operands = {"AND", "OR"};
        return binary_operation(operands, &Parser::comp_expr);
    }

    //Generalizes term/expr rules -> a set s is the accepted tokens to form either term/expr and func is term/expr function
    std::shared_ptr<Node> binary_operation(std::set<string> s, std::shared_ptr<Node> (Parser::*func)(), std::shared_ptr<Node> (Parser::*func_b)() = NULL){

        //no func_b -> use func for both left/right
        if (func_b == NULL){
            func_b = func;
        }

        std::shared_ptr<Node> left = (this->*func)();
        std::shared_ptr<Node> right;
        Token op_tok;

        while (current_matches(s)){
            op_tok = (*current_tok);
            ++(*this);
            right = (this->*func_b)();
            std::shared_ptr<Node> node = std::make_unique<BinOpNode>(left, right, op_tok);
            left = node;
        }
        
        return left;
    }
};