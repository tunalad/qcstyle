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

#ifndef ASTYLE_H
#define ASTYLE_H

#include <cassert>

#include <cctype>
#include <string.h>
#include <string>
#include <vector>

using namespace std;

namespace astyle {

enum FormatStyle {
    STYLE_NONE,
    STYLE_ALLMAN,
    STYLE_KandR,
    STYLE_STROUSTRUP,
    STYLE_WHITESMITH,
    STYLE_BANNER,
    STYLE_GNU,
    STYLE_LINUX
};

enum BracketMode {
    NONE_MODE,
    ATTACH_MODE,
    BREAK_MODE,
    LINUX_MODE,
    STROUSTRUP_MODE
};

enum BracketType {
    NULL_TYPE = 0,
    DEFINITION_TYPE = 8,
    COMMAND_TYPE = 16,
    ARRAY_TYPE = 32, // arrays and enums
    SINGLE_LINE_TYPE = 64
};

class ASSourceIterator {
  public:
    ASSourceIterator() {}
    virtual ~ASSourceIterator() {}
    virtual bool hasMoreLines() const = 0;
    virtual string nextLine(bool emptyLineWasDeleted = false) = 0;
    virtual string peekNextLine() = 0;
    virtual void peekReset() = 0;
};

class ASResource {
  public:
    void buildAssignmentOperators(vector<const string *> &assignmentOperators);
    void buildHeaders(vector<const string *> &headers,
                      bool beautifier = false);
    void buildIndentableHeaders(vector<const string *> &indentableHeaders);
    void
    buildNonAssignmentOperators(vector<const string *> &nonAssignmentOperators);
    void buildNonParenHeaders(vector<const string *> &nonParenHeaders,
                              bool beautifier = false);
    void buildOperators(vector<const string *> &operators);
    void buildPreBlockStatements(vector<const string *> &preBlockStatements);
    void buildPreCommandHeaders(vector<const string *> &preCommandHeaders);
    void buildPreDefinitionHeaders(vector<const string *> &preDefinitionHeaders);

  public:
    static const string AS_IF, AS_ELSE;
    static const string AS_DO, AS_WHILE;
    static const string AS_FOR;
    static const string AS_SWITCH, AS_CASE, AS_DEFAULT;
    static const string AS_STRUCT, AS_UNION;
    static const string AS_ENTITY, AS_VECTOR, AS_STRING;
    static const string AS_CONST;
    static const string AS_OPEN_BRACKET, AS_CLOSE_BRACKET;
    static const string AS_OPEN_LINE_COMMENT, AS_OPEN_COMMENT, AS_CLOSE_COMMENT;
    static const string AS_RETURN;
    static const string AS_LOCAL;
    static const string AS_ASSIGN, AS_PLUS_ASSIGN, AS_MINUS_ASSIGN,
        AS_MULT_ASSIGN;
    static const string AS_DIV_ASSIGN, AS_MOD_ASSIGN, AS_XOR_ASSIGN,
        AS_OR_ASSIGN, AS_AND_ASSIGN;
    static const string AS_GR_GR_ASSIGN, AS_LS_LS_ASSIGN;
    static const string AS_EQUAL, AS_PLUS_PLUS, AS_MINUS_MINUS, AS_NOT_EQUAL,
        AS_GR_EQUAL, AS_GR_GR;
    static const string AS_LS_EQUAL, AS_LS_LS;
    static const string AS_AND, AS_OR;
    static const string AS_PLUS, AS_MINUS, AS_MULT, AS_DIV, AS_MOD, AS_GR,
        AS_LS;
    static const string AS_NOT, AS_BIT_XOR, AS_BIT_OR, AS_BIT_AND, AS_BIT_NOT;
    static const string AS_QUESTION, AS_COLON;
};

class ASBase {
  protected:
    ASBase() {};
    ~ASBase() {};

    // functions definitions are at the end of ASResource.cpp
    bool findKeyword(const string &line, int i, const string &keyword) const;
    string getCurrentWord(const string &line, size_t charNum) const;

  protected:

    // check if a specific character can be used in a legal
    // variable/function/struct name
    bool isLegalNameChar(char ch) const {
        if (isWhiteSpace(ch))
            return false;
        if ((unsigned)ch > 127)
            return false;
        return (isalnum(ch) || ch == '_');
    }

    // check if a specific character can be part of a header
    bool isCharPotentialHeader(const string &line, size_t i) const {
        assert(!isWhiteSpace(line[i]));
        char prevCh = ' ';
        if (i > 0)
            prevCh = line[i - 1];
        if (!isLegalNameChar(prevCh) && isLegalNameChar(line[i]))
            return true;
        return false;
    }

    // check if a specific character can be part of an operator
    bool isCharPotentialOperator(char ch) const {
        assert(!isWhiteSpace(ch));
        if ((unsigned)ch > 127)
            return false;
        return (ispunct(ch) && ch != '{' && ch != '}' && ch != '(' &&
                ch != ')' && ch != '[' && ch != ']' && ch != ';' && ch != ',' &&
                ch != '#' && ch != '\\' && ch != '\'' && ch != '\"');
    }

    // check if a specific character is a whitespace character
    bool isWhiteSpace(char ch) const { return (ch == ' ' || ch == '\t'); }

    // peek at the next unread character.
    char peekNextChar(const string &line, int i) const {
        char ch = ' ';
        size_t peekNum = line.find_first_not_of(" \t", i + 1);
        if (peekNum == string::npos)
            return ch;
        ch = line[peekNum];
        return ch;
    }
};

class ASBeautifier : protected ASResource, protected ASBase {
  public:
    ASBeautifier();
    virtual ~ASBeautifier();
    virtual void
    init(ASSourceIterator *iter); // pointer to dynamically created iterator.
    void init();
    virtual bool hasMoreLines() const;
    virtual string nextLine();
    virtual string beautify(const string &line);
    void setTabIndentation(int length = 4, bool forceTabs = false);
    void setSpaceIndentation(int length = 4);
    void setMaxInStatementIndentLength(int max);
    void setMinConditionalIndentLength(int min);
    void setSwitchIndent(bool state);
    void setCaseIndent(bool state);
    void setBracketIndent(bool state);
    void setBlockIndent(bool state);
    void setLabelIndent(bool state);
    void setEmptyLineFill(bool state);
    void setPreprocessorIndent(bool state);
    int getIndentLength(void);
    string getIndentString(void);
    bool getBracketIndent(void);
    bool getBlockIndent(void);
    bool getCaseIndent(void);
    bool getEmptyLineFill(void);

  protected:
    const string *
    findHeader(const string &line, int i,
               const vector<const string *> &possibleHeaders) const;
    const string *
    findOperator(const string &line, int i,
                 const vector<const string *> &possibleOperators) const;
    int getNextProgramCharDistance(const string &line, int i) const;
    int indexOf(vector<const string *> &container, const string *element);
    string trim(const string &str);

    // variables set by ASFormatter - must be updated in activeBeautifierStack
    bool lineCommentNoBeautify;
    bool isNonInStatementArray;

  private: // functions
    ASBeautifier(const ASBeautifier &copy);
    void operator=(ASBeautifier &); // not to be implemented

    void initStatic();
    void registerInStatementIndent(const string &line, int i, int spaceTabCount,
                                   int minIndent, bool updateParenStack);
    string preLineWS(int spaceTabCount, int tabCount);

    static vector<const string *> headers;
    static vector<const string *> nonParenHeaders;
    static vector<const string *> preBlockStatements;
    static vector<const string *> assignmentOperators;
    static vector<const string *> nonAssignmentOperators;
    static vector<const string *> indentableHeaders;

    ASSourceIterator *sourceIterator;
    vector<ASBeautifier *> *waitingBeautifierStack;
    vector<ASBeautifier *> *activeBeautifierStack;
    vector<int> *waitingBeautifierStackLengthStack;
    vector<int> *activeBeautifierStackLengthStack;
    vector<const string *> *headerStack;
    vector<vector<const string *> *> *tempStacks;
    vector<int> *blockParenDepthStack;
    vector<bool> *blockStatementStack;
    vector<bool> *parenStatementStack;
    vector<bool> *bracketBlockStateStack;
    vector<int> *inStatementIndentStack;
    vector<int> *inStatementIndentStackSizeStack;
    vector<int> *parenIndentStack;
    template <typename T> void deleteContainer(T &container);
    void deleteContainer(vector<vector<const string *> *> *&container);
    template <typename T> void initContainer(T &container, T value);

  private: // variables
    string indentString;
    const string *currentHeader;
    const string *probationHeader;
    bool isInQuote;
    bool haveLineContinuationChar;
    bool isInComment;
    bool isInCase;
    bool isInQuestion;
    bool isInStatement;
    bool isInHeader;
    bool isInDefine;
    bool isInDefineDefinition;
    bool isInClassHeader;
    bool switchIndent;
    bool caseIndent;
    bool bracketIndent;
    bool blockIndent;
    bool labelIndent;
    bool preprocessorIndent;
    bool isInConditional;
    bool isMinimalConditinalIndentSet;
    bool shouldForceTabIndentation;
    bool emptyLineFill;
    bool backslashEndsPrevLine;
    bool blockCommentNoIndent;
    bool blockCommentNoBeautify;
    bool previousLineProbationTab;
    int minConditionalIndent;
    int parenDepth;
    int indentLength;
    int leadingWhiteSpaces;
    int maxInStatementIndent;
    int prevFinalLineSpaceTabCount;
    int prevFinalLineTabCount;
    int defineTabCount;
    char quoteChar;
    char prevNonSpaceCh;
    char currentNonSpaceCh;
};

class ASEnhancer : protected ASBase {
  public: // functions
    ASEnhancer();
    ~ASEnhancer();
    void init(int, string, bool, bool);
    void enhance(string &line);

  private:
    // options from command line or options file
    int indentLength;
    bool useTabs;
    bool caseIndent;
    bool emptyLineFill;

    // parsing variables
    bool isInQuote;
    bool isInComment;
    char quoteChar;

    // unindent variables
    int switchDepth;
    bool lookingForCaseBracket;
    bool unindentNextLine;

    // struct used by the indentation engine
    // contains variables used to unindent the case blocks
    struct switchVariables {
        int switchBracketCount;
        int unindentDepth;
        bool unindentCase;
    };

    switchVariables sw;               // switch variables struct
    vector<switchVariables> swVector; // stack vector of switch variables

  private: // functions
    void unindentLine(string &line, const int unindent) const;
};

class ASFormatter : public ASBeautifier {
  public:
    ASFormatter();
    virtual ~ASFormatter();
    virtual void init(ASSourceIterator *iter);
    virtual bool hasMoreLines() const;
    virtual string nextLine();
    void setFormattingStyle(FormatStyle style);
    void setBracketFormatMode(BracketMode mode);
    void setBreakClosingHeaderBracketsMode(bool state);
    void setOperatorPaddingMode(bool mode);
    void setParensOutsidePaddingMode(bool mode);
    void setParensInsidePaddingMode(bool mode);
    void setParensUnPaddingMode(bool state);
    void setBreakOneLineBlocksMode(bool state);
    void setSingleStatementsMode(bool state);
    void setTabSpaceConversionMode(bool state);
    void setBreakBlocksMode(bool state);
    void setBreakClosingHeaderBlocksMode(bool state);
    void setBreakElseIfsMode(bool state);
    void setDeleteEmptyLinesMode(bool state);

  private:                               // functions
    void ASformatter(ASFormatter &copy); // not to be imlpemented
    void operator=(ASFormatter &);       // not to be implemented
    template <typename T> void deleteContainer(T &container);
    template <typename T> void initContainer(T &container, T value);
    void buildLanguageVectors();
    void checkForFollowingHeader(const string &firstLine);
    void convertTabToSpaces();
    void goForward(int i);
    void trimNewLine();
    char peekNextChar() const;
    BracketType getBracketType();
    bool commentAndHeaderFollows() const;
    bool getNextChar();
    bool getNextLine(bool emptyLineWasDeleted = false);
    bool isBeforeComment() const;
    bool isBeforeLineEndComment(int startPos) const;
    bool isBracketType(BracketType a, BracketType b) const;
    bool isPointerOrReference() const;
    bool isUnaryOperator() const;
    bool isInExponent() const;
    bool isOneLineBlockReached() const;
    bool lineBeginsWith(char charToCheck) const;
    void appendCharInsideComments();
    void appendSequence(const string &sequence, bool canBreakLine = true);
    void appendSpacePad();
    void appendSpaceAfter();
    void breakLine();
    void deleteContainer(vector<BracketType> *&container);
    void initContainer(vector<BracketType> *&container,
                       vector<BracketType> *value);
    void padOperators(const string *newOperator);
    void padParens();
    void formatBrackets(BracketType bracketType);
    void formatArrayBrackets(BracketType bracketType,
                             bool isOpeningArrayBracket);
    void adjustComments();
    void fixOptionVariableConflicts();
    void processPreprocessor();
    string peekNextText(const string &firstLine,
                        bool endOnEmptyLine = false) const;

  private: // variables
    static vector<const string *> headers;
    static vector<const string *> nonParenHeaders;
    static vector<const string *> preDefinitionHeaders;
    static vector<const string *> preCommandHeaders;
    static vector<const string *> operators;
    static vector<const string *> assignmentOperators;

    ASSourceIterator *sourceIterator;
    ASEnhancer *enhancer;

    vector<const string *> *preBracketHeaderStack;
    vector<BracketType> *bracketTypeStack;
    vector<int> *parenStack;
    string readyFormattedLine;
    string currentLine;
    string formattedLine;
    const string *currentHeader;
    char currentChar;
    char previousChar;
    char previousNonWSChar;
    char previousCommandChar;
    char quoteChar;
    int charNum;
    int preprocBracketTypeStackSize;
    int tabIncrementIn;
    int spacePadNum;
    size_t formattedLineCommentNum; // comment location on formattedLine
    size_t previousReadyFormattedLineLength;
    FormatStyle formattingStyle;
    BracketMode bracketFormatMode;
    BracketType previousBracketType;
    bool isVirgin;
    bool shouldPadOperators;
    bool shouldPadParensOutside;
    bool shouldPadParensInside;
    bool shouldUnPadParens;
    bool shouldConvertTabs;
    bool isInLineComment;
    bool isInComment;
    bool isInPreprocessor;
    bool doesLineStartComment;
    bool lineEndsInCommentOnly;
    bool lineIsLineCommentOnly;
    bool lineIsEmpty;
    bool isImmediatelyPostCommentOnly;
    bool isImmediatelyPostEmptyLine;
    bool isInQuote;
    bool haveLineContinuationChar;
    bool isInQuoteContinuation;
    bool isInBlParen;
    bool isSpecialChar;
    bool isNonParenHeader;
    bool foundQuestionMark;
    bool foundPreDefinitionHeader;
    bool foundPreCommandHeader;
    bool isInLineBreak;
    bool endOfCodeReached;
    bool lineCommentNoIndent;
    bool isLineReady;
    bool isPreviousBracketBlockRelated;
    bool isInPotentialCalculation;
    bool isCharImmediatelyPostComment;
    bool isPreviousCharPostComment;
    bool isCharImmediatelyPostLineComment;
    bool isCharImmediatelyPostOpenBlock;
    bool isCharImmediatelyPostCloseBlock;
    bool isCharImmediatelyPostReturn;
    bool shouldBreakOneLineBlocks;
    bool shouldReparseCurrentChar;
    bool shouldBreakOneLineStatements;
    bool shouldBreakClosingHeaderBrackets;
    bool shouldBreakElseIfs;
    bool shouldDeleteEmptyLines;
    bool needHeaderOpeningBracket;
    bool passedSemicolon;
    bool passedColon;
    bool isImmediatelyPostComment;
    bool isImmediatelyPostLineComment;
    bool isImmediatelyPostEmptyBlock;
    bool isImmediatelyPostPreprocessor;
    bool isImmediatelyPostReturn;

    bool shouldBreakBlocks;
    bool shouldBreakClosingHeaderBlocks;
    bool isPrependPostBlockEmptyLineRequested;
    bool isAppendPostBlockEmptyLineRequested;

    bool prependEmptyLine;
    bool appendOpeningBracket;
    bool foundClosingHeader;

    bool isInHeader;
    bool isImmediatelyPostHeader;
    bool isInCase;

  private: // inline functions
    // append a character to the current formatted line.
    void appendChar(char ch, bool canBreakLine) {
        if (canBreakLine && isInLineBreak)
            breakLine();
        formattedLine.append(1, ch);
        isImmediatelyPostCommentOnly = false;
    }

    // append the CURRENT character (curentChar) to the current formatted line.
    void appendCurrentChar(bool canBreakLine = true) {
        appendChar(currentChar, canBreakLine);
    }

    // check if a specific sequence exists in the current placement of the
    // current line
    bool isSequenceReached(const char *sequence) const {
        return currentLine.compare(charNum, strlen(sequence), sequence) == 0;
    }

    // call ASBase::findHeader for the current character
    const string *findHeader(const vector<const string *> &headers) {
        return ASBeautifier::findHeader(currentLine, charNum, headers);
    }

    // call ASBase::findOperator for the current character
    const string *findOperator(const vector<const string *> &headers) {
        return ASBeautifier::findOperator(currentLine, charNum, headers);
    }
};

//----------------------------------------------------------------------------
// astyle namespace global declarations
//----------------------------------------------------------------------------
// sort comparison functions for ASResource
bool sortOnLength(const string *a, const string *b);
bool sortOnName(const string *a, const string *b);

} // namespace astyle

// end of astyle namespace  --------------------------------------------------

#endif // closes ASTYLE_H
