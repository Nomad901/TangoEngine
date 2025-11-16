#pragma once
#include <iostream>

template <typename T>
class Singleton
{
public:
	static void init()
	{
		destroy();
		mInstance = new T();
	}
	static T& getInstance()
	{
		return mInstance;
	}
	static void destroy()
	{
		delete mInstance;
		mInstance = nullptr;
	}

	Singleton() = default;
	virtual ~Singleton() = default;

	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;

	Singleton(Singleton&&) = delete;
	Singleton& operator=(Singleton&&) = delete;

private:
	static T* mInstance;
};

template <typename T>
T* Singleton<T>::mInstance = nullptr;

