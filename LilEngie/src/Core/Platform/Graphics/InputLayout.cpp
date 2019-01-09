#include <string>
#include <Core/Common.h>
#include "InputLayout.h"

namespace LilEngie
{
	uint InputElement::GetSize()
	{
		switch (format)
		{
			case FLOAT_R32G32B32A32:
				return sizeof(float) * 4;
				break;
			case FLOAT_R32G32B32:
				return sizeof(float) * 3;
				break;
			case FLOAT_R32G32:
				return sizeof(float) * 2;
				break;
			case FLOAT_R32:
				return sizeof(float);
				break;
			case UINT_R32:
				return sizeof(uint);
				break;
			default:
				return 0;
				break;
		}
	}
}
