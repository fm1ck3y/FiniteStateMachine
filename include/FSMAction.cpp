#include <iostream>
#include <string>
#include <functional>

#pragma once
#ifndef FSMACTION_H
#define FSMACTION_H
template<typename T>
class FSMAction{
    private:
        std::string message;
        std::function<void(T)> function;
    public:
        FSMAction(){};

        FSMAction(std::string message){
            this->message = message;
        }

        FSMAction(std::string message, std::function<void(T)> function){
            this->message = message;
            this->function = function;
        }

        void operator()(T event){
            if (function != NULL)
                function(event);
            std::cout << event << ": " << this->message << std::endl;
        }
};
#endif