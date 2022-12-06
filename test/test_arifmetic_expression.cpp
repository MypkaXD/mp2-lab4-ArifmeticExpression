#include <gtest.h>
#include "ArifmeticExpression.h"

TEST(Lecsems, can_return_false_if_wrong_expression_v1) {
	ASSERT_ANY_THROW(Lecsems l("2..22"));
}

TEST(Lecsems, can_return_false_if_wrong_expression_v2) {
	ASSERT_ANY_THROW(Lecsems l("."));
}

TEST(Lecsems, can_return_false_if_wrong_expression_v3) {
	ASSERT_NO_THROW(Lecsems l("2.25+4.4+(123.2*3)"));
}

TEST(ArithmeticÑalculation, can_get_postfix) {

	Lecsems l("2+3*4");

	ArithmeticÑalculation ac(l);
	ac.getPostfix();

	std::string temp1 = ac.getPost();
	std::string temp2 = "234*+";

	ASSERT_EQ(temp1, temp2);
}

TEST(ArithmeticÑalculation, can_calculating) {

	Lecsems l("2+3*4");

	ArithmeticÑalculation ac(l);
	ac.getPostfix();

	ASSERT_EQ(ac.calculating(), "14.000000");
}