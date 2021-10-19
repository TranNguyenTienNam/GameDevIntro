#include "ServiceLocator.h"

void CServiceLocator::AddService(CService* service)
{	
	services.push_back(service);
}