#include "Constants.h"

class String{
    string value;
    int length = 0;

    public:
    String(){}

    String(string value){
        this->value = value;
        this->length = value.length();
    }

    int get_length(){
        return length;
    }

    string get_value(){
        return value;
    }

    char index(int position){
        return value.at(position);
    }

    String operator +(String& s1){
        String s2 = String((this->value + s1.get_value()));
        return s2;
    }

    String operator +(int& i){
        String s2 = String((this->value + std::to_string(i)));
        return s2;
    }

    String operator +(String& s1){
        String s2 = String((this->value + s1.get_value()));
        return s2;
    }

};