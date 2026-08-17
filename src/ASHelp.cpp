/*
 *   ASHelp.cpp - QuakeC Style help messages
 *
 *   Copyright (C) 2026 by tunalad <tunalad@proton.me>
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License
 *   as published by the Free Software Foundation; either version 2
 *   of the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *   MA 02110-1301, USA.
 *
 *   This file is part of the Quaketastic Style source code.
 */

#include "ASHelp.h"
using namespace std;

// clang-format off
// compact help message (-h)
void printHelp(std::ostream &out) {
    out << endl;
    out << "                      Quaketastic Style " << VERSION << endl;
    out << endl;
    out << "Usage:" << endl;
    out << "------" << endl;
    out << "    qcstyle [OPTIONS] File1, File2, File3 [...]" << endl;
    out << "    qcstyle [OPTIONS] < Original > Beautified" << endl;
    out << endl;
    out << "    Wildcards (* and ?) may be used. A 'recursive' option can" << endl;
    out << "    process directories recursively." << endl;
    out << endl;
    out << "Options:" << endl;
    out << "--------" << endl;
    out << endl;
    out << "    --style=<string>, -A#                 Set the brace style." << endl;
    out << "      allman, bsd, -A1                      Broken braces." << endl;
    out << "      k&r, k/r, -A2                         Linux braces." << endl;
    out << "      stroustrup, -A3                       Linux braces, broken closing headers." << endl;
    out << "      whitesmith, -A4                       Broken, indented braces." << endl;
    out << "      banner, -A5                           Attached, indented braces." << endl;
    out << "      gnu, -A6                              Broken braces, indented blocks (2 spaces)." << endl;
    out << "      linux, -A7                            Linux braces (8 spaces)." << endl;
    out << "      quakec, -A8                           QuakeC style (Allman, 4 spaces)." << endl;
    out << endl;
    out << "    --brackets=<string>                  Set the brace attachment." << endl;
    out << "      break, -b                             Break braces (ANSI C style)." << endl;
    out << "      attach, -a                            Attach braces (K&R style)." << endl;
    out << "      linux, -l                             Break definition, attach command." << endl;
    out << "      stroustrup, -u                        Attach all except function definitions." << endl;
    out << endl;
    out << "Indentation:" << endl;
    out << "    -s#, --indent=spaces=#                 Indent using # spaces (default: 4)." << endl;
    out << "    -t, --indent=tab[=#]                   Indent using tabs (default: 4 spaces)." << endl;
    out << "    -T#, --indent=force-tab=#              Force tab indentation." << endl;
    out << "    -S, --indent-switches                  Indent switch blocks." << endl;
    out << "    -K, --indent-cases                     Indent case blocks." << endl;
    out << "    -G, --indent-blocks                    Add extra indentation to blocks." << endl;
    out << "    -B, --indent-brackets                  Add extra indentation to braces." << endl;
    out << "    -L, --indent-labels                    Indent labels one less level." << endl;
    out << "    -w, --indent-preprocessor              Indent multi-line #defines." << endl;
    out << "    -M#, --max-instatement-indent=#        Maximal indentation in statements." << endl;
    out << "    -m#, --min-conditional-indent=#        Minimal indentation in conditionals." << endl;
    out << endl;
    out << "Formatting:" << endl;
    out << "    -f, --break-blocks                     Insert empty lines around blocks." << endl;
    out << "    -F, --break-blocks=all                 Also around closing headers." << endl;
    out << "    -y, --break-closing-braces             Break braces before closing headers." << endl;
    out << "    -e, --break-elseifs                    Break 'else if' into two lines." << endl;
    out << "    -x, --delete-empty-lines               Delete empty lines within functions." << endl;
    out << "    -p, --pad-oper                         Insert space padding around operators." << endl;
    out << "    -P, --pad-paren                        Pad parens on both outside and inside." << endl;
    out << "    -d, --pad-paren-out                    Pad parens on the outside only." << endl;
    out << "    -D, --pad-paren-in                     Pad parens on the inside only." << endl;
    out << "    -U, --unpad-paren                      Remove unnecessary paren padding." << endl;
    out << "    -o, --keep-one-line-statements         Don't break multiple statements." << endl;
    out << "    -O, --keep-one-line-blocks             Don't break one-line blocks." << endl;
    out << "    -c, --convert-tabs                     Convert tabs to spaces." << endl;
    out << "    -E, --fill-empty-lines                 Fill empty lines with whitespace." << endl;
    out << endl;
    out << "Other:" << endl;
    out << "    --suffix=####                          Suffix for original filename." << endl;
    out << "    -n, --suffix=none                      Don't retain a backup." << endl;
    out << "    -X, --errors-to-stdout                 Print errors to stdout." << endl;
    out << "    -V, --version                          Print version number." << endl;
    out << "    -h, -?, --help                         Print this help message." << endl;
    out << "    -H, --help-full                        Print full help message." << endl;
    out << endl;
    out << "    Long options must be written one at a time. Short options may be" << endl;
    out << "    appended together. Thus, -bps4 is the same as -b -p -s4." << endl;
}

// full help message (-H)
void printHelpFull(std::ostream &out) {
    out << endl;
    out << "                      Quaketastic Style " << VERSION << endl;
    out << endl;
    out << "Usage:" << endl;
    out << "------" << endl;
    out << "            qcstyle [OPTIONS] File1.qc, File2.qc, File3.qc [...]" << endl;
    out << endl;
    out << "            qcstyle [OPTIONS] < Original > Beautified" << endl;
    out << endl;
    out << "    When indenting a specific file, the resulting indented file RETAINS" << endl;
    out << "    the original file-name. The original pre-indented file is renamed," << endl;
    out << "    with a suffix of '.orig' added to the original filename." << endl;
    out << endl;
    out << "    Wildcards (* and ?) may be used in the filename." << endl;
    out << "    A 'recursive' option can process directories recursively." << endl;
    out << "    Multiple file extensions may be separated by a comma." << endl;
    out << endl;
    out << "    By default, qcstyle is set up to indent with four spaces per indent," << endl;
    out << "    a maximal indentation of 40 spaces inside continuous statements," << endl;
    out << "    and NO formatting options." << endl;
    out << endl;
    out << "Options:" << endl;
    out << "--------" << endl;
    out << "    This  program  follows  the  usual  GNU  command line syntax." << endl;
    out << "    Long options (starting with '--') must be written one at a time." << endl;
    out << "    Short options (starting with '-') may be appended together." << endl;
    out << "    Thus, -bps4 is the same as -b -p -s4." << endl;
    out << endl;
    out << "Brace Style Options:" << endl;
    out << "--------------------" << endl;
    out << "    default brace style" << endl;
    out << "    If no brace style is requested, the opening braces will not be" << endl;
    out << "    changed and closing braces will be broken from the preceding line." << endl;
    out << endl;
    out << "    --style=allman  OR  --style=bsd  OR  -A1" << endl;
    out << "    Allman style formatting/indenting." << endl;
    out << "    Broken braces." << endl;
    out << endl;
    out << "    --style=k&r  OR  --style=k/r  OR  -A2" << endl;
    out << "    Kernighan & Ritchie style formatting/indenting." << endl;
    out << "    Linux braces." << endl;
    out << endl;
    out << "    --style=stroustrup  OR  -A3" << endl;
    out << "    Stroustrup style formatting/indenting." << endl;
    out << "    Linux braces, with broken closing headers." << endl;
    out << endl;
    out << "    --style=whitesmith  OR  -A4" << endl;
    out << "    Whitesmith style formatting/indenting." << endl;
    out << "    Broken, indented braces." << endl;
    out << "    Indented switch blocks." << endl;
    out << endl;
    out << "    --style=banner  OR  -A5" << endl;
    out << "    Banner style formatting/indenting." << endl;
    out << "    Attached, indented braces." << endl;
    out << "    Indented switch blocks." << endl;
    out << endl;
    out << "    --style=gnu  OR  -A6" << endl;
    out << "    GNU style formatting/indenting." << endl;
    out << "    Broken braces, indented blocks, indent is 2 spaces." << endl;
    out << endl;
    out << "    --style=linux  OR  -A7" << endl;
    out << "    Linux style formatting/indenting." << endl;
    out << "    Linux braces, indent is 8 spaces." << endl;
    out << endl;
    out << "Tab and Brace Options:" << endl;
    out << "----------------------" << endl;
    out << "    default indent option" << endl;
    out << "    If no indentation option is set, the default" << endl;
    out << "    option of 4 spaces per indent will be used." << endl;
    out << endl;
    out << "    --indent=spaces=#  OR  -s#" << endl;
    out << "    Indent using # spaces per indent. Not specifying #" << endl;
    out << "    will result in a default of 4 spaces per indent." << endl;
    out << endl;
    out << "    --indent=tab  OR  --indent=tab=#  OR  -t  OR  -t#" << endl;
    out << "    Indent using tab characters, assuming that each" << endl;
    out << "    tab is # spaces long. Not specifying # will result" << endl;
    out << "    in a default assumption of 4 spaces per tab." << endl;
    out << endl;
    out << "    --indent=force-tab=#  OR  -T#" << endl;
    out << "    Indent using tab characters, assuming that each" << endl;
    out << "    tab is # spaces long. Force tabs to be used in areas" << endl;
    out << "    qcstyle would prefer to use spaces." << endl;
    out << endl;
    out << "    default braces option" << endl;
    out << "    If no braces option is set, the braces will not be changed." << endl;
    out << endl;
    out << "    --brackets=break  OR  -b" << endl;
    out << "    Break braces from pre-block code (i.e. ANSI C style)." << endl;
    out << endl;
    out << "    --brackets=attach  OR  -a" << endl;
    out << "    Attach braces to pre-block code (i.e. K&R style)." << endl;
    out << endl;
    out << "    --brackets=linux  OR  -l" << endl;
    out << "    Break definition-block braces and attach command-block braces." << endl;
    out << endl;
    out << "    --brackets=stroustrup  OR  -u" << endl;
    out << "    Attach all braces except function definition braces." << endl;
    out << endl;
    out << "Indentation Options:" << endl;
    out << "---------------------" << endl;
    out << "    --indent-switches  OR  -S" << endl;
    out << "    Indent 'switch' blocks, so that the inner 'case XXX:'" << endl;
    out << "    headers are indented in relation to the switch block." << endl;
    out << endl;
    out << "    --indent-cases  OR  -K" << endl;
    out << "    Indent case blocks from the 'case XXX:' headers." << endl;
    out << "    Case statements not enclosed in blocks are NOT indented." << endl;
    out << endl;
    out << "    --indent-blocks  OR  -G" << endl;
    out << "    Add extra indentation to entire blocks (including braces)." << endl;
    out << endl;
    out << "    --indent-brackets  OR  -B" << endl;
    out << "    Add extra indentation to '{' and '}' block braces." << endl;
    out << endl;
    out << "    --indent-labels  OR  -L" << endl;
    out << "    Indent labels so that they appear one indent less than" << endl;
    out << "    the current indentation level, rather than being" << endl;
    out << "    flushed completely to the left (which is the default)." << endl;
    out << endl;
    out << "    --indent-preprocessor  OR  -w" << endl;
    out << "    Indent multi-line #define statements." << endl;
    out << endl;
    out << "    --max-instatement-indent=#  OR  -M#" << endl;
    out << "    Indent a maximal # spaces in a continuous statement," << endl;
    out << "    relative to the previous line." << endl;
    out << endl;
    out << "    --min-conditional-indent=#  OR  -m#" << endl;
    out << "    Indent a minimal # spaces in a continuous conditional" << endl;
    out << "    belonging to a conditional header." << endl;
    out << endl;
    out << "Formatting Options:" << endl;
    out << "--------------------" << endl;
    out << "    --break-blocks  OR  -f" << endl;
    out << "    Insert empty lines around unrelated blocks, labels, ..." << endl;
    out << endl;
    out << "    --break-blocks=all  OR  -F" << endl;
    out << "    Like --break-blocks, except also insert empty lines" << endl;
    out << "    around closing headers (e.g. 'else', ...)." << endl;
    out << endl;
    out << "    --break-closing-braces  OR  -y" << endl;
    out << "    Break braces before closing headers (e.g. 'else', ...)." << endl;
    out << "    Use with --brackets=attach, --brackets=linux," << endl;
    out << "    or --brackets=stroustrup." << endl;
    out << endl;
    out << "    --break-elseifs  OR  -e" << endl;
    out << "    Break 'else if()' statements into two different lines." << endl;
    out << endl;
    out << "    --delete-empty-lines  OR  -x" << endl;
    out << "    Delete empty lines within a function." << endl;
    out << "    It will NOT delete lines added by the break-blocks options." << endl;
    out << endl;
    out << "    --pad-oper  OR  -p" << endl;
    out << "    Insert space padding around operators." << endl;
    out << endl;
    out << "    --pad-paren  OR  -P" << endl;
    out << "    Insert space padding around parenthesis on both the outside" << endl;
    out << "    and the inside." << endl;
    out << endl;
    out << "    --pad-paren-out  OR  -d" << endl;
    out << "    Insert space padding around parenthesis on the outside only." << endl;
    out << endl;
    out << "    --pad-paren-in  OR  -D" << endl;
    out << "    Insert space padding around parenthesis on the inside only." << endl;
    out << endl;
    out << "    --unpad-paren  OR  -U" << endl;
    out << "    Remove unnecessary space padding around parenthesis. This" << endl;
    out << "    can be used in combination with the 'pad' options above." << endl;
    out << endl;
    out << "    --keep-one-line-statements  OR  -o" << endl;
    out << "    Don't break lines containing multiple statements into" << endl;
    out << "    multiple single-statement lines." << endl;
    out << endl;
    out << "    --keep-one-line-blocks  OR  -O" << endl;
    out << "    Don't break blocks residing completely on one line." << endl;
    out << endl;
    out << "    --convert-tabs  OR  -c" << endl;
    out << "    Convert tabs to the appropriate number of spaces." << endl;
    out << endl;
    out << "    --fill-empty-lines  OR  -E" << endl;
    out << "    Fill empty lines with the white space of their" << endl;
    out << "    previous lines." << endl;
    out << endl;
    out << "Other Options:" << endl;
    out << "---------------" << endl;
    out << "    --suffix=####" << endl;
    out << "    Append the suffix #### instead of '.orig' to original filename." << endl;
    out << endl;
    out << "    --suffix=none  OR  -n" << endl;
    out << "    Do not retain a backup of the original file." << endl;
    out << endl;
    out << "    --errors-to-stdout  OR  -X" << endl;
    out << "    Print errors and help information to standard-output rather than" << endl;
    out << "    to standard-error." << endl;
    out << endl;
    out << "    --version  OR  -V" << endl;
    out << "    Print version number." << endl;
    out << endl;
    out << "    --help  OR  -h  OR  -?" << endl;
    out << "    Print compact help message." << endl;
    out << endl;
    out << "    --help-full  OR  -H" << endl;
    out << "    Print this help message." << endl;
}
// clang-format on
