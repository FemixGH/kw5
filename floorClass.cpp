#include "floorClass.h"
#include "passengerClass.h"
floorClass::floorClass(base *p_head_obj, string s_obj_name) :
        base(p_head_obj, s_obj_name){ cl_num = 3; }
void floorClass::signal(string &mess){ }
void floorClass::handler(string &mess){
    if((mess.substr(0, 1) == " ") && (find_obj_by_name(mess.substr(1))->get_head_object()->get_object_name() == this->get_object_name())){
        mess = mess.substr(1);
        floor_passengers.erase(find(floor_passengers.begin(),floor_passengers.end(), find_obj_name_curr(mess)));
    }
}
void floorClass::set_floor_num(int floor_num){
    this->floor_num = floor_num;
}
int floorClass::get_floor_num(){
    return floor_num;
}
int floorClass::num_of_pass(){
    return floor_passengers.size();
}
int floorClass::num_up(){
    int sum_up = 0;
    for (auto passenger_obj:floor_passengers){
        if(passenger_obj->get_direction() == "up"){
            sum_up ++;
        }
    }
    return sum_up;
}
void floorClass::add_passenger(passengerClass *passenger_obj){
    floor_passengers.push_back(passenger_obj);
}
vector <passengerClass*> floorClass::passengers_up_down(string direction){
    vector <passengerClass*> passengers_one_direction = {};
    for (auto passenger:floor_passengers){
        if(passenger->get_direction() == direction){
            passengers_one_direction.push_back(passenger);
        }
    }
    return passengers_one_direction;
}
