#ifndef __SYSTEMCLASS__H
#define __SYSTEMCLASS__H
#include "base.h"
#include "floorClass.h"
class systemClass : public base{
public:
    systemClass(base *p_head_obj, string s_obj_name = "modeling_system_obj");
    void build_tree();
    int start_app();
    void signal(string &mess);
    void handler(string &mess);
    void set_lift_capacity(int m);
    void set_floor_quant(int n);
    base *passenger_pointer(string name);
private:
    vector <floorClass*> floors;
    string input_data = "";
    int tact = 0;
    int lift_capacity = 0;
    int floors_quant = 0;
    TYPE_SIGNAL get_signal_pointer(int cl_num);
    TYPE_HANDLER get_handler_pointer(int cl_num);
    void set_conn(base *signal_obj, base *handler_obj);
};
#endif