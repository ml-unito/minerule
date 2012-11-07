class NewRule {
  public:
	std::set<ItemType> body, head;
	map<ItemType, BodyMapElement>::iterator lastBody;
	map<ItemType, MapElement>::iterator lastHead;
	int bodySupp;
	GidList gids;
	NewRule (map<ItemType, BodyMapElement>::iterator b, GidList& g) : lastBody(b), gids(g) {
		body.insert(b->first);
		lastHead = b->second.heads.begin();
	}
	NewRule (NewRule& r, map<ItemType, BodyMapElement>::iterator b, GidList& g) : body(r.body), lastBody(b), gids(g) {
		body.insert(b->first);
		lastHead = b->second.heads.begin();
	}
	NewRule (map<ItemType, BodyMapElement>::iterator b, map<ItemType, MapElement>::iterator h, GidList& g, int bs) : lastBody(b), lastHead(h), gids(g), bodySupp(bs) {
		body.insert(b->first);
		head.insert(h->first);
	}
	NewRule (NewRule& r, map<ItemType, BodyMapElement>::iterator b, GidList& g, int bs) : body(r.body), head(r.head), lastBody(b), lastHead(r.lastHead), gids(g), bodySupp(bs) {
		body.insert(b->first);
	}
	NewRule (NewRule& r, map<ItemType, MapElement>::iterator h, GidList& g) : body(r.body), head(r.head), lastBody(r.lastBody), lastHead(h), gids(g), bodySupp(r.bodySupp) {
		head.insert(h->first);
	}
	friend ostream& operator<<(ostream& out, NewRule r) {
		std::set<ItemType>::iterator i;
		for (i=r.body.begin(); i!=r.body.end(); i++) {
			if (i != r.body.begin()) out << ", ";
			out << *i;
		}
		out << " -> ";
		for (i=r.head.begin(); i!=r.head.end(); i++) {
			if (i != r.head.begin()) out << ", ";
			out << *i;
		}
		out << endl;
		return out;
	}
};

class NewRuleSet : public vector<NewRule> {};

class BodyMap : public map<ItemType, BodyMapElement> {
	vector<GidList> bodySupports;
  public:
	int add(ItemType& gid, Transaction& t1, Transaction& t2, bool secondPass = false);
	int add(ItemType& gid, Transaction& t1, bool secondPass = false);
	int add(ItemType& gid, Transaction& t1);
	int add(ItemType& gid, ItemType& t1, ItemType& t2, bool secondPass);
	void saveMap(ostream& out, bool withGids = true);
	void loadMap(istream& in, bool withGids = true);
	void pruneMap(float threshold);
	void updateCount ();
	void extract1Rule (NewRuleSet& rs, float threshold);
	void extendBody (NewRuleSet& rs, float threshold, int maxBody);
	void extendHead (NewRuleSet& rs, NewRuleSet& rs1, float threshold, int maxHead);
	void createBodies (NewRuleSet& rs, float threshold, int maxBody);
	void createHeads (NewRuleSet& rs, NewRuleSet& rs1, float threshold, int maxHead);
	void extractHeads (NewRuleSet& rs, NewRuleSet& rs1, float threshold, int maxHead);
};

