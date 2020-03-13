#include "pch.h"

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
