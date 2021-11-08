#pragma once
#include "shape.h"

#include <QUndoCommand>

class KxSvgCanvas;
enum class mousePosition;
class SvgCommand : public QUndoCommand
{
public:
	SvgCommand(KxSvgCanvas* canvas);
	~SvgCommand();
	void update();
	const qreal getRadio();
	void updatePhysicalPoint(QList<Shape*> list);
	void shapeListRemoveOne(Shape* item);
	void clickListRemoveOne(Shape* item);

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

class AddCommand : public SvgCommand
{
public:
	AddCommand(KxSvgCanvas* canvas, QList<Shape*> items);
	AddCommand(KxSvgCanvas* canvas, Shape* items);
	~AddCommand();

	void undo() override;
	void redo() override;

private:
	QList<Shape*> m_items;
};

class DeleteCommand : public SvgCommand
{
public:
	DeleteCommand(KxSvgCanvas* canvas, QList<Shape*> item);
	~DeleteCommand();

	void undo() override;
	void redo() override;

private:
	QList<Shape*> m_items;
};