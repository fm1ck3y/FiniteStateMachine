#include <iostream>
#include <string>
#include <memory>
#include "../include/FSMAction.cpp"
#include "../include/FSMState.cpp"
#include "../include/FSM.cpp"

int main(){
    std::vector<char> events = {'0','1','2','3','4','5','6','7','8','9','+','-','E','.'};

    int negative = 1, negative_rang = 1;
    double num = 0, rang = 10;
    auto lambda_main_rang = [&num](char event) { num = num*10 + ((int)event-48); };
    auto lambda_negative = [&negative](char event) { negative = -1; };
    auto lambda_after_dot = [&num,&rang](char event) { num += ((int)event-48)/rang; rang *= 10; };

    std::vector<FSMAction<char>> actions {
        FSMAction<char>("Continue.."),
        FSMAction<char>("Bad state.."),
        FSMAction<char>("Good state..", lambda_main_rang),
        FSMAction<char>("Continue..", lambda_negative),
        FSMAction<char>("Continue..", lambda_main_rang),
        FSMAction<char>("Continue.."),
        FSMAction<char>("Good state..", lambda_after_dot),
        FSMAction<char>("Continue.."),
        FSMAction<char>("Continue.."),
        FSMAction<char>("Good state.."),
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

    std::string input_message;
    std::cout << "Enter number: ";
    std::cin >> input_message;
    std::cout << std::endl;

    std::vector<char> data;
    std::copy(input_message.begin(), input_message.end(), std::back_inserter(data));

    fsm_real_numbers.run(data);
    std::cout << negative * num << std::endl;
}
