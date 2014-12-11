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
#include "minerule/Database/SourceRowAttribute.hpp"
#include <sstream>
#include "minerule/Utils/Converter.hpp"

#include "minerule/mrdb/ResultSetMetaData.hpp"

//#define DEBUG

namespace minerule {

  /********* static members *********/

	size_t MemDebugGenericSourceRowAttribute::instanceCount = 0;

  /**
	* Factory method - returns an attribute capable of handling types of the given type
   */
		SourceRowAttribute*
			SourceRowAttribute::createAttribute(mrdb::ResultSetMetaData* rsmd,
				mrdb::ResultSet* rs,

	int elem) {
		mrdb::Types::SQLType colType = (mrdb::Types::SQLType) rsmd->getColumnType(elem);
#ifdef DEBUG
#warning MemDebugGeneric...
		return new MemDebugGenericSourceRowAttribute(rs,elem,colType);
#else
		if(colType==mrdb::Types::INTEGER || colType==mrdb::Types::BIGINT)
			return new NumericSourceRowAttribute(rs,elem);
		else
			return new GenericSourceRowAttribute(rs,elem,colType);
#endif
	}


  /*
	* GENERIC SOURCE ROW ATTRIBUTE
		* Implementation of methods inherited from SourceRowAttribute
   */

			void
	GenericSourceRowAttribute::setValue( mrdb::ResultSet* rs, int col) {
		assert(rs!=NULL);
		assert(col<=rs->getMetaData()->getColumnCount());

		type = (mrdb::Types::SQLType) rs->getMetaData()->getColumnType(col);
		value = rs->getString(col);
	}

	void
	GenericSourceRowAttribute::setValue( const std::string& inStr ) {

		type = (mrdb::Types::SQLType) 4;
		value = inStr;
	}


	int
	GenericSourceRowAttribute::compareTo(const SourceRowAttribute& rhs) const {
		return value.compare(dynamic_cast<const GenericSourceRowAttribute&>(rhs).value);
	}

	mrdb::Types::SQLType
	GenericSourceRowAttribute::getType() const {
		return type;
	}

	std::string
	GenericSourceRowAttribute::asString(const std::string& sep) const {
		if(value == "")
			return "(not set)";
		else
			return value;
	}

	void
	GenericSourceRowAttribute::serialize(std::ostream& os) const {
		os << " @[ " << value << "@] ";
	}

	void
	GenericSourceRowAttribute::deserialize(std::istream& is) {
    // In order to avoid missing spaces etc. we resort to use istream::get method
    // instead of >> operator. This means that we need to read one char at a time
    // and implement a simple automata in order to parse the regular expression / @[ .* @] /
    // Pro: quite general
    // Cons: Nobody ensures that the strings @[ and @] does not appear in the value itself
    //       When this happens everything break
    // Solution: We should escape any '@' character in thestd::string before inserting it
    // into the database. (Cons: ``The insertion process becomes more costly...'')


		typedef enum {
			AS_NORMAL, // normal automa state
				AS_EXITING // The state in which the automa will be when ? has been read as the last char
					} AutomaState;

			std::string buf;
			char ch;

			assert(is);
			is >> buf;
			assert(is);

			if(buf!="@[")
				throw MineruleException(MR_ERROR_INTERNAL, "Expected `@[', but: `"+buf+"' found!");

			is.get(); // throwing away spurious space.

			bool finished = false;
			value = "";

			AutomaState state = AS_NORMAL;
			while(is.good() && !finished) {
				ch=is.get();

				if( ch==']' && state==AS_EXITING )
					finished=true;
				else {
					switch( ch ) {
						case '@':
							state = AS_EXITING;
							break;

						default:
							if(state==AS_EXITING)
								value += '@';

							value += ch;
							break;
					}
				}
			}

			if(!is)
				throw MineruleException(MR_ERROR_INTERNAL,
					"Unfinished Generic value, value read 'till now:"+value);
		}

		std::string
		GenericSourceRowAttribute::getSQLData() const {
			return std::string("'")+value+"'";
		}

  /* NUMERIC SOURCE ROW ATTRIBUTE */


		void
		NumericSourceRowAttribute::setValue( mrdb::ResultSet* rs, int col) {
			assert(rs!=NULL);
			assert(col<=rs->getMetaData()->getColumnCount());
			assert( getType() == (mrdb::Types::SQLType) rs->getMetaData()->getColumnType(col) );

			value = rs->getLong(col);
		}

		void
		NumericSourceRowAttribute::setValue( const std::string& inStr ) {
			value = (long int) strtod(inStr.c_str(), NULL);
		}

		int
		NumericSourceRowAttribute::compareTo(const SourceRowAttribute& rhs) const {
			long int res= value - dynamic_cast<const NumericSourceRowAttribute&>(rhs).value;
			return res;
		}

		std::string
		NumericSourceRowAttribute::asString(const std::string& sep) const {
			std::stringstream ss;
			ss << value;
			return ss.str();
		}

		std::string
		NumericSourceRowAttribute::getSQLData() const {
			std::stringstream ss;
			ss << "'" << value << "'";
			return ss.str();
		}


  /**
		* Serialization methods
   */

			void
		NumericSourceRowAttribute::serialize(std::ostream& os) const {
			os << " " << value << " ";
		}

		void NumericSourceRowAttribute::deserialize(std::istream& is) {
			std::string val;
			is >> val;
			try {
				value = Converter(val).toLong();
			} catch (MineruleException& e) {
				throw MineruleException( MR_ERROR_INTERNAL,
					"Cannot deserialize the requested NumericSourceRowAttribute,"
						"the reason is:" + std::string(e.what()) );
			}
		}

	}
