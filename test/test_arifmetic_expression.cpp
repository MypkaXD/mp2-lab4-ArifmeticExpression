#include <gtest.h>
#include "ArifmeticExpression.h"

TEST(Lecsems, can_return_false_if_wrong_expression_v1) {
	ASSERT_ANY_THROW(Lecsems l("2..22"));
}

TEST(Lecsems, can_return_false_if_wrong_expression_v2) {
	ASSERT_ANY_THROW(Lecsems l("."));
}

TEST(Lecsems, can_return_true_if_normal_expression) {
	ASSERT_NO_THROW(Lecsems l("2.25+4.4+(123.2*3)"));
}


TEST(ArithmeticÑalculation, can_calculating) {

	Lecsems l("2+3*4");

	ArithmeticÑalculation ac(l);
	ac.convertExpression();
	ac.getPostfix();

	ASSERT_EQ(ac.calculating(), "14.000000");
}

TEST(ArithmeticÑalculation, can_translate_to_postfix_1) {
	Lecsems l("2+3");
	ArithmeticÑalculation ex(l);

	ex.convertExpression();
	ex.getPostfix();

	std::string temp = "23+";

	EXPECT_EQ(ex.get_postfix(), temp);
}

TEST(ArithmeticÑalculation, can_translate_to_postfix_2) {
	Lecsems l("15/(7-(1+1))*3-(2+(1+1))*15/(7-(200+1))*3-(2+(1+1))*(15/(7-(1+1))*3-(2+(1+1))+15/(7-(1+1))*3-(2+(1+1)))");
	ArithmeticÑalculation ex(l);

	ex.convertExpression();
	ex.getPostfix();

	std::string tmp = "15 7 1 1 + - / 3 * 2 1 1 + + 15 * 7 200 1 + - / 3 * - 2 1 1 + + 15 7 1 1 + - / 3 * 2 1 1 + + - 15 7 1 1 + - / 3 * + 2 1 1 + + - * -";

	EXPECT_EQ(ex.get_postfix(), tmp);
}