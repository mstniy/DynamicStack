#ifndef DYNAMICSTACK_H
#define DYNAMICSTACK_H

#include <memory>
#include <stdint.h>

class Destructible
{
public:
	virtual ~Destructible(){};
};

class DynamicStack;

class DSSG
{
private:
	DynamicStack& dstack;
	Destructible** objsBackup;
	uint64_t* ptrBackup;
public:
	DSSG(DynamicStack& _dstack);
	DSSG(const DSSG&) = delete;
	DSSG(DSSG&&) = delete;
	~DSSG();

	friend class DynamicStack;
};

class DynamicStack
{
private:
	Destructible** objs;
    std::unique_ptr<uint64_t[]> buf;
    uint64_t* ptr;
public:
	DynamicStack(uint64_t size = 1024*1024/8*2);
	~DynamicStack() = default;
	DynamicStack(const DynamicStack&) = delete;
	DynamicStack(DynamicStack&&) = default;

	template<typename T, typename... Targs>
	T& alloc(Targs... args);

	template<typename T>
	T* allocArray(uint64_t n);


	friend class DSSG;
};

#include "DynamicStack.hpp"

#endif
