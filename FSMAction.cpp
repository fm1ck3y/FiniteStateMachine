#include <iostream>
#include <string>

template<typename T>
class FSMAction{
    private:
        std::string message;
    public:
        FSMAction(){};

        FSMAction(std::string message){
            this->message = message;
        }

        void operator()(T event){
            std::cout << event << ": " << this->message << std::endl;
        }
};