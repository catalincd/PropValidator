#pragma once
#include <string>

enum Operator { NOT, AND, OR, IMP, IFF, YES, UNDEF };

Operator StringToOperator(std::string);

bool ComputeValue(bool a, bool b, Operator op);
