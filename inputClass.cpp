#include "inputClass.h"
inputClass::inputClass(cl_base *p_head_obj, string s_obj_name) :
        cl_base(p_head_obj, s_obj_name){ cl_num = 2; }
void inputClass::signal(string &mess){ }
void inputClass::handler(string &mess){
    string input_data;
    if (mess == "read"){
        getline(cin, input_data);
        this->emit_signal(SIGNAL_D(inputClass::signal), input_data);
    }
}