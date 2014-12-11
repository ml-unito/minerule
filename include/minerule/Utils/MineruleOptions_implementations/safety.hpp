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
class Safety : public OptionBase {
  bool overwriteHomonymMinerules;
  bool allowCascadeDeletes;
public:
  Safety() : overwriteHomonymMinerules(false), 
		 allowCascadeDeletes(false) {}
  virtual ~Safety() {}

  virtual std::string className() const {
	return "safety";
  }
  
  virtual void setOption(const std::string& name, const std::string& value)
	;

  bool getOverwriteHomonymMinerules() const { return overwriteHomonymMinerules; };
  void setOverwriteHomonymMinerules(bool rhs) { overwriteHomonymMinerules = rhs; };

  bool getAllowCascadeDeletes() const { return allowCascadeDeletes; }
  void setAllowCascadeDeletes(bool rhs) { allowCascadeDeletes = rhs; }
};
