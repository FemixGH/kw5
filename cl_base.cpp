#include "cl_base.h"

cl_base::cl_base(cl_base* p_head_object, string s_name)
{
    this->s_name = s_name;
    this->p_head_object = p_head_object;
    if (p_head_object != nullptr)
    {
        p_head_object->p_sub_objects.push_back(this);
    }
}

cl_base::~cl_base()
{
    get_root()->delete_links(this);
    for (int i = 0; i < p_sub_objects.size(); i++)
    {
        delete p_sub_objects[i];
    }
}


void cl_base::print_tree(string delay)
{
    cout << endl << delay << get_name();
    for (auto p_sub : p_sub_objects)
    {
        p_sub->print_tree(delay + "    ");
    }
}


string cl_base::get_name()
{
    return s_name;
}

cl_base* cl_base::get_head()
{
    return p_head_object;
}

cl_base* cl_base::get_sub_obj(string s_name)
{
    for (int i = 0; i < p_sub_objects.size(); i++)
    {
        if (p_sub_objects[i]->s_name == s_name)
        {
            return p_sub_objects[i];
        }
    }
    return nullptr;
}

cl_base* cl_base::search_by_name(string s_obj_name)
{
    if (p_head_object) return p_head_object->search_by_name(s_obj_name);
    return find_obj_name_curr(s_obj_name);
}



bool cl_base::change_head_obj(cl_base* new_head_obj)
{
    if (new_head_obj != nullptr)
    {
        cl_base* temp = new_head_obj;
        while (temp != nullptr)
        {
            temp = temp->p_head_object;
            if (temp == this)
            {
                return false;
            }
        }
        if (new_head_obj->get_sub_obj(get_name()) == nullptr && p_head_object != nullptr)
        {
            p_head_object->p_sub_objects.erase(find(p_head_object->p_sub_objects.begin(), p_head_object->p_sub_objects.end(), this));
            new_head_obj->p_sub_objects.push_back(this);
            p_head_object = new_head_obj;
            return true;
        }
    }
    return false;
}

void cl_base::delete_subordinate_obj(string name)
{
    cl_base* subordinate_obj = get_sub_obj(name);
    if (subordinate_obj != nullptr)
    {
        p_sub_objects.erase(find(p_sub_objects.begin(), p_sub_objects.end(), subordinate_obj));
        delete subordinate_obj;
    }
}

cl_base* cl_base::find_obj_by_coord(string s_object_path)
{
    if (s_object_path.size() == 0) return nullptr;
    if (s_object_path[0] == '.'){
        if (s_object_path.size() == 1) return this;
        return find_obj_name_curr(s_object_path.substr(1, s_object_path.size() - 1));
    }
    cl_base *curr_obj = this;
    if (s_object_path[0] == '/'){
        while (curr_obj->get_head()){
            curr_obj = curr_obj->get_head();
        }
        if (s_object_path.size() == 1){
            return curr_obj;
        }
        if (s_object_path[1] == '/')
            return search_by_name(s_object_path.substr(2, s_object_path.size() - 1));
    }
    string curr_name;
    int curr_coord = 1;
    for (int i = 1; i < s_object_path.size(); i++){
        if (s_object_path[i] == '/'){
            if (s_object_path[0] == '/')
                curr_name = s_object_path.substr(1, i - 1);
            else curr_name = s_object_path.substr(0, i);
            curr_coord = i + 1;
            break;
        }
    }
    if (curr_name == "") {
        return curr_obj->get_sub_obj(s_object_path.substr(1, s_object_path.size() -
                                                                1));
    }
    while (curr_coord < s_object_path.size()){
        curr_obj = curr_obj->get_sub_obj(curr_name);
        if (!curr_obj) return nullptr;
        for (int i = curr_coord; i < s_object_path.size(); i++){
            if (s_object_path[i] == '/'){
                curr_name = s_object_path.substr(curr_coord, i - curr_coord);
                curr_coord = i + 1;
                break;
            }
            if (i >= s_object_path.size() - 1){
                curr_name = s_object_path.substr(curr_coord, i - 1);
                curr_coord = i + 1;
            }
        }
    }
    return curr_obj->get_sub_obj(curr_name);
}


void cl_base::set_connection(TYPE_SIGNAL p_signal, cl_base* p_target, TYPE_HANDLER p_handler)
{
    o_sh* p_value;
    for (int i = 0; i < connects.size(); i++)
    {
        if (connects[i]->p_signal == p_signal &&
            connects[i]->p_handler == p_handler &&
            connects[i]->p_target == p_target)
        {
            return;
        }
    }
    p_value = new o_sh();
    p_value->p_signal = p_signal;
    p_value->p_handler = p_handler;
    p_value->p_target = p_target;

    connects.push_back(p_value);
}


void cl_base::emit_signal(TYPE_SIGNAL p_signal, string s_massege)
{
    if (p_ready != 0)
    {
        TYPE_HANDLER pHandler;
        cl_base* pObject;
        (this->*p_signal)(s_massege);
        for (int i = 0; i < connects.size(); i++)
        {
            if (connects[i]->p_signal == p_signal)
            {
                pHandler = connects[i]->p_handler;
                pObject = connects[i]->p_target;
                if (pObject->p_ready != 0)
                {
                    (pObject->*pHandler)(s_massege);
                }
            }
        }
    }
}


void cl_base::delete_links(cl_base* targ)
{
    for (auto p_it = connects.begin(); p_it != connects.end(); p_it++)
    {
        if ((*p_it)->p_target == targ)
        {
            delete (*p_it);
            connects.erase(p_it);
            p_it--;
        }
    }

    for (auto p_sub : p_sub_objects)
    {
        p_sub->delete_links(targ);
    }
}

cl_base* cl_base::get_root()
{
    if (p_head_object != nullptr)
    {
        p_head_object->get_root();
    }
    return this;
}


cl_base *cl_base::find_obj_name_curr(string s_obj_name){
    if (this->s_name == s_obj_name) return this;
    if (p_sub_objects.empty()) return nullptr;
    cl_base *out_obj = nullptr;
    cl_base *curr;
    for (auto obj: p_sub_objects){
        curr = obj->find_obj_name_curr(s_obj_name);
        if (curr && out_obj) return nullptr;
        if (curr) out_obj = curr;
        else if (obj->get_sub_obj(s_obj_name)) return nullptr;
    }
    return out_obj;
}

void cl_base::print_hierarchy_with_mark_of_read(){
    int tab = -1;
    cl_base *h_obj = p_head_object;
    while (h_obj){
        tab += 1;
        h_obj = h_obj->get_head();
    }
    cout << "\n";
    for (int i = 0; i <= tab; i++){
        cout << " ";
    }
    cout << s_name;
    if (mark == 0) cout << " is not ready";
    else cout << " is ready";
    for (auto obj: p_sub_objects){
        obj->print_hierarchy_with_mark_of_read();
    }
}

bool cl_base::subordinate_objs_empty(){
    return p_sub_objects.empty();
}