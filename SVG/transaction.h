#pragma once
#include "shape.h"

#include <QUndoCommand>

class KxSvgCanvas;
extern enum class mousePosition;
class SvgCommand : public QUndoCommand
{
public:
	SvgCommand(KxSvgCanvas* canvas);
	~SvgCommand();
	void update();
	const qreal getRadio();
	void updatePhysicalPoint(QList<Shape*> list);

private:
	static KxSvgCanvas* m_svgCanvas;
};

class MoveCommand : public SvgCommand
{
public:
	MoveCommand(KxSvgCanvas* canvas, QList<Shape*> item, QPointF offset, mousePosition type);
	~MoveCommand();

	void undo() override;
	void redo() override;

private:
	void editShape(QPointF offset);

private:
	QList<Shape*> m_items;
	QPointF m_offset;
	mousePosition m_moveType;
};