/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *   Copyright (C) 2006-2008 by Jim Pattee <jimp03@email.com>
 *   Copyright (C) 1998-2002 by Tal Davidson
 *   <http://www.gnu.org/licenses/lgpl-3.0.html>
 *
 *   This file is a part of Artistic Style - an indentation and
 *   reformatting tool for C and C++ source files.
 *   <http://astyle.sourceforge.net>
 *
 *   Artistic Style is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU Lesser General Public License as published
 *   by the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   Artistic Style is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public License
 *   along with Artistic Style.  If not, see <http://www.gnu.org/licenses/>.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

#ifndef ASTYLE_MAIN_H
#define ASTYLE_MAIN_H

//----------------------------------------------------------------------------
// headers
//----------------------------------------------------------------------------

#include "astyle.h"

//using namespace astyle;

namespace astyle
{

//----------------------------------------------------------------------------
// ASStreamIterator class
// typename will be istream for console input
// ASSourceIterator is an abstract class defined in astyle.h
//----------------------------------------------------------------------------

template<typename T>
class ASStreamIterator : public ASSourceIterator
{
	public:
		bool checkForEmptyLine;

		// function declarations
		ASStreamIterator(T *in);
		virtual ~ASStreamIterator();
		string nextLine(bool emptyLineWasDeleted);
		string peekNextLine();
		void peekReset();
		void saveLastInputLine();

		// inline functions
		bool compareToInputBuffer(const string &nextLine) const { return nextLine == prevBuffer; }
		const char* getOutputEOL() const { return outputEOL; }
		bool hasMoreLines() const { return !inStream->eof(); }

	private:
		T * inStream;          // pointer to the input stream
		string buffer;         // current input line
		string prevBuffer;     // previous input line
		int eolWindows;        // number of Windows line endings (CRLF)
		int eolLinux;          // number of Linux line endings (LF)
		int eolMacOld;         // number of old Mac line endings (CR)
		int peekStart;         // starting position for peekNextLine()
		char outputEOL[4];     // output end of line char
		bool prevLineDeleted;  // the previous input line was deleted
};

//----------------------------------------------------------------------------
// ASConsole class for console build
//----------------------------------------------------------------------------

class ASConsole
{
	public:
		// command line options
		string origSuffix;                  // suffix= option
		bool noBackup;                      // suffix=none option
		// other variables
		string tempSuffix;                  // temporary file suffix

		vector<string> fileNameVector;      // file paths and names from the command line
		vector<string> optionsVector;       // options from the command line

	public:
		ASConsole() {
			// command line options
			origSuffix = ".orig";
			noBackup = false;
			// other variables
			tempSuffix = ".tmp";
		}
		void processFilePath(string &filePath, ASFormatter &formatter);
		void processOptions(int argc, char *argv[], ASFormatter &formatter);

	private:
		void error(const char *why, const char* what) const;
		bool formatFile(const string &fileName, astyle::ASFormatter &formatter) const;
		void printHelp() const;
		void removeFile(const char* fileName, const char* errMsg) const;
		void renameFile(const char* oldFileName, const char* newFileName, const char* errMsg) const;
};


//----------------------------------------------------------------------------
// global function declarations
// used by both console and library builds
//----------------------------------------------------------------------------

void isOptionError(const string &arg, const string &errorInfo);
bool isParamOption(const string &arg, const char *option);
bool isParamOption(const string &arg, const char *option1, const char *option2);
bool parseOption(astyle::ASFormatter &formatter, const string &arg, const string &errorInfo);

template<typename ITER>
bool parseOptions(astyle::ASFormatter &formatter, const ITER &optionsBegin,
                  const ITER &optionsEnd, const string &errorInfo);

}   // end of namespace astyle


#endif // closes ASTYLE_MAIN_H
