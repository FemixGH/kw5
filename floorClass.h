#ifndef __FLOORCLASS__H
#define __FLOORCLASS__H
#include "base.h"
#include "passengerClass.h"
class floorClass : public base {
private:
    int floor_num;
    vector<passengerClass *> floor_passengers;
public:
    floorClass(base *p_head_obj, string s_obj_name = "Floor");

    void signal(string &mess);

    void handler(string &mess);

    void set_floor_num(int floor_num);

    int get_floor_num();

    int num_of_pass();

    int num_up();

    void add_passenger(passengerClass *passenger_obj);

    vector<passengerClass *> passengers_up_down(string direction);
};
#endif