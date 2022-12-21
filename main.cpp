#include <iostream>
#include <string>
#include "include/FSMAction.cpp"
#include "include/FSMState.cpp"
#include "include/FSM.cpp"

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

    std::vector<char> data;
    std::string input_message ("aaaabbbbbcccc");
    std::copy(input_message.begin(), input_message.end(), std::back_inserter(data));

    fsm.run(data);
    return 0;
}