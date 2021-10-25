#include <stack>
#include "Parser.h"
#include "Operator.h"
#include <iostream>
#include <map>

extern std::string Operators[5];

int OperatorStarts(std::string propText, int i)
{
    for (int t = 0; t < 5; t++)
    {
        int OP_LEN = Operators[t].length();

        if (propText.substr(i, OP_LEN) == Operators[t])
        {
            return OP_LEN;
        }
    }
    return false;
}

std::string Parser::RemoveSpaces(std::string propText)
{
    std::regex globalSpaces("\ ");
    return std::regex_replace(propText, globalSpaces, "");
}


std::string Parser::InsertStringAt(std::string& text, int index, std::string toInsert)
{
    std::string newText = text.substr(0, index);
    text = newText + toInsert + text.substr(index);

    return text;
}

std::string Parser::AddParanthesis(std::string propText)
{
    

    std::string current = "";
    int currentStart = -1;

    if (propText[0] != '(' || propText[propText.length() - 1] != ')')
    {
        propText = InsertStringAt(propText, 0, "(");
        propText += ")";
    }

    for (int i = 0; i < propText.length(); i++)
    {
        char c = propText[i];
        if (c == '(' || c == ')')
        {
            if (current != "")
            {
                propText = InsertStringAt(propText, i - current.length(), "(");
                propText = InsertStringAt(propText, i + 1, ")");
                i += 2;
                current = "";
                currentStart = -1;
            }
        }
        else 
        {
            int OperatorStartsHere = OperatorStarts(propText, i);

            if (OperatorStartsHere)
            {
                if (current != "")
                {
                    propText = InsertStringAt(propText, i - current.length(), "(");
                    propText = InsertStringAt(propText, i + 1, ")");
                    i += 2;
                    current = "";
                    currentStart = -1;
                }

                i += OperatorStartsHere - 1;
            }
            else
            {
                current += c;
                if (currentStart == -1)
                    currentStart = i;
            }
        }
    }

    return propText;
}

std::vector<Prop*> Parser::GetPropsFromString(std::string propText)
{
    std::vector<Prop*> AllProps;

    int LEN = propText.length();
    int OPEN = 0;

    Prop* MainProp = new Prop(0);
    MainProp->SetEndIndex(std::string::npos);

    Prop* CurrentProp = MainProp;
    std::stack<Prop*> Stack;
    Stack.push(MainProp);

    for (int i = 0; i < LEN; i++)
    {
        char C = propText[i];

        if (C == '(')
        {
            OPEN++;
            CurrentProp = new Prop(i + 1);
            Stack.push(CurrentProp);
        }

        if (C == ')')
        {
            OPEN--;
            Stack.pop();
            Prop* topProp = CurrentProp;
            topProp->SetEndIndex(i);
            CurrentProp = Stack.top();
            CurrentProp->AddProp(topProp);
            AllProps.emplace_back(topProp);
        }
    }

    return AllProps;
}

void TableLoop(const int current, const int CNT, const Prop* finalProp, const std::vector<std::string> names, std::map<std::string, std::vector<Prop*>> LeafProps)
{
    if (current == CNT)
    {
        std::map<std::string, std::vector<Prop*>>::iterator it;
        int COUNT = 0;

        std::cout << " +";

        for (int i = 0; i < CNT; i++)
        {
            std::cout << "---+";
        }

        std::cout << "-----+\n";

        for (it = LeafProps.begin(); it != LeafProps.end(); ++it)
        {
            std::cout << " | " << it->second[0]->GetValue();
            COUNT++;
        }
        std::cout << " |  " << finalProp->GetValue() << "  |\n";
        return;
    }

    std::string name = names[current];

    for (int i = 0; i < LeafProps[name].size(); i++)
    {
        LeafProps[name][i]->SetValue(false);
    }

    TableLoop(current + 1, CNT, finalProp, names, LeafProps);

    for (int i = 0; i < LeafProps[name].size(); i++)
    {
        LeafProps[name][i]->SetValue(true);
    }

    TableLoop(current + 1, CNT, finalProp, names, LeafProps);
}

void Parser::ProcessText(std::string propText)
{
    propText = RemoveSpaces(propText);
    propText = AddParanthesis(propText);

    std::vector<Prop*> AllProps = GetPropsFromString(propText);
    std::map<std::string, std::vector<Prop*>> LeafProps;

    //std::cout << "Found current Propositions:\n";
    std::cout << "\nValid Prop!\n\n";

    int CNT = 0;

    std::vector<std::string> names;

    for (int i = 0; i < AllProps.size(); i++)
    {
        std::string name = AllProps[i]->SetRefName(propText);

        if (AllProps[i]->isFinal())
        {
            std::map<std::string, std::vector<Prop*>>::iterator it;
            if ((it = LeafProps.find(name)) != LeafProps.end())
            {
                it->second.emplace_back(AllProps[i]);
            }
            else
            {
                LeafProps[name] = { AllProps[i] };
                names.push_back(name);
                CNT++;
            }
        }
        else
        {
            AllProps[i]->SetOperators(&propText);
        }
    }

    for (int i = 0; i < names.size(); i++)
    {
        std::cout << (CNT ? " | " : "| ") << names[i];
    }

    std::cout << " | " << "EXP |\n";

    int LEN = AllProps.size();
    TableLoop(0, CNT, AllProps[LEN - 1], names, LeafProps);
}


