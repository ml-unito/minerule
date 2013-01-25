#ifndef RULE_H_SP35S3OZ
#define RULE_H_SP35S3OZ

#include "minerule/Database/ItemType.h"

namespace minerule {
	
	class Rule {
	private:
		unsigned int bodyId;
		unsigned int headId;
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

		Rule(const Rule& rule) : support(rule.support), confidence(rule.confidence), bodyId(rule.bodyId), headId(rule.headId) {
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
		
		// Rules id getters and setters		
		void setBodyId(unsigned int bid) { bodyId = bid; }
		void setHeadId(unsigned int hid) { headId = hid; }
		unsigned int getBodyId() const { return bodyId; }
		unsigned int getHeadId() const { return headId; }
			
	};

} // namespace

#endif /* end of include guard: RULE_H_SP35S3OZ */


