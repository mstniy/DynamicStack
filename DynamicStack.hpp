#include <assert.h>
#include <iostream>

DSSG::DSSG(DynamicStack& _dstack):
	dstack(_dstack)
{
	objsBackup = dstack.objs;
	ptrBackup = dstack.ptr;
}

DSSG::~DSSG()
{
	for (; dstack.objs < objsBackup; dstack.objs++)
		(*dstack.objs)->~Destructible();
	dstack.ptr = ptrBackup;
}

DynamicStack::DynamicStack(uint64_t size):
	buf(new uint64_t[size])
{
	ptr = (uint8_t*)buf.get();
	objs = (Destructible**)(buf.get()+size);
}

template<typename T, typename... Targs>
T& DynamicStack::alloc(Targs... args)
{
	static_assert(std::is_base_of<Destructible, T>::value || std::is_trivially_destructible<T>::value,
		"Values allocated from DynamicStack must either inherit from Destructible or be trivially destructible.");
	uint64_t rounded = sizeof(T);
	if (rounded%8)
		rounded += 8 - rounded%8;
    T* res = reinterpret_cast<T*>(ptr);
    new (res) T(args...);
    ptr += rounded;
    if (std::is_base_of<Destructible, T>::value)
	    *(--objs) = (Destructible*)(res);
    return *res;
}

template<typename T>
T* DynamicStack::allocArray(uint64_t n)
{
	static_assert(std::is_base_of<Destructible, T>::value || std::is_trivially_destructible<T>::value,
		"Values allocated from DynamicStack must either inherit from Destructible or be trivially destructible.");
	uint64_t rounded = sizeof(T);
	if (rounded%8)
		rounded += 8 - rounded%8;
    T* res = reinterpret_cast<T*>(ptr);
    ptr += rounded*n;
    for (uint64_t i=0;i<n;i++)
	{
    	new (res+i) T();
    	if (std::is_base_of<Destructible, T>::value)
    		*(--objs) = (Destructible*)(res+i);
	}
    return res;
}
