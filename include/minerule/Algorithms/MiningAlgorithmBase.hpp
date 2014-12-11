//   Minerule - a sql-like language for datamining
//   Copyright (C) 2013 Roberto Esposito (esposito@di.unito.it)
//
//   This program is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
//   (at your option) any later version.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with this program.  If not, see <http://www.gnu.org/licenses/>.
#ifndef __MININGALGORITHM_BASE_H__
#define __MININGALGORITHM_BASE_H__

#include "AlgorithmsOptions.hpp"
#include "minerule/Optimizer/OptimizedMinerule.hpp"
#include "minerule/Utils/AlgorithmTypes.hpp"
#include "minerule/Database/Connection.hpp"
#include "minerule/Database/SourceTableRequirements.hpp"

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
			;
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

			MinMaxPair bodyCards( options.getBodyCardinalities() );
			bodyCards.applyConstraints(mrOptions.getParsers().getBodyCardinalities());
			options.setBodyCardinalities(bodyCards);

			MinMaxPair headCards( options.getHeadCardinalities() );
			headCards.applyConstraints(mrOptions.getParsers().getHeadCardinalities());
			options.setHeadCardinalities(headCards);

			connection.useMRDBConnection(MineruleOptions::getSharedOptions().getMRDB().getMRDBConnection());
			connection.setOutTableName(minerule.getParsedMinerule().tab_result);
			connection.setBodyCardinalities(minerule.getParsedMinerule().bodyCardinalities);
			connection.setHeadCardinalities(minerule.getParsedMinerule().headCardinalities);
			connection.createResultTables(SourceRowMetaInfo(connection.getMRDBConnection(), minerule.getParsedMinerule()));
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
