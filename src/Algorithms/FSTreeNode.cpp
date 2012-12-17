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
