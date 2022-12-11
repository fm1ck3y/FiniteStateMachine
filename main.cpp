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
        bool end_state = false; // need this variable?

    public:
        int id;

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

        void DoAction(){
            this->action.print();
        }

        void AddTransition(T event, FSMState<T>& to_state){
            this->transitions[event] = &to_state;
        };

        FSMState<T>* GetTransitionByEvent(T event){
            try {
                return this->transitions[event];
            }
            catch (const std::out_of_range&){
                std::cout << "Transition not found :(" << std::endl;
                return NULL;
            }
        }
};

template <typename T>
class FSM{
    private:
        FSMState<T>* current_state;
        FSMState<T>* init_state;

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

/*        FSM(std::vector<std::vector<T>> table_of_transitions){
            for(auto q_transitions : table_of_transitions){
                for(T in )
            }
        }
*/
        void Run(std::string input){
            for (size_t i = 0; i < input.size(); ++i){
                // need check, that T(event) contains in list of events
                this->current_state = this->current_state->GetTransitionByEvent(input[i]);
                this->current_state->DoAction();
            }
        };

        void AddState(FSMState<T> state){
            this->states.push_back(state);
        };

        void AddEvent(T event){
            this->events.push_back(event);
        }

        int** GenerateMatrix(){
            // VARIABLE 'STATES' CREATE FOR THIS ;)
            int** array = new int*[this->states.size()];
            for(size_t i = 0; i != this->states.size(); i++){
                // need tag that this state end
                // HOW?
                array[i] = new int[this->events.size()];
                for(size_t j = 0; j != this->events.size(); j++){
                    auto transition = this->states[i].GetTransitionByEvent(this->events[j]);
                    if (transition == NULL){
                        array[i][j] = -1;
                        continue;
                    }
                    array[i][j] = transition->id;
                }
            }
            return array;
        };

        static void Concat(FSM* fsm_1, FSM* fsm_2){
            // DO MERGE 
        };
        static void Intersaction(FSM* fsm_1, FSM* fsm_2){
            // DO MERGE , BUT CHANGE FINAL STATES
        };
        friend FSM& operator+(const FSM lfsm, const FSM& rfsm){
            return nullptr;
        };
        friend FSM& operator*(const FSM lfsm, const FSM& rfsm)
        {
            return nullptr;
        };
        void print(){
            int** array = this->GenerateMatrix();
            for(size_t i = 0; i != this->states.size(); i++){
                std::cout << std::endl;
                for(size_t j = 0; j != this->events.size(); j++){
                    std::cout << array[i][j] << "\t";
                }        
            }
            std::cout << std::endl;
        };
};

template <typename T>
std::ostream& operator<<(std::ostream& out, const FSM<T>& fsm)
{
    return out << std::string(fsm);
}


int main(){
    FSMState<char> init_state(0, FSMAction("Init state"));
    FSMState<char> first_state(1, FSMAction("This state for 'a' symbol"));
    FSMState<char> second_state(2, FSMAction("This state for 'b' symbol"));
    FSMState<char> final_state(3,FSMAction("Final state for 'c'"), true);

    init_state.AddTransition('a',first_state);
    init_state.AddTransition('b',init_state);
    init_state.AddTransition('c',init_state);

    first_state.AddTransition('a',first_state);
    first_state.AddTransition('b',second_state);
    first_state.AddTransition('c',init_state);

    second_state.AddTransition('a',first_state);
    second_state.AddTransition('c',final_state);
    second_state.AddTransition('b',second_state);

    final_state.AddTransition('a',first_state);
    final_state.AddTransition('c',final_state);
    final_state.AddTransition('b',init_state);

    FSM<char> fsm(init_state);
    fsm.AddState(init_state);
    fsm.AddState(first_state);
    fsm.AddState(second_state);
    fsm.AddState(final_state);
    fsm.AddEvent('a');
    fsm.AddEvent('b');
    fsm.AddEvent('c');

    fsm.print();

    fsm.Run("aaaabbbbbcccc");

    // merge_fsm = fsm_1 + fsm_2;
    // intersaction_fsm = fsm * fsm_2;

    exit(0);
}
