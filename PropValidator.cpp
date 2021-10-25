#include <iostream>
#include <sstream>
#include <regex>
#include <vector>
#include <stack>

enum LogicOperator { NOT, AND, OR, IMP, IFF, YES, UNDEF };

typedef unsigned int uint;
typedef std::pair<int, int> pint;
typedef std::pair<LogicOperator, pint> OperatorData;

const std::string Operators[5] = { "!", "&", "|", "=>", "<=>" };

const std::string O_NOT = Operators[0];
const std::string O_AND = Operators[1];
const std::string O_OR  = Operators[2];
const std::string O_IMP = Operators[3];
const std::string O_IFF = Operators[4];


LogicOperator AllLogicOperators[7] = { NOT, AND, OR, IMP, IFF, YES, UNDEF };
/// <summary>
/// Programul considera ca fiecare propozitie are in fata fie YES, fie NOT
/// YES nu trebuie scris in propozitie
/// </summary>
LogicOperator StringToOperator(std::string str)
{
    if (str == "") return YES;
    if (str == O_NOT) return NOT;
    if (str == O_AND) return AND;
    if (str == O_OR)  return OR;
    if (str == O_IMP) return IMP;
    if (str == O_IFF) return IFF;

    return UNDEF;
}

OperatorData GetOperatorFromString(std::string str, int first = 0, int last = std::string::npos)
{
    str = str.substr(first, last);
    std::string CurrentStrings[5] = { "", "", "", "", "" };

    for (int i = 0; i < str.length(); i++)
    {
        for (int t = 0; t < 5; t++)
        {
            int currentLen = CurrentStrings[t].length();
            if (Operators[t].length() > currentLen)
            {
                if(Operators[t][currentLen] == str[i])
                {
                    CurrentStrings[t] += str[i];
                    if (Operators[t] == CurrentStrings[t])
                        return OperatorData(AllLogicOperators[t], pint(i, i + CurrentStrings[t].length()));
                }
            }
        }
    }

    return OperatorData(UNDEF, pint(first, last));
}

std::string InsertStringAt(std::string& text, int index, std::string toInsert)
{
    std::string newText = text.substr(0, index);
    text = text.substr(index);

    return newText + toInsert + text;
}

std::string RemoveSpaces(std::string propText)
{
    std::regex globalSpaces("\ ");
    return std::regex_replace(propText, globalSpaces, "");
}

class Prop
{
    int StartIndex = 0;
    int EndIndex = std::string::npos;
    std::vector<Prop*> ComposingProps;
    std::vector<LogicOperator> Operators;
    std::string* mainText;
    bool Value = false;
public:
    Prop(){}

    Prop(int StartIndex)
    {
        this->StartIndex = StartIndex;
    }

    bool GetValue() const
    {
        return Value;
    }

    void SetValue(bool v)
    {
        Value = v;
    }

    void AddProp(Prop* prop)
    {
        ComposingProps.emplace_back(prop);
    }

    void SetEndingIndex(int i)
    {
        EndIndex = i;
    }

    void SetText(std::string* text)
    {
        mainText = text;
    }

    void DebugPrintText(std::string offset = "")
    {
        std::cout << offset <<mainText->substr(StartIndex, EndIndex - StartIndex + 1) << std::endl;
    }

    void DebugPrintChildren(std::string offset = "")
    {
        DebugPrintText(offset);
        for (int i = 0; i < ComposingProps.size(); i++)
        {
            ComposingProps[i]->DebugPrintChildren(offset + "\t");
        }
    }

    int GetStartingIndex() const
    {
        return StartIndex;
    }

    int GetEndingIndex() const
    {
        return EndIndex;
    }

    void ParseSmallOperators()
    {

    }

    void ParseOperators()
    {
        if (ComposingProps.size() == 0)
        {
            ParseSmallOperators();
            return;
        }

        int lastProp = 0;
        //if (ComposingProps[0]->GetStartingIndex() == StartIndex + 1)
        //{
        //    Operators.emplace_back(YES);
        //}

        std::string CurrentOperator = "";

        for (int i = StartIndex + 1; i < EndIndex; i++)
        {
            if (i >= ComposingProps[lastProp]->GetStartingIndex())
            {
                LogicOperator op = StringToOperator(CurrentOperator);
                if (op != UNDEF)
                {
                    Operators.emplace_back(op);
                    //std::cout << "Recognized Operator '" << CurrentOperator << "'\n";
                }
                else
                {
                    OperatorData od = GetOperatorFromString(CurrentOperator);

                    if (od.first != UNDEF)
                    {
                        std::cout << (*mainText).substr(i - CurrentOperator.length()) << std::endl;
                        std::cout << "Unrecognized Operator '" << CurrentOperator << "' in prop: " << mainText->substr(StartIndex, EndIndex - StartIndex + 1) << "\n";
                    }
                    else
                    {
                        std::cout << "Unrecognized Operator '" << CurrentOperator << "' in prop: " << mainText->substr(StartIndex, EndIndex - StartIndex + 1) << "\n";
                        DebugPrintChildren();
                    }
                }

                CurrentOperator = "";
                i = ComposingProps[lastProp]->GetEndingIndex();
                lastProp++;
                continue;
            }
            else
            CurrentOperator += (*mainText)[i];
        }
    }

    ~Prop(){}
};

Prop MainProp;

void PrintError()
{
    
}

std::vector<Prop*> AllProps;

void ParseProp(std::string propText)
{
    int LEN = propText.length();
    int OPEN = 0;

    Prop* CurrentProp = &MainProp;
    
    std::stack<Prop*> Stack;
    Stack.push(&MainProp);

    for (int i = 0; i < LEN; i++)
    {
        char C = propText[i];

        if (C == '(')
        {
            OPEN++;
            CurrentProp = new Prop(i);
            CurrentProp->SetText(&propText);
            Stack.push(CurrentProp);
        }

        if (C == ')')
        {
            OPEN--;
            Stack.pop();
            Prop* topProp = CurrentProp;
            topProp->SetEndingIndex(i);
            CurrentProp = Stack.top();
            CurrentProp->AddProp(topProp);
            AllProps.emplace_back(topProp);
        }
    }

    std::cout << "OPEN: " << OPEN << std::endl;

    //int len = AllProps.size() - 1;
    //MainProp = *(AllProps[len]);
    //AllProps[len]->DebugPrintChildren();
    //AllProps[len]->ParseOperators();

    for (int i = 0; i < AllProps.size(); i++)
    {
        AllProps[i]->ParseOperators();
    }
}

int main()
{   
    std::string example = "((P & Q) => (!(R & Q)) & ((!(!Q)) & P))";

    std::cout << RemoveSpaces(example) << std::endl;

    ParseProp(RemoveSpaces(example));

    OperatorData od = GetOperatorFromString("P&Q");

    std::string wooo = "wooo";
    std::cout << InsertStringAt(wooo, 1, "O");

    return 0;
}

