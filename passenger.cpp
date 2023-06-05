#include "passenger.h"
passenger::passenger(cl_base *p_head_obj, string s_obj_name) :
        cl_base(p_head_obj, s_obj_name){ cl_num = 6; }
void passenger::signal(string &mess){ }
void passenger::handler(string &mess){ }
void passenger::set_options(int start_floor, int end_floor){
    this->start_floor = start_floor;
    this->end_floor = end_floor;
    if ((start_floor - end_floor) < 0){
        this->direction = "up";
    }
    else{
        this->direction = "down";
    }
}
string passenger::get_direction(){
    return direction;
}