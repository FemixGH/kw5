#ifndef __OUTPUTCLASS__H
#define __OUTPUTCLASS__H
#include "cl_base.h"
class outputs : public cl_base {
public:
    outputs(cl_base *p_head_obj, string s_obj_name = "output_obj");
    void signal(string &mess);
    void handler(string &mess);
};
#endif