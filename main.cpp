#include <iostream>
#include <string>
#include "FSMAction.cpp"
#include "FSMState.cpp"
#include "FSM.cpp"


void testConcatAndIntersaction(){
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

    FSM<char>::concat(&fsm_1, &fsm_2);
    FSM<char>::intersaction(&fsm_1, &fsm_2);
}

int main(){
    std::vector<char> events {'a','b','c'};
    std::vector<FSMAction<char>> actions {
        FSMAction<char>("Init state"),
        FSMAction<char>("This state for 'a' symbol"),
        FSMAction<char>("This state for 'b' symbol"),
        FSMAction<char>("Final state for 'c'")
    };
    std::vector<int> end_states {3};

    std::vector<std::vector<int>> array {
        {1,0,0},
        {1,2,0},
        {1,2,3},
        {1,2,3},
    };

    FSM<char> fsm (events, array, end_states, actions);
    fsm.print();
    fsm.run("aaaabbbbbcccc");

    testConcatAndIntersaction();
    return 0;
}