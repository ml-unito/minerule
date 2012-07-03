#include "bodymap.h"
#include "constraints.h"

bool SumLessThan::check (BodyMap& itemMap, vector<ItemType>& items) {
	int sum = 0;
	for (vector<ItemType>::iterator i = items.begin(); i != items.end() && sum < value; i++)
		sum += itemMap[*i].attribute[attributeIndex].minValue();
	return sum < value;
}

bool SumLessThan::check (Transaction& items) {
	int sum = 0; int j = 0;
	for (Transaction::iterator i = items.begin(); i != items.end() && sum < value; i++)
		sum += items.values[j++];
		//sum += items.values[attributeIndex][j++];
	return sum < value;
}

bool SumLessEq::check (BodyMap& itemMap, vector<ItemType>& items) {
	int sum = 0;
	for (vector<ItemType>::iterator i = items.begin(); i != items.end() && sum <= value; i++)
		sum += itemMap[*i].attribute[attributeIndex].minValue();
	return sum <= value;
}

bool SumLessEq::check (Transaction& items) {
	int sum = 0; int j = 0;
	for (Transaction::iterator i = items.begin(); i != items.end() && sum <= value; i++)
		sum += items.values[j++];
		//sum += items.values[attributeIndex][j++];
	return sum <= value;
}

bool SumGreaterThan::check (BodyMap& itemMap, vector<ItemType>& items) {
	int sum = 0;
	for (vector<ItemType>::iterator i = items.begin(); i != items.end() && sum <= value; i++)
		sum += itemMap[*i].attribute[attributeIndex].maxValue();
	return sum > value;
}

bool SumGreaterThan::check (Transaction& items) {
	int sum = 0; int j = 0;
	for (Transaction::iterator i = items.begin(); i != items.end() && sum <= value; i++)
		sum += items.values[j++];
		//sum += items.values[attributeIndex][j++];
	return sum > value;
}

bool SumGreaterEq::check (BodyMap& itemMap, vector<ItemType>& items) {
	int sum = 0;
	for (vector<ItemType>::iterator i = items.begin(); i != items.end() && sum <= value; i++)
		sum += itemMap[*i].attribute[attributeIndex].maxValue();
	return sum >= value;
}

bool SumGreaterEq::check (Transaction& items) {
	int sum = 0; int j = 0;
	for (Transaction::iterator i = items.begin(); i != items.end() && sum <= value; i++)
		sum += items.values[j++];
		//sum += items.values[attributeIndex][j++];
	return sum >= value;
}

