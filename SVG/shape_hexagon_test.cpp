#include "pch.h"

#ifdef  _DEBUG

class ShapeHexagonTest : public ::testing::Test {
protected:
	Hexagon* m_hexagon = nullptr;
	qreal m_radio = 0;

	ShapeHexagonTest()
	{
		Shape* i = ShapeFactory::getShapeFactory()->getShape(ShapeType::TypeHexagon);
		m_hexagon = dynamic_cast<Hexagon*>(i);
	}

	~ShapeHexagonTest()
	{
		delete m_hexagon;
	}

	void SetUp() override
	{
		m_hexagon->setDrawStart(QF(-1, -1));
		m_hexagon->setDrawEnd(QF(1, 1));
	}

	void TearDown() override
	{
	}
};

TEST_F(ShapeHexagonTest, CreatHexagonIsTrue)
{
	ASSERT_NE(nullptr, m_hexagon);
}

TEST_F(ShapeHexagonTest, MoveHexagon)
{
	m_hexagon->move(QF(1, 1));
	m_hexagon->drawPointToPhysicalPoint(m_radio);
	ASSERT_EQ(QF(0, 0), m_hexagon->getDrawStart());
	ASSERT_EQ(QF(2, 2), m_hexagon->getDrawEnd());
	ASSERT_EQ(QF(0, 0), m_hexagon->getPhysicalStart());
	ASSERT_EQ(QF(2, 2), m_hexagon->getPhysicalEnd());
}

TEST_F(ShapeHexagonTest, MoveTopHexagon)
{
	m_hexagon->moveTop(QF(0, 1));
	m_hexagon->drawPointToPhysicalPoint(m_radio);
	ASSERT_EQ(QF(-1, 0), m_hexagon->getDrawStart());
	ASSERT_EQ(QF(1, 1), m_hexagon->getDrawEnd());
	ASSERT_EQ(QF(-1, 0), m_hexagon->getPhysicalStart());
	ASSERT_EQ(QF(1, 1), m_hexagon->getPhysicalEnd());
}

TEST_F(ShapeHexagonTest, MoveBottomHexagon)
{
	m_hexagon->moveBottom(QF(1, 1));
	m_hexagon->drawPointToPhysicalPoint(m_radio);
	ASSERT_EQ(QF(-1, -1), m_hexagon->getDrawStart());
	ASSERT_EQ(QF(1, 2), m_hexagon->getDrawEnd());
	ASSERT_EQ(QF(-1, -1), m_hexagon->getPhysicalStart());
	ASSERT_EQ(QF(1, 2), m_hexagon->getPhysicalEnd());
}

TEST_F(ShapeHexagonTest, MoveLeftHexagon)
{
	m_hexagon->moveLeft(QF(1, 1));
	m_hexagon->drawPointToPhysicalPoint(m_radio);
	ASSERT_EQ(QF(0, -1), m_hexagon->getDrawStart());
	ASSERT_EQ(QF(1, 1), m_hexagon->getDrawEnd());
	ASSERT_EQ(QF(0, -1), m_hexagon->getPhysicalStart());
	ASSERT_EQ(QF(1, 1), m_hexagon->getPhysicalEnd());
}

TEST_F(ShapeHexagonTest, MoveRightHexagon)
{
	m_hexagon->moveRight(QF(1, 1));
	m_hexagon->drawPointToPhysicalPoint(m_radio);
	ASSERT_EQ(QF(-1, -1), m_hexagon->getDrawStart());
	ASSERT_EQ(QF(2, 1), m_hexagon->getDrawEnd());
	ASSERT_EQ(QF(-1, -1), m_hexagon->getPhysicalStart());
	ASSERT_EQ(QF(2, 1), m_hexagon->getPhysicalEnd());
}

TEST_F(ShapeHexagonTest, MoveUpperLeftHexagon)
{
	m_hexagon->moveUpperLeft(QF(1, 1));
	m_hexagon->drawPointToPhysicalPoint(m_radio);
	ASSERT_EQ(QF(0, 0), m_hexagon->getDrawStart());
	ASSERT_EQ(QF(1, 1), m_hexagon->getDrawEnd());
	ASSERT_EQ(QF(0, 0), m_hexagon->getPhysicalStart());
	ASSERT_EQ(QF(1, 1), m_hexagon->getPhysicalEnd());
}

TEST_F(ShapeHexagonTest, MoveLowerRightHexagon)
{
	m_hexagon->moveLowerRight(QF(1, 1));
	m_hexagon->drawPointToPhysicalPoint(m_radio);
	ASSERT_EQ(QF(-1, -1), m_hexagon->getDrawStart());
	ASSERT_EQ(QF(2, 2), m_hexagon->getDrawEnd());
	ASSERT_EQ(QF(-1, -1), m_hexagon->getPhysicalStart());
	ASSERT_EQ(QF(2, 2), m_hexagon->getPhysicalEnd());
}

TEST_F(ShapeHexagonTest, MoveLowerLeftHexagon)
{
	m_hexagon->moveLowerLeft(QF(1, 1));
	m_hexagon->drawPointToPhysicalPoint(m_radio);
	ASSERT_EQ(QF(0, -1), m_hexagon->getDrawStart());
	ASSERT_EQ(QF(1, 2), m_hexagon->getDrawEnd());
	ASSERT_EQ(QF(0, -1), m_hexagon->getPhysicalStart());
	ASSERT_EQ(QF(1, 2), m_hexagon->getPhysicalEnd());
}

TEST_F(ShapeHexagonTest, MoveUpperRightHexagon)
{
	m_hexagon->moveUpperRight(QF(1, 1));
	m_hexagon->drawPointToPhysicalPoint(m_radio);
	ASSERT_EQ(QF(-1, 0), m_hexagon->getDrawStart());
	ASSERT_EQ(QF(2, 1), m_hexagon->getDrawEnd());
	ASSERT_EQ(QF(-1, 0), m_hexagon->getPhysicalStart());
	ASSERT_EQ(QF(2, 1), m_hexagon->getPhysicalEnd());
}

TEST_F(ShapeHexagonTest, ScaleHexagon)
{
	m_radio = 1;
	m_hexagon->scale(m_radio, m_radio);
	ASSERT_EQ(QF(-2, -2), m_hexagon->getDrawStart());
	ASSERT_EQ(QF(2, 2), m_hexagon->getDrawEnd());
	ASSERT_EQ(QF(-1, -1), m_hexagon->getPhysicalStart());
	ASSERT_EQ(QF(1, 1), m_hexagon->getPhysicalEnd());
}

#endif //  DEBUG