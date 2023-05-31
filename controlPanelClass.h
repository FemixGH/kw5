#ifndef __CONTROLPANELCLASS__H
#define __CONTROLPANELCLASS__H
#include "base.h"
class controlPanelClass : public base {
private:
    int lift_capacity;
    int floor_quant;
    int curr_floor = 1;
    string direction = "none";
    int curr_pass_quant = 0;
    vector<map<string, vector<string>>> all_floor_passengers; //map по имени - начало конец направление
            vector<vector<string>> lift_passengers;
    vector <int> queue;
    set <int> priority_queue;
    vector <int> pass_were_on_floors;
    int nearest_floor;
    bool take_passengers(TYPE_SIGNAL p_signal);
//void next_tact(string mess);
//void add_passenger(string mess);
public:
    controlPanelClass(base *p_head_obj, string s_obj_name ="control_panel_obj");
    void signal(string &mess);
    void handler(string &mess);
    void set_lift_capacity(int m);
    void set_floor_quant(int n);
};
#endif