#include "pch.h"

/* 文字的单测有问题，
error: ASSERT failure in QGuiApplication::font(): "no QGuiApplication instance", file kernel\qguiapplication.cpp, line 2994    QMutex: destroying locked mutex

#ifdef _DEBUG

class ShapeTextEditTest : public ::testing::Test {
protected:
	TextEdit* m_textEdit = nullptr;
	qreal m_radio = 0;

	ShapeTextEditTest()
	{
		Shape* i = ShapeFactory::getShapeFactory()->getShape(ShapeType::TypeText);
		m_textEdit = dynamic_cast<TextEdit*>(i);
	}

	~ShapeTextEditTest()
	{
		delete m_textEdit;
	}

	void SetUp() override
	{
		m_textEdit->setDrawStart(QF(-180, -70));
		m_textEdit->setDrawEnd(QF(-101, -30));
		m_textEdit->setText(QString("123"));
		m_textEdit->drawPointToPhysicalPoint(m_radio);
	}

	void TearDown() override
	{
	}
};

TEST_F(ShapeTextEditTest, CreatTextEditIsTrue)
{
	ASSERT_NE(nullptr, m_textEdit);
}

TEST_F(ShapeTextEditTest, MoveTextEdit)
{
	m_textEdit->move(QF(1, 1));
	m_textEdit->drawPointToPhysicalPoint(m_radio);
	ASSERT_EQ(QF(0, 0), m_textEdit->getDrawStart());
	ASSERT_EQ(QF(2, 2), m_textEdit->getDrawEnd());
	ASSERT_EQ(QF(0, 0), m_textEdit->getPhysicalStart());
	ASSERT_EQ(QF(2, 2), m_textEdit->getPhysicalEnd());
}

TEST_F(ShapeTextEditTest, MoveTopTextEdit)
{
	m_textEdit->moveTop(QF(0, 1));
	m_textEdit->drawPointToPhysicalPoint(m_radio);
	ASSERT_EQ(QF(-1, 0), m_textEdit->getDrawStart());
	ASSERT_EQ(QF(1, 1), m_textEdit->getDrawEnd());
	ASSERT_EQ(QF(-1, 0), m_textEdit->getPhysicalStart());
	ASSERT_EQ(QF(1, 1), m_textEdit->getPhysicalEnd());
}

TEST_F(ShapeTextEditTest, MoveBottomTextEdit)
{
	m_textEdit->moveBottom(QF(1, 1));
	m_textEdit->drawPointToPhysicalPoint(m_radio);
	ASSERT_EQ(QF(-1, -1), m_textEdit->getDrawStart());
	ASSERT_EQ(QF(1, 2), m_textEdit->getDrawEnd());
	ASSERT_EQ(QF(-1, -1), m_textEdit->getPhysicalStart());
	ASSERT_EQ(QF(1, 2), m_textEdit->getPhysicalEnd());
}

TEST_F(ShapeTextEditTest, MoveLeftTextEdit)
{
	m_textEdit->moveLeft(QF(1, 1));
	m_textEdit->drawPointToPhysicalPoint(m_radio);
	ASSERT_EQ(QF(0, -1), m_textEdit->getDrawStart());
	ASSERT_EQ(QF(1, 1), m_textEdit->getDrawEnd());
	ASSERT_EQ(QF(0, -1), m_textEdit->getPhysicalStart());
	ASSERT_EQ(QF(1, 1), m_textEdit->getPhysicalEnd());
}

TEST_F(ShapeTextEditTest, MoveRightTextEdit)
{
	m_textEdit->moveRight(QF(1, 1));
	m_textEdit->drawPointToPhysicalPoint(m_radio);
	ASSERT_EQ(QF(-1, -1), m_textEdit->getDrawStart());
	ASSERT_EQ(QF(2, 1), m_textEdit->getDrawEnd());
	ASSERT_EQ(QF(-1, -1), m_textEdit->getPhysicalStart());
	ASSERT_EQ(QF(2, 1), m_textEdit->getPhysicalEnd());
}

TEST_F(ShapeTextEditTest, MoveUpperLeftTextEdit)
{
	m_textEdit->moveUpperLeft(QF(1, 1));
	m_textEdit->drawPointToPhysicalPoint(m_radio);
	ASSERT_EQ(QF(0, 0), m_textEdit->getDrawStart());
	ASSERT_EQ(QF(1, 1), m_textEdit->getDrawEnd());
	ASSERT_EQ(QF(0, 0), m_textEdit->getPhysicalStart());
	ASSERT_EQ(QF(1, 1), m_textEdit->getPhysicalEnd());
}

TEST_F(ShapeTextEditTest, MoveLowerRightTextEdit)
{
	m_textEdit->moveLowerRight(QF(1, 1));
	m_textEdit->drawPointToPhysicalPoint(m_radio);
	ASSERT_EQ(QF(-1, -1), m_textEdit->getDrawStart());
	ASSERT_EQ(QF(2, 2), m_textEdit->getDrawEnd());
	ASSERT_EQ(QF(-1, -1), m_textEdit->getPhysicalStart());
	ASSERT_EQ(QF(2, 2), m_textEdit->getPhysicalEnd());
}

TEST_F(ShapeTextEditTest, MoveLowerLeftTextEdit)
{
	m_textEdit->moveLowerLeft(QF(1, 1));
	m_textEdit->drawPointToPhysicalPoint(m_radio);
	ASSERT_EQ(QF(0, -1), m_textEdit->getDrawStart());
	ASSERT_EQ(QF(1, 2), m_textEdit->getDrawEnd());
	ASSERT_EQ(QF(0, -1), m_textEdit->getPhysicalStart());
	ASSERT_EQ(QF(1, 2), m_textEdit->getPhysicalEnd());
}

TEST_F(ShapeTextEditTest, MoveUpperRightTextEdit)
{
	m_textEdit->moveUpperRight(QF(1, 1));
	m_textEdit->drawPointToPhysicalPoint(m_radio);
	ASSERT_EQ(QF(-1, 0), m_textEdit->getDrawStart());
	ASSERT_EQ(QF(2, 1), m_textEdit->getDrawEnd());
	ASSERT_EQ(QF(-1, 0), m_textEdit->getPhysicalStart());
	ASSERT_EQ(QF(2, 1), m_textEdit->getPhysicalEnd());
}

TEST_F(ShapeTextEditTest, ScaleTextEdit)
{

}
#endif // DEBUG
*/