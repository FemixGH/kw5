#ifndef __LIFTCLASS__H
#define __LIFTCLASS__H
#include "base.h"
class liftClass : public base {
private:
    int capacity;
    int curr_floor = 1;
    string direction = "none";
    int num_of_lift_pass = 0;
public:
    liftClass(base *p_head_obj, string s_obj_name = "lift_obj");
    void signal(string &mess);
    void handler(string &mess);
    void set_capacity(int capacity);
    int get_curr_floor();
    int get_curr_num_of_pass();
    string get_direction();
};
#endif