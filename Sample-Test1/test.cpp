#include "pch.h"

using namespace std;

TEST(TestCaseName_first, TestName_AssertionsReview) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}

TEST(AccountTest, BankAccountStartsEmpty) {
    BankAccount account;
    EXPECT_EQ(0, account.balance);
}

TEST_F(BankAccountTest, BankAccountStartsEmpty) {
    EXPECT_EQ(0, account->balance);
}

TEST(CodesBay1, Subtest1) {
    ASSERT_EQ(1, 2);
    cout << "After assertion 1" << endl;
}

TEST(CodesBay2, Subtest2) {
    EXPECT_EQ(1, 2);
    cout << "After assertion 2" << endl;
}