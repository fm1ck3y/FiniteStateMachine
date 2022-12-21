#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <map>

template <typename T>
class FSM{
    private:
        FSMState<T>* current_state = NULL;
        FSMState<T>* init_state = NULL;
        int id_counter = 0;
        // NO NEED WITHOUT MERGE OR INTERSCATION
        // CAN THINK OF A WAY FOR FIND ALL STATES
        // AND EVENTS IN CHAIN, BUT I'M TOO LAZY :)
        std::vector<FSMState<T>> states;
        std::vector<T> events;

    public:
        
        const std::vector<FSMState<T>>& getStates() const { return this->states; }
        const std::vector<T>& getEvents() const { return this->events; }

        FSM(){
            this->init_state = NULL;
            this->current_state = NULL;
        };

        FSM(FSMState<T>* init_state){
            this->init_state = init_state;
            this->current_state = init_state;
        };

        FSM(std::vector<T> events, std::vector<std::vector<int>> array, std::vector<int> end_states,  std::vector<FSMAction> actions) {
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

        void run(std::string input){
            for (size_t i = 0; i < input.size(); ++i){
                // need check, that T(event) contains in list of events
                this->current_state = this->current_state->getStateByEvent(input[i]);
                this->current_state->doAction();
            }
            this->current_state = this->init_state;
        };

        FSMState<T>* addState(bool end_state, FSMAction action){
            FSMState<T>* state (this->id_counter, action, end_state);
            this->addState(state);
            return &this->states[this->states.size()-1];
        }

        FSMState<T>* addState(FSMAction action){
            FSMState<T>* state (this->id_counter, action, false);
            this->addState(state);
            return &this->states[this->states.size()-1];
        }

        FSMState<T>* addState(){
            FSMState<T>* state (this->id_counter);
            this->addState(state);
            return &this->states[this->states.size()-1];
        }

        void addState(FSMState<T>* state){
            if (this->init_state == NULL || this->current_state == NULL){
                this->init_state = state;
                this->current_state = state;
            }
            this->id_counter++;
            this->states.push_back(*state);
        };

        void addEvent(T event){
            this->events.push_back(event);
        }

        void addEvents(std::vector<T> events){
            for(auto event : events)
                this->events.push_back(event);
        }

        int** generateMatrix(){
            // VARIABLE 'STATES' CREATE FOR THIS ;)

            // for (auto state : states) std::cout << state.id << std::endl;

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

        void updateEndState(int id, bool end_state){
            for(auto state : this->states)
                if (state.id == id) state.end_state = end_state;
        }

        static void join(FSM* fsm1, FSM* fsm2, FSM* new_fsm){
            std::vector<FSMState<T>> statesFSM1 = fsm1->getStates();
            std::vector<FSMState<T>> statesFSM2 = fsm2->getStates();
            std::vector<T> eventsFSM1 = fsm1->getEvents(); 
            std::vector<FSMState<T>> states;
            int id = 0;
            
            for (auto stateFSM1 : statesFSM1){
                for (auto stateFSM2 : statesFSM2){
                    states.push_back(FSMState<T>(id, stateFSM1.getAction()));
                    id++;
                }
            }
            
            for (auto event : eventsFSM1)
                new_fsm->addEvent(event);

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

                        states[i*statesFSM1.size()+j].addTransition(
                            event, states[indexStateByEventFSM1*statesFSM1.size()+indexStateByEventFSM2]
                        );
                    }
                    new_fsm->addState(&states[i*statesFSM1.size()+j]);
                }
            }

            // GOOD OUTPUT
            new_fsm->print();
        }

        static void concat(FSM* fsm1, FSM* fsm2){
            FSM<char> new_fsm;
            FSM<T>::join(fsm1, fsm2, &new_fsm);
            std::vector<FSMState<T>> statesFSM1 = fsm1->getStates();
            std::vector<FSMState<T>> statesFSM2 = fsm2->getStates();
            for (size_t i=0; i < statesFSM1.size(); i++){
                for (size_t j=0; j < statesFSM2.size(); j++){
                    if (statesFSM1[i].end_state || statesFSM2[j].end_state)
                        new_fsm.updateEndState(i*statesFSM1.size()+j+1, true);
                }
            }
            // new_fsm.print();
        };

        static void intersaction(FSM* fsm1, FSM* fsm2){
            FSM<char> new_fsm;
            FSM<T>::join(fsm1, fsm2, &new_fsm);
            std::vector<FSMState<T>> statesFSM1 = fsm1->getStates();
            std::vector<FSMState<T>> statesFSM2 = fsm2->getStates();
            for (size_t i=0; i < statesFSM1.size(); i++){
                for (size_t j=0; j < statesFSM2.size(); j++){
                    if (statesFSM1[i].end_state && statesFSM2[j].end_state)
                        new_fsm.updateEndState(i*statesFSM1.size()+j+1, true);
                }
            }
            // new_fsm.print();
        };

        friend FSM& operator+(const FSM lfsm, const FSM& rfsm){
            return nullptr;
        };

        friend FSM& operator*(const FSM lfsm, const FSM& rfsm)
        {
            return nullptr;
        };

        void print(){
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
};
