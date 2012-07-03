template< class NODETYPE >
class Counter
{
  friend class Tree< NODETYPE >;
  public:
  typedef map < NODETYPE, cnt<NODETYPE> , NODETYPE > MapType;

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
    //    void addValues(StringTokenizer& mytoken, int peso);

// Aggiunge un cammino nel fp-tree conditional. Vedi struttura cnt
    void addWalk(NODETYPE nt, string transaction,int peso);

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
    void esplodi(double nSup);

// Estrae le rule
    void extractRule();

// Ritorna la map. Ovvero il ptr  alla struttura
    MapType& getMap() { return generalCount; }

  private:

// Dal singolo elemento della lista ritorno verso l'alto e legge il cammino.
// Lo inserisce poi nel fp-tree conditional.
// Per un uso efficiente inserisce i sottocammini letti nei fp-tree conditinal corretti.
    void estrai_cammini(TreeNode<NODETYPE>* tnd, string& path);

    // Sorts "elems" accordingly to the current state of the Counter
    void sortVector(deque<NODETYPE>& elems) const;

    // it is used by sortVector in order to compare to NODETYPEs accordingly
    // to current state of Counter
    bool compareElems(const NODETYPE& el1, const NODETYPE& el2) const;

// Formato map : valore nodo, contatore generale
    MapType generalCount;

};


template< class NODETYPE >
void Counter< NODETYPE >::sortVector(deque<NODETYPE>& elems) const {
    sort(elems.begin(), elems.end(), & Counter<NODETYPE>::compareElems );
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

  return el1<el2;
}
