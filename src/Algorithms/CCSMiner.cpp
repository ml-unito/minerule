#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <string.h>
#include <assert.h>
#include <set>
#include <time.h>
#include "minerule/Algorithms/BitVector.h"
#include "minerule/Algorithms/CCSMiner.h"
#include "minerule/Database/PrepareDataUtils.h"

namespace minerule {

	void
	CCSMiner::prepareData() {
		PrepareDataUtils pdu(minerule.getParsedMinerule(), this->sourceTableRequirements());
		const ParsedMinerule& pm = minerule.getParsedMinerule();
		std::string groupAttrList = pdu.buildAttrListDescription(pm.ga);
		std::string ordAttrList = pdu.buildAttrListDescription(pm.oa);

		std::string sqlQuery = 
			"SELECT "+ groupAttrList+","
			+ pdu.buildAttrListDescription(pm.ba) + ","
			+ ordAttrList + " "
			+ "FROM "+ pm.tab_source + " "
			+ "ORDER BY "+groupAttrList+","+ordAttrList;

		//	std::cout << sqlQuery << std::endl;

		size_t last_elem;

		last_elem = rowDes.setgroupElems(1,pm.ga.size());

		rowDes.setBodyElems(last_elem+1, pm.ba.size());
		
		MRDebug() << "FSMiner query:" << sqlQuery.c_str() << std::endl;

		statement = connection.getODBCConnection()->prepareStatement(sqlQuery.c_str());
	}

	bool CCSMiner::find(std::vector<CCSMSequence*>* vec, CCSMSequence* elem){
	 std::vector<CCSMSequence*>::iterator it;
		bool trovato = false;
		for (it=vec->begin();it!=vec->end()&&!trovato;++it){
			if ((*(*it)) == *elem)
				trovato =true;
		}
		return trovato;
	}



	void CCSMiner::combina(std::vector<CCSMSequence::ResultItems>& result, std::vector<CCSMSequence*>* k2, size_t k, int min_g, int max_g, double threshold, int check){
		size_t seq_length=0;
		time_t init1, end1, init, end;

	 std::vector<size_t> pos_canc;
	 std::vector<CCSMSequence*>* rif_singleton= k2;
	 std::vector<CCSMSequence*>* k_rif;	


	 std::vector<CCSMSequence*>* k_2=k2;
	 std::vector<CCSMSequence*>* k_1=NULL;

		std::list<CCSMSequence*> suff;
		std::list<CCSMSequence*> pre;
		std::list<CCSMSequence*>::iterator pre_it;
		std::list<CCSMSequence*>::iterator suf_it;
	 std::vector<CCSMSequence*>* da_cancellare = new std::vector<CCSMSequence*>() ;
		bool canContinue=true;
		while (seq_length!=k&&canContinue){			
			time(&init1);
			//utilizzato per la fase di transaction reduction, mi memorizza le seq che sono di livello k e da cui al passo
			//successivo andro a generare le seq di livello k+1

			k_rif = new std::vector<CCSMSequence*>();


			k_1 = new std::vector<CCSMSequence*>();
			canContinue=false;			
			int cont=0;
			for (size_t i=0;i<k_2->size();++i){
				//partiziono lo spazio utilizzando le sequenze che hanno (*k_2)[i] come suffisso e come prefisso
				//(quelle di lunghezza k-1)e le combino per ottenere le sequenze di lunghezza k
				suff = (*k_2)[i]->getSuffixSequence();
				pre =  (*k_2)[i]->getPrefixSequence();

				for (pre_it=pre.begin();pre_it!=pre.end();++pre_it){
					for (suf_it=suff.begin();suf_it!=suff.end();++suf_it){					
						time(&init1);
						CCSMSequence* dino = 
							CCSMSequence::merge((*suf_it),(*pre_it)->getLastItem(),min_g,max_g,threshold);
						time(&end1);	
/*				//		std::cout<<"tempo di generazione "<<difftime(end1,init1)<<std::endl;
				//		std::cout<<(*suf_it)->toStdString()<<" + "<<std::endl<<"  "<<(*pre_it)->toStdString();
				//		std::cout<<" = "<<std::endl;
						std::cout<<"			 "<<dino->toStdString()<<" con conteggio "<<dino->getCount()<<std::endl;
*/
						if (dino->getCount()>=threshold){
								//inserisco i punatori alle sequenze di lunghezza k
								//nelle Prefix/Suffix list degli elementi k-1

							k_rif->push_back(dino);//utilizzato per la fase di transaction reduction

							(*suf_it)->addPrefixSequence(dino);			
							(*pre_it)->addSuffixSequence(dino);
								//cout<<"sequenza "<<dino->toStdString()<<std::endl;
							result.push_back(dino->getSequenceItems());
//								std::cout<<"OK"<<std::endl;
							}else 
								delete dino;
						}
					}
				}
			//controllo se al passo successivo e ancora possibile generare dei possibili candidati
			//inserisco nell'insieme k-1 gli elementi che appartengono alla lista dei suffissi o dei prefissi
			//delle sequenze degli elementi di k-2


				for (size_t i=0;i<k_2->size();++i){ 

					pre =  (*k_2)[i]->getPrefixSequence();

					for (pre_it=pre.begin();pre_it!=pre.end();++pre_it){
					//canGenerate testa se per quel determinato elemento c'e almeno un elemento nella lista dei suffissi
					//ed uno in quella dei prefissi, in modo che possa generare una possibile sequenza frequente di livello k
					//cout<<(*pre_it)->toStdString()<<" ";
						if ((*pre_it)->canGenerate()){
						//cout<<"puo generare"<<std::endl;
							bool trovato=find(k_1,(*pre_it));
							if (!trovato)
								k_1->push_back((*pre_it));
							cont++;
						//se ci sono sequenze che possono generare seq di livello k allora si puo continuare nel ciclo
							canContinue=true;
							}//se la sequenza non puo essere usata per generare ulteriori sequenze allora
					// viene inserita in un vettore di seq da cancellare
							else{ 
								bool trovato = find(da_cancellare,(*pre_it));
								if (!trovato)
									da_cancellare->push_back((*pre_it));
						//cout<<"inserisco in da_cancellare"<<std::endl;

							}
						}
						suff = (*k_2)[i]->getSuffixSequence();

						for (suf_it=suff.begin();suf_it!=suff.end();++suf_it){
					//cout<<(*suf_it)->toStdString()<<" ";
							if ((*suf_it)->canGenerate()){
						//se la sequenza non è gia presente allora si inserisce altrimenti niente		
						//cout<<"puo generare"<<std::endl;
								bool trovato=find(k_1,(*suf_it));
								if (!trovato)
									k_1->push_back((*suf_it));
								cont++;
								canContinue=true;
							}
							else {
								bool trovato = find(da_cancellare,(*suf_it));
								if (!trovato)
									da_cancellare->push_back((*suf_it));
						//cout<<"inserisco in da_cancellare"<<std::endl;

							}
						}
					}

				 std::vector<CCSMSequence*>::iterator del;
					if (seq_length!=0){
						for (del = k_2->begin();del!=k_2->end();++del){
							delete (*del);	
						}
						delete k_2;
					}

					time(&end1);
					k_2 = k_1;

					if (check>1&&canContinue){
						time(&init);

						pos_canc = CCSMSequence::reduce_tr(k_rif);
						time(&end);
						//	std::cout<<"tempo per scovare le transazioni non interessanti "<<difftime(end,init)<<std::endl;
						if (pos_canc.size()>1){
							time(&init);
							for (del=rif_singleton->begin();del!=rif_singleton->end();++del){
				//cout<<"ridotto singleton"<<std::endl;
								(*del)->reduction(pos_canc);
							}
							time(&end);
							//	std::cout<<"tempo riduzione di "<<rif_singleton->size()<<" singleton "<<difftime(end,init)<<std::endl;
							time(&init);
							for (del=k_rif->begin();del!=k_rif->end();++del){
					//cout<<"ridotta coppia"<<std::endl;
								(*del)->reduction(pos_canc);
							}
							time(&end);
							//	std::cout<<"tempo riduzione di "<<k_rif->size()<<" non singleton "<<difftime(end,init)<<std::endl;
						}
					}
					delete k_rif;

					//	std::cout<<"transazioni eliminate "<<pos_canc.size()<<" modalita' "<<check<<std::endl;
					seq_length++;
	//		std::cout<<"da cancellare size "<<da_cancellare->size()<<std::endl;
				 std::vector<CCSMSequence*>::iterator da_c;
		/*	canContinue=false;*/

					if (seq_length!=k&&canContinue){
						for ( da_c= da_cancellare->begin();da_c!=da_cancellare->end();++da_c){
	//			//	std::cout<<"cancello "<<(*da_c)->toStdString()<<" che hano sup "<<(*da_c)->getCount()<<std::endl;
							delete (*da_c);
						}
	//			std::cout<<"finita cancellazione: "<<da_cancellare->size()<<std::endl;
						delete da_cancellare;
						da_cancellare = new std::vector<CCSMSequence*>();
					}


				}
			 std::vector<CCSMSequence*>::iterator del, it_kk;
				std::list<CCSMSequence*>::iterator del1;
				for (del = k_1->begin();del!=k_1->end();++del)
					delete (*del);
	//	std::cout<<"fuori dal ciclo"<<std::endl;

			 std::vector<CCSMSequence*>* temp = new std::vector<CCSMSequence*>();

				for (it_kk= da_cancellare->begin();it_kk!=da_cancellare->end();++it_kk){
					suff = (*it_kk)->getSuffixSequence();
					pre =  (*it_kk)->getPrefixSequence();		

					for (del1=suff.begin();del1!=suff.end();del1++)
						temp->push_back(*del1);

					for (del1=pre.begin();del1!=pre.end();++del1)
						temp->push_back(*del1);
				}
				for (it_kk = temp->begin();it_kk!=temp->end();++it_kk){

					bool trov1 =find(da_cancellare,(*it_kk));
					if (!trov1)
						da_cancellare->push_back((*it_kk));
				}	
				delete temp;

				for (del= da_cancellare->begin();del!=da_cancellare->end();++del){
					delete (*del);
				}
	//	std::cout<<"finita cancellazione: "<<da_cancellare->size()<<std::endl;
				delete da_cancellare;
				delete k_1;
			}




			void CCSMiner::mineRules(){
				double time_tot=0;
				time_t init, end;

				const ParsedMinerule& pm = minerule.getParsedMinerule();

				int min_g=pm.sequenceAllowedGaps.getMin();
				int max_g=pm.sequenceAllowedGaps.getMax();

				double threshold=pm.sup;
				if (min_g>max_g)
					assert(false);

				int reduce = 1;
			 std::vector<CCSMSequence::ResultItems> result;
			 std::vector<CCSMSequence*> singleton;
				CCSMSequence input;
				CCSMSequence* single;
			 std::vector<CCSMSequence*>::iterator it;
				size_t number=0;
				time(&init);
				int riga=0;


				prepareData();
				odbc::ResultSet* rs = statement->executeQuery();
				
				rs->next();
				while (!rs->isAfterLast()){
					// SourceRow hbsr(rs,rowDes);
					input.read(rs, rowDes);	
				 	// std::cout<<"LEGGO SEQUENZA: "<<input.toStdString()<<std::endl;
					for(size_t i=0;i<input.size();++i){

						single= new CCSMSequence(input,i,1);

						single->setLastItem(single);	
						bool trovato=false;

						for (it = singleton.begin();it!=singleton.end()&&!trovato;it++){
							CCSMSequence* suppSingle=(*it);
							if (*suppSingle == *single){
								trovato = true;
								(*it)->setPresent(number,1);
								(*it)->setEid(number,i);
							}
						}   
						if (!trovato){
							single->setPresent(number,1);	
							single->setEid(number,i);
							singleton.push_back(single);
						}
						else 
							delete single;
					}
					input.svuota();
					number++;
					riga++;
				}

				std::cout<<std::endl;
				for (size_t i=0;i<singleton.size();i++){
					singleton[i]->setPresent(number-1,0);		
				}
				//	std::cout<<"numbe e uguale a: "<<number<<std::endl;
				//				in.close();
				//	std::cout<<"size "<<singleton.size()<<std::endl;
				time(&end);
				// double ttt = difftime(end,init);
				//			std::cout<<"tempo finora "<<ttt<<std::endl;
				//	std::cout<<"tempo medio per riga"<<ttt/number<<std::endl;
				time_tot=time_tot+difftime(end,init);
				time(&init);

				threshold=number*threshold;
				if (threshold == 0)
					threshold = 0.00001;

			 std::vector<CCSMSequence*>* suppSingleton= new std::vector<CCSMSequence*>();
			 std::vector<CCSMSequence*>::iterator itt;

				for (itt=singleton.begin();itt!=singleton.end();++itt){
					if ((*itt)->getCount()>=threshold){
						suppSingleton->push_back((*itt));
					}
					else delete (*itt);
				}

				//	std::cout<<"tempo finora "<<difftime(end,init)<<std::endl;
				time_tot=time_tot+difftime(end,init);

				if (reduce>0){
					time(&init);
					std::vector<size_t> temp = CCSMSequence::reduce_tr(suppSingleton);

					if (temp.size()!=0){
						for (itt=suppSingleton->begin();itt!=suppSingleton->end();++itt){
							(*itt)->reduction(temp);
						}
					}
					time(&end);
					//	std::cout<<"transazioni eliminate sui singleton"<<temp.size()<<std::endl;
					//cout<<"tempo della riduzione delle transazioni "<<difftime(end,init)<<std::endl;
					time_tot=time_tot+difftime(end,init);
				}

				time(&init);
				time_t init1, end1;
			 std::vector<CCSMSequence*>* coppie= new std::vector<CCSMSequence*>();	
				//	std::cout<<"la threshold e uguale a: "<<threshold<<std::endl;
				for (size_t i=0;i<suppSingleton->size();++i){
					for (size_t j=0;j<suppSingleton->size();++j){

						time(&init1);

						CCSMSequence* dino = 
							CCSMSequence::merge((*suppSingleton)[i],(*suppSingleton)[j]->getLastItem(),min_g,max_g,threshold);
						time(&end1);
						if (dino->getCount()>=threshold){
							coppie->push_back(dino);
							(*suppSingleton)[i]->addPrefixSequence(dino);
							(*suppSingleton)[j]->addSuffixSequence(dino);
							result.push_back(dino->getSequenceItems());

							}else 
								delete dino;
						}
					}
					time(&end);
					time_tot=time_tot+difftime(end,init);
					//cout<<"tempo x generare gli insiemi F1 ed F2 "<<difftime(end,init)<<std::endl;


					if (reduce>0){
						time(&init);
		//std::vector<size_t> 
						std::vector<size_t> temp = CCSMSequence::reduce_tr(coppie);
						if (temp.size()!=0){
							for (itt=suppSingleton->begin();itt!=suppSingleton->end();++itt){
				//cout<<"ridotto singleton"<<std::endl;
								(*itt)->reduction(temp);
							}
							for (itt=coppie->begin();itt!=coppie->end();++itt){
				//cout<<"ridotta coppia"<<std::endl;
								(*itt)->reduction(temp);
							}
						}
						time(&end);
						//	std::cout<<"transazioni eliminate "<<temp.size()<<std::endl;
						//cout<<"tempo della riduzione delle transazioni "<<difftime(end,init)<<std::endl;
						time_tot=time_tot+difftime(end,init);
					}
					//
					//		std::cout<<"prima di combina"<<std::endl;
					//cout<<"cardinalita F1 : "<<suppSingleton->size()<<std::endl;
					time(&init);

					combina(result, suppSingleton ,10, min_g,max_g,threshold,reduce);

					//cout<<"dopo combina"<<std::endl;
					time(&end);	

					for (size_t i=0;i<result.size();++i){
					 std::vector<ItemType>::iterator it_list = result[i].first.begin();
						for (;it_list!=result[i].first.end();++it_list)
							std::cout<<(*it_list)<<" ";
						std::cout<<" con conteggio: "<<result[i].second;
						std::cout<<std::endl;
					}

					time_tot=time_tot+difftime(end,init);
					//	std::cout<<"tempo x generare insiemi da F3.... "<<difftime(end,init)<<std::endl;
					//cout<<"tempo totale "<<time_tot<<std::endl;
					for (itt=suppSingleton->begin();itt!=suppSingleton->end();++itt)
						delete (*itt);
					delete coppie;
/*	for (itt=coppie.begin();itt!=coppie.end();++itt)
					delete (*itt);*/
						delete suppSingleton;
					std::cout<<"cardinalita insieme risultato "<<result.size()<<std::endl;
					//	std::cout<<"threshold "<<threshold<<std::endl;

				}

				} // namespace

