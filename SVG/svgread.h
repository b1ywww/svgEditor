#pragma once
#include <QXmlStreamReader>
#include <QFile>

class SvgRead
{
public:
	static SvgRead* svgRead();

	//void xmlread()
	//{
	//	QFile file("hello.xml");
	//	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	//	{
	//		//����QXmlStreamReader����
	//		QXmlStreamReader reader(&file);
	//		while (!reader.atEnd())
	//		{
	//			//�ж��Ƿ��ǽڵ�Ŀ�ʼ
	//			if (reader.isStartElement())
	//			{
	//				//�����Զ���:���� id="3" age = "23">
	//				QXmlStreamAttributes attributes = reader.attributes();
	//				//�ж��Ƿ��������"id"
	//				if (attributes.hasAttribute("id"))
	//				{
	//					//������id׷��textBrower
	//					qDebug() << QString("id:%1").arg(attributes.value("id").toString());
	//				}
	//				//�жϵ�ǰ�ڵ�������Ƿ�ΪNumber
	//				if (reader.name() == "Number")
	//					qDebug() << QString("Number:%1").arg(reader.readElementText());
	//				//�жϵ�ǰ�ڵ�������Ƿ�ΪName
	//				else if (reader.name() == "Name")
	//					qDebug() << QString("Name:%1").arg(reader.readElementText());
	//				//�жϵ�ǰ�ڵ�������Ƿ�Ϊemail
	//				else if (reader.name() == "email")
	//					qDebug() << QString("email:%1").arg(reader.readElementText());
	//				//�жϵ�ǰ�ڵ�������Ƿ�Ϊwebsite
	//				else if (reader.name() == "website")
	//					qDebug() << QString("website:%1").arg(reader.readElementText());
	//			}

	//			//�ڵ���������ҽڵ�����ΪData�������ӽڵ㣩

	//			else if (reader.isEndElement() && reader.name() == "Data")

	//			{
	//				qDebug() << "--------------------";
	//				qDebug() << "";
	//			}
	//			reader.readNext();
	//		}

	//		file.close();

	//	}

	//	else

	//	{
	//		qDebug() << "Open file hello.xml failure";
	//	}
	//}

	//void readLine();

	//void readSquare();

	//void readPencil();

	//void readCircle();

	//void readHexagon();

	//void writeText();

private:
	SvgRead();
	~SvgRead();
};