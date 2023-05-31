#ifndef __OUTPUTCLASS__H
#define __OUTPUTCLASS__H
#include "base.h"
class outputClass : public base {
public:
    outputClass(base *p_head_obj, string s_obj_name = "output_obj");
    void signal(string &mess);
    void handler(string &mess);
};
#endif