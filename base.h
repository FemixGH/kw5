#ifndef __BASE__H
#define __BASE__H
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <set>
#include <math.h>
#define SIGNAL_D( signal_f ) ( TYPE_SIGNAL ) ( & signal_f )
#define HANDLER_D( handler_f ) ( TYPE_HANDLER ) ( & handler_f )
using namespace std;
class base;
typedef void (base ::* TYPE_SIGNAL) (string&);
typedef void (base ::* TYPE_HANDLER) (string);
class base{
private:
    string s_obj_name;
    base *p_head_obj;
    vector <base*> subordinate_objs;
    int mark = 1;
    struct connect {
        TYPE_SIGNAL p_signal;
        base* base_obj;
        TYPE_HANDLER p_handler;
    };
    vector <connect*> connects;
public:
    unsigned int cl_num;
    base(base *p_head_obj, string s_obj_name = "Base_object");
    ~base();
    bool set_object_name(string s_obj_name);
    string get_object_name();
    base *get_head_object();
    void print_subordinate_objects();
    base *get_subordinate_object(string s_obj_name);
    base *find_obj_name_curr(string s_obj_name);
    base *find_obj_by_name(string s_obj_name);
    void print_hierarchy();
    void print_hierarchy_with_mark_of_read();
    void set_mark_of_read(int mark);
    bool redefin_head(base *p_new_head_obj);
    void delete_sub(string s_obj_name);
    base *get_object_pointer_by_coord(string coord);
    void establish_conn(TYPE_SIGNAL p_signal, base *base_obj, TYPE_HANDLER
    p_handler);
    void delete_conn(TYPE_SIGNAL p_signal, base* base_obj, TYPE_HANDLER
    p_handler);
    void emit_sign(TYPE_SIGNAL p_signal, string s_comm);
    string get_abs_obj_coord();
    bool subordinate_objs_empty();
};
#endif