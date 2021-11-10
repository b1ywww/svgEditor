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

void SvgWrite::writeBackGround(int canvasW, int canvasH, QRgb rgb)
{
	QDomElement background = m_doc.createElement("g");
	QDomElement title = m_doc.createElement("title");
	QDomText text;
	text = m_doc.createTextNode("background");
	background.appendChild(title);
	title.appendChild(text);

	QDomElement rect = m_doc.createElement("rect");
	rect.setAttribute("fill", QString("#%1").arg((QString::number(rgb, 16)).mid(2)));
	rect.setAttribute("id", "canvas_background");
	rect.setAttribute("height", canvasH);
	rect.setAttribute("width", canvasW);
	rect.setAttribute("y", 0);
	rect.setAttribute("x", 0);
	background.appendChild(rect);

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

bool SvgWrite::write(QList<Shape*> list, QString fileDir, int canvasW, int canvasH, QRgb rgb)
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
	writeBackGround(canvasW, canvasH, rgb);

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
	line.setAttribute("y1", shape->getPhysicalStart().y() + m_transfrom.y());
	line.setAttribute("x1", shape->getPhysicalStart().x() + m_transfrom.x());
	line.setAttribute("fill-opacity", "null");
	line.setAttribute("stroke", QString("#%1").arg((QString::number(shape->getPen().color().rgb(), 16)).mid(2)));
	line.setAttribute("fill", "none");
	line.setAttribute("type", "line");
	line.setAttribute("stroke-width", shape->getPen().widthF());

	switch (shape->getPen().style())
	{
	case Qt::SolidLine:
		line.setAttribute("stroke-dasharray", "null");
		break;
	case Qt::DotLine:
		line.setAttribute("stroke-dasharray", "2,2");
		break;
	case Qt::DashDotLine:
		line.setAttribute("stroke-dasharray", "5,2,2,2");
		break;
	case Qt::DashDotDotLine:
		line.setAttribute("stroke-dasharray", "5,2,2,2,2,2");
		break;
	default:
		break;
	}

	m_shapeG.appendChild(line);
}

void SvgWrite::writeSquare(Shape* shape)
{
	QDomElement rect = m_doc.createElement("rect");
	QPointF posStar = shape->getPhysicalStart() + m_transfrom;
	QPointF posEnd = shape->getPhysicalEnd() + m_transfrom;
	qreal width = qAbs(shape->getPhysicalEnd().x() - shape->getPhysicalStart().x());
	qreal height = qAbs(shape->getPhysicalEnd().y() - shape->getPhysicalStart().y());
	rect.setAttribute("id", "1");
	posStar.x() > posEnd.x()? rect.setAttribute("x", posEnd.x()) : rect.setAttribute("x", posStar.x());
	posStar.y() > posEnd.y() ? rect.setAttribute("y", posEnd.y()) : rect.setAttribute("y", posStar.y());
	rect.setAttribute("width", width);
	rect.setAttribute("height", height);
	rect.setAttribute("fill-opacity", "null");
	rect.setAttribute("stroke-opacity", "null");
	rect.setAttribute("stroke", QString("#%1").arg((QString::number(shape->getPen().color().rgb(), 16)).mid(2)));
	rect.setAttribute("fill", QString("#%1").arg((QString::number(shape->getBrush().color().rgb(), 16)).mid(2)));
	rect.setAttribute("type", "rect");
	rect.setAttribute("stroke-width", shape->getPen().widthF());

	switch (shape->getPen().style())
	{
	case Qt::SolidLine:
		rect.setAttribute("stroke-dasharray", "null");
		break;
	case Qt::DotLine:
		rect.setAttribute("stroke-dasharray", "2,2");
		break;
	case Qt::DashDotLine:
		rect.setAttribute("stroke-dasharray", "5,2,2,2");
		break;
	case Qt::DashDotDotLine:
		rect.setAttribute("stroke-dasharray", "5,2,2,2,2,2");
		break;
	default:
		break;
	}
	m_shapeG.appendChild(rect);
}

void SvgWrite::writePencil(Shape* shape)
{
	QDomElement pencil = m_doc.createElement("path");

	QString d = "";

	Pencil* pPencil = dynamic_cast<Pencil*>(shape);
	QList<QPointF> point = pPencil->getPhysicalPoint();
	QString M = "M%1 %2";
	QString L = ",L%1 %2";
	d = d + M.arg((*point.begin()).x() + m_transfrom.x()).arg((*point.begin()).y() + m_transfrom.y());
	for each (QPointF i in point)
	{
		d = d + L.arg(i.x() + m_transfrom.x()).arg(i.y() + m_transfrom.y());
	}

	pencil.setAttribute("d", d);
	pencil.setAttribute("fill-opacity", "null");
	pencil.setAttribute("stroke-opacity", "null");
	pencil.setAttribute("stroke", QString("#%1").arg((QString::number(shape->getPen().color().rgb(), 16)).mid(2)));
	pencil.setAttribute("fill", "none");
	pencil.setAttribute("type", "pencil");
	pencil.setAttribute("stroke-width", shape->getPen().widthF());

	switch (shape->getPen().style())
	{
	case Qt::SolidLine:
		pencil.setAttribute("stroke-dasharray", "null");
		break;
	case Qt::DotLine:
		pencil.setAttribute("stroke-dasharray", "2,2");
		break;
	case Qt::DashDotLine:
		pencil.setAttribute("stroke-dasharray", "5,2,2,2");
		break;
	case Qt::DashDotDotLine:
		pencil.setAttribute("stroke-dasharray", "5,2,2,2,2,2");
		break;
	default:
		break;
	}

	m_shapeG.appendChild(pencil);
}

void SvgWrite::writeCircle(Shape* shape)
{
	QDomElement circle = m_doc.createElement("ellipse");
	QPointF pos = shape->getPhysicalStart() + m_transfrom;
	qreal width = shape->getPhysicalEnd().x() - shape->getPhysicalStart().x();
	qreal height = shape->getPhysicalEnd().y() - shape->getPhysicalStart().y();
	circle.setAttribute("id", "1");
	circle.setAttribute("cx", pos.x() + width / 2);
	circle.setAttribute("cy", pos.y() + height / 2);
	circle.setAttribute("rx", qAbs(width / 2));
	circle.setAttribute("ry", qAbs(height / 2));
	circle.setAttribute("fill-opacity", "null");
	circle.setAttribute("stroke-opacity", "null");
	circle.setAttribute("stroke", QString("#%1").arg((QString::number(shape->getPen().color().rgb(), 16)).mid(2)));
	circle.setAttribute("fill", QString("#%1").arg((QString::number(shape->getBrush().color().rgb(), 16)).mid(2)));
	circle.setAttribute("type", "circle");

	circle.setAttribute("stroke-width", shape->getPen().widthF());

	switch (shape->getPen().style())
	{
	case Qt::SolidLine:
		circle.setAttribute("stroke-dasharray", "null");
		break;
	case Qt::DotLine:
		circle.setAttribute("stroke-dasharray", "2,2");
		break;
	case Qt::DashDotLine:
		circle.setAttribute("stroke-dasharray", "5,2,2,2");
		break;
	case Qt::DashDotDotLine:
		circle.setAttribute("stroke-dasharray", "5,2,2,2,2,2");
		break;
	default:
		break;
	}

	m_shapeG.appendChild(circle);
}

void SvgWrite::writeHexagon(Shape* shape)
{
	QDomElement hexagon = m_doc.createElement("path");

	QString d = "M%1 %2,L%3 %4,L%5 %6,L%7 %8,L%9 %10,L%11 %12,L%13 %14";

	Hexagon* pHexagon = dynamic_cast<Hexagon*>(shape);
	if (nullptr == pHexagon)
		return;
	for each (QPointF i in pHexagon->getVertex())
	{
		d = d.arg(i.x() + m_transfrom.x()).arg(i.y() + m_transfrom.y());
	}

	hexagon.setAttribute("d", d);
	hexagon.setAttribute("fill-opacity", "null");
	hexagon.setAttribute("stroke-opacity", "null");
	hexagon.setAttribute("stroke", QString("#%1").arg((QString::number(shape->getPen().color().rgb(), 16)).mid(2)));
	hexagon.setAttribute("fill", QString("#%1").arg((QString::number(shape->getBrush().color().rgb(), 16)).mid(2)));
	hexagon.setAttribute("type", "hexagon");
	hexagon.setAttribute("stroke-width", shape->getPen().widthF());

	switch (shape->getPen().style())
	{
	case Qt::SolidLine:
		hexagon.setAttribute("stroke-dasharray", "null");
		break;
	case Qt::DotLine:
		hexagon.setAttribute("stroke-dasharray", "2,2");
		break;
	case Qt::DashDotLine:
		hexagon.setAttribute("stroke-dasharray", "5,2,2,2");
		break;
	case Qt::DashDotDotLine:
		hexagon.setAttribute("stroke-dasharray", "5,2,2,2,2,2");
		break;
	default:
		break;
	}

	m_shapeG.appendChild(hexagon);
}

void SvgWrite::writeText(Shape* shape)
{
	QDomElement text = m_doc.createElement("text");

	text.setAttribute("white-space", "preserve");
	text.setAttribute("text-anchor", "start");
	text.setAttribute("font-family", "Microsoft YaHei");
	text.setAttribute("font-size", "30");
	text.setAttribute("x", shape->getPhysicalStart().x() + m_transfrom.x());
	text.setAttribute("y", shape->getPhysicalEnd().y() + m_transfrom.y());
	text.setAttribute("fill-opacity", "null");
	text.setAttribute("stroke-opacity", "null");
	text.setAttribute("stroke", "#000");
	text.setAttribute("fill", "#000000");
	text.setAttribute("xml:space", "preserve");
	text.setAttribute("type", "text");

	QMatrix matrix = dynamic_cast<TextEdit*>(shape)->getMatrix();
	qreal m0, m3, m4, m5;
	m0 = matrix.m11();
	m3 = matrix.m22();
	m4 = matrix.dx() * (shape->getPhysicalStart().x() + m_transfrom.x());
	m5 = matrix.dy() * (shape->getPhysicalEnd().y() + m_transfrom.y());
	text.setAttribute("transform", QString("matrix(%1,0,0,%2,%3,%4)").arg(m0).arg(m3).arg(m4).arg(m5));

	switch (shape->getPen().style())
	{
	case Qt::SolidLine:
		text.setAttribute("stroke-dasharray", "null");
		break;
	case Qt::DotLine:
		text.setAttribute("stroke-dasharray", "2,2");
		break;
	case Qt::DashDotLine:
		text.setAttribute("stroke-dasharray", "5,2,2,2");
		break;
	case Qt::DashDotDotLine:
		text.setAttribute("stroke-dasharray", "5,2,2,2,2,2");
		break;
	default:
		break;
	}

	TextEdit* pTextEdit = dynamic_cast<TextEdit*>(shape);
	if (nullptr == pTextEdit)
		return;
	QDomNode node = m_doc.createTextNode(pTextEdit->getText());
	text.appendChild(node);
	m_shapeG.appendChild(text);
}