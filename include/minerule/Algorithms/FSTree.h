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
#ifndef FSTREE_H
#define FSTREE_H 
#include <fstream>
#include <map>
#include <iostream>
#include <string>
#include <vector>
#include "FSTreeNode.h"
#include "FSTreeSequence.h"
#include "minerule/Optimizer/OptimizedMinerule.h"
//#include "sequenceList.h"

namespace minerule {

/**
 * this class implement the algorithm of FS-MINER (without the incremental part)
 */

class FSTree{
 private:
		
  /**
   * this map contains only the sequence of two elements that i found in the dataset
   */
  std::map<FSTreeSequence,int,FSTreeSequence::less_sequence>* link_S;
  /**
   * this map contains the sequence of two elements with support lower than the threshold
   * a problem is that contains as more elements as link_S, but the element with support > threshold
   * as value equal to 0
   */	
  std::map<FSTreeSequence,int,FSTreeSequence::less_sequence>* link_NS;

  /**
   * this map contains all the potential sequences up to the threshold (a subset of this sequences are the very resultSet)
   */	
  std::map<FSTreeSequence,int,FSTreeSequence::less_sequence>* result;
	 
  /**
   * this object contains all the sequence that hava a support > a threshold and the pointers to the node in the
   * tree (where these items are)
   */
  //sequenceList* m_list;
  std::map<FSTreeSequence, std::vector<FSTreeNode*>*, FSTreeSequence::less_sequence> m_list;
  /**
   * the source file
   */
  std::string source;
	 
  /**
   * the root of the tree
   */
  FSTreeNode* root;
	 
  /**
   * the number of sequence in the dataSet
   */
  int num;
  /**
   * the threshold
   */
  double threshold;

  int n_nodi;
	
  int max_length;
 public:

  /**
   * the constructor of this structure
   * @param in the name of source file
   */
  FSTree(const OptimizedMinerule& opt) : 
    root(new FSTreeNode()), 
    num(0), 
    threshold(0) {
    root= new FSTreeNode();
    num=0;	
    threshold=0;

    max_length = opt.getParsedMinerule().bodyCardinalities.getMax();
    link_S= new std::map<FSTreeSequence,int,FSTreeSequence::less_sequence>();
    link_NS=new std::map<FSTreeSequence,int,FSTreeSequence::less_sequence>();
    result = new std::map<FSTreeSequence,int,FSTreeSequence::less_sequence>();
  }
  /**
   * this method permit to set the threshold
   * @param t  the threshold as percentage
   */
  void setThreshold(double t){
    threshold=t*num;
    //threshold=t;
  }


  /**
   * @return the number of the node in the tree
   */
  int getN_nodi();
	
  /**
   * reset the variable that contains the numbers of the node in tree
   */
  void resetN_nodi();

  /**
   * this method decompose a sequence in a number of sequences, this number is unknow.
   * With the map of non frequent couple, this method decompose the input sequence s in more
   * sequence. The characteristics of each of these sequences is that:
   * - the cardinality of each sequence is major of one
   * - each sequence is a possible frequent sequence, because it no contains an infrequent couple
   * @param s a pointer of a sequence
   * @return a vector of possible frequent sequence
   */
	
  std::vector<FSTreeSequence*>*  fraziona(FSTreeSequence *s);
  /**
   * insert all couple present in the dataSet in the map link_S
   */
  void createLinkSTable(odbc::ResultSet*, const SourceRowColumnIds&);
	
  /**
   * insert all no frequent couple in the map link_NS
   */	
  void createLinkNSTable();
	
  /**
   * this method decompose a sequence in all the couple that it contains for adding the couple in the link_S
   * @see createLinkSTable()
   * @param s the sequence from which i want extract couples
   */
  void insertLink(FSTreeSequence* s);

  /**
   * this method read a branch of the tree from the bottom and add all the sequence that it reads in the set of
   *possible frequent sequence
   * @param n the node from i starting to read a possible frequent sequent, i start to read from this node a sequence with size 2, after a sequence with size 3, ecc..
   */
  void addResult(FSTreeNode* n);
  /**
   * @see addResults(std::vector<node*>* vec)
   * this method counts how many item there are from the node n and the root, it counts the possible sequence that the 
   * method addResult can extract from a branch
   * @param n the node from which i count the number of elements
   * @return the number of node between this node and the root
   */
  int countPath(FSTreeNode* n);

  void stampa(FSTreeNode* n);
	
  /**
   * @see addResult(node* n) 
   * this method call the method addResult for each element in the vector of node, the vector of node is in relation 
   * with a frequent couple in the m_list
   * @param vec the vector of node in relation with a frequent couple in the m_list
   */
  void addResults(std::vector<FSTreeNode*>* vec);
	
  /**
   * @see addResults(std::vector<node*>* vec)
   * @see addResult(node* n)
   * this method extract the frequent sequence from the tree structure
   */
  void mine();

  /**
   * @see addList(node* father, node* children) 
   * this method append a child to node r, the node creates contains thestd::string t
   * @param r the father of the node that the method create
   * @param t thestd::string that is contained in the new node
   * @return a pointer to the new node created
   */
  FSTreeNode* appendChild(FSTreeNode* r, const ItemType& t);

  /** 
   * this method add a new pointer to a node, in m_lsit, from the sequence of two elements that is formed with
   * thestd::string of the father plus thestd::string of the children
   * @param father a node that contains the firststd::string of the sequence of two elements
   * @param children a node that contains the secondstd::string of the sequence of two elements
   */
  void addList(FSTreeNode* father, FSTreeNode* children);

  /**
   * this method insert, in the tree, a sequence t starting from the node r
   * @see appendChild(node* r, std::string t)
   */
  void insertTree(FSTreeNode* r, FSTreeSequence* t);

  /**
   * construct the tree that contains a compressed rappresentation of the dataSet
   * @see fraziona(sequence *s)
   * @see insertTree(node* r, sequence* t)
   */
  void construct_Tree(odbc::ResultSet*, const SourceRowColumnIds&);

  /**
   * the destructor of the FSTree
   */
  ~FSTree();

  /**
   * @return the threshold,as number , that was set
   */
  double getThreshold();


  std::map<FSTreeSequence,int,FSTreeSequence::less_sequence>* getResult();



};

} //minerule

#endif

