////////////////////////////////////////////////////////////////////////////////
// Module Name:  int_stack.h/cpp
// Authors:      Sergey Shershakov
// Version:      0.2.0
// Date:         23.01.2017
//
// This is a part of the course "Algorithms and Data Structures" 
// provided by  the School of Software Engineering of the Faculty 
// of Computer Science at the Higher School of Economics.
////////////////////////////////////////////////////////////////////////////////


#include "int_stack.h"
#include <sstream>
#include <iostream>
#include <stdlib.h>

namespace xi {

IntStack::IntStack(size_t sz)
{
    if (sz <= STACK_SIZE) {
        _ssize = sz;
        _stack = new int[_ssize];
        _head = 0;
    }
    else
        throw std::logic_error("");
}

IntStack::~IntStack()
{
    delete[] _stack;
    _ssize = 0;
    _head = 0;
}

void IntStack::clear()
{
    _head = 0;
    _stack = new int[_ssize];
}

void IntStack::push(int el)
{
    if(_head + 1 > _ssize)
        throw std::logic_error("");
    else {
        _stack[_head] = el;
        _head++;
    }
}

int IntStack::pop()
{
    if (_head == 0)
        throw std::logic_error("");
    else {
        return _stack[--_head];
    }
}

int IntStack::top()
{
    if (_head == 0)
        throw std::logic_error("");
    else {
        return _stack[_head-1];
    }
}

bool IntStack::isFull() const
{
    if(_head == _ssize)
        return true;
    else
        return false;
}

bool IntStack::isEmpty() const
{
    if(_head == 0)
        return true;
    else
        return false;
}

} // namespace xi

