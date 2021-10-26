#include "canvastool.h"

bool tool::ponitInRect(QPointF point, Shape* shape)
{
	if (shape == nullptr)
		return nullptr;

	qreal x = shape->getDrawEnd().x() - shape->getDrawStart().x() + 0.1; /*加0.1是为了防止出现 分母为0的无法选中的情况，因为点是为整数的，所以加上0.1 不影响x的符号*/
	qreal y = shape->getDrawEnd().y() - shape->getDrawStart().y() + 0.1; /*上面是第一版的注释后来把坐标改成了QPointF,理应是会有bug的但是没测出来就放这了*/

	qreal point_x = point.x() - shape->getDrawStart().x();
	qreal point_y = point.y() - shape->getDrawStart().y();

	if (point_x / x <= 1 &&
		point_x / x >= 0 &&
		point_y / y <= 1 &&
		point_y / y >= 0)
	{
		return true;
	}
	return false;
}

bool tool::isShapeIntersect(Shape* rect_1, Shape* rect_2)
{
	qreal width_1 = qAbs(rect_1->getDrawEnd().x() - rect_1->getDrawStart().x());
	qreal height_1 = qAbs(rect_1->getDrawEnd().y() - rect_1->getDrawStart().y());

	qreal width_2 = qAbs(rect_2->getDrawEnd().x() - rect_2->getDrawStart().x());
	qreal height_2 = qAbs(rect_2->getDrawEnd().y() - rect_2->getDrawStart().y());

	qreal minX = rect_1->getDrawStart().x();
	qreal minY = rect_1->getDrawStart().y();
	qreal maxX = rect_1->getDrawStart().x();
	qreal maxY = rect_1->getDrawStart().y();

	minX = qMin((qMin(qMin(rect_1->getDrawEnd().x(), rect_2->getDrawStart().x()), rect_2->getDrawEnd().x())), minX);
	minY = qMin((qMin(qMin(rect_1->getDrawEnd().y(), rect_2->getDrawStart().y()), rect_2->getDrawEnd().y())), minY);
	maxX = qMax((qMax(qMax(rect_1->getDrawEnd().x(), rect_2->getDrawStart().x()), rect_2->getDrawEnd().x())), maxX);
	maxY = qMax((qMax(qMax(rect_1->getDrawEnd().y(), rect_2->getDrawStart().y()), rect_2->getDrawEnd().y())), maxY);

	if (maxX - minX <= width_1 + width_2 && maxY - minY <= height_1 + height_2)
	{
		return true;
	}

	return false;
}