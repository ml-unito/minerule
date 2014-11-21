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
#ifndef NODE_H
#define NODE_H

#include <vector>
#include "minerule/Database/ItemType.hpp"


namespace minerule {
  /**
   * the node of the tree	
   */
  class FSTreeNode{
  private:
    /**
     * a pointer of the child node
     */
    std::vector<FSTreeNode* >* child;
    /**
     * the label associate at this node
     */
    ItemType label;
    /**
     * the count of the sequence that start from root and terminted at this node
     */
    int count;
    /**
     * a pointer to the parent of this node
     */
    FSTreeNode* parent;		
  public: 
    /**
     * constructor
     * @param l the label of this node
     */
    FSTreeNode(const ItemType& l);
			
    /**
     * constructor
     */
    FSTreeNode();

    /**
     * constructor
     * @param p the father of this node
     */
    FSTreeNode(FSTreeNode* p);

    /**
     * the destructor
     */
    ~FSTreeNode();
		
    /**
     * @return the label of this node
     */			
    const ItemType& getLabel();

    /**
     * @return a pointer to the vector of the child node
     */
    std::vector<FSTreeNode*>* getChild();

    /**
     * @return value of variable count
     */
    int getCount();

    /**
     * @return a pointer to the parent node
     */
    FSTreeNode* getParent();
			
    /**
     * @param n the count of this node
     */
    void setCount(int n);
			
    /**
     * @param s the parent node
     */
    void setParent(FSTreeNode* s);

    /**
     * @param s the node that i want insert as a children of this node
     */
    void insertChild(FSTreeNode* s);
  };

} // minerule
#endif

