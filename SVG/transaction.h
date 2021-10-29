#pragma once
#include "shape.h"

#include <QUndoCommand>

class KxSvgCanvas;
class SvgCommand : public QUndoCommand
{
public:
	SvgCommand(KxSvgCanvas* canvas);
	~SvgCommand();
	void update();

private:
	static KxSvgCanvas* m_svgCanvas;
};

class MoveCommand : public SvgCommand
{
public:
	MoveCommand(KxSvgCanvas* canvas, QList<Shape*> item, QPointF offset);
	~MoveCommand();
	void undo() override;
	void redo() override;

private:
	QList<Shape*> m_items;
	QPointF m_offset;
};