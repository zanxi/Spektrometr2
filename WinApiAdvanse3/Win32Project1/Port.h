#pragma once
#include ".\stdwin.h"

namespace Port 
{
	class ComPort 
	{
	public:
		static double getValue(double x)
		{
			return sin(x)+2*cos(x);
		}

	};

}
