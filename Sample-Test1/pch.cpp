//
// pch.cpp
// Include the standard header and generate the precompiled header.
//

#include "pch.h"

using namespace std;

TEST( CodesBay1, Subtest1) {
	ASSERT_EQ(1, 2);
	cout << "After assertion 1" << endl;
}

TEST(CodesBay2, Subtest2) {
	EXPECT_EQ(1, 2);
	cout << "After assertion 2" << endl;
}

int main(int argc, char* argv[])
{
	cout << "Hello google test" << endl;

	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}