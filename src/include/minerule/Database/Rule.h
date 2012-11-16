#ifndef RULE_H_SP35S3OZ
#define RULE_H_SP35S3OZ

namespace minerule {
	
	class Rule {
	protected:
		void clear() {
			if(body!=NULL)
				delete body;
			if(head!=NULL)
				delete head;

			body=NULL;
			head=NULL;
		}

	public:
		ItemSet* body;
		ItemSet* head;
		double support;
		double confidence;

		Rule() : body(NULL), head(NULL), support(0), confidence(0) {} 

		Rule(const Rule& rule) : support(rule.support), confidence(rule.confidence) {
			if(rule.body!=NULL) {
				body= new ItemSet(*rule.body);
			}

			if(rule.head!=NULL) {
				head= new ItemSet(*rule.head);
			}
		}

		~Rule() { 
			clear();
		}

		bool operator<(const Rule& rhs) const {
			return *this->body < *rhs.body || ( *this->body == *rhs.body && *this->head < *rhs.head );
		}

		bool operator<=(const Rule& rhs) const {
			return *this->body <= *rhs.body || (*this->body == *rhs.body && *this->head <= *rhs.head);
		}

		bool operator==(const Rule& rhs) const {
			return *this->body == *rhs.body && *this->head == *rhs.head;
		}

		bool operator>(const Rule& rhs) const {
			return ! (*this <= rhs);
		}

		bool operator>=(const Rule& rhs) const {
			return ! (*this < rhs);
		}

		bool operator!=(const Rule& rhs) const {
			return ! (*this == rhs);
		}

		Rule& operator=(const Rule& rule) {
			clear();
			support=rule.support;
			confidence=rule.confidence;

			if(rule.body!=NULL) {
				body= new ItemSet(*rule.body);
			}

			if(rule.head!=NULL) {
				head= new ItemSet(*rule.head);
			}

			return *this;
		}    
	};

} // namespace

#endif /* end of include guard: RULE_H_SP35S3OZ */


