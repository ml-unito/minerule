/* TREENODE.H
In questo file si definisce la classe del nodo.
Gli elementi sono tutti privati e per accedervi si sono scritti dei metodi.
Si è usato il prefisso set per i metodi che impostano un valore e il prefisso get per
i metodi che richiedono un valore.
Di seguito sono commentati i campi e i metodi.
Vedi file treenode.doc per esempi e immagini di spiegazioni.
*/

#ifndef TREENODE_H
#define TREENODE_H

#include <map>
#include <vector>

#include "Counter.h"
using namespace std;


template< class NODETYPE > class Tree;            // dichiarazione succ.


template< class NODETYPE >
class TreeNodeSorter {
 private:
  const Counter<NODETYPE>* ptr;
  TreeNodeSorter();

 public:
  bool operator()(const NODETYPE& n1, const NODETYPE& n2) {
    return (*ptr)(n1,n2);
  }

  TreeNodeSorter(const Counter<NODETYPE>& counter) :ptr(&counter) {
  }

  const Counter<NODETYPE>& getCounter() const {
    return *ptr;
  }
};


template< class NODETYPE >
class TreeNode
{
  friend class Tree< NODETYPE >;
  public:
    typedef map <NODETYPE,TreeNode< NODETYPE >*, TreeNodeSorter<NODETYPE> >   MapType;

// Costruttore
    TreeNode( const Counter<NODETYPE>& counter, const NODETYPE &d, int val);

// Imposta e ritorna il padre
    void setFather(TreeNode<NODETYPE>* fath) { father=fath; }
    TreeNode<NODETYPE>* getFather() { return father; }

// Imposta e ritorna il valore dell'antenato più lontano. Vedi descrizione più in basso
// haveSameAncestor controlla se hanno lo stesso antenato comune.
    void setAncestor(TreeNode<NODETYPE>* anc)  { ancestor=anc; }
    TreeNode<NODETYPE>* getAncestor() { return ancestor;}
    bool haveSameAncestor(TreeNode<NODETYPE>* next)
      { return ( (this->getAncestor())==(next->getAncestor())); }

// Ritorna il valore del nodo.
    NODETYPE getData() const { return data; }

// Inserisci il valore e il nodo nella map di questo nodo.
    void insertNode(const NODETYPE nt, TreeNode<NODETYPE> * node);

// Imposta e restituisce i valori del numero di figli che ha il nodo
    int getNumChild() const { return numChild; }
    void incChild() { numChild++; }

// Contatore. Posso incrementare, decrementare e ritornare il valore.
    void incCount(int i) { count=count+i; }
    int getCount() const { return count; }
    void decCount(int value) { count=count-value; }

// Valore boolo per sapere su un nodo è marcato o no. Vedere quando serve più avanti.
    bool isMarked() { return hasMarked; }
    void setMarked() { hasMarked=true; }

// Ogni nodo può avere da 0 a n figli. I ptr sono memorizzati in una map!
// La getMap è importantissima torna la mappa ovvero tutte le coppie di valori
// nodo,ptr dei figli del nodo in cui è richiamata questa funzione
    MapType& getMap() { return mapChild; }

// Cerca all'interno della mappa (ovvero dei figli) un valore
    TreeNode<NODETYPE>* find(const NODETYPE nt);

  private:
    TreeNode();

// tipo del dato e dato stesso.
    NODETYPE data;

// Ogni nodo può avere da 0 a n figli. I ptr sono memorizzati in una map!
    MapType  mapChild;

// Ptr al padre diretto.
    TreeNode<NODETYPE>* father;

// Ptr all'antenato più lontano diverso da ROOTNODE (eccezione: per i 
// nodi a livello 1 (i.e., i figli di ROOTNODE), l'ancestor e' ROOTNODE)
    TreeNode<NODETYPE>* ancestor;

// Numero dei figli di questo nodo
    int numChild;

// Contatore del nodo.
    int count;

// Bool che indica se questo nodo è stato marcato o no questo nodo.
    bool hasMarked;
};

/* IMPLEMENTAZIONE DEI METODI DI TREENODE.H */

// Costruttore. Imposto i valori degli elementi principali.
template< class NODETYPE >
TreeNode< NODETYPE >::TreeNode(const Counter<NODETYPE>& counter, const NODETYPE &d, int val) 
  :  data(d), 
     mapChild(TreeNodeSorter<NODETYPE>(counter)), 
     numChild(0), 
     count(val), 
     hasMarked(false) {
}


// Inserisce nella map dei figli il nuovo figlio con i riferimenti.
// Potevo anche usare la primitiva mapChild.insert(make_pair(nt,node))
// Questa scrittura risulta più immediata.
template< class NODETYPE >
void TreeNode< NODETYPE >::insertNode(const NODETYPE nt,TreeNode<NODETYPE> * node) {
  mapChild[nt]=node;
}


// Cerca all'interno della map se esiste un nodo figlio.
template< class NODETYPE >
TreeNode<NODETYPE>* TreeNode< NODETYPE >::find(const NODETYPE nt) {
  typename map<NODETYPE,TreeNode<NODETYPE>*>::iterator i = mapChild.find(nt);

  if (i == mapChild.end()) return NULL;
  else return i->second;
}
#endif
