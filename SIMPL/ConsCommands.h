#include "run.h"

class ConsoleInterface{
    public:

    bool toks = false;
    bool interp = true;
    bool syntax = false;
    bool cont = false;

    ConsoleInterface(){};

    void view_settings(bool& value, string name){
        value = !value;
        if (value){
            std::cout << "=========================" << '\n' << name << " Enabled" << '\n' << "=========================" << '\n';
        }
        else{
            std::cout << "=========================" << '\n' << name << " Disabled" << '\n' << "=========================" << '\n';
        }
    }

    void check_inputs(string& input){
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

        if (input == "-readfile"){
            string output = read_file();
            if (output == "FAIL"){
                cont = true;
            }
            input = output;
        }
    }

    string read_file()
    {
        string filename;
        string input;
        string line;

        std::cout << "Type Filename: ";
        std::getline(std::cin, filename);

        std::ifstream file(filename);

        if (file.fail())
        {
            std::cout << "File Failed To Open" << '\n';
            return "FAIL";
        }

        else
        {
            while (std::getline(file, line))
            {
                input+=line;
            }

            return input;
        }
    }
};
    