#include <iostream>
#include <string>
#include <memory>
#include <stack>
#include "../include/Compiler.cpp"

int main(){
    Compiler compiler;
    
    //std::string input_message = "7+4*8+2*(3+2)$";
    std::string input_message = "";
    std::cout << "Enter expression: ";
    std::cin >> input_message;
    std::cout << std::endl;
    
    std::string result = compiler.run(input_message);
    std::cout << "Assembly code: " << std::endl << std::endl  << result << std::endl;

    return 0;
}
