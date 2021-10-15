#pragma once
#include <QtWidgets/QMainWindow>
#include <QtSvg/qsvgrenderer.h>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QRadioButton>
#include "kxcanvas.h"
#include <QList>

#include "shape.h"
class Line;
class Shape;
class QLineEdit;
class KxSvgCanvas;

class KxLeftToolBarBtn :public QRadioButton
{
	Q_OBJECT

public:
	KxLeftToolBarBtn(QWidget* parent, ShapeType type = ShapeType::tmp);
	~KxLeftToolBarBtn();
	void paintEvent(QPaintEvent*) override;
	void enterEvent(QEvent* event) override;
	void leaveEvent(QEvent* event) override;
	void mousePressEvent(QMouseEvent* e) override;
	
	void setImageDir();

signals:
	void setShapeType(ShapeType type);

private:
	QStringList m_imageDir;
	bool m_isClickIcon = false;
	QSvgRenderer* m_pSvg = nullptr;
	ShapeType m_shapeType;
};

class SVGMainWIndow : public QMainWindow
{
	Q_OBJECT

public:
	SVGMainWIndow(QWidget* parent = Q_NULLPTR);

	void paintEvent(QPaintEvent* event) override;
	void resizeEvent(QResizeEvent* event) override;
	bool eventFilter(QObject* watched, QEvent* event) override;
public slots:
	void setCanvasColor();

private:
	void setCanvasCenter();
	void setToolBar();
	void setSettingPane();

private:

	QHBoxLayout* m_pMainHoriLayout = nullptr;
	QHBoxLayout* m_pCentralLayout = nullptr;
	QVBoxLayout* m_pToolBarLayout = nullptr;
	QGridLayout* m_pGridLayout = nullptr;

	QWidget* m_pEditCanvasWidth = nullptr;
	QWidget* m_pEditCanvasHeight = nullptr;
	QWidget* m_pEditCanvasColor = nullptr;

	QWidget* m_pCentralWidget = nullptr;
	QWidget* m_pToolBarLeftWidget = nullptr;
	QWidget* m_pCanvasWidget = nullptr;
	QWidget* m_pSettingWidget = nullptr;

	QLineEdit* m_pWidthLineEdit = nullptr;		 //设置面板的编辑工具
	QLineEdit* m_pHeightLineEdit = nullptr;
	QPushButton* m_pColorChoose = nullptr;

	KxLeftToolBarBtn* m_pSelectButton = nullptr; //选择工具
	KxLeftToolBarBtn* m_pPencilButton = nullptr; //画笔工具
	KxLeftToolBarBtn* m_pLineButton = nullptr;   //直线工具
	KxLeftToolBarBtn* m_pRectButton = nullptr;   //矩形工具
	KxLeftToolBarBtn* m_pCircleButton = nullptr; //圆形工具
	KxLeftToolBarBtn* m_pPathButton = nullptr;   //绘制曲线工具
	KxLeftToolBarBtn* m_pShapeButton = nullptr;  //自定义图形工具

	QButtonGroup* m_pToolBarGroup = nullptr;

	QAction* m_pActionNew = nullptr;
	QAction* m_pActionOpen = nullptr;
	QAction* m_pActionSave = nullptr;

	QToolBar* m_pToolBarTop = nullptr;

	QSvgRenderer* m_pSvgRender = nullptr;
	KxSvgCanvas* m_pSvgCanvas = nullptr;
};