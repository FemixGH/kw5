#include "systemClass.h"
#include "inputClass.h"
#include "controlPanelClass.h"
#include "liftClass.h"
#include "outputClass.h"
#include "floorClass.h"
#include "passengerClass.h"
#include "base.h"
systemClass::systemClass(base *p_head_obj, string s_obj_name) :
        base(p_head_obj, s_obj_name){ cl_num = 1; }
void systemClass::signal(string &mess){ }
void systemClass::handler(string &mess){
    this->input_data = mess;
    if (mess.substr(0, 11) == "change_head"){
        mess = mess.substr(12);
        this->emit_sign(get_signal_pointer(1), " " + mess);
        find_obj_by_name(mess)->redefin_head(get_object_pointer_by_coord("/control_panel_obj/lift_obj"));
    }
}
TYPE_SIGNAL systemClass::get_signal_pointer(int cl_num){
    switch(cl_num){
        case 1:
            return SIGNAL_D(systemClass::signal);
        case 2:
            return SIGNAL_D(inputClass::signal);
        case 3:
            return SIGNAL_D(floorClass::signal);
        case 4:
            return SIGNAL_D(controlPanelClass::signal);
        case 5:
            return SIGNAL_D(liftClass::signal);
        case 6:
            return SIGNAL_D(passengerClass::signal);
        case 7:
            return SIGNAL_D(outputClass::signal);
    }
    return nullptr;
}
TYPE_HANDLER systemClass::get_handler_pointer(int cl_num){
    switch(cl_num){
        case 1:
            return HANDLER_D(systemClass::handler);
        case 2:
            return HANDLER_D(inputClass::handler);
        case 3:
            return HANDLER_D(floorClass::handler);
        case 4:
            return HANDLER_D(controlPanelClass::handler);
        case 5:
            return HANDLER_D(liftClass::handler);
        case 6:
            return HANDLER_D(passengerClass::handler);
        case 7:
            return HANDLER_D(outputClass::handler);
    }
    return nullptr;
}
void systemClass::set_conn(base *signal_obj, base *handler_obj){
    TYPE_SIGNAL p_signal = get_signal_pointer(signal_obj->cl_num);
    TYPE_HANDLER p_handler = get_handler_pointer(handler_obj->cl_num);
    signal_obj->establish_conn(p_signal, handler_obj, p_handler);
}
void systemClass::set_lift_capacity(int m){ this->lift_capacity = m; }
void systemClass::set_floor_quant(int n){ this->floors_quant = n; }
base *systemClass::passenger_pointer(string name){
    base *passenger_obj;
    for (auto floor_obj:floors){
        passenger_obj = get_object_pointer_by_coord("/floor_" +
                                                    to_string(floor_obj->get_floor_num()) + "/" + name);
        if(passenger_obj){
            this->emit_sign(get_signal_pointer(1), "Passenger condition: " +
                                                   name + " on the floor " + to_string(floor_obj->get_floor_num()));
            return passenger_obj;
        }
    }
    return nullptr;
}
void systemClass::build_tree(){
    int n, m; //этажность здания(n) вместимость лифта (m)
    TYPE_SIGNAL p_signal = get_signal_pointer(1);
    new inputClass(this); //объект ввода
    controlPanelClass *control_panel_class = new controlPanelClass(this);
//объект пульта управления
    new outputClass(this); //объект вывода
//установка связи между объектом
//системы моделирования и объектом чтения
    set_conn(this, get_object_pointer_by_coord("/input_obj"));
    set_conn(get_object_pointer_by_coord("/input_obj"), this);
//установка связи между объектом
//системы моделирования и объектом вывода
    set_conn(this, get_object_pointer_by_coord("/output_obj"));
    this->emit_sign(p_signal, "read");
    while (this->input_data != "End of settings"){
        n = stoi(input_data.substr(0, input_data.find(' ')));
        m = stoi(input_data.substr(input_data.find(' ')));
        this->set_lift_capacity(m);
        this->set_floor_quant(n);
        control_panel_class->set_lift_capacity(m);
        control_panel_class->set_floor_quant(n);
        this->emit_sign(p_signal, "read");
    }
    for (int i = 1; i <= n; i++){
        floorClass* floor_obj = new floorClass(this, "floor_" + to_string(i));
        floor_obj->set_floor_num(i);
        floors.push_back(floor_obj);
    }
    this->emit_sign(p_signal, "Ready to work");
    set_conn(this, get_object_pointer_by_coord("/control_panel_obj"));
    set_conn(get_object_pointer_by_coord("/control_panel_obj"), this);
    for (int i = 1; i <= n; i++){
        set_conn(this, get_object_pointer_by_coord("/floor_" + to_string(i)));
    }
//установка каких-то связей???
}
int systemClass::start_app(){
    TYPE_SIGNAL p_signal = get_signal_pointer(1);
    liftClass *lift_obj = new
            liftClass(get_object_pointer_by_coord("/control_panel_obj")); //объект лифт
    string message;
    set_conn(get_object_pointer_by_coord("/control_panel_obj"),
             get_object_pointer_by_coord("/control_panel_obj/lift_obj"));
    lift_obj->set_capacity(this->lift_capacity);
    while (true){
        tact++;
        this->emit_sign(p_signal, "read");
        if (input_data == "Turn off the system") { this->emit_sign(p_signal,
                                                                   input_data); break; }
        else if(input_data == "System status"){
            message = to_string(tact) + ": Elevator: " + to_string(lift_obj->get_curr_floor()) + " " + to_string(lift_obj->get_curr_num_of_pass()) + "Floors:" ;
            for (auto floor_obj:floors){
                if (!floor_obj->subordinate_objs_empty()){
                    message = message + " (" + to_string(floor_obj->get_floor_num()) + ": " + to_string(floor_obj->num_up()) + "-" +
                              to_string(floor_obj->num_of_pass() - floor_obj->num_up()) + ")";
                }
            }
            this->emit_sign(p_signal, message);
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
            this->emit_sign(p_signal, message);
//вывод состояния вверх вниз или стоит
        }
        else if(input_data.substr(0, 19) == "Passenger condition"){
            string name = input_data.substr(20);
            if(get_object_pointer_by_coord("/control_panel_obj/lift_obj/" +
                                           name)){
                message = "Passenger condition: " + name + " in the elevator";
                this->emit_sign(p_signal, message);
            }
            else if (!passenger_pointer(name)){
                message = "Passenger " + name + " not found";
                this->emit_sign(p_signal, message);
            }
//ищем в векторе в лифте и в векторах по этажам
        }
        else if(input_data.substr(0, 22) == "Condition on the floor"){
            floorClass *floor = floors[stoi(input_data.substr(23)) - 1];
            message = input_data + " -up-";
            for (auto passenger:floor->passengers_up_down("up")){
                message = message + " " + passenger->get_object_name();
            }
            this->emit_sign(p_signal, message);
            message = input_data + " -down-";
            for (auto passenger:floor->passengers_up_down("down")){
                message = message + " " + passenger->get_object_name();
            }
            this->emit_sign(p_signal, message);
//этаж по номеру
        }
        else if(input_data.substr(0, 9) == "Passenger"){
            input_data = input_data.substr(10);
            passengerClass *passenger_obj = nullptr;
            for (int i = input_data.size(); i > 0 ; i--){
                if((input_data[i] == ' ') && (i != input_data.size() - 5))
                {
                    passenger_obj = new
                            passengerClass(get_object_pointer_by_coord("/floor_" + input_data.substr(0,
                                                                                                     input_data.find(' '))), input_data.substr(i + 1));
                    input_data = input_data.substr(0, i);
                    break;
                }
            }
            floors[stoi(input_data.substr(0, input_data.find(' '))) - 1]->add_passenger(passenger_obj);
            passenger_obj->set_options\
(stoi(input_data.substr(0, input_data.find(' '))), \
stoi(input_data.substr(input_data.find(' ') + 1)));
            this->emit_sign(p_signal, "add_passenger " + passenger_obj->get_object_name() + " " + input_data.substr(0, input_data.find(' ')) + " " +input_data.substr(input_data.find(' ') + 1) + " " + passenger_obj->get_direction());
//подход пассажира на этаж
        }
        else if (input_data == "SHOWTREE"){
            print_hierarchy_with_mark_of_read();
        }
//совершение действия соответственно такту
        this->emit_sign(p_signal, "next_tact");
    }
    return 0;
}