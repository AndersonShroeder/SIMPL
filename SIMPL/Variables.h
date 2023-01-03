
// class VariableTable{
//     std::unordered_map<string, string> symbols = {};
//     std::unordered_map<string, string> parent;

//     public:
//     VariableTable(){}

//     string get(string name){
//         if (symbols.count(name)){
//             return symbols.at(name);
//         }
//         else if (parent.count(name)){
//             return symbols.at(name);
//         }
//         else{
//             std::cout << "MAKE ERROR - variable not in table" << '\n';
//             return "";
//         }
//     }

//     void set(string name, string value){
//         symbols[name] = value;
//     }

//     void remove(string name){
//         symbols.erase(name);
//     }
// };