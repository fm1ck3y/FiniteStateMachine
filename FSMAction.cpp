#include <iostream>
#include <string>

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