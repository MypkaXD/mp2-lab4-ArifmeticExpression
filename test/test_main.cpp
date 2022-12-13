#include <gtest.h>
#include "ArifmeticExpression.h"


int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();

	system("pause");
	std:: cout << "Для продолжения работы программы нажмите любую клавишу...";
	std::cin.get();
}