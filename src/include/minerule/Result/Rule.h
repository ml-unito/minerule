#ifndef RULE_H_SP35S3OZ
#define RULE_H_SP35S3OZ

namespace minerule {
	
	class Rule {
	private:
		ItemSet* body;
		ItemSet* head;		
		double support;
		double confidence;
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
		
		void setBody(ItemSet* b) {
			body = b;
		}
		
		const ItemSet& getBody() const { 
			assert(body!=NULL);
			return *body; 
		}
		
		ItemSet& getBody() { 
			assert(body!=NULL);
			return *body; 
		}
		


		void setHead(ItemSet* h) {
			head = h;
		}
		
		const ItemSet& getHead() const { 
			assert(head!=NULL);
			return *head; 
		}
		
		ItemSet& getHead() { 
			assert(head!=NULL);
			return *head; 
		}
		
		
		void setSupport(double s)  {
			support = s;
		}
		
		double getSupport() const {
			return support;
		}
		
		void setConfidence(double c) {
			confidence = c;
		}
		
		double getConfidence() const {
			return confidence;
		}
			
	};

} // namespace

#endif /* end of include guard: RULE_H_SP35S3OZ */


