#include <iostream>
#include <string>
#include "FSMAction.cpp"
#include "FSMState.cpp"
#include "FSM.cpp"

int main(){
    std::vector<char> events = {'0','1','2','3','4','5','6','7','8','9','+','-','E','.'};
    //std::unique_ptr<int> num {0}, rang {1};
    //auto lambda_q2 = [num,rang](char event) { num += num*rang; rang*=10; }

    std::vector<FSMAction> actions {
        FSMAction("Continue.."),
        FSMAction("Bad state.."),
        FSMAction("Good state..", lambda_q2),
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
