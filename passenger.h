#ifndef __PASSENGERCLASS__H
#define __PASSENGERCLASS__H
#include "cl_base.h"
class passenger : public cl_base {
private:
    string direction;
    int start_floor;
    int end_floor;
public:
    passenger(cl_base *p_head_obj, string s_obj_name = "passenger");
    void signal(string &mess);
    void handler(string &mess);
    void set_options(int start_floor, int end_floor);
    string get_direction();
};
#endif