#include "transaction.h"
#include "kxcanvas.h"

#include <QDebug>

KxSvgCanvas* SvgCommand::m_svgCanvas = nullptr;

SvgCommand::SvgCommand(KxSvgCanvas* canvas)
{
	m_svgCanvas = canvas;
}

SvgCommand::~SvgCommand()
{

}

void SvgCommand::update()
{
	if (m_svgCanvas)
		m_svgCanvas->update();
}

MoveCommand::MoveCommand(KxSvgCanvas* canvas, QList<Shape*> items, QPointF offset, mousePosition type)
	:SvgCommand(canvas), m_items(items), m_offset(offset), m_moveType(type)
{
	for(auto i : m_items)
		i->move(-m_offset);
}

MoveCommand::~MoveCommand()
{
}

void MoveCommand::undo()
{
	for (auto i : m_items)
		i->move(-m_offset);
	update();
}

void MoveCommand::redo()
{
	for (auto i : m_items)
		i->move(m_offset);
	update();
}

void MoveCommand::editShape(QPointF offset)
{
	for (auto i : m_items)
	{
		switch (m_moveType)
		{
		case mousePosition::move:
		{
			i->move(offset);
			break;
		}
		case mousePosition::noClick:
		{
			i->move(offset);
			break;
		}
		case mousePosition::top:
		{
			i->moveTop(QPoint(0, offset.y()));
			break;
		}
		case mousePosition::left:
		{
			i->moveLeft(QPoint(offset.x(), 0));
			break;
		}
		case mousePosition::right:
		{
			i->moveRight(QPoint(offset.x(), 0));
			break;
		}
		case mousePosition::bottom:
		{
			i->moveBottom(QPoint(0, offset.y()));
			break;
		}
		case mousePosition::upperLeft:
		{
			i->moveUpperLeft(offset);
			break;
		}
		case mousePosition::lowerLeft:
		{
			i->moveLowerLeft(offset);
			break;
		}
		case mousePosition::upperRight:
		{
			i->moveUpperRight(offset);
			break;
		}
		case mousePosition::lowerRight:
		{
			i->moveLowerRight(offset);
			break;
		}
		default:
			break;
		}
	}
}

//MoveTopCommand::MoveTopCommand(KxSvgCanvas* canvas, Shape* item, QPointF offset)
//	:SvgCommand(canvas), m_item(item), m_offset(offset)
//{
//
//}
//
//MoveTopCommand::~MoveTopCommand()
//{
//
//}
//
//void MoveTopCommand::undo()
//{
//
//}
//
//void MoveTopCommand::redo()
//{
//
//}
