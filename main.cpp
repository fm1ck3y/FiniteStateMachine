#include <iostream>
#include <string>
#include "FSMAction.cpp"
#include "FSMState.cpp"
#include "FSM.cpp"


void testConcatAndIntersaction(){
    std::vector<char> events = {'0','1'};
    std::vector<FSMAction> actions {};

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


void realNumbers(){
    std::vector<char> events = {'0','1','2','3','4','5','6','7','8','9','+','-','E','.'};
    std::vector<FSMAction> actions {
        FSMAction("Continue.."),
        FSMAction("Bad state.."),
        FSMAction("Good state.."),
        FSMAction("Continue.."),
        FSMAction("Continue.."),
        FSMAction("Continue.."),
        FSMAction("Good state.."),
        FSMAction("Continue.."),
        FSMAction("Continue.."),
        FSMAction("Good state.."),
    };
    std::vector<int> end_states {1,2,6,9};
    std::vector<std::vector<int>> array {
        {4,2,2,2,2,2,2,2,2,2,1,3,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {2,2,2,2,2,2,2,2,2,2,1,1,1,5},
        {4,2,2,2,2,2,2,2,2,2,1,1,1,5},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,5},
        {6,6,6,6,6,6,6,6,6,6,1,1,1,1},
        {6,6,6,6,6,6,6,6,6,6,1,1,7,1},
        {1,1,1,1,1,1,1,1,1,1,8,8,1,1},
        {1,9,9,9,9,9,9,9,9,9,1,1,1,1},
        {9,9,9,9,9,9,9,9,9,9,1,1,1,1},
    };
    FSM<char> fsm_real_numbers (events, array, end_states, actions);
    fsm_real_numbers.print();
    fsm_real_numbers.run("0.123E+1");
}

int main(){
    std::vector<char> events {'a','b','c'};
    std::vector<FSMAction> actions {
        FSMAction("Init state"),
        FSMAction("This state for 'a' symbol"),
        FSMAction("This state for 'b' symbol"),
        FSMAction("Final state for 'c'")
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

    //testConcatAndIntersaction();
    realNumbers();
    return 0;
}