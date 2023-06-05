#include "passengerClass.h"
passengerClass::passengerClass(cl_base *p_head_obj, string s_obj_name) :
        cl_base(p_head_obj, s_obj_name){ cl_num = 6; }
void passengerClass::signal(string &mess){ }
void passengerClass::handler(string &mess){ }
void passengerClass::set_options(int start_floor, int end_floor){
    this->start_floor = start_floor;
    this->end_floor = end_floor;
    if ((start_floor - end_floor) < 0){
        this->direction = "up";
    }
    else{
        this->direction = "down";
    }
}
string passengerClass::get_direction(){
    return direction;
}