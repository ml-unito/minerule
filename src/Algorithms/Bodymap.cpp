#include "Algorithms/Bodymap.h"
#include "Utils/Constraints.h"
#include <algorithm>

namespace minerule {

int BodyMap::nextid = 0;

void BodyMapElement::insert(const ItemType& item, const int gid, bool secondPass) {
	map<ItemType, MapElement>::iterator found = heads.find(item);
	if (found == heads.end()) {
		if (!secondPass) heads[item].insert(gid);
	} else found->second.insert(gid);
}

bool BodyMapElement::pruneMap (double threshold, bool onlyBody) {
	if (!moreThan(threshold)) return false;
	else if (onlyBody) return true;
	map<ItemType, MapElement> newMap;
	for (map<ItemType, MapElement>::iterator i = heads.begin(); i != heads.end(); i++)
		if (i->second.counter >= threshold) {i->second.counter = 0; newMap[i->first] = i->second;}
	heads = newMap;
	return heads.size() > 0;
}

bool BodyMapElement::updateCount () {
//	if (size() < threshold) return false;
//	map<ItemType, MapElement> newMap;
//	for (map<ItemType, MapElement>::iterator i = heads.begin(); i != heads.end(); i++)
//{
//		i->second.counter += i->second.count(true);
//}
//		if (i->second.size() >= threshold) newMap[i->first] = i->second;
//	heads = newMap;
	counter += count(true);
	return heads.size() > 0;
}

/*
void BodyMapElement::saveMap (std::ostream& out, bool withGids) {
	out << ' ' << heads.size();
	for (map<ItemType, MapElement>::iterator i = heads.begin(); i != heads.end(); i++) {
		out << ' ' << i->first;
		out << ' ' << i->second.size();
	if (withGids) {
		out  << ' ';
		copy(i->second.begin(), i->second.end(), std::ostream_iterator<ItemType>(out, " "));
	}
		i->second.erase(i->second.begin(), i->second.end());
	}
	out << std::endl;
}
*/

/*
void BodyMapElement::loadMap (istream& in, bool withGids) {
	int n,m; in >> n;
	ItemType item, gid;
	for (int i=0; i<n; i++) {
		in >> item;
		in >> m;
		heads[item].counter += m;
		if (withGids) {
		in >> m;
		for (int j=0; j<m; j++) { in >> gid; heads[item].insert(gid); }
		} //else { me = heads[item]; }
	}
}
*/

void BodyMapElement::setMinMax (int which, int v) {
	MinMax mm;
	for (int i=attribute.size(); i<=which; i++) {
		attribute.insert(attribute.end(),mm);
	} 
	if (attribute[which].minValue() > v) attribute[which].min = v;
	if (attribute[which].maxValue() < v) attribute[which].max = v;
}

int BodyMap::add(Transaction& t1, const int gid) {
	int howMany = 0;
	for (Transaction::iterator i = t1.begin(); i != t1.end(); i++) {
		map<ItemType, BodyMapElement>::iterator found = find(*i);
		if (found == end()) {
			BodyMapElement bme;
//			bme.insert(gid);
			(*this)[*i] = bme;
			found = find(*i);
		}
		found->second.insert(gid);
		int n = t1.values.size()/t1.size();
		for (int k=0; k < n; k++) {
			found->second.setMinMax(k,t1.values[k+howMany*n]);
		}
		howMany++;
	}
	return howMany;
}

int BodyMap::add(const int gid, Transaction& t1, Transaction& t2, bool secondPass) {
	int howMany = 0;
	for (Transaction::iterator i = t1.begin(); i != t1.end(); i++) {
		map<ItemType, BodyMapElement>::iterator found = find(*i);
		if (found == end()) {
			if (secondPass) continue;
			BodyMapElement bme;
//			bme.insert(gid);
			(*this)[*i] = bme;
			found = find(*i);
		}
		found->second.insert(gid);
	        for (Transaction::iterator j = t2.begin(); j != t2.end(); j++)
			if (*i != *j /*&& i->price < j->price*/) {
				howMany++;
				found->second.insert(*j,gid,false);
//				found->second.insert(*j,me,secondPass);
			}
	}
	return howMany;
}

/*
int BodyMap::add(const int gid, ItemType& t1, ItemType& t2, bool secondPass) {
	int howMany = 0;
	map<ItemType, BodyMapElement>::iterator found = find(t1);
	if (found == end() && !secondPass) {
		BodyMapElement bme;
//		bme.insert(gid);
		(*this)[t1] = bme;
		found = find(t1);
	}
	found->second.insert(gid);
	if (t1 != t2) {
		howMany++;
		found->second.insert(t2,gid,secondPass);
	}
	return howMany;
}

int BodyMap::add(ItemType& gid, Transaction& t1) {
	int howMany = 0;
	BodyMapElement bme;
//	bme.insert(gid);
	(*this)[gid] = bme;
	map<ItemType, BodyMapElement>::iterator found = find(gid);
	for (Transaction::iterator i = t1.begin(); i != t1.end(); i++) {
		found->second.insert(*i);
		howMany++;
	}
	return howMany;
}
int BodyMap::add(const int gid, Transaction& t1, bool secondPass) {
	int howMany = 0;
	ItemType prev;
	map<ItemType, BodyMapElement>::iterator found;
	for (Transaction::iterator i = t1.begin(); i != t1.end(); i++) {
		if (prev != *i) {
			found = find(*i);
			if (found == end()) {
				if (secondPass) continue;
				BodyMapElement bme;
//			bme.insert(gid);
				(*this)[*i] = bme;
				found = find(*i);
			}
			prev = *i;
			found->second.insert(gid);
		}
		if (*i != *i) {
			howMany++;
			found->second.insert(*i,gid,secondPass);
		}
	}
	return howMany;
}
*/

/*
void BodyMap::saveMap (std::ostream& out, bool withGids) {
	for (iterator i = begin(); i != end(); i++) {
		out << i->first;
		i->second.saveMap(out, withGids);
	}
}
*/
/*
void BodyMap::loadMap (istream& in, bool withGids) {
	ItemType item;
	while (!in.eof()) {
		in >> item;
		if (in.eof()) break;
		(*this)[item].loadMap(in, withGids);
	}
}
*/

void BodyMap::pruneMap (double threshold, bool onlyBody) {
	//BodyMap newMap(outfile, totGroups);
	for (iterator i = begin(); i != end(); i++)
		if (!i->second.pruneMap(threshold,onlyBody)) {
		//	 newMap[i->first] = i->second;
			i->second.done = true;
		} else i->second.done = false;
	//(*this) = newMap;
}

void BodyMap::updateCount () {
	for (iterator i = begin(); i != end(); i++)
		i->second.updateCount();
}

int BodyMap::buildRules (NewRuleSet& rs1, NewRule& rc, std::vector<BodyMap::iterator>& vb, std::vector<BodyMap::iterator>& vh, int maxBodyCard, int maxHeadCard, float threshold, int j) {
 std::vector<BodyMap::iterator> newVb, newVh;
	NewRuleSet rs;
	int start = rs.size();
	bool intersect = (rc.gids.size() > 0 );
	for (size_t k=j+1; k<vb.size(); k++) {
		//GidList newGidList;
		GidList newGidList(vb[k]->second);
		if (intersect)
			newGidList &= rc.gids;
//		else newGidList = v[k]->second;
		if (newGidList.moreThan(threshold)) {
			NewRule r(rc,vb[k],newGidList,newVb.size());
			rs.insert(rs.end(),r);
			newVb.insert(newVb.end(),vb[k]);
		}
	}
	int nrules = rs.size();
	int howManyRules = 0;
	for (int i=start; i<nrules; i++) {
	    if (i<nrules) {
		NewRuleSet rs2;
//		curBodySupp = rs[i].gids.count(true);
		newVh = buildHead(rs2,rs[i],vh,maxHeadCard,threshold,-1);
		howManyRules += rs2.size();
		saveRules(rs2, rs[i].gids.count());
	     }
		if (newVb.size() > 0 && maxBodyCard > 1) {
			howManyRules += buildRules(rs1, rs[i],newVb,newVh,maxBodyCard-1,maxHeadCard,threshold,rs[i].bodySupp);
		}
	}
	return howManyRules;
}
 std::vector<BodyMap::iterator> BodyMap::buildHead (NewRuleSet& rs, NewRule rc, std::vector<BodyMap::iterator>& v, int maxCard, float threshold, int j) {
 std::vector<BodyMap::iterator> newV;
//	NewRuleSet rs;
	int start = rs.size();
	for (size_t k=j+1; k<v.size(); k++)// {
		//GidList newGidList;
	if (std::find(rc.body.begin(), rc.body.end(),v[k]->first) == rc.body.end()) {
		GidList newGidList(v[k]->second);
	//	if (rc.gids.count(true) > 0 )
			newGidList &= rc.gids;
//		else newGidList = v[k]->second;
		if (newGidList.moreThan(threshold)) {
			NewRule r(rc,v[k],newV.size(),newGidList);
			rs.insert(rs.end(),r);
			newV.insert(newV.end(),v[k]);
		}
	}
	int nrules = rs.size();
	if (newV.size() > 0 && 1 < maxCard) {
		for (int i=start; i<nrules; i++)
			buildHead(rs,rs[i],newV,maxCard-1,threshold,rs[i].bodySupp);
	}
	return newV;
}

bool BodyMap::checkAntiMono (NewRule& rc) {
	bool flag = true;
	for (size_t i=0; i<antiMonoConstr.size() && flag; i++)
		flag = flag && antiMonoConstr[i]->check(*this,rc.body);
	return flag;
}

bool BodyMap::checkMono (NewRule& rc) {
	bool flag = true;
	for (size_t i=0; i<monoConstr.size() && flag; i++)
		flag = flag && monoConstr[i]->check(*this,rc.body);
	return flag;
}

int BodyMap::buildItemset (NewRuleSet& rs1, NewRule& rc, std::vector<BodyMap::iterator>& v, int maxCard, float threshold, int j) {
 std::vector<BodyMap::iterator> newV;
	NewRuleSet rs;
	int start = rs.size();
	bool intersect = rc.gids.size() > 0;
	int howManyRules = 0;
	size_t l = rc.body.size()+1;
	for (size_t k=j+1; k<v.size(); k++) {
		//GidList newGidList;
		//GidList newGidList(v[k]->second);
		NewRule r(rc,v[k],v[k]->second,newV.size());
		if (!checkAntiMono(r)) continue;
		r.satisfy = r.satisfy || checkMono(r);
		if (intersect)
			r.gids &= rc.gids;
//		else newGidList = v[k]->second;
		if (r.gids.moreThan(threshold)) {
			//NewRule r(rc,v[k],newGidList,newV.size());
			if(r.satisfy) howManyRules ++;
			rs.insert(rs.end(),r);
			newV.insert(newV.end(),v[k]);
		}
	}
	int nrules = rs.size();
	while (l >= itemsets.size()) itemsets.insert(itemsets.end(),0);
	itemsets[l] += nrules;
	saveItemsets(rs, -1);
	if (newV.size() > 0 && 1 < maxCard) {
		for (int i=start; i<nrules; i++) {
			//cout << rs[i];
			howManyRules += buildItemset(rs,rs[i],newV,maxCard-1,threshold,rs[i].bodySupp);
		}
	}
	return howManyRules;
}

int BodyMap::generateStartItemSets (NewRuleSet& rs1, NewRule& rc, std::vector<BodyMap::iterator>& v, int maxCard, float threshold, int j) {
 std::vector<BodyMap::iterator> newV;
	NewRuleSet rs;
	//	int start = rs.size();
	bool intersect = rc.gids.size() > 0;
	int howManyRules = 0;
	size_t l = rc.body.size()+1;
	while (l >= itemsets.size()) itemsets.insert(itemsets.end(),0);
	size_t k=j+1;
	if (k<v.size()) {
		//GidList newGidList;
		//GidList newGidList(v[k]->second);
		NewRule r(rc,v[k],v[k]->second,k);
		if (checkAntiMono(r)) {
		r.satisfy = r.satisfy || checkMono(r);
		if (intersect)
			r.gids &= rc.gids;
//		else newGidList = v[k]->second;
		if (r.gids.moreThan(threshold)) {
			//NewRule r(rc,v[k],newGidList,newV.size());
			if(r.satisfy) {
			//	rs.insert(rs.end(),r);
				//cout << r;
				saveItemset(r, -1);
				howManyRules += buildItemset(rs,r,v,maxCard-1,threshold,r.bodySupp) + 1;
				itemsets[l] += 1;
				//howManyRules += generateStartItemSets(rs,rc,v,maxCard,threshold,k);
			} else {
				howManyRules += generateStartItemSets(rs,r,v,maxCard-1,threshold,k);
				if (howManyRules == 0) return howManyRules;
			}
		}
		}
		howManyRules += generateStartItemSets(rs,rc,v,maxCard,threshold,k);
	}
	return howManyRules;
}

int BodyMap::buildRules (NewRuleSet& rs2, NewRule rc, BodyMap::iterator p, float threshold, int maxBody, int maxHead) {
	int howManyRules = 0;

	bool intersect = rc.gids.size() > 0;
	while (p != end()) {
		GidList newGidList(p->second);
		if (intersect) 
			newGidList &= rc.gids;
		//else newGidList = p->second;
		if (newGidList.moreThan(threshold)) {
			NewRule r(rc,p,newGidList);
			howManyRules += buildHead(rs2, threshold, maxHead, r.gids.count(true), r, p->second.heads.begin());
			p++;
		if (r.body.size() < (size_t)maxBody) {
			howManyRules += buildRules(rs2, r, p, threshold, maxBody, maxHead);
		}
		} else p++;
	}
	return howManyRules;
}

int BodyMap::buildHead (NewRuleSet& rs, float threshold, int maxHead, int suppBody, NewRule& rc, std::map<ItemType, MapElement>::iterator j) {
	int nrules = 0;
	if (rc.head.size() < (size_t)maxHead) {
		map<ItemType, MapElement>::iterator eh = rc.lastBody->second.heads.end();
		while (j != eh) {
//cout << "Extending head of " << *i << " with " << j->first  << " size " << j->second.size() << std::endl;
		if (std::find(rc.body.begin(), rc.body.end(),j->first) == rc.body.end()) {
			GidList newGidList(rc.gids);
			newGidList &= j->second;
				if (newGidList.moreThan(threshold)) {
					NewRule r(rc,j,newGidList);
					rs.insert(rs.end(),r);
					nrules += 1;
					nrules += buildHead(rs,threshold,maxHead,suppBody,r,++j);
			} else j++;
		} else j++;
	}
	}
	return nrules;
}

void BodyMap::howManyItemsets () {
	for (size_t i = 1; i < itemsets.size(); i++) {
		std::cout << "Itemset Length: " << i << " -> " << itemsets[i] << std::endl;
	}
}

void BodyMap::saveItemset( const NewRule& r,
                           double bodySupp ) {
    int c = r.gids.count();	
    coreConn->insert_DB( r.body,
                         r.head,
                         c/totGroups,
                         1, true );
}

void BodyMap::saveRules( const NewRuleSet& rs,
                           double bodySupp ) {
    NewRuleSet::const_iterator it = rs.begin();
    bool bodyID = true;
    int c;
    for(it=rs.begin(); it!=rs.end(); it++) {
      c = it->gids.count();	
      coreConn->insert_DB( it->body,
                          it->head,
                          c/totGroups,
                          c/bodySupp, bodyID );
//      bodyID = false;
    }
  }


void BodyMap::saveItemsets( const NewRuleSet& rs,
                           double bodySupp ) {
//    static ofstream out(outfile.c_str());
    NewRuleSet::const_iterator it = rs.begin();
    int c;
    for(it=rs.begin(); it!=rs.end(); it++) {
	c = it->gids.count();	
        coreConn->insert_DB( it->body,
                            it->head,
                            c/totGroups,
                            1, true );
    }
  }

} //end namespace
