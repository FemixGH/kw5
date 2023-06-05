#include "outputs.h"
outputs::outputs(cl_base *p_head_obj, string s_obj_name) :
        cl_base(p_head_obj, s_obj_name){ cl_num = 7; }
void outputs::signal(string &mess){ }
void outputs::handler(string &mess){
    if (mess == "Ready to work"){ cout << mess; return;}
    if (mess != "read" && mess != "next_tact" && mess.substr(0, 13) !=
                                                 "add_passenger" && \
mess.substr(0, 1) != " "){
        cout << endl << mess;
    }
}