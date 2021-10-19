#pragma once
#include <vector>
#include "Service.h"

class CServiceLocator
{
private:
	std::vector<CService*> services;
public:
	void AddService(CService*);
	template <typename T> T* GetService();
};

template<typename T>
inline T* CServiceLocator::GetService()
{
	for (auto service : services)
	{
		T* result = dynamic_cast<T*>(service);
		if (result) return result;
	}
	return nullptr;
}