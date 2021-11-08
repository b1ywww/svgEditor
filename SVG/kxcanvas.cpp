#include "kxcanvas.h"
#include "svgwrite.h"
#include "svgread.h"
#include "canvastool.h"

#include "QPainter"
#include "QDebug"
#include <QTextCursor>
#include <cstdlib>
#include <QMessageBox>
#include <QFileDialog>
#include <QApplication>
#include <QColorDialog>

const int POSITION_DEFAULT = 0;
const int POSITION_LEFT = 1;
const int POSITION_TOP = 2;
const int POSITION_UPPER_LEFT = 3;
const int POSITION_RIGHT = 4;
const int POSITION_UPPER_RIGHT = 6;
const int POSITION_BOTTOM = 8;
const int POSITION_LOWER_LETF = 9;
const int POSITION_LOWER_RIGHT = 12;

KxSvgCanvas::KxSvgCanvas(QWidget* parent)
	:QWidget(parent)
{
	m_currentType = ShapeType::TypeSelect;
	setMouseTracking(true);
	setObjectName(QStringLiteral("svgCanvas"));
	setAttribute(Qt::WA_StyledBackground, true);
	setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
	setFocusPolicy(Qt::ClickFocus);
	setCanvasSize();

	installEventFilter(this);

	//初始化右键菜单
	setRightClickMenu();

	m_pClickRect = ShapeFactory::getShapeFactory()->getShape(ShapeType::TypeSquare);
	QColor color(255, 255, 255);
	color.setAlpha(0);
	m_pClickRect->getBrush().setColor(color);

	m_rgb = color.rgb();

	m_pTextEditWidget = new QLineEdit(this);
	m_pTextEditWidget->resize(10, 40);
	m_pTextEditWidget->setFont(QFont("Microsoft YaHei", 30));
	m_pTextEditWidget->setStyleSheet("border-style: outset;border-width:1px;border-color: blue; ");
	m_pTextEditWidget->hide();

	connect(m_pTextEditWidget, SIGNAL(textChanged(QString)), this, SLOT(changeText(QString)));
	connect(m_pTextEditWidget, SIGNAL(editingFinished()), this, SLOT(setText()));

	m_undoStack = new QUndoStack(this);
}

KxSvgCanvas::KxSvgCanvas(QWidget* parent, qreal x, qreal y, qreal w, qreal h)
	:QWidget(parent)
{
	setGeometry(x, y, w, h);
}

KxSvgCanvas::~KxSvgCanvas()
{
	delete m_pClickRect;
	delete m_undoStack;
	clearCopyList();
	ShapeFactory::getShapeFactory()->deleteShapeFactory();
}

void KxSvgCanvas::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	//坐标变换
	QTransform transform;
	transform.translate(m_transfrom.x(), m_transfrom.y());
	painter.setTransform(transform);

	//绘制所有图像
	for (Shape* i : m_shapeList)
	{
		if (i != nullptr)
		{
			i->drawShape(painter);
		}
	}

	//绘制选中框
	if (m_clickShapeList.size() > 0)
	{
		for(auto i : m_clickShapeList)
		{
			if (false == i->isPaint())
			{
				m_clickShapeList.removeOne(i);
				i->setClickState(false);
				continue;
			}
			i->drawClickRect(painter);
		}
	}
	else
	{
		m_positionType = mousePosition::move;
	}

	//绘制选取框
	if (m_pClickRect && !m_pClickRect->getDrawEnd().isNull())
	{
		m_pClickRect->getPen().setStyle(Qt::DashLine);
		m_pClickRect->getPen().setColor(QColor(67, 142, 419));
		m_pClickRect->drawShape(painter);
	}

	//加载通用的svg图片
	if (m_pSvgRenderer && m_pSvgRenderer->isValid())
		m_pSvgRenderer->render(&painter);
}

void KxSvgCanvas::mousePressEvent(QMouseEvent* event)
{
	QPoint transformPoint = event->pos() - m_transfrom;
	if (Qt::LeftButton == event->button())
	{
		//如果是新建文本
		if (m_currentType == ShapeType::TypeText && m_pTextEditWidget->isHidden())
		{
			for (auto i : m_clickShapeList)
			{
				i->setClickState(false);
			}
			m_clickShapeList.clear();
			m_pCurrentShape = nullptr;
			m_pTextEditWidget->move(event->pos());
			m_pTextEditWidget->setFocus();
			m_pTextEditWidget->show();
		}
		else
		{
			//设置新增图像的开始点
			Shape* tmpShape = nullptr;
			tmpShape = ShapeFactory::getShapeFactory()->getShape(m_currentType);
			if (tmpShape)
			{
				m_shapeList.append(tmpShape);
				m_pCurrentShape = tmpShape;
				m_pCurrentShape->setDrawStart(transformPoint);
			}
		}


		if (m_clickShapeList.isEmpty())
			m_positionType = mousePosition::noClick;

		//单选和点击多选对象
		if (false == isMove && m_currentType == ShapeType::TypeSelect)
		{
			if(m_positionType == mousePosition::noClick || m_positionType == mousePosition::move)
			{
				m_pClickShape = getClickShape(transformPoint);
				if (m_pClickShape && false == m_pClickShape->isPaint())
					m_pClickShape = nullptr;
			}

			if (nullptr == m_pClickShape)
			{
				m_pClickRect->setDrawStart(transformPoint);
			}

			if (nullptr == m_pClickShape || (QApplication::keyboardModifiers() != Qt::ControlModifier && m_clickShapeList.size() < 2))
			{
				for (auto i : m_clickShapeList)
				{
					i->setClickState(false);
					m_clickShapeList.removeOne(i);
				}
			}

			if (nullptr != m_pClickShape && false == m_pClickShape->getClickState())
			{
				if(m_pClickShape->isPaint())
				{
					m_clickShapeList.append(m_pClickShape);
					m_pClickShape->setClickState(true);
				}
			}

			if (m_pClickShape != nullptr)
			{
				m_shapeLastStartPointF = m_pClickShape->getPhysicalStart();
				m_shapeLastEndPointF = m_pClickShape->getPhysicalEnd();
				isMove = true;
			}
		}
		else
		{
			isMove = false;
		}

		m_lastPoint = transformPoint;
	}

	if (Qt::RightButton == event->button())
	{
		if(m_clickShapeList.size() < 2)
		{
			for (Shape* i : m_clickShapeList)
			{
				i->setClickState(false);
				m_clickShapeList.removeOne(i);
			}
			m_pClickShape = getClickShape(transformPoint);
			if (m_pClickShape)
			{
				m_clickShapeList.append(m_pClickShape);
				m_pClickShape->setClickState(true);
			}
		}
		m_pRightClickMenu->move(event->globalPos());
		m_pRightClickMenu->show();
	}

	if (!m_clickShapeList.isEmpty())
	{
		emit setShapePane(m_clickShapeList[0]->getBrush().color(), m_clickShapeList[0]->getPen().color(), m_clickShapeList[0]->getPen().widthF(), m_clickShapeList[0]->getPen().style());
		emit paneIndex(1);
	}
	else {
		emit paneIndex(0);
	}
}

void KxSvgCanvas::mouseMoveEvent(QMouseEvent* event)
{
	QPoint transformPoint = event->pos() - m_transfrom;
	if (m_pCurrentShape)
	{
		m_pCurrentShape->setDrawEnd(transformPoint);
	}

	if (!m_pClickRect->getDrawStart().isNull())
	{
		m_pClickRect->setDrawEnd(transformPoint);
	}

	if (m_clickShapeList.isEmpty())
	{		
		update();
		return;
	}

	if (isMove)
	{
		editShape(transformPoint);
		m_lastPoint = transformPoint;
	}
	else if (m_currentType == ShapeType::TypeSelect)
	{
		setPositionType(transformPoint);
	}

	update();
}

void KxSvgCanvas::mouseReleaseEvent(QMouseEvent* event)
{
	if (m_pCurrentShape)
	{
		if (m_pCurrentShape->getDrawEnd().isNull() && m_currentType != ShapeType::TypePencil)
		{
			delete m_pCurrentShape;
			m_shapeList.removeLast();
			m_pCurrentShape = nullptr;
		}
		else
		{
			m_pClickShape = m_pCurrentShape;
			for (auto i : m_clickShapeList)
			{
				i->setClickState(false);
				m_clickShapeList.removeOne(i);
			}
			m_clickShapeList.append(m_pClickShape);
			m_pClickShape->drawPointToPhysicalPoint(m_radio);
			m_pClickShape->setClickState(true);

			QUndoCommand* addcommand = new AddCommand(this, m_pClickShape);
			m_undoStack->push(addcommand);
		}
	}

	updatePhysicalPoint();
	//移动command
	if (isMove)
	{
		QPointF offset = editoffset(m_shapeLastStartPointF, m_shapeLastEndPointF);
		if(!offset.isNull())
		{
			QUndoCommand* moveCommand = new MoveCommand(this, m_clickShapeList, offset, m_positionType);
			m_undoStack->push(moveCommand);
		}
		isMove = false;
	}

	//多选框的选中处理
	if (!m_pClickRect->getDrawEnd().isNull())
	{
		for (auto j : m_clickShapeList)
		{
			j->setClickState(false);
		}
		m_clickShapeList.clear();
		for (Shape* i : m_shapeList)
		{
			if (tool::isShapeIntersect(i, m_pClickRect))
			{
				if(i->isPaint())
				{
					m_clickShapeList.append(i);
					i->setClickState(true);
				}
			}
		}
		if (false == m_clickShapeList.isEmpty())
		{
			m_pClickShape = m_clickShapeList.first();
		}
	}

	m_pClickRect->setDrawStart(QPoint(0, 0));
	m_pClickRect->setDrawEnd(QPoint(0, 0));

	if (!m_clickShapeList.isEmpty())
	{
		emit setShapePane(m_clickShapeList[0]->getBrush().color(), m_clickShapeList[0]->getPen().color(), m_clickShapeList[0]->getPen().widthF(), m_clickShapeList[0]->getPen().style());
		emit paneIndex(1);
	}
	else {
		emit paneIndex(0);
	}

	m_pCurrentShape = nullptr;
	update();
}

void KxSvgCanvas::mouseDoubleClickEvent(QMouseEvent* event)
{
	return;//屏蔽掉文字编辑内容功能
	if (m_clickShapeList.size() == 1 && m_clickShapeList.first()->getShapeType() == ShapeType::TypeText)
	{
		Shape* shapeText = m_clickShapeList.first();
		m_pTextEditWidget->move(shapeText->getDrawStart().toPoint() + m_transfrom);
		m_pTextEditWidget->resize(shapeText->getDrawEnd().toPoint().x() - m_clickShapeList.first()->getDrawStart().toPoint().x() + 10, m_clickShapeList.first()->getDrawEnd().y() - m_clickShapeList.first()->getDrawStart().y());
		TextEdit* textEdit = dynamic_cast<TextEdit*>(shapeText);
		if (nullptr == textEdit)
			return;
		m_text = textEdit->getText();
		m_pTextEditWidget->setText(m_text);
		m_pTextEditWidget->setFont(QFont("Microsoft YaHei", textEdit->getFontSize()));
		m_pTextEditWidget->setFocus();
		m_pTextEditWidget->show();
	}
}

void KxSvgCanvas::setCurrentType(ShapeType type)
{
	m_currentType = type;
}

void KxSvgCanvas::setCanvasSize()
{
	resize(m_canvasWidth, m_canvasHeight);
	m_transfrom.setX(m_canvasWidth / 2);
	m_transfrom.setY(m_canvasHeight / 2);
	emit setCanvasChooseSize(m_canvasWidth, m_canvasHeight);
	m_radio = 0.0;
	for (auto i : m_shapeList)
	{
		i->scale(m_radio, m_radio);
	}
}

void KxSvgCanvas::setCanvasWidth(QString width)
{
	m_canvasWidth = width.toInt();
}

void KxSvgCanvas::setCanvasHeight(QString height)
{
	m_canvasHeight = height.toInt();
}

void KxSvgCanvas::openSvg()
{
	QString file_path = QFileDialog::getOpenFileName(this, tr("打开文件"), "./", tr("Exe files(*.svg);;All files(*.*)"));
	if (file_path.isEmpty())
	{
		return;
	}
	init();
	if (!SvgRead::svgRead()->read(file_path, m_shapeList, m_canvasWidth, m_canvasHeight, m_rgb))
	{
		m_transfrom = QPoint(0, 0);
		loadSvgRenderer(file_path);
		m_isCloseEvent = true;
		return;
	}
	m_transfrom = QPoint(m_canvasWidth / 2, m_canvasHeight / 2);
	setCanvasSize();
	setCanvasColor(m_rgb);
	m_isCloseEvent = false;
	readAddCommand();
}

void KxSvgCanvas::saveSvg()
{
	QString file_path = QFileDialog::getSaveFileName(this,tr("保存svg"),"",tr("(*.svg)"));
	if (file_path.isEmpty())
		return;

	if(SvgWrite::svgWrite()->write(m_shapeList, file_path, m_canvasWidth, m_canvasHeight, m_rgb))
		QMessageBox::information(NULL, "svg", tr("保存成功"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	else
		QMessageBox::information(NULL, "svg", tr("保存失败"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
}

void KxSvgCanvas::init()
{
	QMessageBox msg(this);
	msg.setStyleSheet("background: #ffffff;");
	msg.setText("是否保存画布");
	msg.setIcon(QMessageBox::Warning);
	msg.setStandardButtons(QMessageBox::Ok | QMessageBox::No | QMessageBox::Cancel);

	int res = msg.exec();
	switch (res)
	{
	case QMessageBox::Ok:
	{
		saveSvg();
		break;
	}
	case QMessageBox::Cancel:
	{
		return;
		break;
	}
	default:
		break;
	}

	m_undoStack->clear();
	deleteShapeList();
	unloadSvgRenderer();
	m_canvasWidth = 500;
	m_canvasHeight = 500;
	setCanvasSize();
	setCanvasColor(qRgb(255, 255, 255));
	m_isCloseEvent = false;
	update();
}

void KxSvgCanvas::setCanvasColor(QRgb rgb)
{
	m_rgb = rgb;
	QString s = QString("background: #%1;").arg(QString::number(m_rgb, 16));
	setStyleSheet(s);

	emit setCanvasChooseColor(rgb);
}

void KxSvgCanvas::changeText(QString text)
{
	QFontMetrics textLength(m_pTextEditWidget->font());
	int length = textLength.width(text);
	length < 10 ? m_pTextEditWidget->resize(10, 40) : m_pTextEditWidget->resize(length + 10, 40); //加个10是为了防止左边字体被遮住一块

	m_text = text;
	if (m_clickShapeList.size() == 1)
	{
		length < 10 ? m_clickShapeList.first()->getDrawEnd().setX(m_clickShapeList.first()->getDrawStart().x() + 10)
					: m_clickShapeList.first()->getDrawEnd().setX(m_clickShapeList.first()->getDrawStart().x() + length);
		m_clickShapeList.first()->drawPointToPhysicalPoint(m_radio);

		length < 10 ? m_pTextEditWidget->resize(10, m_clickShapeList.first()->getDrawEnd().y() - m_clickShapeList.first()->getDrawStart().y()) 
					: m_pTextEditWidget->resize(length + 10, m_clickShapeList.first()->getDrawEnd().y() - m_clickShapeList.first()->getDrawStart().y());
		update();
	}
}

void KxSvgCanvas::setText()
{
	if (m_pTextEditWidget->isHidden())
		return;

	m_pTextEditWidget->hide();

	if (m_clickShapeList.size() == 1 && m_clickShapeList.first()->getShapeType() == ShapeType::TypeText)
	{
		Shape* shape = m_clickShapeList.first();
		if (m_text.isEmpty())
		{
			m_clickShapeList.removeFirst();
			m_shapeList.removeOne(shape);
			delete shape;
			return;
		}
		TextEdit* textEdit = dynamic_cast<TextEdit*>(shape);
		if (nullptr == textEdit)
			return;
		textEdit->setText(m_text);
		m_text = "";
	}

	if (m_text.isEmpty())
		return;

	Shape* i = ShapeFactory::getShapeFactory()->getShape(ShapeType::TypeText);
	i->setDrawStart(m_pTextEditWidget->pos() - m_transfrom);
	i->setDrawEnd(m_pTextEditWidget->pos() + QPointF(m_pTextEditWidget->width(), m_pTextEditWidget->height()) - m_transfrom);
	TextEdit* textEdit = dynamic_cast<TextEdit*>(i);
	if (nullptr == textEdit)
		return;
	textEdit->setText(m_text);
	i->drawPointToPhysicalPoint(m_radio);
	m_shapeList.append(i);
	QUndoCommand* addcommand = new AddCommand(this, i);
	m_undoStack->push(addcommand);
	m_pClickShape = i;
	m_text = "";
	m_pTextEditWidget->clear();
}

void KxSvgCanvas::setStrokeWidth(QString width)
{
	m_penWidth = width.toDouble();
}

void KxSvgCanvas::setStroke()
{
	if (m_clickShapeList.isEmpty())
		return;

	for (auto i : m_clickShapeList)
	{
		i->getPen().setWidthF(m_penWidth);
	}
	update();
}

void KxSvgCanvas::setStrokeStyle(Qt::PenStyle style)
{
	if (m_clickShapeList.isEmpty())
		return;

	for(auto i : m_clickShapeList)
	{
		i->getPen().setStyle(style);
	}
	update();
}

void KxSvgCanvas::shear()
{
	copyClickShape();
	deleteShape();
	update();
}

void KxSvgCanvas::copy()
{
	copyClickShape();
	update();
}

void KxSvgCanvas::paste()
{
	copyListToShapeList();
	update();
}

void KxSvgCanvas::deleteShape()
{
	if (false == m_clickShapeList.isEmpty())
	{
		QUndoCommand* deleteCommand = new DeleteCommand(this, m_clickShapeList);
		m_undoStack->push(deleteCommand);
		m_clickShapeList.clear();
		m_pClickShape = nullptr;
	}
}

void KxSvgCanvas::shapeToUpper()
{
	if (m_clickShapeList.size() != 1)
		return;

	Shape* shape = m_clickShapeList.first();

	int index = m_shapeList.indexOf(shape);
	m_shapeList.removeOne(shape);
	m_shapeList.insert(index + 1, shape);
	update();
}

void KxSvgCanvas::shapeToLower()
{
	if (m_clickShapeList.size() != 1)
		return;

	Shape* shape = m_clickShapeList.first();

	int index = m_shapeList.indexOf(shape);
	m_shapeList.removeOne(shape);
	m_shapeList.insert(index - 1, shape);
	update();
}

void KxSvgCanvas::shapeToTop()
{
	if (m_clickShapeList.size() != 1)
		return;

	Shape* shape = m_clickShapeList.first();
	m_shapeList.removeOne(shape);
	m_shapeList.insert(m_shapeList.size(), shape);
	update();
}

void KxSvgCanvas::shapeToBottom()
{
	if (m_clickShapeList.size() != 1)
		return;

	Shape* shape = m_clickShapeList.first();
	m_shapeList.removeOne(shape);
	m_shapeList.insert(-1, shape);
	update();
}

void KxSvgCanvas::keyPressEvent(QKeyEvent* event)
{
	switch (event->key())
	{
	case Qt::Key_Delete:
	{
		deleteShape();
		break;
	}
	case  Qt::Key_A:
	{
		if (event->modifiers() != Qt::ControlModifier)
			break;

		m_clickShapeList.clear();
		m_clickShapeList = m_shapeList;
		for (auto i : m_clickShapeList)
		{
			i->setClickState(true);
		}
		break;
	}
	case  Qt::Key_C:
	{
		if(event->modifiers() != Qt::ControlModifier)
			break;

		copyClickShape();
		break;
	}
	case Qt::Key_V:
	{
		if(event->modifiers() != Qt::ControlModifier)
			break;

		copyListToShapeList();
		break;
	}
	default:
		break;
	}
	update();
}

void KxSvgCanvas::wheelEvent(QWheelEvent* event)
{
	setFocus();
	QPoint transfromOffset;

	if (event->delta() > 0)
	{
		m_radio += 0.05;
		resize(m_canvasWidth * (1 + m_radio), m_canvasHeight * (1 + m_radio));
		transfromOffset = QPoint((m_canvasWidth * (1 + m_radio)) / 2, (m_canvasHeight * (1 + m_radio)) / 2);
		for (Shape * i : m_shapeList)
		{
			if (i != nullptr)
			{
				i->scale(m_radio, m_radio);
			}
		}
	}
	else if (event->delta() < 0)
	{
		m_radio -= 0.05;
		if (qAbs(m_radio + 1.0) < 0.1)
			m_radio = -0.95;
		resize(m_canvasWidth * (1 + m_radio), m_canvasHeight * (1 + m_radio));
		transfromOffset = QPoint((m_canvasWidth * (1 + m_radio)) / 2, (m_canvasHeight * (1 + m_radio)) / 2);
		for (Shape * i : m_shapeList)
		{
			if (i != nullptr)
			{
				i->scale(m_radio, m_radio);
			}
		}
	}
	m_transfrom = transfromOffset;
	update();
}

bool KxSvgCanvas::eventFilter(QObject* watched, QEvent* event)
{
	if(m_isCloseEvent)
		paintEvent(static_cast<QPaintEvent*>(event));
	return m_isCloseEvent;
}

bool KxSvgCanvas::deleteShapeList()
{
	//m_pClickShape = nullptr;
	//m_clickShapeList.clear();
	//if (false == m_shapeList.isEmpty())
	//{
	//	for each (Shape * i in m_shapeList)
	//	{
	//		m_shapeList.removeOne(i);
	//		delete i;
	//	}
	//	return true;
	//}

	return false;
}

void KxSvgCanvas::loadSvgRenderer(QString fileDir)
{
	if (nullptr == m_pSvgRenderer)
		m_pSvgRenderer = new QSvgRenderer();
	m_pSvgRenderer->load(fileDir);
}

void KxSvgCanvas::unloadSvgRenderer()
{
	if (m_pSvgRenderer && m_pSvgRenderer->isValid())
	{
		m_pSvgRenderer->load(QString());
	}
}

bool KxSvgCanvas::isSvgValid()
{
	if (m_pSvgRenderer)
	{
		return m_pSvgRenderer->isValid();
	}
	return false;
}

int KxSvgCanvas::getShapeCount()
{
	if (false == m_shapeList.isEmpty())
	{
		return m_shapeList.count();
	}
	return 0;
}

void KxSvgCanvas::shapeInClickRect()
{
	for (Shape* i : m_shapeList)
	{
		QPointF topLeft = i->getDrawStart();
		QPointF bottomLeft = QPointF(i->getDrawStart().x(), i->getDrawEnd().y());
		QPointF topRight = QPointF(i->getDrawEnd().x(), i->getDrawStart().y());
		QPointF bottomRight = i->getDrawEnd();
		if (tool::ponitInRect(topLeft, m_pClickRect)
			|| tool::ponitInRect(bottomLeft, m_pClickRect)
			|| tool::ponitInRect(topRight, m_pClickRect)
			|| tool::ponitInRect(bottomRight, m_pClickRect))
		{
			m_clickShapeList.append(i);
			i->setClickState(true);
		}
	}
}

void KxSvgCanvas::setShapeColor(QRgb rgb)
{
	if (m_clickShapeList.isEmpty())
		return;

	for (Shape* i : m_clickShapeList)
	{
		i->getBrush().setColor(rgb);
	}
}

void KxSvgCanvas::setPenColor(QRgb rgb)
{
	if (m_clickShapeList.isEmpty())
		return;

	for (Shape* i : m_clickShapeList)
	{
		i->getPen().setColor(rgb);
	}
}

void KxSvgCanvas::copyClickShape()
{
	if (m_clickShapeList.isEmpty())
		return;

	for (auto i : m_copyShapeList)
	{
		m_copyShapeList.removeOne(i);
	}

	for (auto i : m_clickShapeList)
	{
		Shape* shape = ShapeFactory::getShapeFactory()->getShape(i->getShapeType());
		shape->copyDate(i);
		m_copyShapeList.append(shape);
	}
}

void KxSvgCanvas::clearCopyList()
{
	if (m_copyShapeList.isEmpty())
		return;

	for (auto i : m_copyShapeList)
	{
		m_copyShapeList.removeOne(i);
		delete i;
	}
}

void KxSvgCanvas::copyListToShapeList()
{
	if (m_copyShapeList.isEmpty())
		return;

	m_shapeList.append(m_copyShapeList);
	QUndoCommand* addCommand = new AddCommand(this, m_copyShapeList);
	m_undoStack->push(addCommand);
	for (auto i : m_clickShapeList)
	{
		i->setClickState(false);
		m_clickShapeList.removeOne(i);
	}
	m_clickShapeList = m_copyShapeList;

	for (auto i : m_clickShapeList)
	{
		i->setClickState(true);
		m_copyShapeList.removeOne(i);
		Shape* shape = ShapeFactory::getShapeFactory()->getShape(i->getShapeType());
		shape->copyDate(i);
		m_copyShapeList.append(shape);
	}
}

const QUndoStack* KxSvgCanvas::getUndoStack()
{
	return m_undoStack;
}

const qreal KxSvgCanvas::getRadio()
{
	return m_radio;
}

void KxSvgCanvas::shapeListRemoveOne(Shape* item)
{
	m_shapeList.removeOne(item);
}

void KxSvgCanvas::clickListRemoveOne(Shape* item)
{
	m_copyShapeList.removeOne(item);
}

void KxSvgCanvas::readAddCommand()
{
	if (m_shapeList.isEmpty())
		return;

	QUndoCommand* addCommand = new AddCommand(this, m_shapeList);
	m_undoStack->push(addCommand);
}

Shape* KxSvgCanvas::getClickShape(QPoint point)
{
	//逆序遍历，深度值越小 在链表的位置越靠前
	for (QList<Shape*>::iterator i = --m_shapeList.end(); i != --m_shapeList.begin(); --i)
	{
		if(false == (*i)->isPaint())
			continue;
		if (tool::ponitInRect(point, *i))
			return *i;
	}
	return nullptr;
}

void KxSvgCanvas::setPositionType(QPoint point)
{
	int positionType = 0;  //鼠标位于图形边缘编辑时的类型（向右拉，向左拉 之类的）
	if (nullptr == m_pClickShape)
		return;
	QRectF clickRect = m_pClickShape->getClickRect();
	if (false == tool::pointInRect(point, clickRect) || m_clickShapeList.size() > 1)
	{
		setCursor(Qt::ArrowCursor);
		m_positionType = mousePosition::noClick;
		return;
	}

	if (qAbs(point.x() - clickRect.x()) < 5)
	{
		positionType = positionType ^ 1;
	}
	if (qAbs(point.y() - clickRect.y()) < 5)
	{
		positionType = positionType ^ 2;
	}
	if (qAbs(point.x() - (clickRect.x() + clickRect.width())) < 5)
	{
		positionType = positionType ^ 4;
	}
	if (qAbs(point.y() - (clickRect.y() + clickRect.height())) < 5)
	{
		positionType = positionType ^ 8;
	}

	switch (positionType)
	{
	case POSITION_DEFAULT:setCursor(Qt::ArrowCursor); m_positionType = mousePosition::move; break;
	case POSITION_LEFT:setCursor(Qt::SizeHorCursor); m_positionType = mousePosition::left; break;
	case POSITION_TOP:setCursor(Qt::SizeVerCursor); m_positionType = mousePosition::top; break;
	case POSITION_RIGHT:setCursor(Qt::SizeHorCursor); m_positionType = mousePosition::right; break;
	case POSITION_BOTTOM:setCursor(Qt::SizeVerCursor); m_positionType = mousePosition::bottom; break;
	default:
		break;
	}

	/*下面这么写的原因是对角线存在两种情况，加个if else 来区分，没有其他好点的办法QAQ*/
	if ((m_pClickShape->getDrawEnd().x() - m_pClickShape->getDrawStart().x()) * (m_pClickShape->getDrawEnd().y() - m_pClickShape->getDrawStart().y()) >= 0)
	{
		switch (positionType)
		{
		case POSITION_UPPER_RIGHT:setCursor(Qt::SizeBDiagCursor); m_positionType = mousePosition::upperRight; break;
		case POSITION_UPPER_LEFT:setCursor(Qt::SizeFDiagCursor); m_positionType = mousePosition::upperLeft; break;
		case POSITION_LOWER_LETF:setCursor(Qt::SizeBDiagCursor); m_positionType = mousePosition::lowerLeft; break;
		case POSITION_LOWER_RIGHT:setCursor(Qt::SizeFDiagCursor); m_positionType = mousePosition::lowerRight; break;
		}
	}
	else
	{
		switch (positionType)
		{
		case POSITION_UPPER_RIGHT:setCursor(Qt::SizeFDiagCursor); m_positionType = mousePosition::upperRight; break;
		case POSITION_UPPER_LEFT:setCursor(Qt::SizeBDiagCursor); m_positionType = mousePosition::upperLeft; break;
		case POSITION_LOWER_LETF:setCursor(Qt::SizeFDiagCursor); m_positionType = mousePosition::lowerLeft; break;
		case POSITION_LOWER_RIGHT:setCursor(Qt::SizeBDiagCursor); m_positionType = mousePosition::lowerRight; break;
		}
	}
}

void KxSvgCanvas::setRightClickMenu()
{
	m_pRightClickMenu = new QMenu(this);

	QAction* pActionShear = new QAction("剪切", this);
	connect(pActionShear, SIGNAL(triggered()), this, SLOT(shear()));

	QAction* pActionCopy = new QAction("复制", this);
	connect(pActionCopy, SIGNAL(triggered()), this, SLOT(copy()));

	QAction* pActionPaste = new QAction("粘贴", this);
	connect(pActionPaste, SIGNAL(triggered()), this, SLOT(paste()));

	m_pRightClickMenu->addAction(pActionShear);
	m_pRightClickMenu->addAction(pActionCopy);
	m_pRightClickMenu->addAction(pActionPaste);

	m_pRightClickMenu->addSeparator();
	
	QAction* pActionDelete = new QAction("删除", this);
	connect(pActionDelete, SIGNAL(triggered()), this, SLOT(deleteShape()));

	m_pRightClickMenu->addAction(pActionDelete);

	m_pRightClickMenu->addSeparator();

	QAction* pActionUpper = new QAction("置于上一层", this);
	connect(pActionUpper, SIGNAL(triggered()), this, SLOT(shapeToUpper()));

	QAction* pActionLower = new QAction("置于下一层", this);
	connect(pActionLower, SIGNAL(triggered()), this, SLOT(shapeToLower()));

	QAction* pActionTop = new QAction("置于顶层", this);
	connect(pActionTop, SIGNAL(triggered()), this, SLOT(shapeToTop()));

	QAction* pActionBottom = new QAction("置于低层", this);
	connect(pActionBottom, SIGNAL(triggered()), this, SLOT(shapeToBottom()));

	m_pRightClickMenu->addAction(pActionUpper);
	m_pRightClickMenu->addAction(pActionLower);
	m_pRightClickMenu->addAction(pActionTop);
	m_pRightClickMenu->addAction(pActionBottom);

	m_pRightClickMenu->setStyleSheet("QMenu::item:selected{background-color:#409CE1;}"\
									 "QMenu::item{padding:5px 32px;color:rgba(51, 51, 51, 1);font-size:12px;}"\
									 "QMenu{border: 1px solid rgb(125, 125, 125);border-radius: 5px; }");

	m_pRightClickMenu->hide();
}

void KxSvgCanvas::editShape(QPoint transformPoint)
{
	for(auto i : m_clickShapeList)
	{
		switch (m_positionType)
		{
		case mousePosition::move:
		{
			i->move((transformPoint - m_lastPoint));
			break;
		}
		case mousePosition::noClick:
		{
			i->move((transformPoint - m_lastPoint));
			break;
		}
		case mousePosition::top:
		{
			i->moveTop(QPoint(0, transformPoint.y() - m_lastPoint.y()));
			break;
		}
		case mousePosition::left:
		{
			i->moveLeft(QPoint(transformPoint.x() - m_lastPoint.x(), 0));
			break;
		}
		case mousePosition::right:
		{
			i->moveRight(QPoint(transformPoint.x() - m_lastPoint.x(), 0));
			break;
		}
		case mousePosition::bottom:
		{
			i->moveBottom(QPoint(0, transformPoint.y() - m_lastPoint.y()));
			break;
		}
		case mousePosition::upperLeft:
		{
			i->moveUpperLeft(transformPoint - m_lastPoint);
			break;
		}
		case mousePosition::lowerLeft:
		{
			i->moveLowerLeft(transformPoint - m_lastPoint);
			break;
		}
		case mousePosition::upperRight:
		{
			i->moveUpperRight(transformPoint - m_lastPoint);
			break;
		}
		case mousePosition::lowerRight:
		{
			i->moveLowerRight(transformPoint - m_lastPoint);
			break;
		}
		default:
			break;
		}
	}
}

QPointF KxSvgCanvas::editoffset(QPointF start, QPointF end)
{
	QPointF res;
	if (nullptr == m_pClickShape)
		return res;
	res = m_pClickShape->getPhysicalStart() - start;
	if (qAbs(res.x()) < 0.0001)
		res.setX(m_pClickShape->getPhysicalEnd().x() - end.x());

	if (qAbs(res.y()) < 0.0001)
		res.setY(m_pClickShape->getPhysicalEnd().y() - end.y());
	
	return res;
}

void KxSvgCanvas::updatePhysicalPoint()
{
	if (m_pClickShape)
	{
		m_pClickShape->drawPointToPhysicalPoint(m_radio);
	}
}