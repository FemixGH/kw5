#include "control_panel.h"
control_panel::control_panel(cl_base *p_head_obj, string s_obj_name) :
        cl_base(p_head_obj, s_obj_name){ cl_num = 4; }
void control_panel::signal(string &mess){ }
bool control_panel::take_passengers(TYPE_SIGNAL p_signal){
    vector <string> from_floor_to_lift = {};
    for (auto passenger = all_floor_passengers[curr_floor - 1].begin();
         passenger != all_floor_passengers[curr_floor - 1].end(); passenger++){
        if ((lift_capacity - curr_pass_quant > 0) && ((direction == "none" &&
                                                       curr_pass_quant == 0) || direction == passenger->second[2])){ //
            direction = passenger->second[2];
            this->emit_signal(p_signal, "change_head " + passenger->first.substr(passenger->first.find(' ') + 1));
            from_floor_to_lift.push_back(passenger->first);
            lift_passengers.push_back({passenger->first.substr(passenger->first.find(' ') + 1), passenger->second[1]});
            curr_pass_quant++;
        }
    }
    if(from_floor_to_lift.size() == 0) return false;
    direction = "none";
    this->emit_signal(p_signal, "direction=none");
    this->emit_signal(p_signal, "change_num_of_lift_pass " +
                              to_string(curr_pass_quant));
//удаление пассажиров из общего массива и заполнение приоритетной очереди
    for (auto pass:from_floor_to_lift){
        priority_queue.insert(stoi(all_floor_passengers[curr_floor - 1][pass]
                                   [1]));
        all_floor_passengers[curr_floor - 1].erase(pass);
    }
//удаление(или нет) текущего этажа из очереди
    if (all_floor_passengers[curr_floor - 1].size() == 0){
        for(auto num = queue.begin(); num != queue.end();){
            if((*num) == curr_floor){
                queue.erase(num);
            }
            else num++;
        }
    }
    return true;
}
void control_panel::handler(string &mess){
    if (mess == "next_tact"){
        TYPE_SIGNAL p_signal = SIGNAL_D(control_panel::signal);
        bool none_pass = true;
        for (auto m:all_floor_passengers){
            if(m.size() != 0){
                none_pass = false;
                break;
            }
        }
        if (none_pass && (curr_pass_quant == 0)){
            direction = "none";
            this->emit_signal(p_signal, "direction=none");
            return;
        }
        if(curr_pass_quant == -1){ //если в предыдущем такте при заходе наэтаже остались пассажиры
            curr_pass_quant = lift_passengers.size();
            if(curr_pass_quant != 0){
                if ((*priority_queue.begin()) < curr_floor){ direction =
                                                                     "down"; }
                else { direction = "up"; }
            }
            if(take_passengers(p_signal)) { return; }
        }
        if (curr_pass_quant == 0){ //в лифте нет людей
            if (queue[0] < curr_floor){
                curr_floor -= 1;
                this->emit_signal(p_signal, "curr_floor - 1");
                if (direction != "down"){
                    direction = "down";
                    this->emit_signal(p_signal, "direction=down");
                }
            }
            else if (queue[0] > curr_floor){
                curr_floor += 1;
                this->emit_signal(p_signal, "curr_floor + 1");
                if (direction != "up"){
                    direction = "up";
                    this->emit_signal(p_signal, "direction=up");
                }
            }
            else{
                direction = "none";
                this->emit_signal(p_signal, "direction=none");
                take_passengers(p_signal);
            }
        }
        else{ //в лифте есть люди
//двигаться к близжайшему этажу с проверкой, нет ли людей в эту же сторону на текущем этаже
            int difference = floor_quant + 1;
            if (direction == "none"){
                for (set<int>::iterator floor = priority_queue.begin();
                     floor != priority_queue.end (); floor++){
                    if (abs((*floor) - curr_floor) < difference){
                        difference = abs((*floor) - curr_floor);
                        nearest_floor = (*floor);
                    }
                }
            }
//проверка что на текущем этаже никого нет (или есть)
            if ((all_floor_passengers[curr_floor - 1].size() != 0) &&
                curr_floor != nearest_floor){
                if(take_passengers(p_signal)) return;
            }
            if (nearest_floor < curr_floor){
                curr_floor -= 1;
                this->emit_signal(p_signal, "curr_floor - 1");
                if (direction != "down"){
                    direction = "down";
                    this->emit_signal(p_signal, "direction=down");
                }
            }
            else if (nearest_floor > curr_floor){
                curr_floor += 1;
                this->emit_signal(p_signal, "curr_floor + 1");
                if (direction != "up"){
                    direction = "up";
                    this->emit_signal(p_signal, "direction=up");
                }
            }
            else{
                direction = "none";
                this->emit_signal(p_signal, "direction=none");
//люди, для которых этот этаж был целевым выходят
                for(auto pass = lift_passengers.begin(); pass !=
                                                         lift_passengers.end();){
                    if(stoi((*pass)[1]) == curr_floor){
                        this->emit_signal(p_signal, "delete_pass " +
                                                  (*pass)[0]);
                        lift_passengers.erase(pass);
                        curr_pass_quant--;
                    }
                    else pass++;
                }
                this->emit_signal(p_signal, "change_num_of_lift_pass " +
                                          to_string(curr_pass_quant));
                priority_queue.erase(priority_queue.find(curr_floor));
//если на этаже остались люди
                if (all_floor_passengers[curr_floor - 1].size() != 0)
                { curr_pass_quant = -1; }
            }
        }
    }
    else if(mess.substr(0, 13) == "add_passenger"){
        mess = mess.substr(14);
        string name;
        vector <string> options;
        name = mess.substr(0, mess.find(' ') + 5);
        mess = mess.substr(mess.find(' ') + 6);
        for (int i = 0; i < 2; i++){
            options.push_back(mess.substr(0, mess.find(' ')));
            mess = mess.substr(mess.find(' ') + 1);
        }
        options.push_back(mess);
        name = to_string(pass_were_on_floors[stoi(options[0]) - 1]) + " " +
               name;
        pass_were_on_floors[stoi(options[0]) - 1] += 1;
        all_floor_passengers[stoi(options[0]) - 1][name] = options;
        if (queue.size() == 0 || queue.back() != stoi(options[0])){
            queue.push_back(stoi(options[0]));
        }
    }
}
void control_panel::set_lift_capacity(int m){
    this -> lift_capacity = m;
}
void control_panel::set_floor_quant(int n){
    this -> floor_quant = n;
    for (int i = 0; i < n; i++){
        all_floor_passengers.push_back({});
        pass_were_on_floors.push_back(0);
    }
}
//вывод множества
/*
cout << endl;
for (set<int>::iterator i = priority_queue.begin(); i !=
priority_queue.end (); i++){
cout << " " << (*i);
}*/
//вывод вектора
/* cout << endl;
for (int i = 0; i < queue.size(); i++){
cout << queue[i] << " ";
}*/