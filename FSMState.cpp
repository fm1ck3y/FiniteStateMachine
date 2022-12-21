#include <iostream>
#include <vector>
#include <string>
#include <map>

template <typename T>
class FSMState {
    private:
        FSMAction action;
        
    public:
        int id;
        std::map<T,FSMState*> transitions;
        bool end_state = false;
        FSMState(){};

        FSMState(int id, bool end_state=false){
            this->end_state = end_state;
            this->id = id;
            std::string message = "This state: " + std::to_string(id);
            this->action = FSMAction(message);
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

        void addTransition(T event, FSMState<T>* to_state){
            this->transitions[event] = to_state;
        };

        void addTransition(std::vector<T> events, FSMState<T>& to_state){
            for (auto event : events)
                this->transitions[event] = &to_state;
        };

        void addTransition(std::vector<T> events, FSMState<T>* to_state){
            for (auto event : events)
                this->transitions[event] = to_state;
        };

        void addTransitions(std::map<T,FSMState*> transitions){
            for (auto el : transitions)
                this->transitions[el.first] = &el.second;
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
