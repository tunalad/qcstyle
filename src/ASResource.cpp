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

#include "astyle.h"
#include <algorithm>

namespace astyle {
const string ASResource::AS_IF = string("if");
const string ASResource::AS_ELSE = string("else");
const string ASResource::AS_FOR = string("for");
const string ASResource::AS_DO = string("do");
const string ASResource::AS_WHILE = string("while");
const string ASResource::AS_SWITCH = string("switch");
const string ASResource::AS_CASE = string("case");
const string ASResource::AS_DEFAULT = string("default");
const string ASResource::AS_STRUCT = string("struct");
const string ASResource::AS_UNION = string("union");
const string ASResource::AS_ENTITY = string("entity");
const string ASResource::AS_VECTOR = string("vector");
const string ASResource::AS_STRING = string("string");
const string ASResource::AS_CONST = string("const");

const string ASResource::AS_OPEN_BRACKET = string("{");
const string ASResource::AS_CLOSE_BRACKET = string("}");
const string ASResource::AS_OPEN_LINE_COMMENT = string("//");
const string ASResource::AS_OPEN_COMMENT = string("/*");
const string ASResource::AS_CLOSE_COMMENT = string("*/");

const string ASResource::AS_ASSIGN = string("=");
const string ASResource::AS_PLUS_ASSIGN = string("+=");
const string ASResource::AS_MINUS_ASSIGN = string("-=");
const string ASResource::AS_MULT_ASSIGN = string("*=");
const string ASResource::AS_DIV_ASSIGN = string("/=");
const string ASResource::AS_MOD_ASSIGN = string("%=");
const string ASResource::AS_OR_ASSIGN = string("|=");
const string ASResource::AS_AND_ASSIGN = string("&=");
const string ASResource::AS_XOR_ASSIGN = string("^=");
const string ASResource::AS_GR_GR_ASSIGN = string(">>=");
const string ASResource::AS_LS_LS_ASSIGN = string("<<=");

const string ASResource::AS_RETURN = string("return");
const string ASResource::AS_LOCAL = string("local");

const string ASResource::AS_EQUAL = string("==");
const string ASResource::AS_PLUS_PLUS = string("++");
const string ASResource::AS_MINUS_MINUS = string("--");
const string ASResource::AS_NOT_EQUAL = string("!=");
const string ASResource::AS_GR_EQUAL = string(">=");
const string ASResource::AS_GR_GR = string(">>");
const string ASResource::AS_LS_EQUAL = string("<=");
const string ASResource::AS_LS_LS = string("<<");
const string ASResource::AS_AND = string("&&");
const string ASResource::AS_OR = string("||");

const string ASResource::AS_PLUS = string("+");
const string ASResource::AS_MINUS = string("-");
const string ASResource::AS_MULT = string("*");
const string ASResource::AS_DIV = string("/");
const string ASResource::AS_MOD = string("%");
const string ASResource::AS_GR = string(">");
const string ASResource::AS_LS = string("<");
const string ASResource::AS_NOT = string("!");
const string ASResource::AS_BIT_OR = string("|");
const string ASResource::AS_BIT_AND = string("&");
const string ASResource::AS_BIT_NOT = string("~");
const string ASResource::AS_BIT_XOR = string("^");
const string ASResource::AS_QUESTION = string("?");
const string ASResource::AS_COLON = string(":");

/**
 * Sort comparison function.
 * Compares the length of the value of pointers in the vectors.
 * The LONGEST strings will be first in the vector.
 *
 * @params the string pointers to be compared.
 */
bool sortOnLength(const string *a, const string *b) {
    return (*a).length() > (*b).length();
}

/**
 * Sort comparison function.
 * Compares the value of pointers in the vectors.
 *
 * @params the string pointers to be compared.
 */
bool sortOnName(const string *a, const string *b) { return *a < *b; }

/**
 * Build the vector of assignment operators.
 * Used by BOTH ASFormatter.cpp and ASBeautifier.cpp
 *
 * @param assignmentOperators   a reference to the vector to be built.
 */
void ASResource::buildAssignmentOperators(
    vector<const string *> &assignmentOperators) {
    assignmentOperators.push_back(&AS_ASSIGN);
    assignmentOperators.push_back(&AS_PLUS_ASSIGN);
    assignmentOperators.push_back(&AS_MINUS_ASSIGN);
    assignmentOperators.push_back(&AS_MULT_ASSIGN);
    assignmentOperators.push_back(&AS_DIV_ASSIGN);
    assignmentOperators.push_back(&AS_MOD_ASSIGN);
    assignmentOperators.push_back(&AS_OR_ASSIGN);
    assignmentOperators.push_back(&AS_AND_ASSIGN);
    assignmentOperators.push_back(&AS_XOR_ASSIGN);
    assignmentOperators.push_back(&AS_GR_GR_ASSIGN);
    assignmentOperators.push_back(&AS_LS_LS_ASSIGN);

    sort(assignmentOperators.begin(), assignmentOperators.end(), sortOnLength);
}

/**
 * Build the vector of header words.
 * Used by BOTH ASFormatter.cpp and ASBeautifier.cpp
 *
 * @param headers       a reference to the vector to be built.
 */
void ASResource::buildHeaders(vector<const string *> &headers,
                              bool beautifier) {
    headers.push_back(&AS_IF);
    headers.push_back(&AS_ELSE);
    headers.push_back(&AS_FOR);
    headers.push_back(&AS_WHILE);
    headers.push_back(&AS_DO);
    headers.push_back(&AS_SWITCH);

    if (beautifier) {
        headers.push_back(&AS_CASE);
        headers.push_back(&AS_DEFAULT);
        headers.push_back(&AS_CONST);
    }
    sort(headers.begin(), headers.end(), sortOnName);
}

/**
 * Build the vector of indentable headers.
 * Used by ONLY ASBeautifier.cpp
 *
 * @param indentableHeaders     a reference to the vector to be built.
 */
void ASResource::buildIndentableHeaders(
    vector<const string *> &indentableHeaders) {
    indentableHeaders.push_back(&AS_RETURN);
    indentableHeaders.push_back(&AS_LOCAL);

    sort(indentableHeaders.begin(), indentableHeaders.end(), sortOnName);
}

/**
 * Build the vector of non-assignment operators.
 * Used by ONLY ASBeautifier.cpp
 *
 * @param nonAssignmentOperators       a reference to the vector to be built.
 */
void ASResource::buildNonAssignmentOperators(
    vector<const string *> &nonAssignmentOperators) {
    nonAssignmentOperators.push_back(&AS_EQUAL);
    nonAssignmentOperators.push_back(&AS_PLUS_PLUS);
    nonAssignmentOperators.push_back(&AS_MINUS_MINUS);
    nonAssignmentOperators.push_back(&AS_NOT_EQUAL);
    nonAssignmentOperators.push_back(&AS_GR_EQUAL);
    nonAssignmentOperators.push_back(&AS_GR_GR);
    nonAssignmentOperators.push_back(&AS_LS_EQUAL);
    nonAssignmentOperators.push_back(&AS_LS_LS);
    nonAssignmentOperators.push_back(&AS_AND);
    nonAssignmentOperators.push_back(&AS_OR);

    sort(nonAssignmentOperators.begin(), nonAssignmentOperators.end(),
         sortOnLength);
}

/**
 * Build the vector of header non-paren headers.
 * Used by BOTH ASFormatter.cpp and ASBeautifier.cpp
 *
 * @param nonParenHeaders       a reference to the vector to be built.
 */
void ASResource::buildNonParenHeaders(vector<const string *> &nonParenHeaders,
                                      bool beautifier) {
    nonParenHeaders.push_back(&AS_ELSE);
    nonParenHeaders.push_back(&AS_DO);

    if (beautifier) {
        nonParenHeaders.push_back(&AS_CASE);
        nonParenHeaders.push_back(&AS_DEFAULT);
        nonParenHeaders.push_back(&AS_CONST);
    }
    sort(nonParenHeaders.begin(), nonParenHeaders.end(), sortOnName);
}

/**
 * Build the vector of operators.
 * Used by ONLY ASFormatter.cpp
 *
 * @param operators             a reference to the vector to be built.
 */
void ASResource::buildOperators(vector<const string *> &operators) {
    operators.push_back(&AS_PLUS_ASSIGN);
    operators.push_back(&AS_MINUS_ASSIGN);
    operators.push_back(&AS_MULT_ASSIGN);
    operators.push_back(&AS_DIV_ASSIGN);
    operators.push_back(&AS_MOD_ASSIGN);
    operators.push_back(&AS_OR_ASSIGN);
    operators.push_back(&AS_AND_ASSIGN);
    operators.push_back(&AS_XOR_ASSIGN);
    operators.push_back(&AS_EQUAL);
    operators.push_back(&AS_PLUS_PLUS);
    operators.push_back(&AS_MINUS_MINUS);
    operators.push_back(&AS_NOT_EQUAL);
    operators.push_back(&AS_GR_EQUAL);
    operators.push_back(&AS_GR_GR_ASSIGN);
    operators.push_back(&AS_GR_GR);
    operators.push_back(&AS_LS_EQUAL);
    operators.push_back(&AS_LS_LS_ASSIGN);
    operators.push_back(&AS_LS_LS);
    operators.push_back(&AS_AND);
    operators.push_back(&AS_OR);
    operators.push_back(&AS_PLUS);
    operators.push_back(&AS_MINUS);
    operators.push_back(&AS_MULT);
    operators.push_back(&AS_DIV);
    operators.push_back(&AS_MOD);
    operators.push_back(&AS_QUESTION);
    operators.push_back(&AS_COLON);
    operators.push_back(&AS_ASSIGN);
    operators.push_back(&AS_LS);
    operators.push_back(&AS_GR);
    operators.push_back(&AS_NOT);
    operators.push_back(&AS_BIT_OR);
    operators.push_back(&AS_BIT_AND);
    operators.push_back(&AS_BIT_NOT);
    operators.push_back(&AS_BIT_XOR);

    sort(operators.begin(), operators.end(), sortOnLength);
}

/**
 * Build the vector of pre-block statements.
 * Used by ONLY ASBeautifier.cpp
 * NOTE: Cannot be both a header and a preBlockStatement.
 *
 * @param preBlockStatements        a reference to the vector to be built.
 */
void ASResource::buildPreBlockStatements(
    vector<const string *> &preBlockStatements) {
    preBlockStatements.push_back(&AS_STRUCT);
    preBlockStatements.push_back(&AS_UNION);
    sort(preBlockStatements.begin(), preBlockStatements.end(), sortOnName);
}

/**
 * Build the vector of pre-command headers.
 * Used by ONLY ASFormatter.cpp
 *
 * @param preCommandHeaders     a reference to the vector to be built.
 */
void ASResource::buildPreCommandHeaders(
    vector<const string *> &preCommandHeaders) {
    preCommandHeaders.push_back(&AS_CONST);

    sort(preCommandHeaders.begin(), preCommandHeaders.end(), sortOnName);
}

/**
 * Build the vector of pre-definition headers.
 * Used by ONLY ASFormatter.cpp
 * NOTE: Do NOT add 'enum' here. It is an array type bracket.
 * NOTE: Do NOT add 'extern' here. Do not want an extra indent.
 *
 * @param preDefinitionHeaders      a reference to the vector to be built.
 */
void ASResource::buildPreDefinitionHeaders(
    vector<const string *> &preDefinitionHeaders) {
    preDefinitionHeaders.push_back(&AS_STRUCT);
    preDefinitionHeaders.push_back(&AS_UNION);
    sort(preDefinitionHeaders.begin(), preDefinitionHeaders.end(), sortOnName);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                             ASBase Funtions
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// check if a specific line position contains a keyword.
bool ASBase::findKeyword(const string &line, int i,
                         const string &keyword) const {
    assert(isCharPotentialHeader(line, i));
    // check the word
    const size_t keywordLength = keyword.length();
    if (line.compare(i, keywordLength, keyword) != 0)
        return false;
    // check that this is not part of a longer word
    const size_t wordEnd = i + keywordLength;
    if (wordEnd == line.length())
        return true;
    if (isLegalNameChar(line[wordEnd]))
        return false;
    // is not a keyword if part of a definition
    const char peekChar = peekNextChar(line, wordEnd - 1);
    if (peekChar == ',' || peekChar == ')')
        return false;
    return true;
}

// get the current word on a line
// i must point to the beginning of the word
string ASBase::getCurrentWord(const string &line, size_t charNum) const {
    assert(isCharPotentialHeader(line, charNum));
    size_t lineLength = line.length();
    size_t i;
    for (i = charNum; i < lineLength; i++) {
        if (!isLegalNameChar(line[i]))
            break;
    }
    return line.substr(charNum, i - charNum);
}

} // end namespace astyle
