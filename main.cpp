#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <map>

class FSMAction{
    private:
        std::string message;
    public:
        FSMAction(){};
        FSMAction(std::string message){
            this->message = message;
        }

        void print(){
            std::cout << this->message << std::endl;
        }
};

template <typename T>
class FSMState {
    private:
        FSMAction action;
        std::map<T,FSMState*> transitions;
    public:
        int id;
        bool end_state = false;
        FSMState(){};

        FSMState(int id, bool end_state=false){
            std::string message = "This state: " + std::to_string(id);
            this->action = FSMAction(message);
            this->id = id;
            this->end_state = end_state;
        };

        FSMState(int id, FSMAction action, bool end_state=false){
            this->action = action;
            this->id = id;
            this->end_state = end_state;
        };

        FSMState(int id, FSMAction action, std::map<T,FSMState&> transitions, bool end_state=false){
            this->transitions = transitions;
            this->action = action;
            this->id = id;
            this->end_state = end_state;
        };

        const FSMAction& getAction() const { return this->action; }

        void doAction(){
            this->action.print();
        }

        void addTransition(T event, FSMState<T>& to_state){
            this->transitions[event] = &to_state;
        };

        FSMState<T>* getStateByEvent(T event){
            try {
                return this->transitions[event];
            }
            catch (const std::out_of_range&){
                std::cout << "Transition not found :(" << std::endl;
                return NULL;
            }
        }
        
        friend bool operator==(const FSMState<T>& lhs, const FSMState<T>& rhs) { return lhs.id == rhs.id; }
};

template <typename T>
class FSM{
    private:
        FSMState<T>* current_state = NULL;
        FSMState<T>* init_state = NULL;

        // NO NEED WITHOUT MERGE OR INTERSCATION
        // CAN THINK OF A WAY FOR FIND ALL STATES
        // AND EVENTS IN CHAIN, BUT I'M TOO LAZY :)
        std::vector<FSMState<T>> states;
        std::vector<T> events;

    public:
        FSM(){};
        FSM(FSMState<T> init_state){
            this->init_state = &init_state;
            this->current_state = &init_state;
        };


        const std::vector<FSMState<T>>& getStates() const { return this->states; }
        const std::vector<T>& getEvents() const { return this->events; }

/*        FSM(std::vector<std::vector<T>> table_of_transitions){
            for(auto q_transitions : table_of_transitions){
                for(T in )
            }
        }
*/
        void run(std::string input){
            for (size_t i = 0; i < input.size(); ++i){
                // need check, that T(event) contains in list of events
                this->current_state = this->current_state->getStateByEvent(input[i]);
                this->current_state->doAction();
            }
        };

        void addState(FSMState<T> state){
            if (this->init_state == NULL || this->current_state == NULL){
                this->init_state = &state;
                this->current_state = &state;
            }
            this->states.push_back(state);
        };

        void addEvent(T event){
            this->events.push_back(event);
        }

        int** generateMatrix(){
            // VARIABLE 'STATES' CREATE FOR THIS ;)
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

        static void concat(FSM* fsm1, FSM* fsm2, FSM* new_fsm){
            std::vector<FSMState<T>> statesFSM1 = fsm1->getStates();
            std::vector<FSMState<T>> statesFSM2 = fsm2->getStates();
            std::vector<T> eventsFSM1 = fsm1->getEvents(); 
            std::vector<FSMState<T>> states;
            int id = 0;

            for (auto stateFSM1 : statesFSM1){
                for (auto stateFSM2 : statesFSM2){
                    states.push_back(FSMState<T>(id, stateFSM1.getAction(), stateFSM1.end_state));
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
                        auto itr = std::find(statesFSM1.begin(), statesFSM2.end(), *stateByEventFSM1);
                        int indexStateByEventFSM1 = std::distance(statesFSM1.begin(), itr);
                        itr = std::find(statesFSM2.begin(), statesFSM2.end(), *stateByEventFSM2);
                        int indexStateByEventFSM2 = std::distance(statesFSM2.begin(), itr);

                        states[i*statesFSM1.size()+j].addTransition(
                            event, states[indexStateByEventFSM1*statesFSM1.size()+indexStateByEventFSM2]
                        );
                    }
                    new_fsm->addState(states[i*statesFSM1.size()+j]);
                }
            }

            // GOOD OUTPUT
            new_fsm->print();
        };

        static void intersaction(FSM* fsm1, FSM* fsm2, FSM* new_fsm){
            FSMState<T>::concat(fsm1, fsm2, new_fsm);
            std::vector<FSMState<T>> statesFSM1 = fsm1->getStates();
            std::vector<FSMState<T>> statesFSM2 = fsm2->getStates();
            // statesFSM1[i].end_state and  statesFSM2[j].end_state => statesNewFSM[i*sizeStatesFSM1+j].end_state = true
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

/*template <typename T>
std::ostream& operator<<(std::ostream& out, const FSM<T>& fsm)
{
    return out << std::string(fsm);
}*/


void testconcat(){
    FSMState<char> q_0 (0,true);
    FSMState<char> q_1 (1);
    FSMState<char> q_2 (2);
    FSMState<char> q_3 (3,true);
    
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
    fsm_1.addEvent('0');
    fsm_1.addEvent('1');

    fsm_1.print();

    FSMState<char> q_5 (0);
    FSMState<char> q_6 (1,true);
    FSMState<char> q_7 (2);
    FSMState<char> q_8 (3,true);
    
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
    FSM<char>::concat(&fsm_1, &fsm_2, &concat_fsm);
    // OUTPUT WITH INCORRECT TRANSITIONS IN LAST 5
    // WHY ?
    // concat_fsm.print();
}

int main(){

    FSMState<char> init_state(0, FSMAction("Init state"));
    FSMState<char> first_state(1, FSMAction("This state for 'a' symbol"));
    FSMState<char> second_state(2, FSMAction("This state for 'b' symbol"));
    FSMState<char> final_state(3,FSMAction("Final state for 'c'"), true);

    init_state.addTransition('a',first_state);
    init_state.addTransition('b',init_state);
    init_state.addTransition('c',init_state);

    first_state.addTransition('a',first_state);
    first_state.addTransition('b',second_state);
    first_state.addTransition('c',init_state);

    second_state.addTransition('a',first_state);
    second_state.addTransition('c',final_state);
    second_state.addTransition('b',second_state);

    final_state.addTransition('a',first_state);
    final_state.addTransition('c',final_state);
    final_state.addTransition('b',init_state);

    FSM<char> fsm(init_state);
    fsm.addState(init_state);
    fsm.addState(first_state);
    fsm.addState(second_state);
    fsm.addState(final_state);
    fsm.addEvent('a');
    fsm.addEvent('b');
    fsm.addEvent('c');

    fsm.print();

    fsm.run("aaaabbbbbcccc");
    
    testconcat();
    exit(0);
}
