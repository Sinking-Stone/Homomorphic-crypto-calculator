#pragma once

#include "seal/seal.h"
#include <iostream>
using namespace std;
using namespace seal;

class operation
{
private:


public:
	operation();
	~operation();
	void addOperation(vector<double>& input, IntArray<unsigned __int64>& cipher);
	void subOperation(vector<double>& input, IntArray<unsigned __int64>& cipher);
	void mulOperation(vector<double>& input, IntArray<unsigned __int64>& cipher);
	void divOperation(vector<double>& input, IntArray<unsigned __int64>& cipher);
	void indOperation(double input1, double input2, vector<double>& result, IntArray<unsigned __int64>& cipher);
	void expOperation(double x, vector<double>& result, IntArray<unsigned __int64>& cipher);
};
