#include "../pch.h"

#ifdef _DEBUG

class ShapeLineTest : public ::testing::Test {
protected:
	Line* m_line = nullptr;
	qreal m_radio = 0;

	ShapeLineTest()
	{
		Shape* i = ShapeFactory::getShapeFactory()->getShape(ShapeType::TypeLine);
		m_line = dynamic_cast<Line*>(i);
	}

	~ShapeLineTest() 
	{
		delete m_line;
	}

	void SetUp() override
	{
		m_line->setDrawStart(QF(-1, -1));
		m_line->setDrawEnd(QF(1, 1));
	}


	void TearDown() override
	{
	}
};

TEST_F(ShapeLineTest, CreatLineIsTrue)
{
	ASSERT_NE(nullptr, m_line);
}

TEST_F(ShapeLineTest, MoveLine)
{
	m_line->move(QF(1, 1));
	m_line->drawPointToPhysicalPoint(m_radio);
	ASSERT_EQ(QF(0, 0), m_line->getDrawStart());
	ASSERT_EQ(QF(2, 2), m_line->getDrawEnd());
	ASSERT_EQ(QF(0, 0), m_line->getPhysicalStart());
	ASSERT_EQ(QF(2, 2), m_line->getPhysicalEnd());
}

TEST_F(ShapeLineTest, MoveTopLine)
{
	m_line->moveTop(QF(0, 1));
	m_line->drawPointToPhysicalPoint(m_radio);
	ASSERT_EQ(QF(-1, 0), m_line->getDrawStart());
	ASSERT_EQ(QF(1, 1), m_line->getDrawEnd());
	ASSERT_EQ(QF(-1, 0), m_line->getPhysicalStart());
	ASSERT_EQ(QF(1, 1), m_line->getPhysicalEnd());
}

TEST_F(ShapeLineTest, MoveBottomLine)
{
	m_line->moveBottom(QF(1, 1));
	m_line->drawPointToPhysicalPoint(m_radio);
	ASSERT_EQ(QF(-1, -1), m_line->getDrawStart());
	ASSERT_EQ(QF(1, 2), m_line->getDrawEnd());
	ASSERT_EQ(QF(-1, -1), m_line->getPhysicalStart());
	ASSERT_EQ(QF(1, 2), m_line->getPhysicalEnd());
}

TEST_F(ShapeLineTest, MoveLeftLine)
{
	m_line->moveLeft(QF(1, 1));
	m_line->drawPointToPhysicalPoint(m_radio);
	ASSERT_EQ(QF(0, -1), m_line->getDrawStart());
	ASSERT_EQ(QF(1, 1), m_line->getDrawEnd());
	ASSERT_EQ(QF(0, -1), m_line->getPhysicalStart());
	ASSERT_EQ(QF(1, 1), m_line->getPhysicalEnd());
}

TEST_F(ShapeLineTest, MoveRightLine)
{
	m_line->moveRight(QF(1, 1));
	m_line->drawPointToPhysicalPoint(m_radio);
	ASSERT_EQ(QF(-1, -1), m_line->getDrawStart());
	ASSERT_EQ(QF(2, 1), m_line->getDrawEnd());
	ASSERT_EQ(QF(-1, -1), m_line->getPhysicalStart());
	ASSERT_EQ(QF(2, 1), m_line->getPhysicalEnd());
}

TEST_F(ShapeLineTest, MoveUpperLeftLine)
{
	m_line->moveUpperLeft(QF(1, 1));
	m_line->drawPointToPhysicalPoint(m_radio);
	ASSERT_EQ(QF(0, 0), m_line->getDrawStart());
	ASSERT_EQ(QF(1, 1), m_line->getDrawEnd());
	ASSERT_EQ(QF(0, 0), m_line->getPhysicalStart());
	ASSERT_EQ(QF(1, 1), m_line->getPhysicalEnd());
}

TEST_F(ShapeLineTest, MoveLowerRightLine)
{
	m_line->moveLowerRight(QF(1, 1));
	m_line->drawPointToPhysicalPoint(m_radio);
	ASSERT_EQ(QF(-1, -1), m_line->getDrawStart());
	ASSERT_EQ(QF(2, 2), m_line->getDrawEnd());
	ASSERT_EQ(QF(-1, -1), m_line->getPhysicalStart());
	ASSERT_EQ(QF(2, 2), m_line->getPhysicalEnd());
}

TEST_F(ShapeLineTest, MoveLowerLeftLine)
{
	m_line->moveLowerLeft(QF(1, 1));
	m_line->drawPointToPhysicalPoint(m_radio);
	ASSERT_EQ(QF(0, -1), m_line->getDrawStart());
	ASSERT_EQ(QF(1, 2), m_line->getDrawEnd());
	ASSERT_EQ(QF(0, -1), m_line->getPhysicalStart());
	ASSERT_EQ(QF(1, 2), m_line->getPhysicalEnd());
}

TEST_F(ShapeLineTest, MoveUpperRightLine)
{
	m_line->moveUpperRight(QF(1, 1));
	m_line->drawPointToPhysicalPoint(m_radio);
	ASSERT_EQ(QF(-1, 0), m_line->getDrawStart());
	ASSERT_EQ(QF(2, 1), m_line->getDrawEnd());
	ASSERT_EQ(QF(-1, 0), m_line->getPhysicalStart());
	ASSERT_EQ(QF(2, 1), m_line->getPhysicalEnd());
}

TEST_F(ShapeLineTest, ScaleLine)
{
	m_radio = 1;
	m_line->scale(m_radio, m_radio);
	ASSERT_EQ(QF(-2, -2), m_line->getDrawStart());
	ASSERT_EQ(QF(2, 2), m_line->getDrawEnd());
	ASSERT_EQ(QF(-1, -1), m_line->getPhysicalStart());
	ASSERT_EQ(QF(1, 1), m_line->getPhysicalEnd());
}

#endif // DEBUG