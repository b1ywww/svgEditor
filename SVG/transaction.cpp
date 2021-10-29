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

MoveCommand::MoveCommand(KxSvgCanvas* canvas, QList<Shape*> items, QPointF offset)
	:SvgCommand(canvas), m_items(items), m_offset(offset)
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

