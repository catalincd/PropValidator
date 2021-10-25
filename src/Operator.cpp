#include "Operator.h"

extern std::string Operators[5];

Operator StringToOperator(std::string str)
{
    if (str == "") return YES;
    if (str == Operators[0]) return NOT;
    if (str == Operators[1]) return AND;
    if (str == Operators[2])  return OR;
    if (str == Operators[3]) return IMP;
    if (str == Operators[4]) return IFF;

    return UNDEF;
}

bool ComputeValue(bool a, bool b, Operator op)
{
    if (op == YES) return b;
    if (op == NOT) return !b;
    if (op == AND) return (a && b);
    if (op == OR) return (a || b);
    if (op == IMP) return (!a || b);
    if (op == IFF) return ((!a || b) && (a || !b));

    return false;
}