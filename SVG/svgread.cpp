#include "svgread.h"

SvgRead* SvgRead::svgRead()
{
	static SvgRead* pSvgRead = nullptr;
	if (nullptr == pSvgRead)
		pSvgRead = new SvgRead();

	return pSvgRead;
}

SvgRead::SvgRead()
{

}

SvgRead::~SvgRead()
{

}

