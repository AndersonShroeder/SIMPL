
// class VariableTable{
//     public:
//     std::unordered_map<string, string> symbols = {};
//     VariableTable* parent=NULL;

//     VariableTable(){}

//     VariableTable(VariableTable* parent)
//     {
//         this->parent = parent;
//     }

//     // checks if the variable name is in local scope, if not go up through scopes and check
//     string get(string name){
//         if (contains(name)){
//             return symbols.at(name);
//         }

//         else if (parent != NULL)
//         {
//             return parent->get(name);
//         }

//         std::cout << "MAKE ERROR - variable not in table" << '\n';
//         return "";
        
//     }

//     VariableTable* check_for_var(string name){
//         if (contains(name)){
//             return this;
//         }

//         else if (parent != NULL)
//         {
//             return parent->check_for_var(name);
//         }

//         return NULL;
        
//     }


//     bool contains(string name)
//     {
//         return symbols.count(name);
//     }


//     void set(string name, string value)
//     {
//         VariableTable* ptr = check_for_var(name);
//         if (ptr)
//         {
//             ptr->symbols[name] = value;
//         }
//         else
//         {
//             symbols[name] = value;
//         }
//     }

//     void remove(string name){
//         symbols.erase(name);
//     }
// };