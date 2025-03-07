#include "algebra.h"

#include <gtest/gtest.h>
#include <iostream>

int main(int argc, char **argv) {
	if (false) // make false to run unit-tests
	{
		// debug section

		std::cout << "Test" << std::endl;
	} else {
		::testing::InitGoogleTest(&argc, argv);
		std::cout << "RUNNING TESTS ..." << std::endl;
		int ret{RUN_ALL_TESTS()};
		if (!ret)
			std::cout << "<<<SUCCESS>>>" << std::endl;
		else
			std::cerr << "FAILED" << std::endl;
	}
	return 0;
}
