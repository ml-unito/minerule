#ifndef __MININGALGORITHM_BASE_H__
#define __MININGALGORITHM_BASE_H__

#include "AlgorithmsOptions.h"
#include "minerule/Optimizer/OptimizedMinerule.h"
#include "minerule/Utils/AlgorithmTypes.h"
#include "minerule/Database/Connection.h"
#include "minerule/Database/SourceTableRequirements.h"

namespace minerule {

/**
	* Base class from which each new mining algorithm will be derived
 */

	class MiningAlgorithmBase {
	protected:
		const OptimizedMinerule& minerule;
	public:
		MiningAlgorithmBase( const OptimizedMinerule& mr ) : minerule(mr) {}
		virtual ~MiningAlgorithmBase() {}
  
		virtual void execute() {
			throw MineruleException( MR_ERROR_INTERNAL, "This method should never be executed!");
		}

		virtual SourceTableRequirements sourceTableRequirements() const {
			return SourceTableRequirements();
		};

	
		virtual bool canHandleMinerule() const {
			return false;
		}
  
		virtual const OptimizedMinerule& optimizedMinerule() const { return minerule; }

  // Instantiate the algorithm specified by t
		static MiningAlgorithmBase* algorithmForType(AlgorithmTypes t, const OptimizedMinerule&) 
			throw(MineruleException);
	};

	class MiningAlgorithm : public MiningAlgorithmBase {
	protected:
		AlgorithmsOptions options;
		Connection connection;
	public:
		MiningAlgorithm(const OptimizedMinerule& m) : MiningAlgorithmBase(m) {}
		
		virtual void initialize() {
			MineruleOptions& mrOptions = MineruleOptions::getSharedOptions();

			options.setSupport( minerule.getParsedMinerule().sup );
			options.setConfidence( minerule.getParsedMinerule().conf );
			options.setBodyCardinalities( minerule.getParsedMinerule().bodyCardinalities);
			options.setHeadCardinalities( minerule.getParsedMinerule().headCardinalities);
			options.getBodyCardinalities().applyConstraints(mrOptions.getParsers().getBodyCardinalities());
			options.getHeadCardinalities().applyConstraints(mrOptions.getParsers().getHeadCardinalities());		
	  
			connection.useODBCConnection(MineruleOptions::getSharedOptions().getODBC().getODBCConnection());
			connection.setOutTableName(minerule.getParsedMinerule().tab_result);
			connection.setBodyCardinalities(minerule.getParsedMinerule().bodyCardinalities);
			connection.setHeadCardinalities(minerule.getParsedMinerule().headCardinalities);
			connection.createResultTables(SourceRowMetaInfo(connection.getODBCConnection(), minerule.getParsedMinerule()));
			connection.init();			
		}
		
		virtual void execute() {
			initialize();
			mineRules();
		}
		
		virtual void mineRules() { 
				throw MineruleException(MR_ERROR_INTERNAL, "This method should be implemented in sub-classes!");
		}
	
	};



}
#endif
