#include "Prop.h"
#include <iostream>

Prop::Prop()
{

}

Prop::Prop(int StartIndex)
{
	SetStartIndex(StartIndex);
}

void Prop::SetStartIndex(int StartIndex)
{
	this->StartIndex = StartIndex;
}

int Prop::GetStartIndex() const
{
	return StartIndex;
}

void Prop::SetEndIndex(int EndIndex)
{
	this->EndIndex = EndIndex;
}

int Prop::GetEndIndex() const
{
	return EndIndex;
}

void Prop::SetValue(bool Value)
{
	this->Value = Value;
}

bool Prop::ComputeChildrenValues() const
{
    bool current = true;

    std::cout << "";

    for (int i = 0; i < Operators.size(); i++)
    {
        current = ComputeValue(current, Props[i]->GetValue(), Operators[i]);
    }

    return current;
}

bool Prop::GetValue() const
{
	return ((isFinal()) ? Value : ComputeChildrenValues());
}

void Prop::AddProp(Prop* prop)
{
	Props.emplace_back(prop);
}

bool Prop::isFinal() const
{
	return (!(Props.size()));
}

void Prop::SetName(std::string name)
{
	m_name = name;
}

std::string Prop::SetRefName(std::string ref)
{
	std::string now = ref.substr(StartIndex, EndIndex - StartIndex);
	SetName(now);
	return now;
}

std::string Prop::GetName() const
{
	return m_name;
}

void Prop::SetOperators(std::string* mainText)
{
    std::string CurrentOperator = "";
    int lastProp = 0;

    for (int i = StartIndex; i < EndIndex; i++)
    {
        if (i >= Props[lastProp]->GetStartIndex())
        {
            Operator op = StringToOperator(CurrentOperator.substr(0, CurrentOperator.length() - 1));
            if (op != UNDEF)
            {
                Operators.emplace_back(op);
                //std::cout << "Recognized Operator '" << CurrentOperator << "' in prop: " << mainText->substr(StartIndex, EndIndex - StartIndex) << "\n";
            }
            else
            {
                std::cout << "Unrecognized Operator '" << CurrentOperator << "' in prop: " << mainText->substr(StartIndex, EndIndex - StartIndex) << "\n";
                exit(-1);
            }

            CurrentOperator = "";
            i = Props[lastProp]->GetEndIndex();
            lastProp++;
            continue;
        }
        else
            CurrentOperator += (*mainText)[i];
    }
}
