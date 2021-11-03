#include "../pch.h"

#ifdef _DEBUG

class ShapeSquareTest : public ::testing::Test {
protected:
	Square* m_square = nullptr;
	qreal m_radio = 0;

	ShapeSquareTest()
	{
		Shape* i = ShapeFactory::getShapeFactory()->getShape(ShapeType::TypeSquare);
		m_square = dynamic_cast<Square*>(i);
	}

	~ShapeSquareTest()
	{
		delete m_square;
	}

	void SetUp() override
	{
		m_square->setDrawStart(QF(-1, -1));
		m_square->setDrawEnd(QF(1, 1));
	}

	void TearDown() override
	{
	}
};

TEST_F(ShapeSquareTest, CreatSquareIsTrue)
{
	ASSERT_NE(nullptr, m_square);
}

TEST_F(ShapeSquareTest, MoveSquare)
{
	m_square->move(QF(1, 1));
	m_square->drawPointToPhysicalPoint(m_radio);
	ASSERT_EQ(QF(0, 0), m_square->getDrawStart());
	ASSERT_EQ(QF(2, 2), m_square->getDrawEnd());
	ASSERT_EQ(QF(0, 0), m_square->getPhysicalStart());
	ASSERT_EQ(QF(2, 2), m_square->getPhysicalEnd());
}

TEST_F(ShapeSquareTest, MoveTopSquare)
{
	m_square->moveTop(QF(0, 1));
	m_square->drawPointToPhysicalPoint(m_radio);
	ASSERT_EQ(QF(-1, 0), m_square->getDrawStart());
	ASSERT_EQ(QF(1, 1), m_square->getDrawEnd());
	ASSERT_EQ(QF(-1, 0), m_square->getPhysicalStart());
	ASSERT_EQ(QF(1, 1), m_square->getPhysicalEnd());
}

TEST_F(ShapeSquareTest, MoveBottomSquare)
{
	m_square->moveBottom(QF(1, 1));
	m_square->drawPointToPhysicalPoint(m_radio);
	ASSERT_EQ(QF(-1, -1), m_square->getDrawStart());
	ASSERT_EQ(QF(1, 2), m_square->getDrawEnd());
	ASSERT_EQ(QF(-1, -1), m_square->getPhysicalStart());
	ASSERT_EQ(QF(1, 2), m_square->getPhysicalEnd());
}

TEST_F(ShapeSquareTest, MoveLeftSquare)
{
	m_square->moveLeft(QF(1, 1));
	m_square->drawPointToPhysicalPoint(m_radio);
	ASSERT_EQ(QF(0, -1), m_square->getDrawStart());
	ASSERT_EQ(QF(1, 1), m_square->getDrawEnd());
	ASSERT_EQ(QF(0, -1), m_square->getPhysicalStart());
	ASSERT_EQ(QF(1, 1), m_square->getPhysicalEnd());
}

TEST_F(ShapeSquareTest, MoveRightSquare)
{
	m_square->moveRight(QF(1, 1));
	m_square->drawPointToPhysicalPoint(m_radio);
	ASSERT_EQ(QF(-1, -1), m_square->getDrawStart());
	ASSERT_EQ(QF(2, 1), m_square->getDrawEnd());
	ASSERT_EQ(QF(-1, -1), m_square->getPhysicalStart());
	ASSERT_EQ(QF(2, 1), m_square->getPhysicalEnd());
}

TEST_F(ShapeSquareTest, MoveUpperLeftSquare)
{
	m_square->moveUpperLeft(QF(1, 1));
	m_square->drawPointToPhysicalPoint(m_radio);
	ASSERT_EQ(QF(0, 0), m_square->getDrawStart());
	ASSERT_EQ(QF(1, 1), m_square->getDrawEnd());
	ASSERT_EQ(QF(0, 0), m_square->getPhysicalStart());
	ASSERT_EQ(QF(1, 1), m_square->getPhysicalEnd());
}

TEST_F(ShapeSquareTest, MoveLowerRightSquare)
{
	m_square->moveLowerRight(QF(1, 1));
	m_square->drawPointToPhysicalPoint(m_radio);
	ASSERT_EQ(QF(-1, -1), m_square->getDrawStart());
	ASSERT_EQ(QF(2, 2), m_square->getDrawEnd());
	ASSERT_EQ(QF(-1, -1), m_square->getPhysicalStart());
	ASSERT_EQ(QF(2, 2), m_square->getPhysicalEnd());
}

TEST_F(ShapeSquareTest, MoveLowerLeftSquare)
{
	m_square->moveLowerLeft(QF(1, 1));
	m_square->drawPointToPhysicalPoint(m_radio);
	ASSERT_EQ(QF(0, -1), m_square->getDrawStart());
	ASSERT_EQ(QF(1, 2), m_square->getDrawEnd());
	ASSERT_EQ(QF(0, -1), m_square->getPhysicalStart());
	ASSERT_EQ(QF(1, 2), m_square->getPhysicalEnd());
}

TEST_F(ShapeSquareTest, MoveUpperRightSquare)
{
	m_square->moveUpperRight(QF(1, 1));
	m_square->drawPointToPhysicalPoint(m_radio);
	ASSERT_EQ(QF(-1, 0), m_square->getDrawStart());
	ASSERT_EQ(QF(2, 1), m_square->getDrawEnd());
	ASSERT_EQ(QF(-1, 0), m_square->getPhysicalStart());
	ASSERT_EQ(QF(2, 1), m_square->getPhysicalEnd());
}

TEST_F(ShapeSquareTest, ScaleSquare)
{
	m_radio = 1;
	m_square->scale(m_radio, m_radio);
	ASSERT_EQ(QF(-2, -2), m_square->getDrawStart());
	ASSERT_EQ(QF(2, 2), m_square->getDrawEnd());
	ASSERT_EQ(QF(-1, -1), m_square->getPhysicalStart());
	ASSERT_EQ(QF(1, 1), m_square->getPhysicalEnd());
}

#endif // DEBUG