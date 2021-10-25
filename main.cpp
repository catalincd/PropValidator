#include <iostream>
#include <sstream>
#include <regex>
#include <vector>
#include <stack>

#include "src/Operator.h"
#include "src/Parser.h"

/// <summary>
/// You can change any of these operators
/// 
/// DO NOT CHANGE THEIR ORDER
/// </summary>
std::string Operators[5] = { "!", "&", "|", "=>", "<=>" };



int main(int argc, char** argv)
{   
    std::string input;

    std::cout << "Input your proposition: ";

    getline(std::cin, input);

    Parser::ProcessText(input);

    return 0;
}

