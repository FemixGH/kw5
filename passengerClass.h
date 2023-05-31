#ifndef __PASSENGERCLASS__H
#define __PASSENGERCLASS__H
#include "base.h"
class passengerClass : public base {
private:
    string direction;
    int start_floor;
    int end_floor;
public:
    passengerClass(base *p_head_obj, string s_obj_name = "passenger");
    void signal(string &mess);
    void handler(string &mess);
    void set_options(int start_floor, int end_floor);
    string get_direction();
    string get_name();
};
#endif