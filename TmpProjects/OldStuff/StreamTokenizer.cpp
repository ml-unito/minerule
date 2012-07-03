/*
 * @(#)StreamTokenizer.cpp	1.0 98/03/09
 * 
 * Copyright (c) 1998 Universita` di Torino. All Rights Reserved.
 * 
 * CopyrightVersion 1.0_beta
 * 
 */

/**
 * This is based on James Gosling's StreamTokenizer.java file.
 *
 * The <code>StreamTokenizer</code> class takes an input stream and 
 * parses it into "tokens", allowing the tokens to be 
 * read one at a time. The parsing process is controlled by a table 
 * and a number of flags that can be set to various states. The 
 * stream tokenizer can recognize identifiers, numbers, quoted 
 * strings, and various comment styles. 
 * <p>
 * Each byte read from the input stream is regarded as a character 
 * in the range <code>'&#92;u0000'</code> through <code>'&#92;u00FF'</code>. 
 * The character value is used to look up five possible attributes of 
 * the character: <i>white space</i>, <i>alphabetic</i>, 
 * <i>numeric</i>, <i>string quote</i>, and <i>comment character</i>. 
 * Each character can have zero or more of these attributes. 
 * <p>
 * In addition, an instance has four flags. These flags indicate: 
 * <ul>
 * <li>Whether line terminators are to be returned as tokens or treated 
 *     as white space that merely separates tokens. 
 * <li>Whether C-style comments are to be recognized and skipped. 
 * <li>Whether C++-style comments are to be recognized and skipped. 
 * <li>Whether the characters of identifiers are converted to lowercase. 
 * </ul>
 * <p>
 * A typical application first constructs an instance of this class, 
 * sets up the syntax tables, and then repeatedly loops calling the 
 * <code>nextToken</code> method in each iteration of the loop until 
 * it returns the value <code>TT_EOF</code>. 
 *
 * @author  Marco Botta
 * @version 1.0, 03/09/98
 */

#include "StreamTokenizer.h"
#include <ctype.h>

void StreamTokenizer::Init() {
    buflength = 50;
	buf = new char[buflength];
	LINENO = 1;
	peekc = 0;
    eolIsSignificantP = false;
    slashSlashCommentsP = false;
    slashStarCommentsP = false;
	pushedBack = false;
	forceLower = false;
	ttype = TT_NOTHING;
	resetSyntax();
	wordChars('a', 'z');
	wordChars('A', 'Z');
	wordChars(128 + 32, 255);
	whitespaceChars(0, ' ');
	commentChar('/');
	quoteChar('"');
	quoteChar('\'');
	parseNumbers();
}

    /** 
     * Resets this tokenizer's syntax table so that all characters are 
     * "ordinary." See the <code>ordinaryChar</code> method 
     * for more information on a character being ordinary. 
     *
     * @see     StreamTokenizer#ordinaryChar(int)
     */
void StreamTokenizer::resetSyntax() {
		for (int i = 256; --i >= 0;)
			ctype[i] = 0;
    }

void StreamTokenizer::wordChars(int low, int hi) {
		if (low < 0)
			low = 0;
		if (hi >= 256)
		    hi = 255;  
		while (low <= hi)
			ctype[low++] |= CT_ALPHA;
    }

void StreamTokenizer::whitespaceChars(int low, int hi) {
		if (low < 0)
			low = 0;
		if (hi >= 256)
			hi = 255;
		while (low <= hi)
			ctype[low++] = CT_WHITESPACE;
    }

void StreamTokenizer::ordinaryChars(int low, int hi) {
		if (low < 0)
			low = 0;
		if (hi >= 256)
			hi = 255;
		while (low <= hi)
			ctype[low++] = 0;
    }

void StreamTokenizer::ordinaryChar(int ch) {
        if (ch >= 0 && ch < 256)
	  	    ctype[ch] = 0;
    }

void StreamTokenizer::commentChar(int ch) {
        if (ch >= 0 && ch < 256)
		    ctype[ch] = CT_COMMENT;
    }

void StreamTokenizer::quoteChar(int ch) {
        if (ch >= 0 && ch < 256)
 			ctype[ch] = CT_QUOTE;
    }

void StreamTokenizer::parseNumbers() {
		for (int i = '0'; i <= '9'; i++)
			ctype[i] |= CT_DIGIT;
		ctype['.'] |= CT_DIGIT;
		ctype['-'] |= CT_DIGIT;
    }

    /** 
     * Parses the next token from the input stream of this tokenizer. 
     * The type of the next token is returned in the <code>ttype</code> 
     * field. Additional information about the token may be in the 
     * <code>nval</code> field or the <code>sval</code> field of this 
     * tokenizer. 
     * <p>
     * Typical clients of this
     * class first set up the syntax tables and then sit in a loop
     * calling nextToken to parse successive tokens until TT_EOF
     * is returned. 
     *
     * @return     the value of the <code>ttype</code> field.
     * @exception  IOException  if an I/O error occurs.
     * @see        StreamTokenizer#nval
     * @see        StreamTokenizer#sval
     * @see        StreamTokenizer#ttype
     */
int StreamTokenizer::nextToken() {
	if (pushedBack) {
		pushedBack = false;
		return ttype;
	}
	char * ct = ctype;
	int c; 
	sval = "";

	if (ttype == TT_NOTHING) {
	    c = read();
	    if (c >= 0)    // ttype is surely overwritten below to its correct value.
	        ttype = c; // for now we just make sure it isn't TT_NOTHING
	} else {
	    c = peekc;
	}
	
	if (c < 0)
	    return ttype = TT_EOF;
	int ctype = c < 256 ? ct[c] : CT_ALPHA;
	while ((ctype & CT_WHITESPACE) != 0) {
	    if (c == '\r') {
		LINENO++;
		c = read();
		if (c == '\n')
		    c = read();
		if (eolIsSignificantP) {
		    peekc = c;
		    return ttype = TT_EOL;
		}
	    } else {
		if (c == '\n') {
		    LINENO++;
		    if (eolIsSignificantP) {
			peekc = read();
			return ttype = TT_EOL;
		    }
		}
		c = read();
	    }
	    if (c < 0)
		return ttype = TT_EOF;
	    ctype = c < 256 ? ct[c] : CT_ALPHA;
	}
	if ((ctype & CT_DIGIT) != 0) {
	    boolean neg = false;
	    if (c == '-') {
		c = read();
		if (c != '.' && (c < '0' || c > '9')) {
		    peekc = c;
		    return ttype = '-';
		}
		neg = true;
	    }
	    double v = 0;
	    int decexp = 0;
	    int seendot = 0;
	    while (true) {
		if (c == '.' && seendot == 0)
		    seendot = 1;
		else if ('0' <= c && c <= '9') {
		    v = v * 10 + (c - '0');
		    decexp += seendot;
		} else
		    break;
		c = read();
	    }
	    if (c == 'e' || c == 'E') {
	    	boolean negexp = false;
	    	int exp = 0;
		c = read();
		if (c == '-') { negexp = true; c = read(); }
	    while (true) {
		if ('0' <= c && c <= '9') {
		    exp = exp * 10 + (c - '0');
		} else {
		    decexp += (negexp ? exp : -exp);
		    break;
		}
		c = read();
	    }
	    }
	    peekc = c;
	    if (decexp > 0) {
		double denom = 10;
		decexp--;
		while (decexp > 0) {
		    denom *= 10;
		    decexp--;
		}
		/* do one division of a likely-to-be-more-accurate number */
		v = v / denom;
	    } else if (decexp < 0) {
		double denom = 10;
		decexp++;
		while (decexp < 0) {
		    denom *= 10;
		    decexp++;
		}
		/* do one multiplicaiton of a likely-to-be-more-accurate number */
		v = v * denom;
	    }
	    nval = neg ? -v : v;
	    return ttype = TT_NUMBER;
	}
	if ((ctype & CT_ALPHA) != 0) {
	    int i = 0;
	    do {
		if (i >= buflength) {
		    char * nb = new char[buflength * 2];
		    strncpy(nb,buf,buflength);
			delete buf;
		    buf = nb;
			buflength *= 2;
		}
		buf[i++] = (char) c;
		c = read();
		ctype = c < 0 ? CT_WHITESPACE : c < 256 ? ct[c] : CT_ALPHA;
	    } while ((ctype & (CT_ALPHA | CT_DIGIT)) != 0);
	    peekc = c;
	    if (forceLower)
			for (int j = 0; j<i; j++) buf[j] = tolower(buf[j]);
	    buf[i] = '\0';
#ifdef __GNUC__
	    sval = buf;
#else
	    sval.assign(buf, i);
#endif
	    return ttype = TT_WORD;
	}
	if ((ctype & CT_COMMENT) != 0) {
	    while ((c = read()) != '\n' && c != '\r' && c >= 0);
	    peekc = c;
	    return nextToken();
	}
	if ((ctype & CT_QUOTE) != 0) {
	    ttype = c;
	    int i = 0;
	    // invariants (because \Octal needs a lookahead):
	    //      (i)  c contains char value 
	    //      (ii) peekc contains the lookahead
	    peekc = read(); 
	    while (peekc >= 0 && peekc != ttype && peekc != '\n' && peekc != '\r') {
	        if (peekc == '\\') {
   		    c = read();
		    int first = c;   // to allow \377, but not \477
		    if (c >= '0' && c <= '7') {
			c = c - '0';
			int c2 = read();
			if ('0' <= c2 && c2 <= '7') {
			    c = (c << 3) + (c2 - '0');
			    c2 = read();
			    if ('0' <= c2 && c2 <= '7' && first <= '3') {
				c = (c << 3) + (c2 - '0');
				peekc = read();
			    } else
				peekc = c2;
			} else
			  peekc = c2;
		    } else {
  		        switch (c) {
			case 'a':
			    c = 0x7;
			    break;
			case 'b':
			    c = '\b';
			    break;
			case 'f':
			    c = 0xC;
			    break;
			case 'n':
			    c = '\n';
			    break;
		        case 'r':
			    c = '\r';
			    break;
			case 't':
			    c = '\t';
			    break;
			case 'v':
			    c = 0xB;
			    break;
			}
			peekc = read();
		    }
		} else {
		    c = peekc;
		    peekc = read();
		}
		
		if (i >= buflength) {
		    char * nb = new char[buflength * 2];
		    strncpy(nb,buf,buflength);
			delete buf;
		    buf = nb;
			buflength *= 2;
		}
		buf[i++] = (char) c;
	    }
	    if (peekc == ttype)  // keep \n or \r intact in peekc
	        peekc = read();
	    buf[i] = '\0';
#ifdef __GNUC__
	    sval = buf;
#else
	    sval.assign(buf, i);
#endif
	    return ttype;
	}
	if (c == '/' && (slashSlashCommentsP || slashStarCommentsP)) {
	    c = read();
	    if (c == '*' && slashStarCommentsP) {
		int prevc = 0;
		while ((c = read()) != '/' || prevc != '*') {
		    if (c == '\r') {
			LINENO++;
			c = read();
			if (c == '\n') {
			    c = read();
			}
		    } else {
		        if (c == '\n') {
			    LINENO++;
			    c = read();
			}
		    }
		    if (c < 0)
		        return ttype = TT_EOF;
		    prevc = c;
		}
		peekc = read();
		return nextToken();
	    } else if (c == '/' && slashSlashCommentsP) {
	        while ((c = read()) != '\n' && c != '\r' && c >= 0);
	        peekc = c;
		return nextToken();
	    } else {
		peekc = c;
		return ttype = '/';
	    }
	}
	peekc = read();
	return ttype = c;
    }

    /**
     * Returns the string representation of the current stream token. 
     *
     * @return  a string representation of the token specified by the
     *          <code>ttype</code>, <code>nval</code>, and <code>sval</code>
     *          fields.
     * @see     java.io.StreamTokenizer#nval
     * @see     java.io.StreamTokenizer#sval
     * @see     java.io.StreamTokenizer#ttype
     */
/*
String SttreamTokenizer::toString() {
	String ret;
	switch (ttype) {
	  case TT_EOF:
	    ret = "EOF";
	    break;
	  case TT_EOL:
	    ret = "EOL";
	    break;
	  case TT_WORD:
	    ret = sval;
	    break;
	  case TT_NUMBER:
	    ret = "n=" + nval;
	    break;
   	  case TT_NOTHING:  
	    ret = "NOTHING";
	    break;
	  default:{
		char s[[3];
		s[0] = s[2] = '\'';
		s[1] = (char) ttype;
		ret = new String(s);
		break;
	    }
	}
	return "Token[" + ret + "], line " + LINENO;
    }

*/
