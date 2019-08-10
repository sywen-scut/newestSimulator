// #pragma once
#include <algorithm>
#include <functional>
#include <assert.h>
#include <vector>
#include <fstream>
using namespace std;
template <typename T>
std::vector<T> operator+ (const std::vector<T>& a, const std::vector<T>& b) 
{
	vector<T> result;
	assert(a.size() == b.size());
	int numElements = a.size();
	for (int i = 0; i < numElements; i++) {
		result.push_back(a[i] + b[i]);
	}
	return result;
};

template <typename T>
std::vector<T> operator-(const std::vector<T>& a, const std::vector<T>& b)
{
	vector <T> result;
	assert(a.size() == b.size());
	int numElements = a.size();
	for (int i = 0; i < numElements; i++) {
		result.push_back(a[i] - b[i]);
	}
	return result;
};

enum State { unmapped,waiting, receiving, executing,tranfering, finished,running };

