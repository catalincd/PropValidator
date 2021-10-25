#pragma once
#include <vector>
#include <string>
#include "Operator.h"

class Prop
{
	std::vector<Prop*> Props;
	std::vector<Operator> Operators;
	std::string m_name;
	int StartIndex;
	int EndIndex;
	int Value = -1;
public:
	Prop();
	Prop(int StartIndex);

	void SetStartIndex(int StartIndex);
	int GetStartIndex() const;

	void SetEndIndex(int EndIndex);
	int GetEndIndex() const;

	void SetValue(bool Value);
	bool GetValue() const;

	void AddProp(Prop* prop);
	bool isFinal() const;

	void SetName(std::string name);
	std::string SetRefName(std::string ref);
	std::string GetName() const;

	bool ComputeChildrenValues() const;
	void SetOperators(std::string* mainText);
};