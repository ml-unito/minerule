#ifndef Sequence_H
#define Sequence_H 
#include <list>
#include <string>
#include <iostream>
#include "Database/ItemType.h"
#include "Database/SourceRowCommon.h"

namespace minerule {

  /**
   * The class that contains the concept of a FSTreeSequence, a sequenze is a ordered list of item
   */
  class FSTreeSequence{
  public:
    typedef std::list<ItemType> ListType;
  private:
      /**
       * The vector that represent a FSTreeSequence
       */
      ListType* seq;
  public:
    /**
     * this struct define the order between two FSTreeSequence
     */	
    struct less_sequence{
      bool operator()(const FSTreeSequence& first, const FSTreeSequence& second){			
	return (first < second);
      }
    };

    /**
     * the constructor of an object of type FSTreeSequence
     */
    FSTreeSequence() {
      seq=new ListType();
    };

    /**
     *	A copy constructor
     *      @param in a FSTreeSequence object
     */
    FSTreeSequence(const FSTreeSequence &in){
      seq=new ListType();
      *seq = *in.seq;
    };
		
    /**
     *	This constructor allows to create a new FSTreeSequence object that contains only n_item
     *	from FSTreeSequence in starting from the item in position start
     *      @param in the FSTreeSequence from i want to copy n_item items
     *	@param start the position from start to copy items
     *	@param n_item the numbers of item that i want to copy
     */
    FSTreeSequence(const FSTreeSequence &in, size_t start, size_t n_item);

    /**
     * The distructor of a FSTreeSequence
     */
    ~FSTreeSequence();
		
    /**
     * this method return a pointer to the vector that contains the items as string
     * @return a pointer of a vector ofstd::string that contains items
     */
    ListType* getFSTreeSequence();

    /**
     *  with this method you can add an item at the bottom of a FSTreeSequence
     *  @param the item s that you want to append at the FSTreeSequence
     */
    void add(const ItemType& s);

		
    /**
     * a simple method for print to standard output the items
     */
    void stampa();
		
    /**
     * the number of items in this FSTreeSequence
     * @return the number of items in this FSTreeSequence
     */
    size_t size();

    /**
     * remove the first element in the FSTreeSequence
     * @return the first element in the FSTreeSequence
     */
    ItemType removeHead();
		
    /**
     * insert an item in the front of the FSTreeSequence
     * @param s the item that i want insert
     */
    void insertHead(const ItemType& s);

    /**
     * @return astd::string that contains the items in this FSTreeSequence
     */
    std::string toStdString();
		
    /**
     * delete all the items in this FSTreeSequence, after this operation the FSTreeSequence is empty
     */
    void svuota();
		
    /**
     * the operator ==
     * @return true if the two FSTreeSequence are equal (same element and same size), false otherwise
     */
    bool operator==(FSTreeSequence& s);

    /**
     * decompose astd::string in a FSTreeSequence
     * every item in thestd::string must be separated with a space
     * @param s thestd::string that you want decompose
     */
    void read(odbc::ResultSet* rs, const HeadBodySourceRowDescription&);

    // ItemType pre_proc();
    /**
     * operator<
     * @return true if this is less than in otherwise return false, a FSTreeSequence is less than another if the size is less than another in the case the size is equal i consider the lexographyc order
     */
    bool operator<(const FSTreeSequence& in) const;

  };

} //namespace

#endif


