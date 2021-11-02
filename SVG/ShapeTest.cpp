#include "gtest/gtest.h"

#include "shape.h"
int Factorial(int a)
{
	return a;
}

TEST(FactorialTest, HandlesPositiveInput)
{
	EXPECT_EQ(Factorial(1), 1);
	EXPECT_EQ(Factorial(2), 2);
	EXPECT_EQ(Factorial(3), 3);
	EXPECT_EQ(Factorial(8), 8);
}