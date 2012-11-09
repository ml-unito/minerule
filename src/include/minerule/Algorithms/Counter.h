/* COUNTER.H
In questo file si definisce la classe del Contatore Generale.
Di seguito sono commentati i campi e i metodi.
Vedi file Counter.doc per la struttura ed esempio
*/

#ifndef COUNTER_H
#define COUNTER_H

#include <map>
#include <string>
#include <list>
#include <fstream>
#include <iterator>
#include "utils.h"
#include "Database/Connection.h"
#include "Utils/MineruleOptions.h"
#include "Database/SourceRowAttribute.h"


template< class NODETYPE > class Tree;            // dichiarazione succ.
template< class NODETYPE > class Counter;         // dichiarazione succ.
template< class NODETYPE > class TreeNode;        // dichiarazione succ.

// Struttura associata al valore key della mappa.
template < class NODETYPE >
struct cnt
{
  unsigned int count;                             // contatore assoluto
  unsigned int ordine;                            // ordine di inserimento
  std::list< TreeNode<NODETYPE>*> lista;               // lista dei nodi
  Tree <NODETYPE> fptree;                         // conditional FP-Tree
};

template< class NODETYPE >
class Counter
{
  friend class Tree< NODETYPE >;
  public:
  bool operator()(const NODETYPE& n1, const NODETYPE& n2) const {
    return compareElems(n1,n2);
  }

  typedef std::map < NODETYPE, cnt<NODETYPE> , NODETYPE > MapType;

// Costruttore
    Counter();

// Serve per inizializzare la tabella contatore generale. Vedi dettaglio sotto.
    void pushElements();

// Aggiunge un nodo nella struttura. [MAP]
    void add(NODETYPE nt, unsigned int counterVal=0);
    void print();
// Esiste la possibilità di inserire i valori sia indicando un peso che non indicando nulla
// Il valore di default è 1
    void addValues(deque<NODETYPE>&);
    void addValue(NODETYPE&);
    //    void addValues(StringTokenizer& mytoken, int peso);

// Aggiunge un cammino nel fp-tree conditional. Vedi struttura cnt
    void addWalk(NODETYPE nt, const std::vector<NODETYPE>& transaction,int peso);

// Crea la lista di nodi tutti eguali.
    void insertInList(NODETYPE node,TreeNode<NODETYPE>* value);

// Stampa a video la tabella con nodo, valore, ordine. Importante per debug
    void leggiCounter();

// Incrementa il contatore o di una unità o del peso passato.
    void incrCount(NODETYPE nt);
    void incrCount(NODETYPE nt,int peso);

// Imposta il campo ordine in modo da poterlo usare come ordine nel reinserimento
// dei cammini o nei nuovi inserimenti.
    void setOrdina();

// Ritorna il campo ordine. Serve per fare l'ordinamento dei valori da inserire.
    int getValueOrdina(NODETYPE node) { return generalCount.find(node)->second.ordine; }

// Legge le liste dei nodi e cerca di tagliare anticipatamente ed effettua il taglio finale
// tramite la procedure cut su albero.
    void esplodi(Connection&, double, double, bool originalFPGrowth=false );

// Estrae le rule
    void extractRule(Connection&, double, double);

// Ritorna la map. Ovvero il ptr  alla struttura
    MapType& getMap() { return generalCount; }

  private:

// Dal singolo elemento della lista ritorno verso l'alto e legge il cammino.
// Lo inserisce poi nel fp-tree conditional.
// Per un uso efficiente inserisce i sottocammini letti nei fp-tree conditinal corretti.
    void estrai_cammini(TreeNode<NODETYPE>* tnd, std::vector<NODETYPE>& path);
    void estrai_cammini_lazy(TreeNode<NODETYPE>* tnd, std::vector<NODETYPE>& path);

    // Sorts "elems" accordingly to the current state of the Counter
    void sortVector(deque<NODETYPE>& elems) const;

    // it is used by sortVector in order to compare to NODETYPEs accordingly
    // to current state of Counter
    bool compareElems(const NODETYPE& el1, const NODETYPE& el2) const;

    double getSupportForItemSet(deque<NODETYPE>&);

// Formato map : valore nodo, contatore generale
    MapType generalCount;

};

// Costruttore
template< class NODETYPE >
Counter< NODETYPE >::Counter() { }

/*
// Serve per inizializzare la tabella contatore generale.
// Apre il file ncoppie.txt e legge tutti gli id che esistono e li inserisce nella struttura.
// Simulo quello che dovrà essere fatto da una query.
template< class NODETYPE >
void Counter< NODETYPE >::pushElements() {
  NODETYPE elem;
  char buffer[255];
  FILE *infile2;
 std::string tokenT;

// Qui ci vorrà la query SQL per vedere quanti n sono presenti nella tavola

  if ( (infile2=fopen("example/ncoppie.txt","r"))==NULL)
    { std::cout << "Error opening file ncoppie"; exit (0); }

    if (fgets(buffer,255,infile2)==NULL )
      { std::cout << "Error read"; exit (0); }

      tokenT=buffer;
  StringTokenizer mytoken2((string&)tokenT,";\r\n");

  while (mytoken2.hasMoreTokens()) {
    elem= (NODETYPE) mytoken2.nextToken();
    add(elem);
  }

  fclose(infile2);
}
*/

// Inserisce un valore e inizializza i valori.
template< class NODETYPE >
void Counter< NODETYPE >::add(NODETYPE nt, unsigned int counterVal) {
  generalCount[nt].count=counterVal;
  generalCount[nt].ordine=0;
  generalCount[nt].fptree.addRoot(*this, Tree<NODETYPE>::ROOTNODE);
}


// Per tutti i token inserisce il nodo e incrementa il contatore
template< class NODETYPE >
void Counter< NODETYPE >::addValue(NODETYPE& value) {
  typename MapType::iterator foundElem = generalCount.find(value);
  if( foundElem==generalCount.end() ) {
    add(value,1);
  } 
  else {
    foundElem->second.count++;
  }
}


// Per tutti i token inserisce il nodo e incrementa il contatore
template< class NODETYPE >
void Counter< NODETYPE >::addValues(deque<NODETYPE>& itemsInGroup) {
  NODETYPE value;

  typename deque<NODETYPE>::iterator it = itemsInGroup.begin();

  for(; it!=itemsInGroup.end(); it++ ) {
    typename MapType::iterator foundElem = generalCount.find(*it);
    if( foundElem==generalCount.end() ) {
      add(*it,1);
    } 
    else {
      foundElem->second.count++;
    }
  }
}

template< class NODETYPE >
bool Counter< NODETYPE >::compareElems(const NODETYPE& el1, const NODETYPE& el2) const {
  typename MapType::const_iterator it1 = generalCount.find(el1);
  typename MapType::const_iterator it2 = generalCount.find(el2);
  assert(it1!=generalCount.end());
  assert(it2!=generalCount.end());

  if( it1->second.ordine==it2->second.ordine ) {
    return (!NODETYPE::lessThan(it1->first, it2->first)) && !(it1->first==it2->first);
  } else {
    return it1->second.ordine>it2->second.ordine;
  }
}

template< class NODETYPE >
void Counter< NODETYPE >::sortVector(deque<NODETYPE>& elems) const {
  sort(elems.begin(), elems.end(), *this  );
}

/*
// Come sopra ma passa un valore di incremento
template< class NODETYPE >
void Counter< NODETYPE >::addValues(StringTokenizer& mytoken,int peso) {
  NODETYPE value;

  value= (NODETYPE) mytoken.nextToken();
  incrCount(value,peso);
  if (mytoken.hasMoreTokens())
    { addValues(mytoken,peso); }
}
*/

// In questa procedura nessun controllo poichè so che le posizioni esistono tutte
// Incremento il contatore assoluto
template< class NODETYPE >
void Counter< NODETYPE >::incrCount(NODETYPE nt) { 
  generalCount.find(nt)->second.count++; 
}

// Incrementa il contatore con il peso
template< class NODETYPE >
void Counter< NODETYPE >::incrCount(NODETYPE nt,int peso) {
  typename MapType::iterator i;

  i=generalCount.find(nt);
  if (i==generalCount.end()) {
    generalCount[nt].count=peso;
    generalCount[nt].ordine=0;
  }
  else { i->second.count+=peso; }
}


template< class NODETYPE >
double Counter< NODETYPE >::getSupportForItemSet(deque<NODETYPE>& body) { 
  NODETYPE lastElem = body.back();
  body.pop_back();

  typename MapType::iterator i;

  i=generalCount.find(lastElem);
  assert(i!=generalCount.end());

  if( body.empty() ) 
    return i->second.count;
  else
    return i->second.fptree.getSupportForItemSet(body);
}

// Copia i valori assoluti in quel momento nel campo ordine.
// Serve per riordinare l'albero in modo corretto
template< class NODETYPE >
void Counter< NODETYPE >::setOrdina() {
  MapType& m1 = getMap();
  typename MapType::iterator i;

  for ( i=m1.begin( ) ; i != m1.end( ) ; i++ )
    i->second.ordine=i->second.count;
}


// Funzione utile per debug.
// Stampa il contatore generale. Stampa valore, count e ordine
template< class NODETYPE >
void Counter< NODETYPE >::leggiCounter() {
  std::cout <<  "Contatore Generale! " << std::endl;
  MapType& m1 = getMap();
  typename MapType::iterator i;

  for (i=m1.begin( ) ; i !=m1.end( ) ; i++ )
    std::cout << i->first.c_str() << ", " << i->second.count<< ", " << i->second.ordine <<std::endl;

}



// In questa procedura creo tutti i cammini possibili.
// In pratica genero il lattice.
// Genero tutte le sottostringhe possibili e con il vettore traduco in cammini da inserire
// nel fp-tree
// Ricreo un StringTokenizer e poi inserisco nell'albero fp-tree conditional
template< class NODETYPE >
void Counter< NODETYPE >::addWalk(NODETYPE nt,const std::vector<NODETYPE>& transaction,int peso) {
  typename MapType::iterator it;

  unsigned int y=transaction.size();
  assert(y<32);
  unsigned int numIt=1<<y;
  //cout << "NUMIT" <<  numIt << std::endl;
  deque<NODETYPE> result;

  // Si usa un unsigned int per calcolare tutti
  // i possibili sottoinsiemi del vettore in input (transaction)

  it=generalCount.find(nt);
  assert(it!=generalCount.end());

  for (unsigned k=1;k<numIt;k++) {
    result.clear();

    for(unsigned int i=0;i<y; i++) {
      size_t index = 1<<i;
      if( index & k ) 
	result.push_back(transaction[i]);
    }

    // Inserisco il cammino nell'albero fp-tree conditional
    if(!result.empty()) {
      //      #warning DEBUG
      //cout << "!!! transa" << std::endl;
      //copy( transaction.begin(), transaction.end(), std::ostream_iterator<NODETYPE>(cout, ";"));
      //cout << "!!! " << std::endl;
      //cout << "*** result" << std::endl;
      //copy( result.begin(), result.end(), std::ostream_iterator<NODETYPE>(cout, ";"));
      //cout << "*** " << std::endl;
      it->second.fptree.insertAddWalk(result,peso);
    }
  }
}


// Ciclo su tutta la struttura e leggo gli alberelli.
template< class NODETYPE >
void Counter< NODETYPE >::extractRule(Connection& connection, double nSup, double totGroups) {
  MapType& m1 = getMap();
  typename MapType::iterator i;
  //  odbc::ResultSet* result;

  //connection.deleteDestTable();
  //connection.create_db_rule(0);

  //  MRLog() <<" ---- RULE WITH SUPPORT OK ----" <<std::endl;
  for( i=m1.begin( ) ; i !=m1.end( ) ; i++ ) {
    if(i->second.count>=nSup) {
      //      std::cout << "prima di printRules ord:"<< i->second.count <<"elem:" << i->first << std::endl;
      i->second.fptree.printRules(i->first,connection, totGroups, *this);
      //      std::cout << "dopo printRules" << std::endl;
    }
  }
}


// Dal singolo elemento della lista ritorno verso l'alto e legge il cammino.
// Lo inserisce poi nel fp-tree conditional.
// Per un uso efficiente inserisce i sottocammini letti nei fp-tree conditinal corretti.
// Man mano che inserisco i sottocamini marco i nodi in modo da non ripetere il lavoro
// di lettura
template< class NODETYPE >
void Counter< NODETYPE >::estrai_cammini(TreeNode<NODETYPE>* tnd, std::vector<NODETYPE>& path) {
  TreeNode<NODETYPE>* dad;
  NODETYPE node;
  int peso;
  dad=tnd->getFather();

  if (dad->getData()!=Tree<NODETYPE>::ROOTNODE) {
    node=tnd->getData();
    // std::cout<<"nodo "<<node<<"papa "<<dad->getData()<<"dentro estrai"<<std::endl;
    estrai_cammini(dad,path);
    path.push_back(dad->getData());
    if (!tnd->isMarked()) {
      peso=tnd->getCount();
      // std::cout<<"add walk"<< path<<" in node "<<(String)node<<" peso "<<peso<<std::endl;
      //#warning DEBUG 
      //      std::cout << "before addWalk:";
      //      std::cout << node << std::endl;
      //      copy(path.begin(), path.end(), std::ostream_iterator<NODETYPE>(cout,";"));
      //      std::cout << std::endl;

      addWalk(node,path,peso);
      tnd->setMarked();
    }
  }
}

// Dal singolo elemento della lista ritorno verso l'alto e legge il cammino.
// Lo inserisce poi nel fp-tree conditional.
// Per un uso efficiente inserisce i sottocammini letti nei fp-tree conditinal corretti.
// Man mano che inserisco i sottocamini marco i nodi in modo da non ripetere il lavoro
// di lettura
template< class NODETYPE >
void Counter< NODETYPE >::estrai_cammini_lazy(TreeNode<NODETYPE>* tnd, std::vector<NODETYPE>& path) {
  TreeNode<NODETYPE>* dad = tnd->getFather();

  while(dad->getData()!=Tree<NODETYPE>::ROOTNODE) {
    path.push_back(dad->getData());
    dad=dad->getFather();
  }

  reverse(path.begin(), path.end());

  addWalk(tnd->getData(),path,tnd->getCount());
}


// Legge tutte le liste partendo da quelle con il nodo con il contatore più basso
// Terminata la lista fa il cut finale del fp-tree conditional.
template< class NODETYPE >
void Counter< NODETYPE >::esplodi(Connection& connection,double nSup, double totGroups, bool originalFPGrowth) {
  MapType& m1 = getMap();
  typename MapType::reverse_iterator i;
  typename MapType::iterator j;
  typedef multimap <int, NODETYPE> mmap;
  mmap coll;
  typename mmap::iterator k;
  TreeNode<NODETYPE>* tempor;
  NODETYPE node;
  std::vector<NODETYPE> pth;

  for( i=m1.rbegin( ) ; i !=m1.rend( ) ; i++ ) {
    coll.insert( make_pair(i->second.ordine,i->first) );
  }

  for( k=coll.begin( ) ; k !=coll.end( ) ; k++ ) {
    j=generalCount.find(k->second);
    assert(j!=generalCount.end());
    //    std::cout<<"dentro ESPLODI... ordine:"<<k->first<<" nodo :"<<k->second<<std::endl;
    if( k->first>=nSup ) {
      node=k->second;
      while (!j->second.lista.empty() ) {
	tempor=j->second.lista.front();
	pth.clear();
	//#warning estrai cammini lazy!!!#warning estrai cammini lazy!!!
	//estrai_cammini_lazy(tempor,pth);
	estrai_cammini(tempor,pth);
	j->second.lista.pop_front();

	// Taglio anticipato
	if (!originalFPGrowth && 
	    !j->second.lista.empty() &&
	    !tempor->haveSameAncestor(j->second.lista.front()) && 
	    (tempor->getAncestor()->getData())!=Tree<NODETYPE>::ROOTNODE) {
	  j->second.fptree.cut(tempor->getAncestor()->getData(),nSup);
	}
      }
      
      j->second.fptree.cut(nSup);
      //      j->second.fptree.printRules(node, connection, totGroups);
      //      #warning abilitato clearTree...
      //      j->second.fptree.clearTree();
    }
  }
}


// Crea la lista dei nodi. E' importante che si generi garantendo che questa lista
// vada sempre da sinistra verso destra. In tal modo con gli ancestor possiamo
// utilizzare la teoria dei tagli anticipati
template< class NODETYPE >
void Counter< NODETYPE >:: insertInList(NODETYPE node,TreeNode<NODETYPE>* value) {
  typename MapType::iterator i;

  i=generalCount.find(node);
  assert(i!=generalCount.end());
  i->second.lista.push_back(value);
}


template< class NODETYPE >
void Counter< NODETYPE >::print() {
  typename MapType::iterator i;
  MRLog() << "Inizio Contatore Generale"<<std::endl;
  for( i=generalCount.begin( ) ; i !=generalCount.end( ) ; i++ )
    std::cout<<"elemento "<<i->first.c_str()<<" count "<<i->second.count<<std::endl;

  MRLog() <<"fine Contatore Generale"<<std::endl;
}
#endif
