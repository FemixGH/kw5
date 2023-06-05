//#ifndef __BASE__H
//#define __BASE__H
//#include <iostream>
//#include <string>
//#include <vector>
//#include <map>
//#include <algorithm>
//#include <set>
//#include <math.h>
//#define SIGNAL_D( signal_f ) ( TYPE_SIGNAL ) ( & signal_f )
//#define HANDLER_D( handler_f ) ( TYPE_HANDLER ) ( & handler_f )
//using namespace std;
//class cl_base;
//typedef void (cl_base ::* TYPE_SIGNAL) (string&);
//typedef void (cl_base ::* TYPE_HANDLER) (string);
//class cl_base{
//private:
//    string s_obj_name;
//    cl_base *p_head_obj;
//    vector <cl_base*> subordinate_objs;
//    int mark = 1;
//    struct connect {
//        TYPE_SIGNAL p_signal;
//        cl_base* base_obj;
//        TYPE_HANDLER p_handler;
//    };
//    vector <connect*> connects;
//public:
//    unsigned int cl_num;
//    cl_base(cl_base *p_head_obj, string s_obj_name = "Base_object");
//    ~cl_base();
//    bool set_object_name(string s_obj_name);
//    string get_object_name();
//    cl_base *get_head_object();
//    void print_subordinate_objects();
//    cl_base *get_subordinate_object(string s_obj_name);
//    cl_base *find_obj_name_curr(string s_obj_name);
//    cl_base *find_obj_by_name(string s_obj_name);
//    void print_hierarchy();
//    void print_hierarchy_with_mark_of_read();
//    void set_mark_of_read(int mark);
//    bool redefin_head(cl_base *p_new_head_obj);
//    void delete_sub(string s_obj_name);
//    cl_base *get_object_pointer_by_coord(string coord);
//    void establish_conn(TYPE_SIGNAL p_signal, cl_base *base_obj, TYPE_HANDLER
//    p_handler);
//    void delete_conn(TYPE_SIGNAL p_signal, cl_base* base_obj, TYPE_HANDLER
//    p_handler);
//    void emit_sign(TYPE_SIGNAL p_signal, string s_comm);
//    string get_abs_obj_coord();
//    bool subordinate_objs_empty();
//};
//#endif

#ifndef __CL_BASE_H__
#define __CL_BASE_H__
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <map>
#define SIGNAL_D(signal_f) (TYPE_SIGNAL)(&signal_f)
#define HANDLER_D(handler_f) (TYPE_HANDLER)(&handler_f)
using namespace std;
class cl_base;

typedef void (cl_base::* TYPE_SIGNAL) (string& msg);
typedef void (cl_base::* TYPE_HANDLER) (string msg);

struct o_sh
{
    TYPE_SIGNAL p_signal;
    TYPE_HANDLER p_handler;
    cl_base* p_target;
};

class cl_base
{
private:
    string s_name;
    cl_base* p_head_object;
    vector <cl_base*> p_sub_objects;
    int p_ready = 1;
    struct o_sh
    {
        TYPE_SIGNAL p_signal;
        TYPE_HANDLER p_handler;
        cl_base* p_target;
    };
    vector<o_sh*> connects;
    int mark = 1;
public:
    unsigned int cl_num;
    cl_base(cl_base* p_head_object, string s_name = "Base Object");

    string get_name();
    cl_base* get_head();
    void print_tree(string delay = "");
    cl_base* get_sub_obj(string s_name);
    ~cl_base();
    cl_base* search_by_name(string name);
    bool change_head_obj(cl_base* new_head_obj);
    void delete_subordinate_obj(string name);
    cl_base* find_obj_by_coord(string s_object_path);
    void set_connection(TYPE_SIGNAL p_signal, cl_base* p_target, TYPE_HANDLER p_handler);
    void emit_signal(TYPE_SIGNAL p_signal, string massege);
    typedef void (cl_base::* TYPE_SIGNAL)(string&);
    typedef void (cl_base::* TYPE_HANDLER)(string);
    void delete_links(cl_base* targ);
    cl_base* get_root();

    cl_base *find_obj_name_curr(string s_obj_name);


    bool subordinate_objs_empty();

    void get_ready(string name);
};
#endif