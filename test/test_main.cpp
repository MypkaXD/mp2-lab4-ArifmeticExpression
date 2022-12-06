#include <gtest.h>
#include "ArifmeticExpression.h"


int main(int argc, char** argv)
{

	//setlocale(LC_ALL, "Russian");

	//Lecsems lecsems("3+4*5-12");
	//
	//std::cout << lecsems << std::endl;
	//
	//ArithmeticÑalculation other = lecsems;
	//other.getPostfix();
	//other.printPostfix();
	//std::cout << std::endl;
	//std::cout << other.calculating() << std::endl;

	//Lecsems l("2+3*4");
	//ArithmeticÑalculation ac(l);
	//std::cout << ac.getPost() << std::endl;

	//Lecsems l("2+3*4");
	//ArithmeticÑalculation ac(l);
	//ac.getPostfix();
	//std::string temp1 = ac.getPost();
	//std::cout << temp1 << "   asdasdsad";
	//std::string temp2 = "234*+";

	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();

}