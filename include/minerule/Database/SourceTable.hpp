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
#ifndef SOURCETABLE_H_GE1S8NL9
#define SOURCETABLE_H_GE1S8NL9

#include "minerule/Database/SourceRow.hpp"
#include "minerule/Database/PrepareDataUtils.hpp"
#include "minerule/Algorithms/MiningAlgorithmBase.hpp"

namespace minerule {
class SourceTable {
public:
  typedef enum { BodyIterator, HeadIterator, FullIterator } IteratorKind;

  class Iterator {
    friend class SourceTable;

  private:
    mrdb::ResultSet *_resultSet;
    SourceRowColumnIds _columnIds;
    SourceRow *_sourceRow;
    size_t _rowCounter;
    bool _silent;

    Iterator(mrdb::ResultSet *resultSet, const SourceRowColumnIds &columnIds,
             bool silent)
        : _resultSet(resultSet), _columnIds(columnIds),
          _sourceRow(new SourceRow()), _rowCounter(0), _silent(silent) {

      assert(_resultSet != NULL);
      if (_resultSet->isBeforeFirst()) {
        next();
      }
    }

  public:
    Iterator() : _resultSet(NULL), _columnIds(), _sourceRow(NULL) {}
    Iterator(const Iterator &it)
        : _resultSet(it._resultSet), _columnIds(it._columnIds),
          _sourceRow(it._sourceRow) {}

    virtual ~Iterator() {}

    bool next();
    bool isAfterLast() const;

    SourceRow *get() {
      assert(_sourceRow != NULL);
      return _sourceRow;
    }
    SourceRow *operator->() { return get(); }

    Iterator &operator++() {
      next();
      return *this;
    }
  };

  SourceTable(const MiningAlgorithm &algorithm)
      : _minerule(algorithm.optimizedMinerule().getParsedMinerule()),
        _sourceTableRequirements(algorithm.sourceTableRequirements()),
        _pdu(_minerule, _sourceTableRequirements), _usesCrossProduct(false),
        _bodyStatement(NULL), _headStatement(NULL), _fullStatement(NULL) {
    init();
  };

  SourceTable(const ParsedMinerule &minerule,
              const SourceTableRequirements &requirements)
      : _minerule(minerule), _sourceTableRequirements(requirements),
        _pdu(_minerule, _sourceTableRequirements), _usesCrossProduct(false),
				_bodyStatement(NULL), _headStatement(NULL), _fullStatement(NULL)  {
    init();
  }

  virtual ~SourceTable();

  bool usesCrossProduct() const { return _usesCrossProduct; }

  void init();
  size_t getTotGroups();

  Iterator newIterator(IteratorKind);

private:
  /* data */
  const ParsedMinerule &_minerule;
  SourceTableRequirements _sourceTableRequirements;
  PrepareDataUtils _pdu;
  bool _usesCrossProduct;

  SourceRowColumnIds _columnIds;
  mrdb::PreparedStatement *_bodyStatement;
  mrdb::PreparedStatement *_headStatement;
  mrdb::PreparedStatement *_fullStatement;

  std::vector<mrdb::ResultSet *> _managedResults;

  /* methods */
  void initBodyHeadResultSets();
  void initFullResultSet();
};

} // namespace

#endif /* end of include guard: SOURCETABLE_H_GE1S8NL9 */
