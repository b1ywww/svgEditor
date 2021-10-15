#include "svgwrite.h"
#include <QDir>
#include "Shape.h"
SvgWrite* SvgWrite::svgWrite()
{
	static SvgWrite* pSvgWrite = nullptr;

	if (nullptr == pSvgWrite)
		pSvgWrite = new SvgWrite();

	return pSvgWrite;
}

bool SvgWrite::setFile(QString fileDir)
{
	QDir dir(fileDir);
	int count = 1;
	QString searchText = ".svg";
	QString replaceText = "(%1).svg";
	//文件如果存在就重命名
	while (dir.exists(fileDir))
	{
		fileDir = fileDir.replace(searchText, replaceText.arg(count));
		count++;
	}

	m_file.setFileName(fileDir);

	if (!m_file.open(QFile::WriteOnly | QFile::Truncate)) //可以用QIODevice，Truncate表示清空原来的内容
		return false;

	return true;
}

void SvgWrite::writeHead()
{
	QDomProcessingInstruction instruction; //添加处理命令
	instruction = m_doc.createProcessingInstruction("xml", "version=\"1.0\" standalone=\"no\"");
	m_doc.appendChild(instruction);
}

void SvgWrite::writeBackGround(int canvasW, int canvasH)
{
	QDomElement background = m_doc.createElement("g");
	QDomElement title = m_doc.createElement("title");
	QDomText text;
	text = m_doc.createTextNode("background");
	background.appendChild(title);
	title.appendChild(text);

	QDomElement rect = m_doc.createElement("rect");
	rect.setAttribute("fill", "#ffffff");
	rect.setAttribute("id", "canvas_background");
	rect.setAttribute("height", 502);
	rect.setAttribute("width", 502);
	rect.setAttribute("y", -1);
	rect.setAttribute("x", -1);
	background.appendChild(rect);

	QDomElement g = m_doc.createElement("g");
	g.setAttribute("dispaly", "none");
	g.setAttribute("overflow", "visible");
	g.setAttribute("y", 0);
	g.setAttribute("x", 0);
	g.setAttribute("height", "100%");
	g.setAttribute("width", "100%");
	g.setAttribute("id", "canvasGrid");

	QDomElement rect_1 = m_doc.createElement("rect");
	rect_1.setAttribute("fill", "url(#gridpattern)");
	rect_1.setAttribute("stroke-width", 0);
	rect_1.setAttribute("y", 0);
	rect_1.setAttribute("x", 0);
	rect_1.setAttribute("height", "100%");
	rect_1.setAttribute("width", "100%");

	g.appendChild(rect_1);
	background.appendChild(g);
	m_svg.appendChild(background);
}

void SvgWrite::writeShape(QList<Shape*> list)
{
	ShapeType shape;
	for each (Shape * i in list)
	{
		shape = i->getShapeType();
		switch (shape)
		{
		case ShapeType::TypeLine:
			writeLine(i);
			break;
		case ShapeType::TypeSquare:
			writeSquare(i);
			break;
		case ShapeType::TypePencil:
			writePencil(i);
			break;
		case ShapeType::TypeCircle:
			writeCircle(i);
			break;
		case ShapeType::TypeHexagon:
			writeHexagon(i);
			break;
		case ShapeType::TypeText:
			writeText(i);
			break;
		default:
			break;
		}
	}
}

bool SvgWrite::write(QList<Shape*> list, QString fileDir, int canvasW, int canvasH)
{
	m_transfrom = QPointF(canvasW / 2, canvasH / 2);

	if(false == setFile(fileDir))
		return false;

	m_svg = m_doc.createElement("svg");
	m_svg.setAttribute("width", canvasW);
	m_svg.setAttribute("height", canvasH);
	m_svg.setAttribute("xmlns", "http://www.w3.org/2000/svg");
	m_svg.setAttribute("type", "wps");

	writeHead();
	writeBackGround(canvasW, canvasH);

	m_shapeG = m_doc.createElement("g");

	writeShape(list);

	m_svg.appendChild(m_shapeG);
	m_doc.appendChild(m_svg);
	QTextStream out_stream(&m_file);
	m_doc.save(out_stream, 4); //缩进4格
	m_file.close();

	m_doc.clear();
	return true;
}

void SvgWrite::writeLine(Shape* shape)
{
	QDomElement line = m_doc.createElement("line");
	line.setAttribute("stroke-linecap", "null");
	line.setAttribute("stroke-linejoin", "null");
	line.setAttribute("id", 1);
	line.setAttribute("y2", shape->getPhysicalEnd().y() + m_transfrom.y());
	line.setAttribute("x2", shape->getPhysicalEnd().x() + m_transfrom.x());
	line.setAttribute("y1", shape->getPhysicalStar().y() + m_transfrom.y());
	line.setAttribute("x1", shape->getPhysicalStar().x() + m_transfrom.x());
	line.setAttribute("fill-opacity", "null");
	line.setAttribute("stroke-width", 1.5);
	line.setAttribute("stroke", "#000");
	line.setAttribute("fill", "none");

	m_shapeG.appendChild(line);
}

void SvgWrite::writeSquare(Shape* shape)
{
	QDomElement rect = m_doc.createElement("rect");
	QPointF pos = shape->getPhysicalStar() + m_transfrom;
	qreal width = qAbs(shape->getPhysicalEnd().x() - shape->getPhysicalStar().x());
	qreal height = qAbs(shape->getPhysicalEnd().y() - shape->getPhysicalStar().y());
	rect.setAttribute("id", "1");
	rect.setAttribute("x", pos.x());
	rect.setAttribute("y", pos.y());
	rect.setAttribute("width", width);
	rect.setAttribute("height", height);
	rect.setAttribute("fill-opacity", "null");
	rect.setAttribute("stroke-opacity", "null");
	rect.setAttribute("stroke-width", "1.5");
	rect.setAttribute("stroke", "#000");
	rect.setAttribute("fill", "#fff");

	m_shapeG.appendChild(rect);
}

void SvgWrite::writePencil(Shape* shape)
{
	QDomElement pencil = m_doc.createElement("path");

	QString d = "";

	QList<QPointF> point = dynamic_cast<Pancil*>(shape)->getPhysicalPoint();
	QString M = "M%1 %2";
	QString L = "L%1 %2";
	d = d + M.arg((*point.begin()).x() + m_transfrom.x()).arg((*point.begin()).y() + m_transfrom.y());
	for each (QPointF i in point)
	{
		d = d + L.arg(i.x() + m_transfrom.x()).arg(i.y() + m_transfrom.y());
	}

	qDebug() << d;

	pencil.setAttribute("d", d);
	pencil.setAttribute("fill-opacity", "null");
	pencil.setAttribute("stroke-opacity", "null");
	pencil.setAttribute("stroke-width", "1.5");
	pencil.setAttribute("stroke", "#000");
	pencil.setAttribute("fill", "none");

	m_shapeG.appendChild(pencil);
}

void SvgWrite::writeCircle(Shape* shape)
{
	QDomElement circle = m_doc.createElement("ellipse");
	QPointF pos = shape->getPhysicalStar() + m_transfrom;
	qreal width = shape->getPhysicalEnd().x() - shape->getPhysicalStar().x();
	qreal height = shape->getPhysicalEnd().y() - shape->getPhysicalStar().y();
	circle.setAttribute("id", "1");
	circle.setAttribute("cx", pos.x() + width / 2);
	circle.setAttribute("cy", pos.y() + height / 2);
	circle.setAttribute("rx", qAbs(width / 2));
	circle.setAttribute("ry", qAbs(height / 2));
	circle.setAttribute("fill-opacity", "null");
	circle.setAttribute("stroke-opacity", "null");
	circle.setAttribute("stroke-width", "1.5");
	circle.setAttribute("stroke", "#000");
	circle.setAttribute("fill", "#fff");

	m_shapeG.appendChild(circle);
}

void SvgWrite::writeHexagon(Shape* shape)
{
	QDomElement hexagon = m_doc.createElement("path");

	QString d = "M%1 %2, L%3 %4, L%5 %6, L%7 %8, L%9 %10, L%11 %12, L%13 %14";

	for each (QPointF i in dynamic_cast<Hexagon*>(shape)->getVertex())
	{
		d = d.arg(i.x() + m_transfrom.x()).arg(i.y() + m_transfrom.y());
	}

	hexagon.setAttribute("d", d);
	hexagon.setAttribute("fill-opacity", "null");
	hexagon.setAttribute("stroke-opacity", "null");
	hexagon.setAttribute("stroke-width", "1.5");
	hexagon.setAttribute("stroke", "#000");
	hexagon.setAttribute("fill", "#fff");

	m_shapeG.appendChild(hexagon);
}

void SvgWrite::writeText(Shape* shape)
{

}