#ifndef __FLOORCLASS__H
#define __FLOORCLASS__H
#include "cl_base.h"
#include "passenger.h"
class floor : public cl_base {
private:
    int floor_num;
    vector<passenger *> floor_passengers;
public:
    floor(cl_base *p_head_obj, string s_obj_name = "Floor");

    void signal(string &mess);

    void handler(string &mess);

    void set_floor_num(int floor_num);

    int get_floor_num();

    int num_of_pass();

    int num_up();

    void add_passenger(passenger *passenger_obj);

    vector<passenger *> passengers_up_down(string direction);
};
#endif