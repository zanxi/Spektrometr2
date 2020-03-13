#pragma once

#include ".\stdwin.h"
#include ".\Port.h"
#include ".\DataStruct.h"

namespace TimerWorkSystem
{
	Port::ComPort rs;
	void Add(double x)
	{
		Data::DataRS[x]=Port::ComPort::getValue(x);
	}

	void OnTimer(double x)
	{
		Add(x);
	}
}

