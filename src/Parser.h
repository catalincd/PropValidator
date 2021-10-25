#pragma once
#include <sstream>
#include <regex>

#include "Prop.h"

class Parser
{
public:
    static void ProcessText(std::string propText);

    static std::string RemoveSpaces(std::string propText);
    static std::string InsertStringAt(std::string& text, int index, std::string toInsert);
    static std::string AddParanthesis(std::string propText);
    static std::vector<Prop*> GetPropsFromString(std::string propText);
};