////////////////////////////////////////////////////////////////////////////////
// Module Name:  stack_machine.h/cpp
// Authors:      Sergey Shershakov
// Version:      0.2.0
// Date:         23.01.2017
//
// This is a part of the course "Algorithms and Data Structures" 
// provided by  the School of Software Engineering of the Faculty 
// of Computer Science at the Higher School of Economics.
////////////////////////////////////////////////////////////////////////////////

#include "stack_machine.h"

#include <vector>
#include <sstream>
#include <iostream>
#include <stdlib.h>

namespace xi {

//==============================================================================
// Free functions -- helpers
//==============================================================================

std::vector<std::string> splitStr(const std::string &string, char separator)
{
    std::vector<std::string> temp;
    std::stringstream str;
    str.str(string);
    std::string token;
    while (std::getline(str, token, separator))
        temp.push_back(token);
    return temp;
}

bool strToInt(const std::string &s, int &i)
{
    if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+')))
        return false;
    char* p = nullptr;
    i = std::strtol(s.c_str(), &p, 10);
    return (*p == '\0');
}

int ChoiceOp::operation(char op, int a, int b, int c)
{
    if(op != '?')
        throw std::logic_error("Operation other than Choice (?) are not supported");
    if(a != 0)
        return b;
    else
        return c;

};

IOperation::Arity ChoiceOp::getArity() const
{
    return arTre;
}

int InverOp::operation(char op, int a, int /*b*/, int /*c*/)
{
    if(op != '~')
        throw std::logic_error("Operation other than Invert (~) are not supported");
    return ~a;
}

IOperation::Arity InverOp::getArity() const
{
    return arUno;
}

int AndOp::operation(char op, int a, int b, int /*c*/)
{
    if(op != '&')
        throw std::logic_error("Operation other than And (&) are not supported");
    return a & b;
}

IOperation::Arity AndOp::getArity() const
{
    return arDue;
}

//==============================================================================
// class PlusOp
//==============================================================================


int PlusOp::operation(char op, int a, int b, int /*c*/) // < just commented unused argument. This does not affect the code anyhow.
{
    if(op != '+')
        throw std::logic_error("Operation other than Plus (+) are not supported");

    // here we just ignore unused operands
    return a + b;
}

IOperation::Arity PlusOp::getArity() const
{
    return arDue;
}


//==============================================================================
// class StackMachine
//==============================================================================


void StackMachine::registerOperation(char symb, xi::IOperation* oper)
{
    SymbolToOperMap::const_iterator el = _opers.find(symb);
    if (el != _opers.end())
        throw std::logic_error("An operation with the same symbol is already registered");
    _opers[symb] = oper;
}

IOperation* StackMachine::getOperation(char symb)
{
    SymbolToOperMap::const_iterator el = _opers.find(symb);
    if (el == _opers.end()) return nullptr;
    return _opers[symb];
}

int StackMachine::calculate(const std::string &expr, bool clearStack)
{
    if (clearStack)_s.clear();

    if (expr.empty()) throw std::logic_error("Expression is empty.");

    std::vector<std::string> tokens = splitStr(expr, ' ');

    int res = 0;
    for (std::vector<std::string>::const_iterator it = tokens.begin(); it != tokens.end(); ++it)
    {
        std::string currentToken = *it;
        int num;

        if (strToInt(currentToken, num))
        {
            _s.push(num);
            continue;
        }

        if (currentToken.size() != 1) throw std::logic_error("Unknown token: " + currentToken);

        IOperation* operation = getOperation(currentToken[0]);

        if (operation == nullptr) throw std::logic_error("Undefined operation: " + currentToken);

        IOperation::Arity arity = operation->getArity();

        int a, b, c;
        switch (arity)
        {
            case IOperation::arUno:
                res = operation->operation(currentToken[0], _s.pop());
                break;
            case IOperation::arDue:
                b = _s.pop();
                a = _s.pop();
                res = operation->operation(currentToken[0], a, b);
                break;
            case IOperation::arTre:
                c = _s.pop();
                b = _s.pop();
                a = _s.pop();
                res = operation->operation(currentToken[0], a, b, c);
                break;
        }
        _s.push(res);
    }

    return res;
}

} // namespace xi
