#pragma once
#include <QXmlStreamReader>
#include <QFile>

class SvgRead
{
public:
	static SvgRead* svgRead();

	void xmlread()
	{
		QFile file("E:\test1.svg");
		if (file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			QXmlStreamReader reader(&file);

		}
	}

private:
	SvgRead();
	~SvgRead();
};