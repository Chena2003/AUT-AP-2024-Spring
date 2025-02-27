#include <gtest/gtest.h>

#include <iostream>

#include "Account.h"
#include "Bank.h"
#include "Person.h"
#include "Utils.h"

int main(int argc, char **argv) {
	if (false)  // make false to run unit-tests
	{
		// debug section
		Person p1("Chena", 18, "Male", "hdiahid", 9, true);
		Person p2("Chenaa", 19, "Male", "hdiahid", 9, true);

		p1.get_info();
		p2.get_info();

		if (p1 <=> p2 == 0) {
			std::cout << "p1 == p2" << std::endl;
		}

		Account ac1(&p1, nullptr, "123456789");
		Account ac2(&p2, nullptr, "123456789");

		ac1.get_info();
		ac2.get_info();


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
