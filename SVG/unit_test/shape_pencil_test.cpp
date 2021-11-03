#include "../pch.h"

#ifdef _DEBUG

class ShapePencilTest : public ::testing::Test {
protected:
	Pencil* m_pencil = nullptr;
	qreal m_radio = 0;

	ShapePencilTest()
	{
		Shape* i = ShapeFactory::getShapeFactory()->getShape(ShapeType::TypePencil);
		m_pencil = dynamic_cast<Pencil*>(i);
	}

	~ShapePencilTest()
	{
		delete m_pencil;
	}

	void SetUp() override
	{
		m_pencil->setDrawStart(QF(-1, -1));
		m_pencil->setDrawEnd(QF(1, 1));
	}

	void TearDown() override
	{
	}
};

TEST_F(ShapePencilTest, CreatPencilIsTrue)
{
	ASSERT_NE(nullptr, m_pencil);
}

TEST_F(ShapePencilTest, MovePencil)
{
	m_pencil->move(QF(1, 1));
	m_pencil->drawPointToPhysicalPoint(m_radio);
	ASSERT_EQ(QF(0, 0), m_pencil->getDrawStart());
	ASSERT_EQ(QF(2, 2), m_pencil->getDrawEnd());
	ASSERT_EQ(QF(0, 0), m_pencil->getPhysicalStart());
	ASSERT_EQ(QF(2, 2), m_pencil->getPhysicalEnd());
}

TEST_F(ShapePencilTest, MoveTopPencil)
{
	m_pencil->moveTop(QF(0, 1));
	m_pencil->drawPointToPhysicalPoint(m_radio);
	ASSERT_EQ(QF(-1, 0), m_pencil->getDrawStart());
	ASSERT_EQ(QF(1, 1), m_pencil->getDrawEnd());
	ASSERT_EQ(QF(-1, 0), m_pencil->getPhysicalStart());
	ASSERT_EQ(QF(1, 1), m_pencil->getPhysicalEnd());
}

TEST_F(ShapePencilTest, MoveBottomPencil)
{
	m_pencil->moveBottom(QF(1, 1));
	m_pencil->drawPointToPhysicalPoint(m_radio);
	ASSERT_EQ(QF(-1, -1), m_pencil->getDrawStart());
	ASSERT_EQ(QF(1, 2), m_pencil->getDrawEnd());
	ASSERT_EQ(QF(-1, -1), m_pencil->getPhysicalStart());
	ASSERT_EQ(QF(1, 2), m_pencil->getPhysicalEnd());
}

TEST_F(ShapePencilTest, MoveLeftPencil)
{
	m_pencil->moveLeft(QF(1, 1));
	m_pencil->drawPointToPhysicalPoint(m_radio);
	ASSERT_EQ(QF(0, -1), m_pencil->getDrawStart());
	ASSERT_EQ(QF(1, 1), m_pencil->getDrawEnd());
	ASSERT_EQ(QF(0, -1), m_pencil->getPhysicalStart());
	ASSERT_EQ(QF(1, 1), m_pencil->getPhysicalEnd());
}

TEST_F(ShapePencilTest, MoveRightPencil)
{
	m_pencil->moveRight(QF(1, 1));
	m_pencil->drawPointToPhysicalPoint(m_radio);
	ASSERT_EQ(QF(-1, -1), m_pencil->getDrawStart());
	ASSERT_EQ(QF(2, 1), m_pencil->getDrawEnd());
	ASSERT_EQ(QF(-1, -1), m_pencil->getPhysicalStart());
	ASSERT_EQ(QF(2, 1), m_pencil->getPhysicalEnd());
}

TEST_F(ShapePencilTest, MoveUpperLeftPencil)
{
	m_pencil->moveUpperLeft(QF(1, 1));
	m_pencil->drawPointToPhysicalPoint(m_radio);
	ASSERT_EQ(QF(0, 0), m_pencil->getDrawStart());
	ASSERT_EQ(QF(1, 1), m_pencil->getDrawEnd());
	ASSERT_EQ(QF(0, 0), m_pencil->getPhysicalStart());
	ASSERT_EQ(QF(1, 1), m_pencil->getPhysicalEnd());
}

TEST_F(ShapePencilTest, MoveLowerRightPencil)
{
	m_pencil->moveLowerRight(QF(1, 1));
	m_pencil->drawPointToPhysicalPoint(m_radio);
	ASSERT_EQ(QF(-1, -1), m_pencil->getDrawStart());
	ASSERT_EQ(QF(2, 2), m_pencil->getDrawEnd());
	ASSERT_EQ(QF(-1, -1), m_pencil->getPhysicalStart());
	ASSERT_EQ(QF(2, 2), m_pencil->getPhysicalEnd());
}

TEST_F(ShapePencilTest, MoveLowerLeftPencil)
{
	m_pencil->moveLowerLeft(QF(1, 1));
	m_pencil->drawPointToPhysicalPoint(m_radio);
	ASSERT_EQ(QF(0, -1), m_pencil->getDrawStart());
	ASSERT_EQ(QF(1, 2), m_pencil->getDrawEnd());
	ASSERT_EQ(QF(0, -1), m_pencil->getPhysicalStart());
	ASSERT_EQ(QF(1, 2), m_pencil->getPhysicalEnd());
}

TEST_F(ShapePencilTest, MoveUpperRightPencil)
{
	m_pencil->moveUpperRight(QF(1, 1));
	m_pencil->drawPointToPhysicalPoint(m_radio);
	ASSERT_EQ(QF(-1, 0), m_pencil->getDrawStart());
	ASSERT_EQ(QF(2, 1), m_pencil->getDrawEnd());
	ASSERT_EQ(QF(-1, 0), m_pencil->getPhysicalStart());
	ASSERT_EQ(QF(2, 1), m_pencil->getPhysicalEnd());
}

TEST_F(ShapePencilTest, ScalePencil)
{
	m_radio = 1;
	m_pencil->scale(m_radio, m_radio);
	ASSERT_EQ(QF(-2, -2), m_pencil->getDrawStart());
	ASSERT_EQ(QF(2, 2), m_pencil->getDrawEnd());
	ASSERT_EQ(QF(-1, -1), m_pencil->getPhysicalStart());
	ASSERT_EQ(QF(1, 1), m_pencil->getPhysicalEnd());
}

#endif // DEBUG