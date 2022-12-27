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


TEST(Arithmetic�alculation, can_calculating_numbers) {

	Lecsems l("2+3*4");

	Arithmetic�alculation ac(l);

	ASSERT_EQ(ac.calculating(), "14.000000");
}

TEST(Arithmetic�alculation, can_translate_to_postfix_1) {
	Lecsems l("2+3");
	Arithmetic�alculation ex(l);

	ex.getPostfix();

	std::string temp = "23+";

	EXPECT_EQ(ex.getStringOfPostfix(), temp);
}

TEST(Arithmetic�alculation, can_translate_to_postfix_2) {
	Lecsems l("15/(7-(1+1))*3-(2+(1+1))*15/(7-(200+1))*3-(2+(1+1))*(15/(7-(1+1))*3-(2+(1+1))+15/(7-(1+1))*3-(2+(1+1)))");
	Arithmetic�alculation ex(l);

	ex.getPostfix();

	std::string temp = "15711+-/3*211++15*72001+-/3*-211++15711+-/3*211++-15711+-/3*+211++-*-";

	EXPECT_EQ(ex.getStringOfPostfix(), temp);
}

TEST(Arithmetic�alculation, can_calculating_func_of_sin) {

	Lecsems l("sin(30)");

	Arithmetic�alculation ac(l);

	ASSERT_EQ(ac.calculating(), "-0.988032");
}

TEST(Arithmetic�alculation, can_calculating_fact) {

	Lecsems l("10!");

	Arithmetic�alculation ac(l);

	ASSERT_EQ(ac.calculating(), "3628800.000000");
}

TEST(Arithmetic�alculation, can_calculating_pow) {

	Lecsems l("2^10");

	Arithmetic�alculation ac(l);

	ASSERT_EQ(ac.calculating(), "1024.000000");
}

TEST(Arithmetic�alculation, can_calculating_difficult_pow) {

	Lecsems l("10^(10+1/2-1/2)");

	Arithmetic�alculation ac(l);

	ASSERT_EQ(ac.calculating(), "10000000000.000000");
}

TEST(Arithmetic�alculation, can_calc_with_const) {

	Lecsems l("Pi+30");

	Arithmetic�alculation ac(l);

	ASSERT_EQ(ac.calculating(), "33.140000");
}