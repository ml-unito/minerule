//   Minerule - a sql-like language for datamining
//   Copyright (C) 2013 Dino Ienco (dino.ienco@teledetection.fr)
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
#include "minerule/Algorithms/FSTreeNode.h"
#include <iostream>


namespace minerule {

FSTreeNode::FSTreeNode(const ItemType& l){
  child = new std::vector<FSTreeNode*>();
  count=0;
  label=l;
}
	
FSTreeNode::FSTreeNode(){
  child = new std::vector<FSTreeNode*>();
  count=0;
}
		
FSTreeNode::FSTreeNode(FSTreeNode* p){
  parent = p;
  count=0;
  child = new std::vector<FSTreeNode*>();
}

FSTreeNode::~FSTreeNode(){
  FSTreeNode* temp;
  for (size_t i=0;i<child->size();i++){
    temp=(*child)[i];
    delete temp;
  }
  delete child;
				
}

const ItemType& FSTreeNode::getLabel() { 
  return label;
}

std::vector<FSTreeNode*>* FSTreeNode::getChild(){ 
  return child; 
}

int FSTreeNode::getCount() {
  return count;
}

FSTreeNode* FSTreeNode::getParent() {
  return parent;
}

void FSTreeNode::setCount(int n) {
  count = n;
}
			
void FSTreeNode::setParent(FSTreeNode* s) {
  parent = s;
}

void FSTreeNode::insertChild(FSTreeNode* s) { 
  child->push_back(s);
}

} // namespace
