/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *   Copyright (C) 2006-2008 by Jim Pattee <jimp03@email.com>
 *   Copyright (C) 1998-2002 by Tal Davidson
 *   Copyright (C) 2026 by tunalad <tunalad@proton.me>
 *
 *   Quaketastic Style is a derivative of Artistic Style (astyle) 1.23,
 *   trimmed and modified for formatting QuakeC source files.
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU Lesser General Public License as
 *   published by the Free Software Foundation, either version 3 of the
 *   License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *   GNU Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public
 *   License along with this program. If not, see
 *   <http://www.gnu.org/licenses/>.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

#include "astyle_main.h"
#include "ASHelp.h"
#include "astyle.h"

#include <cstdlib>
#include <errno.h>
#include <fstream>
#include <iostream>

namespace astyle {

#define IS_OPTION(arg, op) ((arg).compare(op) == 0)
#define IS_OPTIONS(arg, a, b) (IS_OPTION((arg), (a)) || IS_OPTION((arg), (b)))

#define GET_PARAM(arg, op) ((arg).substr(strlen(op)))
#define GET_PARAMS(arg, a, b)                                                  \
    (isParamOption((arg), (a)) ? GET_PARAM((arg), (a)) : GET_PARAM((arg), (b)))

// console build variables
ostream *_err = &cerr; // direct error messages to cerr
ASConsole g_console;   // class to encapsulate console variables

#ifndef VERSION
#define VERSION "1.23"
#endif

const char *_version = VERSION;

/**
 * parse the options vector
 * ITER can be an optionsVector (command line)
 *
 * Style options (--style=*, -A*) are always applied first, regardless of
 * argument order, so that additional options can override them.
 *
 * @return        true if no errors, false if errors
 */
template <typename ITER>
bool parseOptions(ASFormatter &formatter, const ITER &optionsBegin,
                  const ITER &optionsEnd, const string &errorInfo) {
    ITER option;
    bool ok = true;

    auto isStyleOption = [](const string &normalized) {
        return normalized.compare(0, 6, "style=") == 0 ||
               isParamOption(normalized, "A");
    };

    // apply sub-options from a raw arg, filtering by style/non-style
    auto applyArg = [&](const string &raw, bool styleOnly) -> bool {
        bool result = true;
        if (raw.compare(0, 2, "--") == 0) {
            string normalized = raw.substr(2);
            bool match = isStyleOption(normalized);
            if (match == styleOnly)
                result &= parseOption(formatter, normalized, errorInfo);
        } else if (raw[0] == '-') {
            // combined short options: -A8s4 → A8, s4
            string subArg;
            for (size_t i = 1; i < raw.length(); ++i) {
                if (isalpha(raw[i]) && i > 1) {
                    bool match = isStyleOption(subArg);
                    if (match == styleOnly)
                        result &= parseOption(formatter, subArg, errorInfo);
                    subArg.clear();
                }
                subArg.append(1, raw[i]);
            }
            if (!subArg.empty()) {
                bool match = isStyleOption(subArg);
                if (match == styleOnly)
                    result &= parseOption(formatter, subArg, errorInfo);
            }
        } else {
            bool match = isStyleOption(raw);
            if (match == styleOnly)
                result &= parseOption(formatter, raw, errorInfo);
        }
        return result;
    };

    // pass 1: apply style options first
    for (option = optionsBegin; option != optionsEnd; ++option)
        ok &= applyArg(*option, true);

    // pass 2: apply all non-style options
    for (option = optionsBegin; option != optionsEnd; ++option)
        ok &= applyArg(*option, false);

    return ok;
}

bool isParamOption(const string &arg, const char *option) {
    bool retVal = arg.compare(0, strlen(option), option) == 0;
    // if comparing for short option, 2nd char of arg must be numeric
    if (retVal && strlen(option) == 1 && arg.length() > 1)
        if (!isdigit(arg[1]))
            retVal = false;
    return retVal;
}

void isOptionError(const string &arg, const string &errorInfo) {
    if (errorInfo.length() > 0) // to avoid a compiler warning
        (*_err) << "Error in param: " << arg << endl;
}

bool isParamOption(const string &arg, const char *option1,
                   const char *option2) {
    return isParamOption(arg, option1) || isParamOption(arg, option2);
}

bool parseOption(ASFormatter &formatter, const string &arg,
                 const string &errorInfo) {
    if (IS_OPTION(arg, "style=allman") || IS_OPTION(arg, "style=ansi") ||
        IS_OPTION(arg, "style=bsd")) {
        formatter.setFormattingStyle(STYLE_ALLMAN);
    } else if (IS_OPTION(arg, "style=k&r") || IS_OPTION(arg, "style=k/r")) {
        formatter.setFormattingStyle(STYLE_KandR);
    } else if (IS_OPTION(arg, "style=stroustrup")) {
        formatter.setFormattingStyle(STYLE_STROUSTRUP);
    } else if (IS_OPTION(arg, "style=whitesmith")) {
        formatter.setFormattingStyle(STYLE_WHITESMITH);
    } else if (IS_OPTION(arg, "style=banner")) {
        formatter.setFormattingStyle(STYLE_BANNER);
    } else if (IS_OPTION(arg, "style=gnu")) {
        formatter.setFormattingStyle(STYLE_GNU);
    } else if (IS_OPTION(arg, "style=linux")) {
        formatter.setFormattingStyle(STYLE_LINUX);
    } else if (IS_OPTION(arg, "style=quakec")) {
        formatter.setFormattingStyle(STYLE_QUAKEC);
    } else if (isParamOption(arg, "A")) {
        int style = 0;
        string styleParam = GET_PARAM(arg, "A");
        if (styleParam.length() > 0)
            style = atoi(styleParam.c_str());
        if (style < 1 || style > 8)
            isOptionError(arg, errorInfo);
        else if (style == 1)
            formatter.setFormattingStyle(STYLE_ALLMAN);
        else if (style == 2)
            formatter.setFormattingStyle(STYLE_KandR);
        else if (style == 3)
            formatter.setFormattingStyle(STYLE_STROUSTRUP);
        else if (style == 4)
            formatter.setFormattingStyle(STYLE_WHITESMITH);
        else if (style == 5)
            formatter.setFormattingStyle(STYLE_BANNER);
        else if (style == 6)
            formatter.setFormattingStyle(STYLE_GNU);
        else if (style == 7)
            formatter.setFormattingStyle(STYLE_LINUX);
        else if (style == 8)
            formatter.setFormattingStyle(STYLE_QUAKEC);
    }
    // must check for mode=c
    else if (isParamOption(arg, "t", "indent=tab=")) {
        int spaceNum = 4;
        string spaceNumParam = GET_PARAMS(arg, "t", "indent=tab=");
        if (spaceNumParam.length() > 0)
            spaceNum = atoi(spaceNumParam.c_str());
        if (spaceNum < 1 || spaceNum > 20)
            isOptionError(arg, errorInfo);
        else
            formatter.setTabIndentation(spaceNum, false);
    } else if (IS_OPTION(arg, "indent=tab")) {
        formatter.setTabIndentation(4);
    } else if (isParamOption(arg, "T", "indent=force-tab=")) {
        int spaceNum = 4;
        string spaceNumParam = GET_PARAMS(arg, "T", "indent=force-tab=");
        if (spaceNumParam.length() > 0)
            spaceNum = atoi(spaceNumParam.c_str());
        if (spaceNum < 1 || spaceNum > 20)
            isOptionError(arg, errorInfo);
        else
            formatter.setTabIndentation(spaceNum, true);
    } else if (IS_OPTION(arg, "indent=force-tab")) {
        formatter.setTabIndentation(4, true);
    } else if (isParamOption(arg, "s", "indent=spaces=")) {
        int spaceNum = 4;
        string spaceNumParam = GET_PARAMS(arg, "s", "indent=spaces=");
        if (spaceNumParam.length() > 0)
            spaceNum = atoi(spaceNumParam.c_str());
        if (spaceNum < 1 || spaceNum > 20)
            isOptionError(arg, errorInfo);
        else
            formatter.setSpaceIndentation(spaceNum);
    } else if (IS_OPTION(arg, "indent=spaces")) {
        formatter.setSpaceIndentation(4);
    } else if (isParamOption(arg, "m", "min-conditional-indent=")) {
        int minIndent = 8;
        string minIndentParam = GET_PARAMS(arg, "m", "min-conditional-indent=");
        if (minIndentParam.length() > 0)
            minIndent = atoi(minIndentParam.c_str());
        if (minIndent > 40)
            isOptionError(arg, errorInfo);
        else
            formatter.setMinConditionalIndentLength(minIndent);
    } else if (isParamOption(arg, "M", "max-instatement-indent=")) {
        int maxIndent = 40;
        string maxIndentParam = GET_PARAMS(arg, "M", "max-instatement-indent=");
        if (maxIndentParam.length() > 0)
            maxIndent = atoi(maxIndentParam.c_str());
        if (maxIndent > 80)
            isOptionError(arg, errorInfo);
        else
            formatter.setMaxInStatementIndentLength(maxIndent);
    } else if (IS_OPTIONS(arg, "B", "indent-brackets")) {
        formatter.setBracketIndent(true);
    } else if (IS_OPTIONS(arg, "G", "indent-blocks")) {
        formatter.setBlockIndent(true);
    } else if (IS_OPTIONS(arg, "S", "indent-switches")) {
        formatter.setSwitchIndent(true);
    } else if (IS_OPTIONS(arg, "K", "indent-cases")) {
        formatter.setCaseIndent(true);
    } else if (IS_OPTIONS(arg, "L", "indent-labels")) {
        formatter.setLabelIndent(true);
    } else if (IS_OPTIONS(arg, "y", "break-closing-brackets")) {
        formatter.setBreakClosingHeaderBracketsMode(true);
    } else if (IS_OPTIONS(arg, "b", "brackets=break")) {
        formatter.setBracketFormatMode(BREAK_MODE);
    } else if (IS_OPTIONS(arg, "a", "brackets=attach")) {
        formatter.setBracketFormatMode(ATTACH_MODE);
    } else if (IS_OPTIONS(arg, "l", "brackets=linux")) {
        formatter.setBracketFormatMode(LINUX_MODE);
    } else if (IS_OPTIONS(arg, "u", "brackets=stroustrup")) {
        formatter.setBracketFormatMode(STROUSTRUP_MODE);
    } else if (IS_OPTIONS(arg, "O", "keep-one-line-blocks")) {
        formatter.setBreakOneLineBlocksMode(false);
    } else if (IS_OPTIONS(arg, "o", "keep-one-line-statements")) {
        formatter.setSingleStatementsMode(false);
    } else if (IS_OPTIONS(arg, "P", "pad-paren")) {
        formatter.setParensOutsidePaddingMode(true);
        formatter.setParensInsidePaddingMode(true);
    } else if (IS_OPTIONS(arg, "d", "pad-paren-out")) {
        formatter.setParensOutsidePaddingMode(true);
    } else if (IS_OPTIONS(arg, "D", "pad-paren-in")) {
        formatter.setParensInsidePaddingMode(true);
    } else if (IS_OPTIONS(arg, "U", "unpad-paren")) {
        formatter.setParensUnPaddingMode(true);
    } else if (IS_OPTIONS(arg, "p", "pad-oper")) {
        formatter.setOperatorPaddingMode(true);
    } else if (IS_OPTIONS(arg, "E", "fill-empty-lines")) {
        formatter.setEmptyLineFill(true);
    } else if (IS_OPTIONS(arg, "w", "indent-preprocessor")) {
        formatter.setPreprocessorIndent(true);
    } else if (IS_OPTIONS(arg, "c", "convert-tabs")) {
        formatter.setTabSpaceConversionMode(true);
    } else if (IS_OPTIONS(arg, "F", "break-blocks=all")) {
        formatter.setBreakBlocksMode(true);
        formatter.setBreakClosingHeaderBlocksMode(true);
    } else if (IS_OPTIONS(arg, "f", "break-blocks")) {
        formatter.setBreakBlocksMode(true);
    } else if (IS_OPTIONS(arg, "e", "break-elseifs")) {
        formatter.setBreakElseIfsMode(true);
    } else if (IS_OPTIONS(arg, "x", "delete-empty-lines")) {
        formatter.setDeleteEmptyLinesMode(true);
    }
    // Options used by console
    else if (IS_OPTIONS(arg, "n", "suffix=none")) {
        g_console.noBackup = true;
    } else if (isParamOption(arg, "suffix=")) {
        string suffixParam = GET_PARAM(arg, "suffix=");
        if (suffixParam.length() > 0) {
            g_console.origSuffix = suffixParam;
        }
    } else if (IS_OPTIONS(arg, "X", "errors-to-stdout")) {
        _err = &cout;
    } else {
        (*_err) << errorInfo << arg << endl;
        return false; // invalid option
    }
    // End of parseOption function
    return true; // o.k.
}

//--------------------------------------------------------------------------------------
// ASStreamIterator class
// typename will be istream for console input
//--------------------------------------------------------------------------------------

template <typename T> ASStreamIterator<T>::ASStreamIterator(T *in) {
    inStream = in;
    buffer.reserve(200);
    eolWindows = eolLinux = 0;
    peekStart = 0;
    prevLineDeleted = false;
    checkForEmptyLine = false;
}

template <typename T> ASStreamIterator<T>::~ASStreamIterator() {}

// save the last input line after input has reached EOF
template <typename T> void ASStreamIterator<T>::saveLastInputLine() {
    assert(inStream->eof());
    prevBuffer = buffer;
}

/**
 * read the input stream, delete any end of line characters,
 *     and build a string that contains the input line.
 *
 * @return        string containing the next input line minus any end of line
 * characters
 */
template <typename T>
string ASStreamIterator<T>::nextLine(bool emptyLineWasDeleted) {
    // verify that the current position is correct
    assert(peekStart == 0);

    // a deleted line may be replaced if break-blocks is requested
    // this sets up the compare to check for a replaced empty line
    if (prevLineDeleted) {
        prevLineDeleted = false;
        checkForEmptyLine = true;
    }
    if (!emptyLineWasDeleted)
        prevBuffer = buffer;
    else
        prevLineDeleted = true;

    // read the next record
    buffer.clear();
    char ch;
    inStream->get(ch);

    while (!inStream->eof() && ch != '\n' && ch != '\r') {
        buffer.append(1, ch);
        inStream->get(ch);
    }

    if (inStream->eof()) {
        return buffer;
    }

    int peekCh = inStream->peek();

    // find input end-of-line characters
    if (!inStream->eof()) {
        if (ch == '\r') // CR+LF or CR is windows
        {
            if (peekCh == '\n') {
                inStream->get(ch);
            }
            eolWindows++;
        } else // LF is Linux, allow for improbable LF/CR
        {
            if (peekCh == '\r') {
                inStream->get(ch);
                eolWindows++;
            } else
                eolLinux++;
        }
    } else {
        inStream->clear();
    }

    // set output end of line characters
    if (eolWindows >= eolLinux)
        strcpy(outputEOL, "\r\n"); // Windows (CR+LF)
    else
        strcpy(outputEOL, "\n"); // Linux (LF)

    return buffer;
}

// save the current position and get the next line
// this can be called for multiple reads
// when finished peeking you MUST call peekReset()
// call this function from ASFormatter ONLY
template <typename T> string ASStreamIterator<T>::peekNextLine() {
    assert(hasMoreLines());
    string nextLine;
    char ch;

    if (peekStart == 0)
        peekStart = inStream->tellg();

    // read the next record
    inStream->get(ch);
    while (!inStream->eof() && ch != '\n' && ch != '\r') {
        nextLine.append(1, ch);
        inStream->get(ch);
    }

    if (inStream->eof()) {
        return nextLine;
    }

    int peekCh = inStream->peek();

    // remove end-of-line characters
    if (!inStream->eof()) {
        if ((peekCh == '\n' || peekCh == '\r') &&
            peekCh != ch) /////////////  changed  //////////
            inStream->get(ch);
    }

    return nextLine;
}

// reset current position and EOF for peekNextLine()
template <typename T> void ASStreamIterator<T>::peekReset() {
    assert(peekStart != 0);
    inStream->clear();
    inStream->seekg(peekStart);
    peekStart = 0;
}

//--------------------------------------------------------------------------------------
// ASConsole class
//--------------------------------------------------------------------------------------

void ASConsole::error(const char *why, const char *what) const {
    (*_err) << why << ' ' << what << '\n' << endl;
    (*_err) << "Quaketastic Style has terminated!" << endl;
    exit(EXIT_FAILURE);
}

/**
 * Open input file, format it, and close the output.
 *
 * @param fileName      The path and name of the file to be processed.
 * @param formatter     The formatter object.
 * @return              true if the file was formatted, false if it was not (no
 * changes).
 */
bool ASConsole::formatFile(const string &fileName,
                           ASFormatter &formatter) const {
    bool isFormatted = false; // return value

    // open input file
    ifstream in(fileName.c_str(), ios::binary);
    if (!in)
        error("Could not open input file", fileName.c_str());

    // open tmp file
    string tmpFileName = fileName + tempSuffix;
    // remove the pre-existing temp file, if present
    removeFile(tmpFileName.c_str(), "Could not remove pre-existing temp file");
    ofstream out(tmpFileName.c_str(), ios::binary);
    if (!out)
        error("Could not open output file", tmpFileName.c_str());

    // format the file

    ASStreamIterator<istream> streamIterator(&in);
    formatter.init(&streamIterator);

    bool filesAreIdentical = true; // input and output files are identical
    string nextLine;               // next output line
    while (formatter.hasMoreLines()) {
        nextLine = formatter.nextLine();
        out << nextLine;
        if (formatter.hasMoreLines())
            out << streamIterator.getOutputEOL();
        else
            streamIterator
                .saveLastInputLine(); // to compare the last input line

        if (filesAreIdentical) {
            if (streamIterator.checkForEmptyLine) {
                if (nextLine.find_first_not_of(" \t") != string::npos)
                    filesAreIdentical = false;
            } else if (!streamIterator.compareToInputBuffer(nextLine))
                filesAreIdentical = false;
            streamIterator.checkForEmptyLine = false;
        }
    }
    out.flush();
    out.close();
    in.close();

    // create output files

    // if input and output are identical, don't change anything
    if (filesAreIdentical) {
        removeFile(tmpFileName.c_str(), "Could not remove current temp file");
    } else {
        // create a backup
        if (!noBackup) {
            string origFileName = fileName + origSuffix;
            removeFile(origFileName.c_str(),
                       "Could not remove pre-existing backup file");
            renameFile(fileName.c_str(), origFileName.c_str(),
                       "Could not create backup file");
        } else
            removeFile(fileName.c_str(), "Could not remove previous file");

        // change tmp name to current (reformatted)
        renameFile(tmpFileName.c_str(), fileName.c_str(),
                   "Could not rename tmp file");

        isFormatted = true;
    }

    return isFormatted;
}

// printHelp and printHelpFull are in ASHelp.cpp

// process a command-line file path, including wildcards
void ASConsole::processFilePath(string &filePath, ASFormatter &formatter) {
    if (filePath.length() == 0)
        error("Missing filename in", filePath.c_str());

    // format the file
    bool isFormatted = formatFile(filePath, formatter);

    if (isFormatted)
        cout << "formatted  " << filePath.c_str() << endl;
    else
        cout << "unchanged* " << filePath.c_str() << endl;
}

// process options from the command line
// build the vectors fileNameVector and optionsVector
void ASConsole::processOptions(int argc, char *argv[], ASFormatter &formatter) {
    string arg;
    bool ok = true;

    // get command line options
    for (int i = 1; i < argc; i++) {
        arg = string(argv[i]);

        if (IS_OPTION(arg, "-h") || IS_OPTION(arg, "--help") ||
            IS_OPTION(arg, "-?")) {
            printHelp(*_err);
            exit(EXIT_SUCCESS);
        } else if (IS_OPTION(arg, "-H") || IS_OPTION(arg, "--help-full")) {
            printHelpFull(*_err);
            exit(EXIT_SUCCESS);
        } else if (IS_OPTION(arg, "-V") || IS_OPTION(arg, "--version")) {
            (*_err) << "Quaketastic Style Version " << _version << endl;
            exit(EXIT_SUCCESS);
        }

        else if (arg[0] == '-') {
            optionsVector.push_back(arg);
        } else // file-name
        {
            fileNameVector.push_back(arg);
        }
    }

    // parse the command line options vector for errors
    ok = parseOptions(formatter, optionsVector.begin(), optionsVector.end(),
                      string("Invalid command line option: "));
    if (!ok) {
        (*_err) << "For help on options, type 'qcstyle -h' \n" << endl;
        exit(EXIT_FAILURE);
    }
}

// remove a file and check for an error
void ASConsole::removeFile(const char *fileName, const char *errMsg) const {
    remove(fileName);
    if (errno == ENOENT) // no file is OK
        errno = 0;
    if (errno) {
        perror("errno message");
        error(errMsg, fileName);
    }
}

// rename a file and check for an error
void ASConsole::renameFile(const char *oldFileName, const char *newFileName,
                           const char *errMsg) const {
    rename(oldFileName, newFileName);
    if (errno) {
        perror("errno message");
        error(errMsg, oldFileName);
    }
}

// *******************   end of console functions
// ***********************************************

} // end of namespace astyle

// *******************   end of astyle namespace
// ***********************************************

using namespace astyle;

// **************************   main function
// ***************************************************

int main(int argc, char *argv[]) {
    ASFormatter formatter;

    // process command line options
    // build the vectors fileNameVector and optionsVector
    g_console.processOptions(argc, argv, formatter);

    // if no files have been given, use cin for input and cout for output
    // this is used to format text for text editors like TextWrangler
    // do NOT display any console messages when this branch is used
    if (g_console.fileNameVector.empty()) {
        ASStreamIterator<istream> streamIterator(
            &cin); // create iterator for cin
        formatter.init(&streamIterator);

        while (formatter.hasMoreLines()) {
            cout << formatter.nextLine();
            if (formatter.hasMoreLines())
                cout << streamIterator.getOutputEOL();
        }
        cout.flush();
        return EXIT_SUCCESS;
    }

    // indent the given files

    // loop thru input fileNameVector formatting the files
    for (size_t i = 0; i < g_console.fileNameVector.size(); i++) {
        g_console.processFilePath(g_console.fileNameVector[i], formatter);
    }

    return EXIT_SUCCESS;
}
