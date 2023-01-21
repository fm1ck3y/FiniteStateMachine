#include <iostream>
#include <string>
#include <memory>
#include <stack>
#include "../include/Interpreter.cpp"

int main(){
    Interpreter interpreter;
    
    std::string input_message;
    std::cout << "Enter expression: ";
    std::cin >> input_message;
    std::cout << std::endl;
    
    double result = interpreter.run(input_message);
    std::cout << "Result: " << result << std::endl;

    return 0;
}
