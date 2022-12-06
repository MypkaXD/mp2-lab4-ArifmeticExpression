#include <gtest.h>
#include "ArifmeticExpression.h"


int main(int argc, char** argv)
{

	Lecsems lecsems("1+2*3");

	Arithmetic—alculation other = lecsems;
	other.getPostfix();
	other.printPostfix();
	std::cout << std::endl;
	std::cout << other.calculating() << std::endl;

	//::testing::InitGoogleTest(&argc, argv);
	//return RUN_ALL_TESTS();

}