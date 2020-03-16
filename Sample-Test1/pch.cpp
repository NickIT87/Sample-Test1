//
// pch.cpp
// Include the standard header and generate the precompiled header.
//

#include "pch.h"

using namespace std;


class MyClass {
	string id;
public:
	MyClass(string _id) : id(_id) {}
	string GetId() { return id; }
};

TEST(CodesBayClasses, checkID) {
	// Arrange
	MyClass mc("root");

	// Act
	string value = mc.GetId();

	// Assert
	//ASSERT_STREQ(value.c_str(), "root");
	EXPECT_STREQ(value.c_str(), "root");
}


int main(int argc, char* argv[])
{
	cout << "Hello google test" << endl;

	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}