#include "floor.h"
#include "passenger.h"
floor::floor(cl_base *p_head_obj, string s_obj_name) :
        cl_base(p_head_obj, s_obj_name){ cl_num = 3; }
void floor::signal(string &mess){ }
void floor::handler(string &mess){
    if((mess.substr(0, 1) == " ") && (search_by_name(mess.substr(1))->get_head()->get_name() == this->get_name())){
        mess = mess.substr(1);
        floor_passengers.erase(find(floor_passengers.begin(),floor_passengers.end(), find_obj_name_curr(mess)));
    }
}
void floor::set_floor_num(int floor_num){
    this->floor_num = floor_num;
}
int floor::get_floor_num(){
    return floor_num;
}
int floor::num_of_pass(){
    return floor_passengers.size();
}
int floor::num_up(){
    int sum_up = 0;
    for (auto passenger_obj:floor_passengers){
        if(passenger_obj->get_direction() == "up"){
            sum_up ++;
        }
    }
    return sum_up;
}
void floor::add_passenger(passenger *passenger_obj){
    floor_passengers.push_back(passenger_obj);
}
vector <passenger*> floor::passengers_up_down(string direction){
    vector <passenger*> passengers_one_direction = {};
    for (auto passenger:floor_passengers){
        if(passenger->get_direction() == direction){
            passengers_one_direction.push_back(passenger);
        }
    }
    return passengers_one_direction;
}
