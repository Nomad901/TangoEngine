#pragma once
#include <iostream>

template <typename T>
class Singleton
{
public:
	static void init();
	static T& getInstance();
	static void destroy();

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

template<typename T>
inline void Singleton<T>::init()
{
	destroy();
	mInstance = new T();
}

template<typename T>
inline T& Singleton<T>::getInstance()
{
	if (mInstance != nullptr)
		return *mInstance;
}

template<typename T>
inline void Singleton<T>::destroy()
{
	delete mInstance;
	mInstance = nullptr;
}
