#include <iostream>
#include <string>
#include "FSMAction.cpp"
#include "FSMState.cpp"
#include "FSM.cpp"

/*
void testconcat(){
    std::vector<char> events = {'0','1'};

    FSMState<char> q_0(true), q_1(false), q_2(false), q_3(true);
    q_0.addTransition('0',q_2);
    q_0.addTransition('1',q_3);
    q_1.addTransition('0',q_0);
    q_1.addTransition('1',q_1);
    q_2.addTransition('0',q_2);
    q_2.addTransition('1',q_2);
    q_3.addTransition('0',q_1);
    q_3.addTransition('1',q_0);
    FSM<char> fsm_1(q_0);
    fsm_1.addState(q_0);
    fsm_1.addState(q_1);
    fsm_1.addState(q_2);
    fsm_1.addState(q_3);
    fsm_1.addEvents(events);
    fsm_1.print();

    FSMState<char> q_5 (false), q_6 (true), q_7 (false), q_8 (true);
    q_5.addTransition('0',q_7);
    q_5.addTransition('1',q_8);
    q_6.addTransition('0',q_5);
    q_6.addTransition('1',q_6);
    q_7.addTransition('0',q_7);
    q_7.addTransition('1',q_7);
    q_8.addTransition('0',q_6);
    q_8.addTransition('1',q_5);
    FSM<char> fsm_2(q_5);
    fsm_2.addState(q_5);
    fsm_2.addState(q_6);
    fsm_2.addState(q_7);
    fsm_2.addState(q_8);
    fsm_2.addEvent('0');
    fsm_2.addEvent('1');
    fsm_2.print();

    FSM<char> concat_fsm;
    //FSM<char>::concat(&fsm_1, &fsm_2, &concat_fsm);
    //FSM<char>::intersaction(&fsm_1, &fsm_2, &concat_fsm);
    // OUTPUT WITH INCORRECT TRANSITIONS IN LAST 5
    // WHY ?
    // concat_fsm.print();
}


void RealNumbers(){
    FSMState<char> q0(),q1(),q2(),q3(),q4(),q5(),q6(),q7(),q8(),q9();
    std::vector<char> events = {'0','1','2','3','4','5','6','7','8','9','+','-','E','.'};

    q0.addTransition('0',q4);
    q0.addTransition('-',q3);
    q0.addTransition(std::vector<char>{'E',',','+'},q1);
    q0.addTransition(std::vector<char>{'1','2','3','4','5','6','7','8','9'},q2);

    q1.addTransition(events,q1);

    q2.addTransition('.',q5);
    q2.addTransition(std::vector<char>{'0','1','2','3','4','5','6','7','8','9'},q2);

}*/

int main(){
    FSM<char> fsm;

    FSMState<char> *init_state = fsm.addState(FSMAction("Init state"));
    FSMState<char> *first_state = fsm.addState(FSMAction("This state for 'a' symbol"));
    FSMState<char> *second_state = fsm.addState(FSMAction("This state for 'b' symbol"));
    FSMState<char> *final_state = fsm.addState(true, FSMAction("Final state for 'c'"));

    fsm.addEvent('a');
    fsm.addEvent('b');
    fsm.addEvent('c');

    init_state->addTransition('a',first_state);
    init_state->addTransition('b',init_state);
    init_state->addTransition('c',init_state);

    first_state->addTransition('a',first_state);
    first_state->addTransition('b',second_state);
    first_state->addTransition('c',init_state);

    second_state->addTransition('a',first_state);
    second_state->addTransition('c',final_state);
    second_state->addTransition('b',second_state);

    final_state->addTransition('a',first_state);
    final_state->addTransition('c',final_state);
    final_state->addTransition('b',init_state);

    fsm.print();
    fsm.run("aaaabbbbbcccc");
    exit(0);
}
