/*
 * @(#)StringTokenizer.cpp	1.0 98/03/11
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

#include "StringTokenizer.h"

StringTokenizer::StringTokenizer(String& str, char * delim, boolean returnTokens)
{
	currentPosition = 0;
	this->str = str;
//	maxPosition = str.Length();
	maxPosition = str.size();
	delimiters = delim;
	retTokens = returnTokens;
}

void StringTokenizer::skipDelimiters()
{
	int pos;
	while (!retTokens &&
	       (currentPosition < maxPosition) &&
//		   ((pos = delimiters.Index(str[currentPosition])) >= 0)) {
		   ((pos = delimiters.find_first_of(str[currentPosition])) >= 0)) {
	    currentPosition++;
	}
}

String StringTokenizer::nextToken()
{
	skipDelimiters();

	if (currentPosition >= maxPosition) {
	    cerr << "No such element " << currentPosition << endl;
	    exit(1);
	}

	int start = currentPosition;
	int pos;
	while ((currentPosition < maxPosition) &&
//	       ((pos = delimiters.Index(str[currentPosition])) < 0)) {
	       ((pos = delimiters.find_first_of(str[currentPosition])) < 0)) {
	    currentPosition++;
	}
	if (retTokens && (start == currentPosition) &&
//	    ((pos = delimiters.Index(str[currentPosition])) >= 0)) {
	    ((pos = delimiters.find_first_of(str[currentPosition])) >= 0)) {
	    currentPosition++;
	}
//	return str.SubString(start, currentPosition-1);
	return str.substr(start, currentPosition-start);
}

int StringTokenizer::countTokens()
{
	int count = 0;
	int currpos = currentPosition;
	int pos;

	while (currpos < maxPosition) {
	    /*
	     * This is just skipDelimiters(); but it does not affect
	     * currentPosition.
	     */
	    while (!retTokens &&
		   (currpos < maxPosition) &&
//		   ((pos = delimiters.Index(str[currpos])) >= 0)) {
		   ((pos = delimiters.find_first_of(str[currpos])) >= 0)) {
			currpos++;
	    }

	    if (currpos >= maxPosition) {
			break;
	    }

	    int start = currpos;
	    while ((currpos < maxPosition) &&
//		   ((pos = delimiters.Index(str[currpos])) < 0)) {
		   ((pos = delimiters.find_first_of(str[currpos])) < 0)) {
			currpos++;
	    }
	    if (retTokens && (start == currpos) &&
//			((pos = delimiters.Index(str[currpos])) >= 0)) {
			((pos = delimiters.find_first_of(str[currpos])) >= 0)) {
			currpos++;
	    }
	    count++;

	}
	return count;
}
