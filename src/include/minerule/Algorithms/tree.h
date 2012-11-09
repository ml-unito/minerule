/* TREE.H
In questo file si definisce la classe TREE [ALBERO].
Esso è composto dalle istanze di TREENODE.
Gli elementi sono tutti privati e per accedervi si sono scritti dei metodi.
Di seguito sono commentati i campi e i metodi.
Ci sono molte funzioni xxxHelper. Servono per la ricorsione.
*/

#ifndef TREE_H
#define TREE_H

#include <string>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <map>

#include "treenode.h"
#include "Counter.h"
#include "Database/Connection.h"



template< class NODETYPE > class Counter;         // dich. successiva

template< class NODETYPE >
class Tree
{
  public:
  static const NODETYPE ROOTNODE;


// Costruttore
    Tree();

// Funzione ricorsiva. Inserisce tutti i nodi di una transazione
    void insertNode( deque<NODETYPE>& elems, 
		     int sum,
		     Counter <NODETYPE>& counter, 
		     bool madeListCounter,
		     bool originalFPGrowth=false,
		     bool useStdOrder=false);

// Aggiunge la root all'albero.
    void addRoot(const Counter<NODETYPE>&, const NODETYPE &);

// Legge e stampa l'albero in profondità da sinistra a destra. [Ricorsiva]
    void preOrderTraversal();

// Legge l'albero e genera i file di testo per disegnare con il tool ViZuale
// il Fp-Tree e i Fp-Tree Conditional
    void preOrderTraversalToDesign(std::string Radice);

// Crea la lista dei nodi nel contatore generale
    void makeList(Counter <NODETYPE>& counter);

// Ordina l'albero seguendo i valori del contatore generale.
    void makeOrdina(Tree <NODETYPE> *,Counter <NODETYPE>*,bool );

// Taglio anticipato. So che l'antenato è cambiato. Posso fare un taglio definitivo
// che coinvolge l'ancestor (antenato più lontano)
    void cut(NODETYPE ancestor,double nSup);

// Taglio dei cammini che non hanno supporto sufficiente
    void cut(double nSup);

// Ordina la stringa in ingresso in base al contatore generale.
   std::string ordinaSequenza(string,Counter <NODETYPE>* );

// Inserisce il cammino nell'albero e anche il peso corretto.
    void insertAddWalk( deque<NODETYPE>& path, int peso);

// Stampa tutte le regole da ogni Fp-tree Conditional
    void printRules(const NODETYPE&, sqlCoreConn&, double, Counter<NODETYPE>&);

// Funzione che dealloca e cancella i ptr e i nodi. [Distruttore]
    void clearTree() {
      deallocaErase(rootPtr);
      setNullRootPtr();
    }

    double getSupportForItemSet(deque<NODETYPE>&);   

  private:
// Root
    TreeNode< NODETYPE > *rootPtr;

// Funzioni di Utilità. _Funzioni Helper leggi considerazione iniziale.
//    void insertNodeHelper( TreeNode< NODETYPE > *, StringTokenizer&, int, bool, TreeNode< NODETYPE > *  );
    
    
    void insertNodeHelper(
			  TreeNode< NODETYPE > *ptr, 
			  deque<NODETYPE>& elems, 
			  int sum,
			  bool madeListCounter,
			  TreeNode< NODETYPE > *anc,
			  Counter<NODETYPE>& counter,
			  bool originalFPGrowth=false);


    void preOrderHelper( TreeNode< NODETYPE > *, int ) ;
    void makeListHelper( TreeNode< NODETYPE > *, Counter <NODETYPE>& ) ;
    void preOrderToDesignHelper( TreeNode< NODETYPE > *, int,string,string&,string,fstream&) ;
    int makeOrdinaHelper(TreeNode< NODETYPE > *ptr,
			 deque<NODETYPE> trans,
			 Counter <NODETYPE> *linkCount,
			 bool madeListCounter );
    void cutHelper(TreeNode< NODETYPE > *, double );
    void printRulesHelper(TreeNode< NODETYPE > *,
			  std::vector<NODETYPE>,
			  sqlCoreConn&, 
			  const NODETYPE&, 
			  double totGroups,
			  Counter<NODETYPE>&);
    void explodeRules( sqlCoreConn& coreConn, 
		       std::vector<NODETYPE>& curElem,
		       double myGroups,
		       double totGroups ,
		       Counter<NODETYPE>&);

    //    void insertAddWalkHelper(TreeNode< NODETYPE > *, stringTokenizer& , int);
    void insertAddWalkHelper(TreeNode< NODETYPE > *ptr, deque<NODETYPE>& path, int sum);

    void deallocaErase( TreeNode< NODETYPE > *ptr);
    void setNullRootPtr() {
      rootPtr=NULL;
    }
};

/* IMPLEMENTAZIONE DEI METODI DI TREE.H */

template<class NODETYPE>
const NODETYPE Tree<NODETYPE>::ROOTNODE;

// Costruttore
template< class NODETYPE >
Tree< NODETYPE >::Tree() { rootPtr = NULL; }

// Aggiunge la root all'albero.
template< class NODETYPE >
void Tree< NODETYPE >::addRoot( const Counter<NODETYPE>& counter, const NODETYPE &value ) {
// Il valore di default per la root è 0.
  rootPtr = new TreeNode < NODETYPE > (counter, value,0);
  rootPtr->setFather(NULL);
  rootPtr->setAncestor(NULL);
}


// Inserisce una sequenza di token nell'albero.
template< class NODETYPE >
void Tree< NODETYPE >::insertNode( 
				  deque<NODETYPE>& elems, 
				  int sum,
				  Counter <NODETYPE>& counter, 
				  bool madeListCounter,
				  bool originalFPGrowth,
				  bool useStdOrder) {
  if(useStdOrder) {
    sort(elems.begin(), elems.end());
  } else {
    counter.sortVector(elems);
  }

  insertNodeHelper( rootPtr, 
		    elems,
		    sum,
		    madeListCounter,
		    rootPtr,
		    counter,
		    originalFPGrowth);
}


// **** Controllare quali parametri non servono!!!!
// Per ogni token controlla se esiste come figlio
// Se non esiste lo crea, inserisce il ptr nell'elenco dei figli e continua sugli altri token
// Se esiste incrementa solo il contatore del valore passato.
template< class NODETYPE >
void Tree< NODETYPE >::insertNodeHelper(
					TreeNode< NODETYPE > *ptr, 
					deque<NODETYPE>& elems, 
					int sum,
					bool madeListCounter,
					TreeNode< NODETYPE > *anc,
					Counter<NODETYPE>& counter,
					bool originalFPGrowth) {
  NODETYPE value;
  bool trovato;
  trovato=false;

  TreeNode<NODETYPE>* node;
  assert(!elems.empty());
  assert(ptr!=NULL);
  value= elems.front();

  elems.pop_front();

  //  std::cout<<"value"<<value<<std::endl;
  if (anc->getData()==ROOTNODE)  
    anc=ptr;

  if ((node=ptr->find(value))!=NULL) {
    //    std::cout<<"trovato"<<std::endl;
    node->incCount(sum);
  }
  else {
    //cout<<"non trovato"<<std::endl;
    ptr->incChild();
    node = new TreeNode<NODETYPE>(ptr->getMap().key_comp().getCounter(), value,sum);
    ptr->insertNode(value,node);
    if (madeListCounter==true) {
      //Aggiunto il 4-01-03. Potevo aggiungere un parametro nel costruttore...
      //ma creo il padre e l'ancestor solo nell'albero finale!
      node->setFather(ptr);
      node->setAncestor(anc);

      if(originalFPGrowth) {
	counter.insertInList(value, node);
      }
    }
  }

  if(!elems.empty())
    insertNodeHelper( node, elems, sum,madeListCounter,anc, counter, originalFPGrowth);
}


// Legge e stampa a video l'albero. Lettura in profondità da sx a dx
template< class NODETYPE >
void Tree< NODETYPE >::preOrderTraversal() {
  std::cout<<" --------------------- "<<std::endl;
  std::cout<<"         Albero        "<<std::endl;
  std::cout<<" --------------------- "<<std::endl;
  preOrderHelper( rootPtr,0 );
  std::cout<<" --------------------- "<<std::endl;
}


// Funzione ricorsiva di preOrderTraversal.
// Qui vengono stampati il nodo, contatore_relativo,ancestor
// lev viene utilizzato per dare alla visualizzazione a video una struttura a livelli
template< class NODETYPE >
void Tree< NODETYPE >::preOrderHelper( TreeNode< NODETYPE > *ptr, int lev ) {
  typename TreeNode<NODETYPE>::MapType& mapCh = ptr->getMap();
  typename TreeNode<NODETYPE>::MapType::iterator i;
  NODETYPE ancer;

  ancer="--";

//cout<<"ptr"<<ptr->getAncestor()->getData()<<std::endl;
  if (ptr->getAncestor()!=NULL){ ancer=ptr->getAncestor()->getData(); }

  std::cout <<" "<< ptr->getData() << ":" << ptr->getCount()<<"      ancestor: "<<ancer<< std::endl;

  for( i=mapCh.begin( ) ; i != mapCh.end( ) ; i++ ) {
    for (int j=0; j<lev;j++)
      std::cout<<"  ";

    preOrderHelper(i->second,lev+1);
  }
}


// **** forse il parametro lev non serve qui!!!
// Stampa le rule. Prende il fp-tree conditional del nodo passato.
// Vedi printRuleHelper. Funziona Ricorsiva.
template< class NODETYPE >
void Tree< NODETYPE >::printRules(const NODETYPE& node, 
				  sqlCoreConn& saveRules, 
				  double totGroups,
				  Counter<NODETYPE>& gCounter) {
  std::vector<NODETYPE> rule;
  printRulesHelper( rootPtr, rule, saveRules, node, totGroups, gCounter);
  //  std::cout << "dopo printRulesHelper" << std::endl;
}


// Entra nell'albero e tramite la funzione combina (file util.c) genera tutte
// le rule possibili.
template< class NODETYPE >
void Tree< NODETYPE >::printRulesHelper( 
					TreeNode< NODETYPE > *ptr, 
				 std::vector<NODETYPE> curElem, 
					sqlCoreConn& saveRules,
					const NODETYPE& elem,
					double totGroups,
					Counter<NODETYPE>& gCounter) {

  typename TreeNode<NODETYPE>::MapType& m1 = ptr->getMap();
  typename TreeNode<NODETYPE>::MapType::iterator i;


  if (ptr->getData()!=ROOTNODE) {
      curElem.push_back(ptr->getData());
  }

  for ( i=m1.begin( ) ; i != m1.end( ) ; i++ ) { 
    printRulesHelper(i->second,curElem,saveRules, elem, totGroups, gCounter); 
  }

  curElem.push_back(elem);
  explodeRules(saveRules, curElem, ptr->getCount(), totGroups, gCounter);

  // temp="INSERT INTO "+saveRules.getOutTableName()+" VALUES ('"+rule+","+node+"','0')";
  //const char * dains=temp.c_str();
  //  saveRules.insert_DB(dains);
}

template< class NODETYPE >
double Tree< NODETYPE >::getSupportForItemSet(deque<NODETYPE>& body) {
  TreeNode<NODETYPE>* curPtr = rootPtr;

  typename deque<NODETYPE>::iterator it = body.begin();
  for(; it!=body.end(); it++ ) {
    curPtr = curPtr->find(*it);
    assert( curPtr!= NULL );
  }

  return curPtr->getCount();
}

template< class NODETYPE > 
void Tree< NODETYPE >::explodeRules( sqlCoreConn& coreConn, 
				     std::vector<NODETYPE>& curElem,
				     double myGroups,
				     double totGroups,
				     Counter<NODETYPE>& gCounter  ) {
  float cSupp = myGroups / totGroups;
  if( curElem.size() <= 1 )
    return;
  
  assert( curElem.size()<=32 );
  // N.B. : maxVal = 2^curElem.size() -1
  unsigned int maxVal = (1 << curElem.size())-1;

  //  MRErr() << "maxVal:" << maxVal << std::endl;
  //  copy( curElem.begin(), curElem.end(), std::ostream_iterator<NODETYPE>(MRErr(),";"));
  //  MRErr() << std::endl;

  for(unsigned int i=1; i<maxVal; i++ ) {
    //    MRErr() << "i==" << i << std::endl;
    std::vector<NODETYPE> body;
    deque<NODETYPE> bodyCopy;
    std::vector<NODETYPE> head;

    // Here we build the current rule's head and body elements
    for(unsigned int j=0; j<curElem.size(); j++) {
      if(i&(1<<j)) {
	body.push_back(curElem[j]);
	bodyCopy.push_back(curElem[j]); 
      }
      else {
	head.push_back(curElem[j]);
      }
    }

    float cConf = myGroups/gCounter.getSupportForItemSet(bodyCopy); 
    // float cConf = 1.0;
    // Building up thestd::string representation of the generated rule...
    // Body part...

    /*
    typename std::vector<NODETYPE>::iterator it;
   std::string regola;

    it=body.begin();
    if(it!=body.end()) {
      regola=string("<")+it->c_str()+">";
      it++;
    }

    for(; it!=body.end(); it++ ) {
      regola+=string(",")+"<"+it->c_str()+">";
    }

    // a nice looking -> ;-)
    regola+=" => ";

    // Head part...
    it=head.begin();
    if(it!=head.end()) {
      regola+=string("<")+it->c_str()+">";
      it++;
    }

    for(; it!=head.end(); it++ ) {
      regola+=string(",")+"<"+it->c_str()+">";
      } */

    // Inserting the new rule into the db.

    //    std::cout << "ItemSet:";
    //copy( curElem.begin(), curElem.end(), std::ostream_iterator<NODETYPE>(cout, ";"));
    //cout << std::endl << "Regola:" << regola << std::endl;

    /*    stringstream strbuf;
    strbuf << "INSERT INTO "+coreConn.getOutTableName()+" VALUES ('"
	   <<regola<<"',"
	   <<cSupp<<","
	   <<cConf<<");"; */
    coreConn.insert_DB(body, head, cSupp, cConf);
  }

  //  MRErr() << "Finito!!!!" << std::endl;
}


// Ordina l'albero seguendo i valori del contatore generale.
// Funzione ricorsiva: makeOrdinaHelper
template< class NODETYPE >
void Tree< NODETYPE >::makeOrdina(Tree<NODETYPE>  *tree, 
				  Counter <NODETYPE> *linkCounter,
				  bool madeListCounter) {
  deque<NODETYPE> trans;
  makeOrdinaHelper( tree->rootPtr,trans,linkCounter, madeListCounter);
}


// Questa funziona reinserisce nel nuovo albero le transazioni nell'ordine giusto e con i pesi
// corretti
template< class NODETYPE >
int Tree< NODETYPE >::makeOrdinaHelper(TreeNode< NODETYPE > *ptr,
				       deque<NODETYPE> trans,
				       Counter <NODETYPE> *linkCount,
				       bool madeListCounter ) {
  typename TreeNode<NODETYPE>::MapType& m1 = ptr->getMap();
  typename TreeNode<NODETYPE>::MapType::iterator i;
  NODETYPE node;
  int peso,pFiglio,tvolte,diff;

  tvolte=0;
// node=(std::string) ptr->getData();
  node=ptr->getData();
  peso=ptr->getCount();

//cout << node << " " << peso << std::endl;

  if (node!=ROOTNODE) 
    trans.push_back(node);

  for ( i=m1.begin() ; i != m1.end() ; i++ ) {
// std::cout<<"Ho un figlio"<<std::endl;
    pFiglio=makeOrdinaHelper(i->second,trans,linkCount,madeListCounter);
//cout << trans <<  " Peso Figlio "<< pFiglio << std::endl;
    tvolte+=pFiglio;
  }

  diff=peso-tvolte;

  if (diff>0) {
//cout <<  "sono transazione da inserire :" << trans << "peso da ins " << diff << std::endl;
    insertNode(trans,diff,*linkCount,madeListCounter);
  }

  return peso;
}

/*
// Funzione di ordinamento della stringa letta dall'albero non ordinato.
// la ordina e restituisce in ordine i token da inserire.
// Funziona di utilità. E' possibile in futuro inserirla in un file di utilità
template< class NODETYPE >std::string Tree< NODETYPE >::ordinaSequenzastd::string value,Counter <NODETYPE>* linkCount) {
  typedef multimap <int, NODETYPE> mmap;
  mmap coll;
  typename mmap::iterator k;
 std::string newstring,okstring,okstring2;
  NODETYPE node;
  int i;

  string::reverse_iterator rev2;
  for(rev2 = value.rbegin(); rev2 != value.rend(); rev2++)
    okstring2+=*rev2;

  okstring="";

  stringTokenizer mytoken((string&)okstring2,",\r\n");

  while(mytoken.hasMoreTokens()) {
    node= (NODETYPE) mytoken.nextToken();
    i=linkCount->getValueOrdina(node);
    coll.insert( make_pair(i,node) );
// std::cout << "valore di i"<<i<<"nodetype"<<node<<"fine"<<std::endl;
  }

  for( k=coll.begin( ) ; k !=coll.end( ) ; k++ )
    newstring=newstring+(std::string)k->second +",";

//cout << "Nuova stringa: "<<newstring<<std::endl;

  string::reverse_iterator rev;
  for(rev = newstring.rbegin(); rev != newstring.rend(); rev++)
    okstring+=*rev;

//cout<<"stringa finale :"<<okstring<<std::endl;
  return okstring;
}

*/
// Richiamata su un albero taglia i nodi con un supporto insuff.
template< class NODETYPE >
void Tree< NODETYPE >::cut(double nSup) {
  cutHelper( rootPtr,nSup);
}


// Quando ho verificato che non ho lo stesso Ancestor posso controllare in modo
// definitivo il supporto della parte a sinistra.
// All'interno ho commentato in modo dettagliato cosa viene fatto
template< class NODETYPE >
void Tree< NODETYPE >::cut(NODETYPE ancestor,double nSup) {
  typename TreeNode<NODETYPE>::MapType& m1 = rootPtr->getMap();
  typename TreeNode<NODETYPE>::MapType::iterator i;

  i=m1.find(ancestor);
  assert(i!=m1.end());
// std::cout<<"Non ho più lo stesso antenato"<<std::endl;
// std::cout<<i->first<<std::endl;
// std::cout<<"entro in cutHelper standard ma con ptr di ancestor"<<std::endl;
  if (i->second->getCount()<nSup) {
// Il mio ancestor non ha supporto sufficiente. Sicchè anche i miei successori
// non avranno supp. suff. -> dealloco tutto ed elimino tutto.
    deallocaErase(i->second);
    m1.erase(i);
  }
  else {
// Ancestor ha supporto sufficiente. Controllo i supporti degli eredi e poi
// lo marco così nel taglio finale non devo ricontrollare di nuovo tutto.
    cutHelper(i->second,nSup);
    i->second->setMarked();
  }
}


// Controlla e taglia i nodi con supporto insufficiente.
// Controlla se il nodo è marcato. Se fosse marcato indicherebbe che è già
// stato controllato in precedenza quando !haveSameAncestor.
// Vedi dettagli in file osservazioni.doc
template< class NODETYPE >
void Tree< NODETYPE >::cutHelper( TreeNode< NODETYPE > *ptr, double nSup ) {
  typename TreeNode<NODETYPE>::MapType& m1 = ptr->getMap();
  typename TreeNode<NODETYPE>::MapType::iterator i;

  std::vector<NODETYPE> elemsToBeDeleted;

  for ( i=m1.begin( ) ; i != m1.end( ) ; i++ ) {
    //cout<<"nodo successivo "<<i->second->getData()<<" cont "<<i->second->getCount()<<std::endl;
    //    std::cout << "NODO:" << i->first.c_str() << " count:" << i->second->getCount() << std::endl;
    if (i->second->getCount()<nSup) {
      //cout << "dealloco!!!!" << std::endl;
      deallocaErase(i->second);
      i->second=NULL;
      elemsToBeDeleted.push_back(i->first);
    }
    else {
      //cout << "NON dealloco!"<<std::endl;
      cutHelper(i->second,nSup);
    }
  }

  typename std::vector<NODETYPE>::iterator it = elemsToBeDeleted.begin();
  for(; it!=elemsToBeDeleted.end(); it++ ) {
    m1.erase(*it);
  }
}


// Dealloca in modo corretto tutti ptr creati e la mappa.
// Ricorsiva. Vado fino al fondo e poi cancello tornando su.
template< class NODETYPE >
void Tree< NODETYPE >::deallocaErase( TreeNode< NODETYPE > *ptr) {
  typename TreeNode<NODETYPE>::MapType& m1 = ptr->getMap();
  typename TreeNode<NODETYPE>::MapType::iterator i;

// std::cout <<"DEALLOCA_ERASE"<< ptr->getData() << ":" << ptr->getCount() << std::endl;

  for( i=m1.begin( ) ; i != m1.end( ) ; i++ ) { 
    deallocaErase(i->second); 
  }

  m1.clear();
  delete ptr;
}


// Inserimento nel lattice
template< class NODETYPE >
void Tree< NODETYPE >::insertAddWalk( deque<NODETYPE>& path, int peso) {
  insertAddWalkHelper( rootPtr, path,peso);
}


// Leggendo i token inserisce i nodi all'interno degli alberi
// Usato per inserire i cammini negli fp-tree conditional
template< class NODETYPE >
void Tree< NODETYPE >::insertAddWalkHelper(
TreeNode< NODETYPE > *ptr, deque<NODETYPE>& path, int sum) {
  NODETYPE value;

  assert(!path.empty());
  assert( ptr!=NULL );
  value= path.front();
  path.pop_front();

  TreeNode<NODETYPE> * node;

  if(path.empty()) {
    if ((node=ptr->find(value))!=NULL) {
      node->incCount(sum);
    }
    else {
      ptr->incChild();
      node = new TreeNode<NODETYPE>(ptr->getMap().key_comp().getCounter(), value,sum);
      ptr->insertNode(value,node);
      node->setFather(ptr);
    }
  } else {
    node = ptr->find(value);
    assert(node!=NULL);
    insertAddWalkHelper( node, path, sum);
  }
}


// Funzione di utilità. Crea diversi file a secondo della radice passata.
// Questi file vengono dati in input al programma vizuale.exe scritto in Vbasic
// permette di disegnare o salvare una gif con tutta la struttura ad albero.
template< class NODETYPE >
void Tree< NODETYPE >::preOrderTraversalToDesign(std::string Radice) {
  fstream   f1;
 std::string files;

  files="design/file"+Radice+".txt";
  f1.open(files.c_str(), ios::out);

  if (!f1) {
    std::cout << "Impossibile aprire file.txt.";
    exit(1);
  }
  f1<<"digraph minerule { "<<std::endl;
  f1<<"graph[fontsize=8]; edge  [fontsize=8]; node  [fontsize=8]; ranksep = .30; nodesep = .25;"<<std::endl;
  f1<<"node [fontname=\"Courier\"]; "<<std::endl;
//f1<<"node [shape=record];"<<std::endl;
 std::string ff="";
  preOrderToDesignHelper( rootPtr,0,"",ff,Radice,f1);
  f1<<"}"<<std::endl;
  f1.close();
}


// Segue la sintassi per creare un file testo per legare i vari nodi.
// Vedi WinGraph e programma vizuale.exe
template< class NODETYPE >
void Tree< NODETYPE >::preOrderToDesignHelper( TreeNode< NODETYPE > *ptr, int lev, std::string prec, std::string& ff, std::string Radice,fstream& f1) {
  typename TreeNode<NODETYPE>::MapType& m1 = ptr->getMap();
  typename TreeNode<NODETYPE>::MapType::iterator i;
 std::string label1,l1,count,newprec,data,address;

  std::stringstream o;
  o << ptr->getCount();
  count=o.str();
  data=(std::string)ptr->getData().c_str();
  if (ptr->getData()==ROOTNODE) 
    data=Radice;

  std::stringstream o2;
  o2 << ptr;
  address=o2.str();

  label1="node"+address+"[label=\""+data+":"+count+"\"]";
  newprec=address;
  l1="node"+newprec;

  f1<<label1<<std::endl;

  if (ff!="")  f1<<ff<<"->"<<l1<<";"<<std::endl;

  for( i=m1.begin( ) ; i != m1.end( ) ; i++ ) {
    preOrderToDesignHelper(i->second,lev+1,newprec,l1,Radice,f1);
  }
}


// Legge l'albero in ordine e crea la lista nel contatore generale.
// La lista è creata in base al nodetype data.
// Ci serve per garantire che la lista si sposti da sinistra verso destra.
// Con questa assunzione e controllando gli ancestor possiamo fare tagli prima
// di terminare la lettura della lista.
template< class NODETYPE >
void Tree< NODETYPE >::makeList(Counter <NODETYPE>& counter) {
  makeListHelper( rootPtr,counter);
}


// Funzione di supporto. Crea effettivamente la lista.
template< class NODETYPE >
void Tree< NODETYPE >::makeListHelper( TreeNode< NODETYPE > *ptr,Counter <NODETYPE>& counter ) {
  typename TreeNode<NODETYPE>::MapType& m1 = ptr->getMap();
  typename TreeNode<NODETYPE>::MapType::iterator i;

//cout <<"makelisthelper "<< ptr->getData() << ":" << ptr->getCount() << std::endl;
  if (ptr->getData()!=ROOTNODE)
    counter.insertInList(ptr->getData(),ptr);

  for ( i=m1.begin( ) ; i != m1.end( ) ; i++ )
    { makeListHelper(i->second,counter); }
}
#endif
