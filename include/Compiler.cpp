#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>

#pragma once
#ifndef COMPILER_H
#define COMPILER_H

bool isNumber(const std::string& s)
{
    for (char const &ch : s) {
        if (std::isdigit(ch) == 0) 
            return false;
    }
    return true;
 }

class Compiler {
    private:
        std::string result = "";
        char operation;

        int it = 1;

        std::stack<std::string> operands;
        std::stack<char> operations;

        std::vector<char> events = {'+','*','(',')','$'};
        std::vector<char> events_float = {'0','1','2','3','4','5','6','7','8','9','-','.'};
        std::map<char,std::vector<int>> transitions {
            {'+', {5,5,2,0,2}},
            {'*', {2,5,2,0,2}},
            {'(', {2,2,2,0,2}},
            {')', {5,5,6,0,3}},
            {'$', {5,5,3,0,4}},
        };

    public:
        Compiler(){};

        bool validateInput(std::string input){
            std::set<char> unique_chars;
            
            for (size_t i = 0; i < input.size(); i++) unique_chars.insert(input[i]);
            for (size_t i = 0; i < this->events.size(); i++) unique_chars.insert(this->events[i]);
            for (size_t i = 0; i < this->events_float.size(); i++) unique_chars.insert(this->events_float[i]);

            if (unique_chars.size() > this->events.size() + this->events_float.size()){
                std::cout << "[ERROR] Incorrect input." << std::endl;
                return false;
            }
            if (unique_chars.size() == 0){
                std::cout << "[ERROR] Input is empty." << std::endl;
                return false;
            }
            if (unique_chars.find('$') == unique_chars.end()){
                std::cout << "[ERROR] End char '$' not found." << std::endl;
                return false;
            }
            std::cout << "Validation success." << std::endl;
            return true;
        }

        std::string run(std::string input){
            if (!this->validateInput(input))
                exit(1);


            this->it = 1;
            std::string number = "";
            for (size_t i = 0; i < input.size(); i++)
            {
                if (std::find(this->events_float.begin(), this->events_float.end(), input[i]) != this->events_float.end()){
                    number += input[i];
                } else {
                    if (number != ""){
                        double _number = std::stod(number);
                        this->operands.push(number);
                    }
                    this->operation = input[i];
                    this->handleOperation();
                    number = "";
                }
            }

            if (this->operations.top() == '$'){
                this->operation = '$';            
                this->handleOperation();
            }
            return this->result;
        }

        void handleOperation(){
            auto actions = this->transitions[this->operation];
            this->printStacks();
            if (this->operations.size() > 0){
                char top_operation = this->operations.top();
                auto itr = std::find(this->events.begin(), this->events.end(), top_operation);
                int index_action = std::distance(this->events.begin(), itr);
                this->handleAction(actions[index_action]);
            } else{
                this->operations.push(this->operation);
            }
        }

        void printStacks(){
            std::cout << "Operations: [";
            for (std::stack<char> dump = this->operations; !dump.empty(); dump.pop())
                std::cout << dump.top() << ",";
            std::cout << "]; ";

            std::cout << "Operands: [";
            for (std::stack<std::string> dump = this->operands; !dump.empty(); dump.pop())
                std::cout << dump.top() << ",";
            std::cout << "];" << std::endl << std::endl;
        }

        void handleAction(int action = 0){
            switch (action){
                case 2:
                    this->operations.push(this->operation);
                    break;
                case 3:
                    std::cout << "ERROR" << std::endl;
                    exit(1);
                case 4:
                    this->result = this->operands.top();
                    this->operands.pop();
                    break;
                case 5:
                    this->solve();
                    this->handleOperation();
                    break;
                case 6:
                    while(true){
                        this->operation = this->operations.top();
                        this->operations.pop();
                        if (this->operation == '(')
                            break;
                        this->solve();
                    }
                    break;
            }
        }

        void solve(){
            auto a = this->operands.top();
            this->operands.pop();
            auto b = this->operands.top();
            this->operands.pop();
            auto operation = this->operations.top();
            this->operations.pop();

            std::string res = "";
            if (this->it == 1) res = "LOAD=";

            auto l = isNumber(b) ? a : b;
            auto r = isNumber(b) ? b : a;

            if (operation == '+'){
                res += l + "\nSTORE $" + std::to_string(this->it) + "\nLOAD=" + r + "\nADD $" + std::to_string(this->it);
            } else if (operation == '*'){
                res += l + "\nSTORE $" + std::to_string(this->it) + "\nLOAD=" + r + "\nMPY $" + std::to_string(this->it);
            }
            this->operands.push(res);
            this->it++;
        }
};
#endif