#ifndef __LIFTCLASS__H
#define __LIFTCLASS__H
#include "cl_base.h"
class lift : public cl_base {
private:
    int capacity;
    int curr_floor = 1;
    string direction = "none";
    int num_of_lift_pass = 0;
public:
    lift(cl_base *p_head_obj, string s_obj_name = "lift_obj");
    void signal(string &mess);
    void handler(string &mess);
    void set_capacity(int capacity);
    int get_curr_floor();
    int get_curr_num_of_pass();
    string get_direction();
};
#endif