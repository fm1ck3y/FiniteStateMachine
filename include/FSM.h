#pragma once
#ifndef FSM_H
#define FSM_H
template <typename T>
class FSM{
    private:
        FSMState<T>* current_state = NULL;
        FSMState<T>* init_state = NULL;
        int id_counter = 0;
        std::vector<FSMState<T>> states;
        std::vector<T> events;
        int** generateMatrix();

    public:
        FSM();
        FSM(FSMState<T>* init_state);
        FSM(std::vector<T> events, std::vector<std::vector<int>> array, std::vector<int> end_states,  std::vector<FSMAction<T>> actions);
        void run(std::vector<T> input);
        FSMState<T>* addState(bool end_state, FSMAction<T> action);
        FSMState<T>* addState(FSMAction<T> action);
        FSMState<T>* addState();
        void addState(FSMState<T>* state);
        void addEvent(T event);
        void addEvents(std::vector<T> events);
        void updateEndState(int id, bool end_state);
        static void join(FSM<T>* fsm1, FSM<T>* fsm2, FSM<T>* new_fsm);
        static void concat(FSM<T>* fsm1, FSM<T>* fsm2);
        static void intersaction(FSM<T>* fsm1, FSM<T>* fsm2);
        void print();
        const std::vector<FSMState<T>>& getStates() const { return this->states; }
        const std::vector<T>& getEvents() const { return this->events; }

};
#endif