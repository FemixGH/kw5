#include "lift.h"
lift::lift(cl_base *p_head_obj, string s_obj_name) : cl_base(p_head_obj, s_obj_name){ cl_num = 5; }
void lift::signal(string &mess){ }
void lift::handler(string &mess){
    if (mess.substr(0, 10) == "direction="){
        this->direction = mess.substr(10);
    }
    else if (mess == "curr_floor - 1"){
        curr_floor -= 1;
    }
    else if (mess == "curr_floor + 1"){
        curr_floor += 1;;
    }
    else if(mess.substr(0, 23) == "change_num_of_lift_pass"){
        num_of_lift_pass = stoi(mess.substr(24));
    }
    else if (mess.substr(0, 11) == "delete_pass"){
        mess = mess.substr(12);
        this->delete_subordinate_obj(mess);
    }
}
void lift::set_capacity(int capacity){
    this->capacity = capacity;
}
int lift::get_curr_floor(){
    return curr_floor;
}
int lift::get_curr_num_of_pass(){
    return num_of_lift_pass;
}
string lift::get_direction(){
    return direction;
}