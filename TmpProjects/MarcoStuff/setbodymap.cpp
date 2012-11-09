// CORE.CPP

#include "constraints.h"
#include "setbodymap.h"
#include <algorithm>

  bool BodyMapElement::updateCount () {
    for (map<ItemType, MapElement>::iterator i = heads.begin(); i != heads.end(); i++)
      i->second.counter += i->second.size();
    return heads.size() > 0;
  }


  /*void BodyMap::saveMap (ostream& out, bool withGids) {
    for (iterator i = begin(); i != end(); i++) {
    out << i->first;
    i->second.saveMap(out, withGids);
    }
    }

    void BodyMap::loadMap (istream& in, bool withGids) {
    ItemType item;
    while (!in.eof()) {
    in >> item;
    if (in.eof()) break;
    (*this)[item].loadMap(in, withGids);
    }
    }
  */


int BodyMap::nextid = 0;

void BodyMapElement::insert(const ItemType& item, const int gid, bool secondPass) {
	map<ItemType, MapElement>::iterator found = heads.find(item);
	if (found == heads.end()) {
		if (!secondPass) heads[item].insert(gid);
	} else found->second.insert(gid);
}

bool BodyMapElement::pruneMap (float threshold, bool onlyBody) {
	if (!moreThan(threshold)) return false;
	else if (onlyBody) return true;
	map<ItemType, MapElement> newMap;
	for (map<ItemType, MapElement>::iterator i = heads.begin(); i != heads.end(); i++)
		if (i->second.counter >= threshold) {i->second.counter = 0; newMap[i->first] = i->second;}
	heads = newMap;
	return heads.size() > 0;
}

/*
void BodyMapElement::saveMap (ostream& out, bool withGids) {
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
	out << endl;
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

void BodyMapElement::setMinMax (int which, int v) {
	MinMax mm;
	for (int i=attribute.size(); i<=which; i++) {
		attribute.insert(attribute.end(),mm);
	} 
	if (attribute[which].minValue() > v) attribute[which].min = v;
	if (attribute[which].maxValue() < v) attribute[which].max = v;
}
*/

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
//		int n = t1.values.size()/t1.size();
//		for (int k=0; k < n; k++) {
//			found->second.setMinMax(k,t1.values[k+howMany*n]);
//		}
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
void BodyMap::saveMap (ostream& out, bool withGids) {
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

void BodyMap::pruneMap (float threshold, bool onlyBody) {
	//BodyMap newMap(outfile, totGroups);
	for (iterator i = begin(); i != end(); i++)
		if (!i->second.pruneMap(threshold,onlyBody)) {
		//	 newMap[i->first] = i->second;
			i->second.done = true;
		}
	//(*this) = newMap;
}

void BodyMap::updateCount () {
	for (iterator i = begin(); i != end(); i++)
		i->second.updateCount();
}

int BodyMap::buildRules (NewRuleSet& rs1, NewRule& rc, vector<BodyMap::iterator>& vb, vector<BodyMap::iterator>& vh, int maxBodyCard, int maxHeadCard, float threshold, int j) {
	vector<BodyMap::iterator> newVb, newVh;
	NewRuleSet rs;
	int start = rs.size();
	bool intersect = (rc.gids.size() > 0 );
	for (int k=j+1; k<vb.size(); k++) {
		GidList newGidList;
		//GidList newGidList(vb[k]->second);
		if (intersect)
			set_intersection(vb[k]->second.begin(), vb[k]->second.end(),
				rc.gids.begin(), rc.gids.end(),
				inserter(newGidList,newGidList.begin()));
//			newGidList &= rc.gids;
		else newGidList = vb[k]->second;
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
		saveRules(rs2, rs[i].gids.size());
	     }
		if (newVb.size() > 0 && maxBodyCard > 1) {
			howManyRules += buildRules(rs1, rs[i],newVb,newVh,maxBodyCard-1,maxHeadCard,threshold,rs[i].bodySupp);
		}
	}
	return howManyRules;
}

vector<BodyMap::iterator> BodyMap::buildHead (NewRuleSet& rs, NewRule rc, vector<BodyMap::iterator>& v, int maxCard, float threshold, int j) {
	vector<BodyMap::iterator> newV;
//	NewRuleSet rs;
	int start = rs.size();
	for (int k=j+1; k<v.size(); k++)// {
	if (std::find(rc.body.begin(), rc.body.end(),v[k]->first) == rc.body.end()) {
		GidList newGidList;
		//GidList newGidList(v[k]->second);
	//	if (rc.gids.count(true) > 0 )
			set_intersection(v[k]->second.begin(), v[k]->second.end(),
				rc.gids.begin(), rc.gids.end(),
				inserter(newGidList,newGidList.begin()));
//			newGidList &= rc.gids;
		//else newGidList = v[k]->second;
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
	for (int i=0; i<antiMonoConstr.size() && flag; i++)
		flag = flag && antiMonoConstr[i]->check(*this,rc.body);
	return flag;
}

bool BodyMap::checkMono (NewRule& rc) {
	bool flag = true;
	for (int i=0; i<monoConstr.size() && flag; i++)
		flag = flag && monoConstr[i]->check(*this,rc.body);
	return flag;
}

int BodyMap::buildItemset (NewRuleSet& rs1, NewRule& rc, vector<BodyMap::iterator>& v, int maxCard, float threshold, int j) {
	vector<BodyMap::iterator> newV;
	NewRuleSet rs;
	int start = rs.size();
	bool intersect = rc.gids.size() > 0;
	int howManyRules = 0;
	int l = rc.body.size()+1;
	for (int k=j+1; k<v.size(); k++) {
		//GidList newGidList;
		//GidList newGidList(v[k]->second);
		NewRule r(rc,v[k],v[k]->second,newV.size());
		if (!checkAntiMono(r)) continue;
		r.satisfy = r.satisfy || checkMono(r);
		if (intersect)
			set_intersection(v[k]->second.begin(), v[k]->second.end(),
				rc.gids.begin(), rc.gids.end(),
				inserter(r.gids,r.gids.begin()));
//			r.gids &= rc.gids;
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

int BodyMap::generateStartItemSets (NewRuleSet& rs1, NewRule& rc, vector<BodyMap::iterator>& v, int maxCard, float threshold, int j) {
	vector<BodyMap::iterator> newV;
	NewRuleSet rs;
	int start = rs.size();
	bool intersect = rc.gids.size() > 0;
	int howManyRules = 0;
	int l = rc.body.size()+1;
	while (l >= itemsets.size()) itemsets.insert(itemsets.end(),0);
	int k=j+1;
	if (k<v.size()) {
		//GidList newGidList;
		//GidList newGidList(v[k]->second);
		NewRule r(rc,v[k],v[k]->second,k);
		if (checkAntiMono(r)) {
		r.satisfy = r.satisfy || checkMono(r);
		if (intersect)
			set_intersection(v[k]->second.begin(), v[k]->second.end(),
				rc.gids.begin(), rc.gids.end(),
				inserter(r.gids,r.gids.begin()));
		//	r.gids &= rc.gids;
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

void BodyMap::howManyItemsets () {
	for (int i = 1; i < itemsets.size(); i++) {
		std::cout << "Itemset Length: " << i << " -> " << itemsets[i] << endl;
	}
}

void BodyMap::saveItemset( const NewRule& r,
                           double bodySupp ) {
    int rid = nextID();
    int bodyID = nextID();
    int headID = -1;
    int c = r.gids.size();	
    outR << rid << "\t" << bodyID << "\t" << headID << "\t" << c/totGroups << "\t" << -1 << endl;
    for (int i = 0; i < r.body.size(); i++) outHB << bodyID << "\t" << r.body[i] << endl;
}

void BodyMap::saveRules( const NewRuleSet& rs,
                           double bodySupp ) {
//    static ofstream out(outfile.c_str());
    NewRuleSet::const_iterator it = rs.begin();
    int bodyID = -1;
    int c,rid,headID;
    for(it=rs.begin(); it!=rs.end(); it++) {
	if (bodyID == -1) {
        	bodyID = nextID();
	        for (int i = 0; i < it->body.size(); i++) outHB << bodyID << "\t" << it->body[i] << endl;
	}
    	rid = nextID();
	headID = nextID();
	c = it->gids.size();	
    	outR << rid << "\t" << bodyID << "\t" << headID << "\t" << c/totGroups << "\t" << c/bodySupp << endl;
    for (int i = 0; i < it->head.size(); i++) outHB << headID << "\t" << it->head[i] << endl;
/*
      connection.insert( it->body,
                          it->head,
                          c/totGroups,
                          c/bodySupp );
*/
    }
  }


void BodyMap::saveItemsets( const NewRuleSet& rs,
                           double bodySupp ) {
//    static ofstream out(outfile.c_str());
    NewRuleSet::const_iterator it = rs.begin();
    int bodyID = -1;
    int c,rid,headID;
    for(it=rs.begin(); it!=rs.end(); it++) {
        bodyID = nextID();
	for (int i = 0; i < it->body.size(); i++) outHB << bodyID << "\t" << it->body[i] << endl;
    	rid = nextID();
	headID = -1;
	c = it->gids.size();	
    	outR << rid << "\t" << bodyID << "\t" << headID << "\t" << c/totGroups << "\t" << -1 << endl;
    }
  }

