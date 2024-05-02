
#pragma once
#include <string>
using namespace std;

// This file is for creating a basic template for storing a users data. 
struct account {
public:
	string username;
	string password;
	string name;
	account(string uN = "", string pW = "", string n = "") {
		username = uN; password = pW; name = n;
	}
};