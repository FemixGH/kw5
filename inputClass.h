#ifndef __INPUTCLASS__H
#define __INPUTCLASS__H
#include "cl_base.h"
class inputClass : public cl_base {
public:
    inputClass(cl_base *p_head_obj, string s_obj_name = "input_obj");
    void signal(string &mess);
    void handler(string &mess);
};
#endif