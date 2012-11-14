#ifndef __SUBSET_ITERATOR__
#define __SUBSET_ITERATOR__

namespace minerule {

class SubsetIterator {
 public:
  //typedef std::set<ItemType> ItemTypeSet;

  ItemSet itemset;
  size_t state;
 public:
  SubsetIterator(const ItemSet& its) : itemset(its), state(0) {}

  bool ok() { return (unsigned int)(1<<itemset.size())>state; }

  bool next() { state++; return ok(); }

  void getCurrentSet(ItemSet& result) const {
    size_t setsize=itemset.size();
    ItemSet::const_iterator it = itemset.begin();
    for( size_t i=0; i<setsize; i++, it++ ) {
      if( 1<<i & state ) {
	result.push_back(*it);
      }
    }
  }
};

}

#endif
