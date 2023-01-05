#include "run.h"

void view_settings(bool& value, string name){
    value = !value;
    if (value){
        std::cout << "=========================" << '\n' << name << " Enabled" << '\n' << "=========================" << '\n';
    }
    else{
        std::cout << "=========================" << '\n' << name << " Disabled" << '\n' << "=========================" << '\n';
    }
}

class Commands{
    public:

    bool toks = false;
    bool interp = true;
    bool syntax = true;
    bool cont = false;

    Commands(){};

    void check_inputs(string input){
        cont = false;
        if (input == "-t"){
            view_settings(toks, "Token View");
            cont = true;
        }

        if (input == "-stop"){
            cont = true;
        }

        if (input == "-i"){
            view_settings(interp, "Interpretor");
            cont = true;
        }

        if (input == "-s"){
            view_settings(syntax, "AST View");
            cont = true;
        }

        // if (input == "-readfile"){
        //     string output = read_file();
        //     if (output == "FAIL"){
        //         cont = true;
        //     }
        //     input = output;
        // }
    }
};
    