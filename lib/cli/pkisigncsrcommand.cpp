/******************************************************************************
 * Icinga 2                                                                   *
 * Copyright (C) 2012-2014 Icinga Development Team (http://www.icinga.org)    *
 *                                                                            *
 * This program is free software; you can redistribute it and/or              *
 * modify it under the terms of the GNU General Public License                *
 * as published by the Free Software Foundation; either version 2             *
 * of the License, or (at your option) any later version.                     *
 *                                                                            *
 * This program is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU General Public License for more details.                               *
 *                                                                            *
 * You should have received a copy of the GNU General Public License          *
 * along with this program; if not, write to the Free Software Foundation     *
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.             *
 ******************************************************************************/

#include "cli/pkisigncsrcommand.hpp"
#include "cli/pkiutility.hpp"
#include "base/logger.hpp"

using namespace icinga;
namespace po = boost::program_options;

REGISTER_CLICOMMAND("pki/sign-csr", PKISignCSRCommand);

String PKISignCSRCommand::GetDescription(void) const
{
	return "Reads a Certificate Signing Request from stdin and prints a signed certificate on stdout.";
}

String PKISignCSRCommand::GetShortDescription(void) const
{
	return "signs a CSR";
}

void PKISignCSRCommand::InitParameters(boost::program_options::options_description& visibleDesc,
    boost::program_options::options_description& hiddenDesc) const
{
	visibleDesc.add_options()
	    ("csrfile", po::value<std::string>(), "CSR file path (input)")
	    ("certfile", po::value<std::string>(), "Certificate file path (output)");
}

std::vector<String> PKISignCSRCommand::GetArgumentSuggestions(const String& argument, const String& word) const
{
	if (argument == "csrfile" || argument == "certfile")
		return GetBashCompletionSuggestions("file", word);
	else
		return CLICommand::GetArgumentSuggestions(argument, word);
}

/**
 * The entry point for the "pki sign-csr" CLI command.
 *
 * @returns An exit status.
 */
int PKISignCSRCommand::Run(const boost::program_options::variables_map& vm, const std::vector<std::string>& ap) const
{
	if (!vm.count("csrfile")) {
		Log(LogCritical, "cli", "Certificate signing request file path (--csrfile) must be specified.");
		return 1;
	}

	if (!vm.count("certfile")) {
		Log(LogCritical, "cli", "Certificate file path (--certfile) must be specified.");
		return 1;
	}

	return PkiUtility::SignCsr(vm["csrfile"].as<std::string>(), vm["certfile"].as<std::string>());
}
