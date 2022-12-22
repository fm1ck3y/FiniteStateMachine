#include <iostream>
#include <string>
#include "../include/FSMAction.cpp"
#include "../include/FSMState.cpp"
#include "../include/FSM.cpp"

int main(){
    std::vector<char> events = {'0','1'};
    std::vector<FSMAction<char>> actions {};

    std::vector<int> end_states_1 {0,3};
    std::vector<std::vector<int>> array_1 {
        {2,3},
        {0,1},
        {2,2},
        {1,0},
    };

    FSM<char> fsm_1 (events, array_1, end_states_1, actions);
    fsm_1.print();

    std::vector<int> end_states_2 {1,3};
    std::vector<std::vector<int>> array_2 {
        {2,2},
        {3,1},
        {1,2},
        {0,2},
    };

    FSM<char> fsm_2 (events, array_2, end_states_2, actions);
    fsm_2.print();


    std::cout << std::endl << "Concat: " << std::endl; 
    auto fsm_concat = FSM<char>::concat(fsm_1, fsm_2);
    fsm_concat->print();

    std::cout << std::endl << "Intersaction: " << std::endl; 
    auto fsm_intersaction = FSM<char>::intersaction(fsm_1, fsm_2);
    fsm_intersaction->print();
}
