#include <fstream>
#include <map>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "Algorithms/FSTree.h"




namespace minerule {

void FSTree::insertTree(FSTreeNode* r, FSTreeSequence* t){
  //se la sequenza e composta da un solo elemento
  //la inserisco come figlio
		
  if (t->size()==1){
    appendChild(r,t->removeHead());
  }
  else{	
    //altrimenti creo un nuovo nodo temp con il primo item della sequenza
    //creo il collegamento nella lista e inserisco il nodo con appendChild
    //richiamo insertTree sul resto della sequenza 

    FSTreeNode* nod = appendChild(r,t->removeHead());

    insertTree(nod,t);
  }
}

void FSTree::createLinkNSTable(){
  //COSTRUISCO MAPPA DEI LINK FREQUENTI E DEI LINK NON FREQUENTI RISPETTO ALLA THRESHOLD
  //std::vector<sequence*>::iterator it;
  std::map<FSTreeSequence,int,FSTreeSequence::less_sequence>::iterator it;
  int count;	
  for (it=link_S->begin();it!=link_S->end();it++){
			
    count = (*it).second;
    if (count<threshold){
      (*link_NS)[(*it).first]=count;
    }
  }
}

void FSTree::createLinkSTable(odbc::ResultSet* rs, const HeadBodySourceRowDescription& rowDes) {
  FSTreeSequence* input = new FSTreeSequence();
  rs->next();
 while(!rs->isAfterLast()) {
    input->read(rs,rowDes);
    if (input->size()>1){
      insertLink(input);
    }
    

    input->svuota();
    num++;		
    //canRead=rs->next();
  }
		
  MRLog() << link_S->size() <<" bigrams have been read." << std::endl;
  std::cout<< link_S->size()<<" bigrams have been read." << std::endl;
  delete input;
}

void FSTree::insertLink(FSTreeSequence* s){
  FSTreeSequence* out;
  for (size_t i=0;i<s->size()-1;i++){
    out=new FSTreeSequence(*s,i,2);
    (*link_S)[*out]=(*link_S)[*out]+1;
    delete out;
  }
}

void FSTree::construct_Tree(odbc::ResultSet* rs, const HeadBodySourceRowDescription& rowDes){
  std::vector<FSTreeSequence*>* collezione;
  FSTreeSequence* seq;
  size_t num=0;
  rs->next();
  while (!rs->isAfterLast()){
    num++;
    seq=new FSTreeSequence();
    seq->read(rs,rowDes);
				
    if (seq->size()>1){
      collezione=fraziona(seq);

      for (size_t i=0; i<collezione->size();i++){
	if (((*collezione)[i])->size()>1){
	  insertTree(root,(*collezione)[i]);
	}
	delete (*collezione)[i];
      }
      delete collezione;
    }
    delete seq;
  }
}
 std::vector<FSTreeSequence* >* FSTree::fraziona(FSTreeSequence *s){
  std::vector<int> vec;
  std::vector<FSTreeSequence*>* res = new std::vector<FSTreeSequence*>();
		
		
  //creo un vector con i riferimenti alla posizione in cui ho trovato l'inizio delle
  //seq lunghe due non a supp sufficiente
  FSTreeSequence* te;
  for (size_t i =0;i<s->size()-1;i++){
    te=new FSTreeSequence(*s,i,2);	
    if ((*link_NS)[*te]!=0)
      vec.push_back(i);
    delete te;
  }
  //ordino il vettore di riferimenti all'inizio di sequenze non frequenti
  sort(vec.begin(),vec.end());
		
  int nchar;
  //se non ci sono sequenze e la sequenza e piu lunga di 1 allora ritorno un vector in cui ho inserito un solo	
  //elemento: la sequenza iniziale
			
  if (vec.size()==0){
    if (s->size()>1){
      FSTreeSequence* sost=new FSTreeSequence(*s);
      res->push_back(sost);
    }
    return res;
  }
		
  FSTreeSequence *temp;
  //inserisco la prima sottosequenza nel vettore risultato
  temp=new FSTreeSequence(*s,0,vec[0]+1);
		
  res->push_back(temp);
  //se il vector vec e piu lungo di 1
  for (size_t i=0;i<vec.size()-1;i++){
    //skip di quelle posizioni consecutive  
    if (vec[i]+1!=vec[i+1]){
      //numero di caratteri da prendere
      nchar=vec[i+1]-vec[i];
      //creo la sottosequenza che parte da vec[i]1 ed e lunga nchar
      //lunga almeno 2
      if (nchar>1){
	temp=new FSTreeSequence(*s,vec[i]+1,nchar);
	res->push_back(temp);
      } 
    }
  }
  nchar=s->size()-(vec[vec.size()-1]+1);
  //se non mi trovo gia alla fine della sequenza
  //faccio lo stesso per l'ultima sottosequenza
  if (nchar>1){
    FSTreeSequence* last=new FSTreeSequence(*s,vec[vec.size()-1]+1,nchar);
    res->push_back(last);

  }
  return res;		
}


void FSTree::resetN_nodi(){
  n_nodi=0;
}
	
int FSTree::getN_nodi(){
  return n_nodi;
}

int FSTree::countPath(FSTreeNode* n){
  //la radice di tutte le sequenze dell'albero e ad altezza -1
  //il primo elemento e ad altezza 0
  FSTreeNode* temp= n;
  int c=0;
  while (temp->getParent()!=root){
    c++;
    temp=temp->getParent();
  }
  return c;	
}

void FSTree::addResult(FSTreeNode* n){
  FSTreeSequence* s;
  //setto il primo UpperLimit per prendere come prima sequenza quella lunga 2
  FSTreeNode* UpperLimit=n->getParent();
  UpperLimit=UpperLimit->getParent();
  FSTreeNode* temp;
  int c = countPath(n);
  temp=n;
  s=new FSTreeSequence();
  //inserisco l'elemento coda presente in tutte le sottosequenze di questo cammino
  s->insertHead(temp->getLabel());		
  if (c>=max_length && max_length !=0)
    c=max_length-1;

  for (int i=0;i<c;i++){//prendo tutte le sequenze di lunghezza >=2
    temp=temp->getParent();
    s->insertHead(temp->getLabel());					
    (*result)[*s]=(*result)[*s]+n->getCount();
    UpperLimit=UpperLimit->getParent();//estendo l' UpperLimit per prendere la sequenza lunga |s|+1
  }
  delete s;	
}



void FSTree::stampa(FSTreeNode* n){
  if (n!=root)
    std::cout<<n->getLabel()<<":"<<n->getCount()<<" ";
  std::vector<FSTreeNode*>* temp= n->getChild();
  if (temp->size()==0)//se non ha figli stampo un \n e poi esco dal metodo
    std::cout<<std::endl;
  else {  
    for (size_t i=0;i<temp->size();i++){
      stampa((*temp)[i]);
    }
  }
}

void FSTree::addResults(std::vector<FSTreeNode*>* vec){
  FSTreeNode* temp;
  for (size_t i=0;i<vec->size();i++)	{
    temp= (*vec)[i];//inserisco ogni elemento del vettore nel risultato	
    addResult(temp);
  }
}

void FSTree::mine(){
  //threshold=tsl*num/100;
		
  // std::vector<sequence*>::iterator m_it;
  std::map<FSTreeSequence, std::vector<FSTreeNode*>*, FSTreeSequence::less_sequence>::iterator m_it;
  //itero sulla mappa che contiene le sequenze lunghe 2 e i puntatori alle sequenze
  std::cout<<"prima del ciclo di mine"<<std::endl;
  std::cout<<"nella m_list ci sono "<<m_list.size()<<" sequenze"<<std::endl;
  //cout<<"nella link_NS ci sono "<<link_NS->size()<<" sequenze"<<std::endl;
  std::cout<<"nella link_S ci sono "<<link_S->size()<<" sequenze"<<std::endl;
		
  for (m_it=m_list.begin();m_it!=m_list.end();m_it++){			
    //addResults(m_list->getList(*(*m_it)));
    addResults((*m_it).second);
  }
  std::cout<<"possibili stringhe frequenti da scremare ulteriormente result.size()"<<std::endl;
  std::cout<<result->size()<<std::endl;
  //std::vector<sequence*>::iterator res_it;
  //std::map<FSTreeSequence,int,FSTreeSequence::less_sequence>::iterator res_it;
  // itero sulla mappa che contiene il risultato per stamparlo
  //int count=0;
		
  //sequenceCount* t= new sequenceCount(*result);
  /*
    std::map<sequence,int,sequence::less_sequence>* t = new std::map<sequence,int,sequence::less_sequence>(*result); 
    for (res_it=t->begin();res_it!=t->end();res_it++){
    count=(*res_it).second;
    if (count>=threshold){
    sequence temp=(*res_it).first;
    temp.stampa();
    std::cout<<": "<<count<<std::endl;
    }
    }
    delete t;*/
  std::cout<<"threshold: "<<threshold<<std::endl;
}

//aggiungo la sequenza alla lista delle sequenze lunghe 2
void FSTree::addList(FSTreeNode* father, FSTreeNode* children){
  //creo la sequenza lunga 2
  FSTreeSequence* m_string=new FSTreeSequence();
  m_string->add(father->getLabel());
  m_string->add(children->getLabel());
  //se la sequenza ha come father la radice allora esco dal metodo
  if (father==root) {
    delete m_string;
    return;
  }
  //se l'elemento non e gia presente lo aggiungo e creo il collegamento
  // inserendo il riferimento al dodo
  //m_list->add(m_string,children);
  if (m_list[*m_string]==0)
    m_list[*m_string]=new std::vector<FSTreeNode*>();
  (m_list[*m_string])->push_back(children);
  delete m_string;
}




FSTreeNode* FSTree::appendChild(FSTreeNode* r, const ItemType& t){
  bool notFound=1;
  //int i=0;
  FSTreeNode* nod = NULL;
  std::vector<FSTreeNode*>* ve = r->getChild();
  //Cerco l'item t della sequenza tra i figli di r
  for (size_t i=0;i<ve->size()&&notFound;i++){
    nod = (*ve)[i];
    if (nod->getLabel()==t){
      notFound=0;
    }
  }
  //se non l'ho trovata creo un nuovo dodo e l'ho aggiungo all albero
  if (notFound){
    //creo il nuovo nodo
    FSTreeNode* children = new FSTreeNode(t);
    n_nodi++;
    children->setParent(r);
    //aggiungo children ai figli di r
    std::vector<FSTreeNode*>* temp=r->getChild();
    temp->push_back(children);
				
    children->setCount(children->getCount()+1);
    /*formo una sequenza e aggiungo la sequenza
      alla lista che attraversa l'albero e che ha come riferimento iniziale la std::mappa
      il metodo addList controlla che non si formino sequenze lunghe uno e controlla anche
      quando si arriva alla radice
    */
				
    addList(r,children);
				
    //ritorno il puntatore al dodo creato
    return children;
  }
  //se l'ho trovato incremento il contatore di quello trovato
  else{
    nod->setCount(nod->getCount()+1);
    return nod;
  }
		

}
	
		
FSTree::~FSTree(){
  //cout<<"sono nel distruttore di FSTree"<<std::endl;
  delete root;
  //	std::cout<<"esco dal distruttore di FSTree"<<std::endl;
  delete link_NS;
  delete link_S;
		
  //delete s_result;
  //delete m_list;
		
  //delete s_result;
  delete result;

}

double FSTree::getThreshold(){
  return threshold;
}

	 
map<FSTreeSequence,int,FSTreeSequence::less_sequence>* FSTree::getResult(){
  std::map<FSTreeSequence,int,FSTreeSequence::less_sequence>::iterator res_it;
  std::map<FSTreeSequence,int,FSTreeSequence::less_sequence> t(*result);
  std::map<FSTreeSequence,int,FSTreeSequence::less_sequence>* t2 = new  std::map<FSTreeSequence,int,FSTreeSequence::less_sequence>(); 
  int count;
  for (res_it=t.begin();res_it!=t.end();res_it++){
    count=(*res_it).second;
    if (count>=threshold){
      FSTreeSequence temp=(*res_it).first;
      (*t2)[temp]=count;
    }
  } 
  return t2;
}

} // namespace
