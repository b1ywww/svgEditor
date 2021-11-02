#include "pch.h"

#ifdef _DEBUG

class ShapeCircleTest : public ::testing::Test {
protected:
	Circle* m_circle = nullptr;
	qreal m_radio = 0;

	ShapeCircleTest()
	{
		Shape* i = ShapeFactory::getShapeFactory()->getShape(ShapeType::TypeCircle);
		m_circle = dynamic_cast<Circle*>(i);
	}

	~ShapeCircleTest()
	{
		delete m_circle;
	}

	void SetUp() override
	{
		m_circle->setDrawStart(QF(-1, -1));
		m_circle->setDrawEnd(QF(1, 1));
	}

	void TearDown() override
	{
	}
};

TEST_F(ShapeCircleTest, CreatCircleIsTrue)
{
	ASSERT_NE(nullptr, m_circle);
}

TEST_F(ShapeCircleTest, MoveCircle)
{
	m_circle->move(QF(1, 1));
	m_circle->drawPointToPhysicalPoint(m_radio);
	ASSERT_EQ(QF(0, 0), m_circle->getDrawStart());
	ASSERT_EQ(QF(2, 2), m_circle->getDrawEnd());
	ASSERT_EQ(QF(0, 0), m_circle->getPhysicalStart());
	ASSERT_EQ(QF(2, 2), m_circle->getPhysicalEnd());
}

TEST_F(ShapeCircleTest, MoveTopCircle)
{
	m_circle->moveTop(QF(0, 1));
	m_circle->drawPointToPhysicalPoint(m_radio);
	ASSERT_EQ(QF(-1, 0), m_circle->getDrawStart());
	ASSERT_EQ(QF(1, 1), m_circle->getDrawEnd());
	ASSERT_EQ(QF(-1, 0), m_circle->getPhysicalStart());
	ASSERT_EQ(QF(1, 1), m_circle->getPhysicalEnd());
}

TEST_F(ShapeCircleTest, MoveBottomCircle)
{
	m_circle->moveBottom(QF(1, 1));
	m_circle->drawPointToPhysicalPoint(m_radio);
	ASSERT_EQ(QF(-1, -1), m_circle->getDrawStart());
	ASSERT_EQ(QF(1, 2), m_circle->getDrawEnd());
	ASSERT_EQ(QF(-1, -1), m_circle->getPhysicalStart());
	ASSERT_EQ(QF(1, 2), m_circle->getPhysicalEnd());
}

TEST_F(ShapeCircleTest, MoveLeftCircle)
{
	m_circle->moveLeft(QF(1, 1));
	m_circle->drawPointToPhysicalPoint(m_radio);
	ASSERT_EQ(QF(0, -1), m_circle->getDrawStart());
	ASSERT_EQ(QF(1, 1), m_circle->getDrawEnd());
	ASSERT_EQ(QF(0, -1), m_circle->getPhysicalStart());
	ASSERT_EQ(QF(1, 1), m_circle->getPhysicalEnd());
}

TEST_F(ShapeCircleTest, MoveRightCircle)
{
	m_circle->moveRight(QF(1, 1));
	m_circle->drawPointToPhysicalPoint(m_radio);
	ASSERT_EQ(QF(-1, -1), m_circle->getDrawStart());
	ASSERT_EQ(QF(2, 1), m_circle->getDrawEnd());
	ASSERT_EQ(QF(-1, -1), m_circle->getPhysicalStart());
	ASSERT_EQ(QF(2, 1), m_circle->getPhysicalEnd());
}

TEST_F(ShapeCircleTest, MoveUpperLeftCircle)
{
	m_circle->moveUpperLeft(QF(1, 1));
	m_circle->drawPointToPhysicalPoint(m_radio);
	ASSERT_EQ(QF(0, 0), m_circle->getDrawStart());
	ASSERT_EQ(QF(1, 1), m_circle->getDrawEnd());
	ASSERT_EQ(QF(0, 0), m_circle->getPhysicalStart());
	ASSERT_EQ(QF(1, 1), m_circle->getPhysicalEnd());
}

TEST_F(ShapeCircleTest, MoveLowerRightCircle)
{
	m_circle->moveLowerRight(QF(1, 1));
	m_circle->drawPointToPhysicalPoint(m_radio);
	ASSERT_EQ(QF(-1, -1), m_circle->getDrawStart());
	ASSERT_EQ(QF(2, 2), m_circle->getDrawEnd());
	ASSERT_EQ(QF(-1, -1), m_circle->getPhysicalStart());
	ASSERT_EQ(QF(2, 2), m_circle->getPhysicalEnd());
}

TEST_F(ShapeCircleTest, MoveLowerLeftCircle)
{
	m_circle->moveLowerLeft(QF(1, 1));
	m_circle->drawPointToPhysicalPoint(m_radio);
	ASSERT_EQ(QF(0, -1), m_circle->getDrawStart());
	ASSERT_EQ(QF(1, 2), m_circle->getDrawEnd());
	ASSERT_EQ(QF(0, -1), m_circle->getPhysicalStart());
	ASSERT_EQ(QF(1, 2), m_circle->getPhysicalEnd());
}

TEST_F(ShapeCircleTest, MoveUpperRightCircle)
{
	m_circle->moveUpperRight(QF(1, 1));
	m_circle->drawPointToPhysicalPoint(m_radio);
	ASSERT_EQ(QF(-1, 0), m_circle->getDrawStart());
	ASSERT_EQ(QF(2, 1), m_circle->getDrawEnd());
	ASSERT_EQ(QF(-1, 0), m_circle->getPhysicalStart());
	ASSERT_EQ(QF(2, 1), m_circle->getPhysicalEnd());
}

TEST_F(ShapeCircleTest, ScaleCircle)
{
	m_radio = 1;
	m_circle->scale(m_radio, m_radio);
	ASSERT_EQ(QF(-2, -2), m_circle->getDrawStart());
	ASSERT_EQ(QF(2, 2), m_circle->getDrawEnd());
	ASSERT_EQ(QF(-1, -1), m_circle->getPhysicalStart());
	ASSERT_EQ(QF(1, 1), m_circle->getPhysicalEnd());
}

#endif //  DEBUG