//
// pch.h
// Header for standard system include files.
//

#pragma once

#include "gtest/gtest.h"

#include <iostream>

struct BankAccount
{
	int balance = 0;

	BankAccount()
	{

	}

	explicit BankAccount(const int balance)
		:balance{ balance }
	{

	}
};

struct BankAccountTest : testing::Test
{
	BankAccount* account;

	BankAccountTest()
	{
		account = new BankAccount;
	}

	~BankAccountTest()
	{
		delete account;
	}
};