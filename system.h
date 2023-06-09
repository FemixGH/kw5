#ifndef __SYSTEMCLASS__H
#define __SYSTEMCLASS__H
#include "cl_base.h"
#include "floor.h"
class system : public cl_base{
private:
    vector <floor*> floors;
    string input_data = "";
    int tact = 0;
    int lift_capacity = 0;
    int floors_q = 0;
    TYPE_SIGNAL get_signal_pointer(int cl_num);
    TYPE_HANDLER get_handler_pointer(int cl_num);
    void set_conn(cl_base *signal_obj, cl_base *handler_obj);
public:
    system(cl_base *p_head_obj, string s_obj_name = "modeling_system_obj");
    void build_tree();
    int start_app();
    void signal(string &mess);
    void handler(string &mess);
    void set_lift_capacity(int m);
    void set_floor_q(int n);
    cl_base *passenger_pointer(string name);

};
#endif