#ifndef __INPUTCLASS__H
#define __INPUTCLASS__H
#include "base.h"
class inputClass : public base {
public:
    inputClass(base *p_head_obj, string s_obj_name = "input_obj");
    void signal(string &mess);
    void handler(string &mess);
};
#endif