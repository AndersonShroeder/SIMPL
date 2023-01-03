#include "Constants.h"

class Float{
    float value;
    string string_repr;

    public:
    Float(){
        value = 0;
        string_repr = "0";
    };

    Float(float value){
        this->value = value;
        this->string_repr = std::to_string(value);
    }

};