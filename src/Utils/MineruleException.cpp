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
#include "minerule/Utils/MineruleException.hpp"
#include "minerule/Utils/StringUtils.hpp"

#include <iostream>

namespace minerule {

	#undef MineruleException

	MineruleException::MineruleException(std::string sourceFile, int sourceLine,  size_t errCode, std::string msg) : message(msg), file(sourceFile), line(sourceLine), errorCode(errCode) {
				formatMessage();
	}

	void MineruleException::formatMessage() _NOEXCEPT {
		std::stringstream ss;

		ss << "\n\t" << StringUtils::toBold("class:") << "\tMineruleException " << std::endl
			<< StringUtils::toBold("\tsource:\t") << file << ":" << line << std::endl
			<< StringUtils::toBold("\tcode:\t")<< errorCode
			<< " - " << me_error_name((MineruleErrors) errorCode) << std::endl
			<< StringUtils::toBold("\tmessage:") << StringUtils::toBoldRed("[");

		// std::vector<std::string>* chunks = StringUtils::splitToLength(message, 70);
		// ss << StringUtils::join(*chunks, "\n");
		ss << message;
		ss << StringUtils::toBoldRed("]");

		// delete chunks;

		formattedMessage = ss.str();
	}

	const char* MineruleException::what() const _NOEXCEPT {
		return formattedMessage.c_str();
	}

	const char* MineruleException::unformattedMessage() const _NOEXCEPT {
     return message.c_str();
	}

}
