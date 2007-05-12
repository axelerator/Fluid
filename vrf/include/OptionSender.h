#ifndef __OPTIONSENDER_H__
#define __OPTIONSENDER_H__

#include <vector>
#include "UDPDataSets.h"
//#include "UDPReceiver.h"

class UDPReceiver;

class OptionSender
{
// Operationen
public:
	OptionSender();
	virtual ~OptionSender();

	void SetNewOption(DATASET dataset, int w, int h, void* targetData);

private:
	unsigned int deleteFromVector(DATASET dataset);

// Attribute
private:
	SOCKET sendSok;
	SOCKET recvSok;
	std::vector<UDPReceiver*> receiver;
};

#endif
