/*
 * @(#)StringTokenizer.h	1.0 98/03/11
 *
 * Copyright (c) 1998 Dipartimento di Informatica. All Rights Reserved.
 *
 * CopyrightVersion 1.0_beta
 *
 */

/**
 * This is derived from James Gosling's StringTokenizer for JAVA.
 *
 * The string tokenizer class allows an application to break a
 * string into tokens. The tokenization method is much simpler than
 * the one used by the <code>StreamTokenizer</code> class. The
 * <code>StringTokenizer</code> methods do not distinguish among
 * identifiers, numbers, and quoted strings, nor do they recognize
 * and skip comments.
 * <p>
 * The set of delimiters (the characters that separate tokens) may
 * be specified either at creation time or on a per-token basis.
 * <p>
 * An instance of <code>StringTokenizer</code> behaves in one of two
 * ways, depending on whether it was created with the
 * <code>returnTokens</code> flag having the value <code>true</code>
 * or <code>false</code>:
 * <ul>
 * <li>If the flag is <code>false</code>, delimiter characters serve to
 *     separate tokens. A token is a maximal sequence of consecutive
 *     characters that are not delimiters.
 * <li>If the flag is <code>true</code>, delimiter characters are considered to
 *     be tokens. A token is either one delimiter character, or a maximal
 *     sequence of consecutive characters that are not delimiters.
 * </ul>
 * <p>
 * The following is one example of the use of the tokenizer. The code:
 * <blockquote><pre>
 *     StringTokenizer st = new StringTokenizer("this is a test");
 *     while (st.hasMoreTokens()) {
 *         println(st.nextToken());
 *     }
 * </pre></blockquote>
 * <p>
 * prints the following output:
 * <blockquote><pre>
 *     this
 *     is
 *     a
 *     test
 * </pre></blockquote>
 *
 * @author  Marco Botta
 * @version 1.0, 03/11/98
 * @see     StreamTokenizer
 */

//#include <wxstrgnu.h>
#include <string>
#include <iostream>
using namespace std;
typedef string String;
//#if !defined(WIN32)
typedef int boolean;
//#endif

class StringTokenizer {
private:
    int currentPosition;
    int maxPosition;
    String str;
    String delimiters;
    boolean retTokens;

    /**
     * Constructs a string tokenizer for the specified string. The
     * characters in the <code>delim</code> argument are the delimiters
     * for separating tokens.
     * <p>
     * If the <code>returnTokens</code> flag is <code>true</code>, then
     * the delimiter characters are also returned as tokens. Each
     * delimiter is returned as a string of length one. If the flag is
     * <code>false</code>, the delimiter characters are skipped and only
     * serve as separators between tokens.
     *
     * @param   str            a string to be parsed.
     * @param   delim          the delimiters.
     * @param   returnTokens   flag indicating whether to return the delimiters
     *                         as tokens.
     */
public:
    StringTokenizer(String& str, char * delim = " \t\n\r", boolean returnTokens = false);

    /**
     * Skips delimiters.
     */
private:
    void skipDelimiters();

    /**
     * Tests if there are more tokens available from this tokenizer's string.
     *
     * @return  <code>true</code> if there are more tokens available from this
     *          tokenizer's string; <code>false</code> otherwise.
     */
public:
    boolean hasMoreTokens() {
		skipDelimiters();
		return (currentPosition < maxPosition);
    }

    /**
     * Returns the next token from this string tokenizer.
     *
     * @return     the next token from this string tokenizer.
     * @exception  NoSuchElementException  if there are no more tokens in this
     *               tokenizer's string.
     */
    String nextToken();

    /**
     * Returns the next token in this string tokenizer's string. The new
     * delimiter set remains the default after this call.
     *
     * @param      delim   the new delimiters.
     * @return     the next token, after switching to the new delimiter set.
     * @exception  NoSuchElementException  if there are no more tokens in this
     *               tokenizer's string.
     */
    String nextToken(char * delim) {
		delimiters = delim;
		return nextToken();
    }

    /**
     * Calculates the number of times that this tokenizer's
     * <code>nextToken</code> method can be called before it generates an
     * exception.
     *
     * @return  the number of tokens remaining in the string using the current
     *          delimiter set.
     * @see     StringTokenizer#nextToken()
     */
    int countTokens();
};
