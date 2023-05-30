#include "base.h"
base::base(base *p_head_obj, string s_obj_name){
    this->p_head_obj = p_head_obj;
    this->s_obj_name = s_obj_name;
    if (p_head_obj)
        p_head_obj->subordinate_objs.push_back(this);
}
base::~base(){
    for (int i = 0; i < subordinate_objs.size(); i++){
        delete subordinate_objs[i];
    }
}
bool base::set_object_name(string s_obj_name){
    if (p_head_obj && (get_subordinate_object(s_obj_name) == p_head_obj))
        return false;
    this->s_obj_name = s_obj_name;
    return true;
}
string base::get_object_name(){
    return s_obj_name;
}
base *base::get_head_object(){
    return p_head_obj;
}
void base::print_subordinate_objects(){
    if(subordinate_objs.size() == 0) return;
    cout << "\n" << s_obj_name;
    for (int i = 0; i < subordinate_objs.size(); i++){
        cout << " " << subordinate_objs[i]->get_object_name();
    }
    subordinate_objs[subordinate_objs.size() - 1]->print_subordinate_objects();
}
base *base::get_subordinate_object(string s_obj_name){
    for (int i = 0; i < subordinate_objs.size(); i++){
        if (subordinate_objs[i]->get_object_name() == s_obj_name){
            return subordinate_objs[i];
        }
    }
    return nullptr;
}
base *base::find_obj_name_curr(string s_obj_name){
    if (this->s_obj_name == s_obj_name) return this;
    if (subordinate_objs.empty()) return nullptr;
    base *out_obj = nullptr;
    base *curr;
    for (auto obj: subordinate_objs){
        curr = obj->find_obj_name_curr(s_obj_name);
        if (curr && out_obj) return nullptr;
        if (curr) out_obj = curr;
        else if (obj->get_subordinate_object(s_obj_name)) return nullptr;
    }
    return out_obj;
}
base *base::find_obj_by_name(string s_obj_name){
    if (p_head_obj) return p_head_obj->find_obj_by_name(s_obj_name);
    return find_obj_name_curr(s_obj_name);
}
void base::print_hierarchy(){
    int tab = -1;
    base *h_obj = p_head_obj;
    while (h_obj){
        tab += 1;
        h_obj = h_obj->get_head_object();
    }
    cout << "\n";
    for (int i = 0; i <= tab; i++){
        cout << " ";
    }
    cout << s_obj_name;
    for (auto obj: subordinate_objs){
        obj->print_hierarchy();
    }
}
void base::print_hierarchy_with_mark_of_read(){
    int tab = -1;
    base *h_obj = p_head_obj;
    while (h_obj){
        tab += 1;
        h_obj = h_obj->get_head_object();
    }
    cout << "\n";
    for (int i = 0; i <= tab; i++){
        cout << " ";
    }
    cout << s_obj_name;
    if (mark == 0) cout << " is not ready";
    else cout << " is ready";
    for (auto obj: subordinate_objs){
        obj->print_hierarchy_with_mark_of_read();
    }
}
void base::set_mark_of_read(int mark){
    if (mark == 0){
        this->mark = 0;
        for (auto obj: subordinate_objs){
            obj->set_mark_of_read(0);
        }
    }
    else {
        base *h_obj = p_head_obj;
        while (h_obj) {
            if (h_obj->mark == 0) { this-> mark = 0; return; }
            h_obj = h_obj->get_head_object();
        }
        this->mark = mark;
    }
}
bool base::redefin_head(base *p_new_head_obj){
    if ((p_head_obj == nullptr) || (p_new_head_obj == nullptr) ||\
(p_head_obj == p_new_head_obj)) return false;
    if(p_new_head_obj->get_subordinate_object(s_obj_name)) {
        return false;
    }
    base *curr_obj = p_new_head_obj;
    while (curr_obj){
        curr_obj = curr_obj->get_head_object();
        if (curr_obj == this) return false;
    }
    for (int i = 0; i < p_head_obj->subordinate_objs.size(); i++){
        if (p_head_obj->subordinate_objs[i] == this) {
            p_head_obj->subordinate_objs.erase(p_head_obj-
                                               >subordinate_objs.begin() + i);
            break;
        }
    }
    p_head_obj = p_new_head_obj;
    p_head_obj->subordinate_objs.push_back(this);
    return true;
}
void base::delete_sub(string s_obj_name){
    base *obj = get_subordinate_object(s_obj_name);
    for (int i = 0; i < subordinate_objs.size(); i++){
        if (subordinate_objs[i] == obj) {
            subordinate_objs.erase(subordinate_objs.begin() + i);
            break;
        }
    }
    for (auto sub_obj: obj->subordinate_objs) {
        sub_obj->redefin_head(this);
    }
    delete obj;
}
base *base::get_object_pointer_by_coord(string coord){
    if (coord.size() == 0) return nullptr;
    if (coord[0] == '.'){
        if (coord.size() == 1) return this;
        return find_obj_name_curr(coord.substr(1, coord.size() - 1));
    }
    base *curr_obj = this;
    if (coord[0] == '/'){
        while (curr_obj->get_head_object()){
            curr_obj = curr_obj->get_head_object();
        }
        if (coord.size() == 1){
            return curr_obj;
        }
        if (coord[1] == '/')
            return find_obj_by_name(coord.substr(2, coord.size() - 1));
    }
    string curr_name;
    int curr_coord = 1;
    for (int i = 1; i < coord.size(); i++){
        if (coord[i] == '/'){
            if (coord[0] == '/')
                curr_name = coord.substr(1, i - 1);
            else curr_name = coord.substr(0, i);
            curr_coord = i + 1;
            break;
        }
    }
    if (curr_name == "") {
        return curr_obj->get_subordinate_object(coord.substr(1, coord.size() -
                                                                1));
    }
    while (curr_coord < coord.size()){
        curr_obj = curr_obj->get_subordinate_object(curr_name);
        if (!curr_obj) return nullptr;
        for (int i = curr_coord; i < coord.size(); i++){
            if (coord[i] == '/'){
                curr_name = coord.substr(curr_coord, i - curr_coord);
                curr_coord = i + 1;
                break;
            }
            if (i >= coord.size() - 1){
                curr_name = coord.substr(curr_coord, i - 1);
                curr_coord = i + 1;
            }
        }
    }
    return curr_obj->get_subordinate_object(curr_name);
}
void base::establish_conn(TYPE_SIGNAL p_signal, base* base_obj, TYPE_HANDLER
p_handler) {
    for (int i = 0; i < connects.size(); i++) {
        if ((connects[i]->p_signal == p_signal) \
&& (connects[i]->base_obj == base_obj) \
&& (connects[i]->p_handler == p_handler))
            return;
    }
    connect* conn = new connect;
    conn->p_signal = p_signal;
    conn->base_obj = base_obj;
    conn->p_handler = p_handler;
    connects.push_back(conn);
}
void base::delete_conn(TYPE_SIGNAL p_signal, base* base_obj, TYPE_HANDLER
p_handler){
    for (int i = 0; i < connects.size(); i++) {
        if ((connects[i]->p_signal == p_signal) \
&& (connects[i]->base_obj == base_obj) \
&& (connects[i]->p_handler == p_handler)){
            connects.erase(connects.begin() + i);
        }
    }
}
void base::emit_sign(TYPE_SIGNAL p_signal, string s_comm){
    if (mark == 0) return;
    TYPE_HANDLER p_handler;
    base* p_obj;
    (this->* p_signal)(s_comm);
    for (int i = 0; i < connects.size(); i++) {
        if ((connects[i]->p_signal == p_signal) && (connects[i]->base_obj-
                                                    >mark != 0)){
            p_handler = connects[i]->p_handler;
            p_obj = connects[i]->base_obj;
            (p_obj->* p_handler)(s_comm);
        }
    }
}
string base::get_abs_obj_coord(){
    if (!p_head_obj) return "/";
    string abs_coord = "/" + s_obj_name;
    base *curr = get_head_object();
    while (curr->get_head_object()){
        abs_coord = '/' + curr->get_object_name() + abs_coord;
        curr = curr->get_head_object();
    }
    return abs_coord;
}
bool base::subordinate_objs_empty(){
    return subordinate_objs.empty();
}