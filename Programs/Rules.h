#ifndef __RULES_H__
#define __RULES_H__

namespace minerule {

class RuleStrings {
 public:
  std::vector<std::string> vbody;
  std::vector<std::string> vhead;
  std::string body;
  std::string head;
  double supp;
  double conf;

  Rule(const std::vector<std::string>& _body,
       const std::vector<std::string>& _head,
       double _supp,
       double _conf): vbody(_body), vhead(_head), 
		      body(""),head(""),supp(_supp),conf(_conf) {
    std::vector<std::string>::const_iterator it;
    for(it=_body.begin(); it!=_body.end(); it++) {
      body+=*it;
    }

    for(it=_head.begin(); it!=_head.end(); it++) {
      head+=*it;
    }
  }

  
};


class SortBodyHeadSuppConf {
public:
  bool operator()(const Rule& r1, const Rule& r2) const {
    if( r1.body > r2.body ) 
      return true;

    if( r1.body == r2.body ) {
      if(r1.head > r2.head )
	return true;

      if(r1.head == r2.head) {
	return r1.supp > r2.supp || 
	  (r1.supp==r2.supp && r1.conf > r2.conf);
      }
    }

    return false;
  }
};


class SortHeadBodySuppConf {
public:
  bool operator()(const Rule& r1, const Rule& r2) const {
    if( r1.head > r2.head ) 
      return true;

    if( r1.head == r2.head ) {
      if(r1.body > r2.body )
	return true;

      if(r1.body == r2.body) {
	return r1.supp > r2.supp || 
	  (r1.supp==r2.supp && r1.conf > r2.conf);
      }
    }

    return false;
  }
};

class SortSuppConf {
public:
  bool operator()(const Rule& r1, const Rule& r2) const {
    return r1.supp > r2.supp || 
      (r1.supp==r2.supp && r1.conf > r2.conf);
  }
};

} // namespace

#endif
