#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <memory>
#include "FSM.h"

template <typename T>
int** FSM<T>::generateMatrix(){
    int** array = new int*[this->states.size()];
    for(size_t i = 0; i != this->states.size(); i++){
        array[i] = new int[this->events.size()];
        for(size_t j = 0; j != this->events.size(); j++){
            auto transition = this->states[i].getStateByEvent(this->events[j]);
            if (transition == NULL){
                array[i][j] = -1;
                continue;
            }
            array[i][j] = transition->id;
        }
    }
    return array;
};

template <typename T>
FSM<T>::FSM(){
    this->init_state = NULL;
    this->current_state = NULL;
};

template <typename T>
FSM<T>::FSM(FSMState<T>* init_state){
    this->init_state = init_state;
    this->current_state = init_state;
};

template <typename T>
FSM<T>::FSM(std::vector<T> events, std::vector<std::vector<int>> array, std::vector<int> end_states,  std::vector<FSMAction<T>> actions) {
    this->addEvents(events);

    for (size_t i = 0; i < array.size(); i++){
        auto end_state = std::find(end_states.begin(), end_states.end(), i) != end_states.end();
        if (actions.size() >= i+1){
            FSMState<T> state(i, actions[i], end_state);
            this->addState(&state);
        } else {
            FSMState<T> state(i, end_state);
            this->addState(&state);
        }
    }

    for (size_t i = 0; i < array.size(); i++){
        for (size_t j = 0; j < array[i].size(); j++){
            this->states[i].addTransition(this->events[j], this->states[array[i][j]]);
        }
    }
    this->init_state = &this->states[0];
    this->current_state = &this->states[0];
}

template <typename T>
void FSM<T>::run(std::vector<T> input){
    for (size_t i = 0; i < input.size(); ++i){
        // need check, that T(event) contains in list of events
        this->current_state = this->current_state->getStateByEvent(input[i]);
        this->current_state->doAction(input[i]);
    }
    this->current_state = this->init_state;
};

template <typename T>
FSMState<T>* FSM<T>::addState(bool end_state, FSMAction<T> action){
    FSMState<T>* state (this->id_counter, action, end_state);
    this->addState(state);
    return &this->states[this->states.size()-1];
}

template <typename T>
FSMState<T>* FSM<T>::addState(FSMAction<T> action){
    FSMState<T>* state (this->id_counter, action, false);
    this->addState(state);
    return &this->states[this->states.size()-1];
}

template <typename T>
FSMState<T>* FSM<T>::addState(){
    FSMState<T>* state (this->id_counter);
    this->addState(state);
    return &this->states[this->states.size()-1];
}

template <typename T>
void FSM<T>::addState(FSMState<T>* state){
    if (this->init_state == NULL || this->current_state == NULL){
        this->init_state = state;
        this->current_state = state;
    }
    this->id_counter++;
    this->states.push_back(*state);
};

template <typename T>
void FSM<T>::addEvent(T event){
    this->events.push_back(event);
}

template <typename T>
void FSM<T>::addEvents(std::vector<T> events){
    for(auto event : events)
        this->events.push_back(event);
}

template <typename T>
void FSM<T>::join(const FSM<T>& fsm1, const FSM<T>& fsm2, bool isConcat, bool isIntersaction ){
    std::vector<FSMState<T>> statesFSM1 = fsm1.getStates();
    std::vector<FSMState<T>> statesFSM2 = fsm2.getStates();
    std::vector<T> eventsFSM1 = fsm1.getEvents(); 
    
    int id = 0;
    
    for (size_t i = 0; i < statesFSM1.size(); i++){
        for (size_t j = 0; j < statesFSM2.size(); j++){
            bool end_state = false;
            if (isIntersaction && (statesFSM1[i].end_state && statesFSM2[j].end_state))
                end_state = true;
            if (isConcat && (statesFSM1[i].end_state || statesFSM2[j].end_state))
                end_state = true;

            FSMState<T> state (id, statesFSM1[i].getAction(), end_state);
            this->addState(&state);
            id++;
        }
    }
    
    for (auto event : eventsFSM1)
        this->addEvent(event);

    for (size_t i=0; i < statesFSM1.size(); i++){
        for (size_t j=0; j < statesFSM2.size(); j++){
            for(auto event : eventsFSM1){                        
                // FIND STATE BY EVENT IN LIST OF TRANSITIONS
                FSMState<T>* stateByEventFSM1 = statesFSM1[i].getStateByEvent(event);
                FSMState<T>* stateByEventFSM2 = statesFSM2[j].getStateByEvent(event);
                
                // FIND INDEX
                auto itr = std::find(statesFSM1.begin(), statesFSM1.end(), *stateByEventFSM1);
                int indexStateByEventFSM1 = std::distance(statesFSM1.begin(), itr);
                itr = std::find(statesFSM2.begin(), statesFSM2.end(), *stateByEventFSM2);
                int indexStateByEventFSM2 = std::distance(statesFSM2.begin(), itr);

                this->states[i*statesFSM1.size()+j].addTransition(
                    event, this->states[indexStateByEventFSM1*statesFSM1.size()+indexStateByEventFSM2]
                );
            }
        }
    }
}

template <typename T>
std::shared_ptr<FSM<T>> FSM<T>::concat(const FSM<T>& fsm1, const FSM<T>& fsm2){
    std::shared_ptr<FSM<char>> new_fsm(new FSM<char>);
    new_fsm->join(fsm1, fsm2, true, false);
    return new_fsm;
};

template <typename T>
std::shared_ptr<FSM<T>> FSM<T>::intersaction(const FSM<T>& fsm1, const FSM<T>& fsm2){
    
    std::shared_ptr<FSM<char>> new_fsm(new FSM<char>);
    new_fsm->join(fsm1, fsm2, false, true);
    return new_fsm;
};

template <typename T>
void FSM<T>::print(){
    int** array = this->generateMatrix();
    std::cout << "q - state, qe - end state" << std::endl;
    
    std::cout << "\t";
    for(size_t i = 0; i < this->events.size(); i++)
        std::cout << this->events[i] << "\t";

    for(size_t i = 0; i < this->states.size(); i++){
        std::cout << std::endl;
        if (this->states[i].end_state)
            std::cout << "qe" << i << "\t";
        else
            std::cout << "q" << i << "\t";


        for(size_t j = 0; j < this->events.size(); j++){
            std::cout << array[i][j] << "\t";
        }        
    }
    std::cout << std::endl;
};
