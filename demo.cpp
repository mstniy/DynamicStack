#include <iostream>

#include "DynamicStack.h"

using namespace std;

DynamicStack stack;

class Base : public Destructible
{
public:
	virtual ~Base(){cout << "\t~Base" << endl;}
};

class Der1 : public Base
{
public:
	int64_t x;
public:
	Der1(int64_t _x=0):x(_x){}
	~Der1() override {cout << "Der1::~Des()" << ' ' << x;}
};

class Der2 : public Base
{
public:
	~Der2() override {cout << "Der2::~Des() -";}
};

Base* getObj(bool b)
{
	Base* res;
	if (b)
		res = &stack.alloc<Der1>(2);
	else
		res = &stack.alloc<Der2>();
	return res;
}

int main()
{
	DSSG guard(stack);
	Base* obj1 = getObj(true);
	Base* obj2 = getObj(false);
	if (true)
	{
		DSSG guard(stack);
		Der1& der1 = stack.alloc<Der1>(1);
	}
	if (true)
	{
		DSSG guard(stack);
		Der2& der2 = stack.alloc<Der2>();
	}
	stack.alloc<int>();
	stack.allocArray<Der1>(3);
	return 0;
}
