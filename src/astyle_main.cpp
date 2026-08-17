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

const char *_version = "1.23";

/**
 * parse the options vector
 * ITER can be an optionsVector (command line)
 *
 * @return        true if no errors, false if errors
 */
template <typename ITER>
bool parseOptions(ASFormatter &formatter, const ITER &optionsBegin,
                  const ITER &optionsEnd, const string &errorInfo) {
    ITER option;
    bool ok = true;
    string arg, subArg;

    for (option = optionsBegin; option != optionsEnd; ++option) {
        arg = *option;

        if (arg.compare(0, 2, "--") == 0)
            ok &= parseOption(formatter, arg.substr(2), errorInfo);
        else if (arg[0] == '-') {
            size_t i;

            for (i = 1; i < arg.length(); ++i) {
                if (isalpha(arg[i]) && i > 1) {
                    ok &= parseOption(formatter, subArg, errorInfo);
                    subArg = "";
                }
                subArg.append(1, arg[i]);
            }
            ok &= parseOption(formatter, subArg, errorInfo);
            subArg = "";
        } else {
            ok &= parseOption(formatter, arg, errorInfo);
            subArg = "";
        }
    }
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
    } else if (isParamOption(arg, "A")) {
        int style = 0;
        string styleParam = GET_PARAM(arg, "A");
        if (styleParam.length() > 0)
            style = atoi(styleParam.c_str());
        if (style < 1 || style > 7)
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

void ASConsole::printHelp() const {
    (*_err) << endl;
    (*_err) << "                           Quaketastic Style " << _version
            << endl;
    (*_err) << "                         Maintained by: Jim Pattee\n";
    (*_err) << "                       Original Author: Tal Davidson\n";
    (*_err) << endl;
    (*_err) << "Usage  :  qcstyle [options] Source1.qc Source2.qc  [...]\n";
    (*_err) << "          qcstyle [options] < Original > Beautified\n";
    (*_err) << endl;
    (*_err) << "When indenting a specific file, the resulting indented file "
               "RETAINS the\n";
    (*_err) << "original file-name. The original pre-indented file is renamed, "
               "with a\n";
    (*_err) << "suffix of \".orig\" added to the original filename.\n";
    (*_err) << endl;
    (*_err)
        << "By default, qcstyle is set up to indent source files, with 4 spaces\n";
    (*_err) << "per indent, a maximal indentation of 40 spaces inside "
               "continuous statements,\n";
    (*_err) << "and NO formatting.\n";
    (*_err) << endl;
    (*_err) << "Options Format:\n";
    (*_err) << "----------------\n";
    (*_err) << "    Long options (starting with '--') must be written one at a "
               "time.\n";
    (*_err)
        << "    Short options (starting with '-') may be appended together.\n";
    (*_err) << "    Thus, -bps4 is the same as -b -p -s4.\n";
    (*_err) << endl;
    (*_err) << "Predefined Style Options:\n";
    (*_err) << "-------------------------\n";
    (*_err) << "    --style=allman  OR  --style=bsd  OR  -A1\n";
    (*_err) << "    Allman style formatting/indenting.\n";
    (*_err) << "    Broken brackets.\n";
    (*_err) << endl;
    (*_err) << "    --style=k&r  OR  --style=k/r  OR  -A2\n";
    (*_err) << "    Kernighan & Ritchie style formatting/indenting.\n";
    (*_err) << "    Linux brackets.\n";
    (*_err) << endl;
    (*_err) << "    --style=stroustrup  OR  -A3\n";
    (*_err) << "    Stroustrup style formatting/indenting.\n";
    (*_err) << "    Stroustrup brackets.\n";
    (*_err) << endl;
    (*_err) << "    --style=whitesmith  OR  -A4\n";
    (*_err) << "    Whitesmith style formatting/indenting.\n";
    (*_err) << "    Broken, indented brackets.\n";
    (*_err) << "    Indented switch blocks.\n";
    (*_err) << endl;
    (*_err) << "    --style=banner  OR  -A5\n";
    (*_err) << "    Banner style formatting/indenting.\n";
    (*_err) << "    Attached, indented brackets.\n";
    (*_err) << "    Indented switch blocks.\n";
    (*_err) << endl;
    (*_err) << "    --style=gnu  OR  -A6\n";
    (*_err) << "    GNU style formatting/indenting.\n";
    (*_err) << "    Broken brackets, indented blocks, indent is 2 spaces.\n";
    (*_err) << endl;
    (*_err) << "    --style=linux  OR  -A7\n";
    (*_err) << "    Linux style formatting/indenting.\n";
    (*_err) << "    Linux brackets, indent is 8 spaces.\n";
    (*_err) << endl;
    (*_err) << "Tab and Bracket Options:\n";
    (*_err) << "------------------------\n";
    (*_err) << "    default indent option\n";
    (*_err) << "    If no indentation option is set,\n";
    (*_err) << "    the default option of 4 spaces will be used.\n";
    (*_err) << endl;
    (*_err) << "    --indent=spaces=#  OR  -s#\n";
    (*_err) << "    Indent using # spaces per indent. Not specifying #\n";
    (*_err) << "    will result in a default of 4 spaces per indent.\n";
    (*_err) << endl;
    (*_err) << "    --indent=tab  OR  --indent=tab=#  OR  -t  OR  -t#\n";
    (*_err) << "    Indent using tab characters, assuming that each\n";
    (*_err) << "    tab is # spaces long. Not specifying # will result\n";
    (*_err) << "    in a default assumption of 4 spaces per tab.\n";
    (*_err) << endl;
    (*_err) << "    --indent=force-tab=#  OR  -T#\n";
    (*_err) << "    Indent using tab characters, assuming that each\n";
    (*_err) << "    tab is # spaces long. Force tabs to be used in areas\n";
    (*_err) << "    qcstyle would prefer to use spaces.\n";
    (*_err) << endl;
    (*_err) << "    default brackets option\n";
    (*_err) << "    If no brackets option is set,\n";
    (*_err) << "    the brackets will not be changed.\n";
    (*_err) << endl;
    (*_err) << "    --brackets=break  OR  -b\n";
    (*_err) << "    Break brackets from pre-block code (i.e. ANSI C style).\n";
    (*_err) << endl;
    (*_err) << "    --brackets=attach  OR  -a\n";
    (*_err) << "    Attach brackets to pre-block code (i.e. K&R style).\n";
    (*_err) << endl;
    (*_err) << "    --brackets=linux  OR  -l\n";
    (*_err) << "    Break definition-block brackets and attach command-block\n";
    (*_err) << "    brackets.\n";
    (*_err) << endl;
    (*_err) << "    --brackets=stroustrup  OR  -u\n";
    (*_err) << "    Attach all brackets except function definition brackets.\n";
    (*_err) << endl;
    (*_err) << "Indentation options:\n";
    (*_err) << "--------------------\n";
    (*_err) << "    --indent-switches  OR  -S\n";
    (*_err) << "    Indent 'switch' blocks, so that the inner 'case XXX:'\n";
    (*_err) << "    headers are indented in relation to the switch block.\n";
    (*_err) << endl;
    (*_err) << "    --indent-cases  OR  -K\n";
    (*_err) << "    Indent case blocks from the 'case XXX:' headers.\n";
    (*_err) << "    Case statements not enclosed in blocks are NOT indented.\n";
    (*_err) << endl;
    (*_err) << "    --indent-blocks  OR  -G\n";
    (*_err)
        << "    Add extra indentation entire blocks (including brackets).\n";
    (*_err) << endl;
    (*_err) << "    --indent-brackets  OR  -B\n";
    (*_err) << "    Add extra indentation to '{' and '}' block brackets.\n";
    (*_err) << endl;
    (*_err) << "    --indent-labels  OR  -L\n";
    (*_err) << "    Indent labels so that they appear one indent less than\n";
    (*_err) << "    the current indentation level, rather than being\n";
    (*_err) << "    flushed completely to the left (which is the default).\n";
    (*_err) << endl;
    (*_err) << "    --indent-preprocessor  OR  -w\n";
    (*_err) << "    Indent multi-line #define statements.\n";
    (*_err) << endl;
    (*_err) << "    --max-instatement-indent=#  OR  -M#\n";
    (*_err) << "    Indent a maximal # spaces in a continuous statement,\n";
    (*_err) << "    relative to the previous line.\n";
    (*_err) << endl;
    (*_err) << "    --min-conditional-indent=#  OR  -m#\n";
    (*_err) << "    Indent a minimal # spaces in a continuous conditional\n";
    (*_err) << "    belonging to a conditional header.\n";
    (*_err) << endl;
    (*_err) << "Formatting options:\n";
    (*_err) << "-------------------\n";
    (*_err) << "    --break-blocks  OR  -f\n";
    (*_err) << "    Insert empty lines around unrelated blocks, labels, ...\n";
    (*_err) << endl;
    (*_err) << "    --break-blocks=all  OR  -F\n";
    (*_err) << "    Like --break-blocks, except also insert empty lines \n";
    (*_err) << "    around closing headers (e.g. 'else', ...).\n";
    (*_err) << endl;
    (*_err) << "    --break-closing-brackets  OR  -y\n";
    (*_err)
        << "    Break brackets before closing headers (e.g. 'else', ...).\n";
    (*_err) << "    Use with --brackets=attach, --brackets=linux, \n";
    (*_err) << "    or --brackets=stroustrup.\n";
    (*_err) << endl;
    (*_err) << "    --break-elseifs  OR  -e\n";
    (*_err) << "    Break 'else if()' statements into two different lines.\n";
    (*_err) << endl;
    (*_err) << "    --delete-empty-lines  OR  -x\n";
    (*_err) << "    Delete empty lines within a function.\n";
    (*_err)
        << "    It will NOT delete lines added by the break-blocks options.\n";
    (*_err) << endl;
    (*_err) << "    --pad-oper  OR  -p\n";
    (*_err) << "    Insert space paddings around operators.\n";
    (*_err) << endl;
    (*_err) << "    --pad-paren  OR  -P\n";
    (*_err)
        << "    Insert space padding around parenthesis on both the outside\n";
    (*_err) << "    and the inside.\n";
    (*_err) << endl;
    (*_err) << "    --pad-paren-out  OR  -d\n";
    (*_err)
        << "    Insert space padding around parenthesis on the outside only.\n";
    (*_err) << endl;
    (*_err) << "    --pad-paren-in  OR  -D\n";
    (*_err)
        << "    Insert space padding around parenthesis on the inside only.\n";
    (*_err) << endl;
    (*_err) << "    --unpad-paren  OR  -U\n";
    (*_err)
        << "    Remove unnecessary space padding around parenthesis.  This\n";
    (*_err) << "    can be used in combination with the 'pad' options above.\n";
    (*_err) << endl;
    (*_err) << "    --keep-one-line-statements  OR  -o\n";
    (*_err) << "    Don't break lines containing multiple statements into\n";
    (*_err) << "    multiple single-statement lines.\n";
    (*_err) << endl;
    (*_err) << "    --keep-one-line-blocks  OR  -O\n";
    (*_err) << "    Don't break blocks residing completely on one line.\n";
    (*_err) << endl;
    (*_err) << "    --convert-tabs  OR  -c\n";
    (*_err) << "    Convert tabs to the appropriate number of spaces.\n";
    (*_err) << endl;
    (*_err) << "    --fill-empty-lines  OR  -E\n";
    (*_err) << "    Fill empty lines with the white space of their\n";
    (*_err) << "    previous lines.\n";
    (*_err) << endl;
    (*_err) << "Other options:\n";
    (*_err) << "--------------\n";
    (*_err) << "    --suffix=####\n";
    (*_err) << "    Append the suffix #### instead of '.orig' to original "
               "filename.\n";
    (*_err) << endl;
    (*_err) << "    --suffix=none  OR  -n\n";
    (*_err) << "    Do not retain a backup of the original file.\n";
    (*_err) << endl;
    (*_err) << "    --errors-to-stdout  OR  -X\n";
    (*_err) << "    Print errors and help information to standard-output "
               "rather than\n";
    (*_err) << "    to standard-error.\n";
    (*_err) << endl;
    (*_err) << "    --version  OR  -V\n";
    (*_err) << "    Print version number.\n";
    (*_err) << endl;
    (*_err) << "    --help  OR  -h  OR  -?\n";
    (*_err) << "    Print this help message.\n";
    (*_err) << endl;
}

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
            printHelp();
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
