//   Minerule - a sql-like language for datamining
//   Copyright (C) 2013 Roberto Esposito (esposito@di.unito.it)
//
//   This program is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
//   (at your option) any later version.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with this program.  If not, see <http://www.gnu.org/licenses/>.
#include "minerule/Algorithms/Bodymap.hpp"
#include "minerule/Utils/Constraints.hpp"

namespace minerule {



bool SumLessThan::check (BodyMap& itemMap, std::vector<ItemType>& items) {
	int sum = 0;
	for(std::vector<ItemType>::iterator i = items.begin(); i != items.end() && sum < value; i++)
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

bool SumLessEq::check (BodyMap& itemMap, std::vector<ItemType>& items) {
	int sum = 0;
	for (std::vector<ItemType>::iterator i = items.begin(); i != items.end() && sum <= value; i++)
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

bool SumGreaterThan::check (BodyMap& itemMap, std::vector<ItemType>& items) {
	int sum = 0;
	for (std::vector<ItemType>::iterator i = items.begin(); i != items.end() && sum <= value; i++)
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

bool SumGreaterEq::check (BodyMap& itemMap, std::vector<ItemType>& items) {
	int sum = 0;
	for (std::vector<ItemType>::iterator i = items.begin(); i != items.end() && sum <= value; i++)
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

} //end namespace
