#include "system.h"
#include "input.h"
#include "control_panel.h"
#include "lift.h"
#include "outputs.h"
#include "floor.h"
#include "passenger.h"
#include "cl_base.h"
system::system(cl_base *p_head_obj, string s_obj_name) :
        cl_base(p_head_obj, s_obj_name){ cl_num = 1; }
void system::signal(string &mess){ }
void system::handler(string &mess){
    this->input_data = mess;
    if (mess.substr(0, 11) == "change_head"){
        mess = mess.substr(12);
        this->emit_signal(get_signal_pointer(1), " " + mess);
        search_by_name(mess)->change_head_obj(find_obj_by_coord("/control_panel_obj/lift_obj"));
    }
}
TYPE_SIGNAL system::get_signal_pointer(int cl_num){
    switch(cl_num){
        case 1:
            return SIGNAL_D(system::signal);
        case 2:
            return SIGNAL_D(input::signal);
        case 3:
            return SIGNAL_D(floor::signal);
        case 4:
            return SIGNAL_D(control_panel::signal);
        case 5:
            return SIGNAL_D(lift::signal);
        case 6:
            return SIGNAL_D(passenger::signal);
        case 7:
            return SIGNAL_D(outputs::signal);
    }
    return nullptr;
}
TYPE_HANDLER system::get_handler_pointer(int cl_num){
    switch(cl_num){
        case 1:
            return HANDLER_D(system::handler);
        case 2:
            return HANDLER_D(input::handler);
        case 3:
            return HANDLER_D(floor::handler);
        case 4:
            return HANDLER_D(control_panel::handler);
        case 5:
            return HANDLER_D(lift::handler);
        case 6:
            return HANDLER_D(passenger::handler);
        case 7:
            return HANDLER_D(outputs::handler);
    }
    return nullptr;
}
void system::set_conn(cl_base *signal_obj, cl_base *handler_obj){
    TYPE_SIGNAL p_signal = get_signal_pointer(signal_obj->cl_num);
    TYPE_HANDLER p_handler = get_handler_pointer(handler_obj->cl_num);
    signal_obj->set_connection(p_signal, handler_obj, p_handler);
}
void system::set_lift_capacity(int m){ this->lift_capacity = m; }
void system::set_floor_q(int n){ this->floors_q = n; }
cl_base *system::passenger_pointer(string name){
    cl_base *passenger_obj;
    for (auto floor_obj:floors){
        passenger_obj = find_obj_by_coord("/floor_" +
                                                    to_string(floor_obj->get_floor_num()) + "/" + name);
        if(passenger_obj){
            this->emit_signal(get_signal_pointer(1), "Passenger condition: " +
                                                   name + " on the floor " + to_string(floor_obj->get_floor_num()));
            return passenger_obj;
        }
    }
    return nullptr;
}
void system::build_tree(){
    int n, m; //этажность здания(n) вместимость лифта (m)
    TYPE_SIGNAL p_signal = get_signal_pointer(1);
    new input(this); //объект ввода
    control_panel *control_panel_class = new control_panel(this);
//объект пульта управления
    new outputs(this); //объект вывода
//установка связи между объектом
//системы моделирования и объектом чтения
    set_conn(this, find_obj_by_coord("/input_obj"));
    set_conn(find_obj_by_coord("/input_obj"), this);
//установка связи между объектом
//системы моделирования и объектом вывода
    set_conn(this, find_obj_by_coord("/output_obj"));
    this->emit_signal(p_signal, "read");
    while (this->input_data != "End of settings"){
        n = stoi(input_data.substr(0, input_data.find(' ')));
        m = stoi(input_data.substr(input_data.find(' ')));
        this->set_lift_capacity(m);
        this->set_floor_q(n);
        control_panel_class->set_lift_capacity(m);
        control_panel_class->set_floor_quant(n);
        this->emit_signal(p_signal, "read");
    }
    for (int i = 1; i <= n; i++){
        floor* floor_obj = new floor(this, "floor_" + to_string(i));
        floor_obj->set_floor_num(i);
        floors.push_back(floor_obj);
    }
    this->emit_signal(p_signal, "Ready to work");
    set_conn(this, find_obj_by_coord("/control_panel_obj"));
    set_conn(find_obj_by_coord("/control_panel_obj"), this);
    for (int i = 1; i <= n; i++){
        set_conn(this, find_obj_by_coord("/floor_" + to_string(i)));
    }
//установка каких-то связей???
}
int system::start_app(){
    TYPE_SIGNAL p_signal = get_signal_pointer(1);
    lift *lift_obj = new
            lift(find_obj_by_coord("/control_panel_obj")); //объект лифт
    string message;
    set_conn(find_obj_by_coord("/control_panel_obj"),
             find_obj_by_coord("/control_panel_obj/lift_obj"));
    lift_obj->set_capacity(this->lift_capacity);
    while (true){
        tact++;
        this->emit_signal(p_signal, "read");
        if (input_data == "Turn off the system") { this->emit_signal(p_signal,
                                                                   input_data); break; }
        else if(input_data == "System status"){
            message = to_string(tact) + ": Elevator: " + to_string(lift_obj->get_curr_floor()) + " " + to_string(lift_obj->get_curr_num_of_pass()) + " Floors:" ;
            for (auto floor_obj:floors){
                if (!floor_obj->subordinate_objs_empty()){
                    message = message + " (" + to_string(floor_obj->get_floor_num()) + ": " + to_string(floor_obj->num_up()) + "-" +
                              to_string(floor_obj->num_of_pass() - floor_obj->num_up()) + ")";
                }
            }
            this->emit_signal(p_signal, message);
//цикл с выписываеним состояния на этажах
        }
        else if(input_data == "Elevator condition"){
            message = "Elevator condition: " + to_string(lift_obj->get_curr_floor()) + " " + to_string(lift_obj->get_curr_num_of_pass()) + " ";
            if(lift_obj->get_direction() == "none"){
                message += "stop";
            }
            else{
                message = message + "direction " + lift_obj->get_direction();
            }
            this->emit_signal(p_signal, message);
//вывод состояния вверх вниз или стоит
        }
        else if(input_data.substr(0, 19) == "Passenger condition"){
            string name = input_data.substr(20);
            if(find_obj_by_coord("/control_panel_obj/lift_obj/" +
                                           name)){
                message = "Passenger condition: " + name + " in the elevator";
                this->emit_signal(p_signal, message);
            }
            else if (!passenger_pointer(name)){
                message = "Passenger " + name + " not found";
                this->emit_signal(p_signal, message);
            }
//ищем в векторе в лифте и в векторах по этажам
        }
        else if(input_data.substr(0, 22) == "Condition on the floor"){
            floor *floor = floors[stoi(input_data.substr(23)) - 1];
            message = input_data + " -up-";
            for (auto passenger:floor->passengers_up_down("up")){
                message = message + " " + passenger->get_name();
            }
            this->emit_signal(p_signal, message);
            message = input_data + " -down-";
            for (auto passenger:floor->passengers_up_down("down")){
                message = message + " " + passenger->get_name();
            }
            this->emit_signal(p_signal, message);
//этаж по номеру
        }
        else if(input_data.substr(0, 9) == "Passenger"){
            input_data = input_data.substr(10);
            passenger *passenger_obj = nullptr;
            for (int i = input_data.size(); i > 0 ; i--){
                if((input_data[i] == ' ') && (i != input_data.size() - 5))
                {
                    passenger_obj = new
                            passenger(find_obj_by_coord("/floor_" + input_data.substr(0,
                                                                                      input_data.find(' '))), input_data.substr(i + 1));
                    input_data = input_data.substr(0, i);
                    break;
                }
            }
            floors[stoi(input_data.substr(0, input_data.find(' '))) - 1]->add_passenger(passenger_obj);
            passenger_obj->set_options\
(stoi(input_data.substr(0, input_data.find(' '))), \
stoi(input_data.substr(input_data.find(' ') + 1)));
            this->emit_signal(p_signal, "add_passenger " + passenger_obj->get_name() + " " + input_data.substr(0, input_data.find(' ')) + " " +input_data.substr(input_data.find(' ') + 1) + " " + passenger_obj->get_direction());
//подход пассажира на этаж
        }
        else if (input_data == "SHOWTREE"){
            get_ready(input_data);
        }
//совершение действия соответственно такту
        this->emit_signal(p_signal, "next_tact");
    }
    return 0;
}