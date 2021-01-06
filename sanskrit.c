/*-
 * Copyright (c) 2021 Michael Roe
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
 * sankrit.c - Takes as input (on stdin) Sanskrit text transliterated in
 * the Latin alphabet, and outputs it in Devanagari.
 */

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>

static wchar_t buff[80];

static int state;
#define STATE_SPACE 0
#define STATE_VOWEL 1
#define STATE_CONSONANT 2

void vowel(wchar_t lone, wchar_t diacritic)
{
  switch (state)
  {
    case STATE_SPACE:
    case STATE_VOWEL:
      wprintf(L"%lc", lone);
      break;
    case STATE_CONSONANT:
      wprintf(L"%lc", diacritic);
      break;
  }
  state = STATE_VOWEL;
}

void consonant(wchar_t c)
{
  if (state == STATE_CONSONANT)
    wprintf(L"%lc", 0x94d); /* virama */
  wprintf(L"%lc", c);
  state = STATE_CONSONANT;
}

void whitespace(wchar_t c)
{
  if (state == STATE_CONSONANT)
  {
    wprintf(L"%lc%lc", 0x94d, c); /* virama */
  }
  else
  {
    wprintf(L"%lc", c);
  }
  state = STATE_SPACE;
}

int main(int argc, char **argv)
{
wchar_t *cp;
int len;

  setlocale(LC_ALL, getenv("LANG"));

  state = STATE_SPACE;

  while (fgetws(buff, 80, stdin))
  {
    cp = buff;
    while (*cp)
    {
      switch (*cp)
      {
        case ' ':
          whitespace(*cp);
	  break;
        case '\n':
	  whitespace(*cp);
	  break;
	case '\'':
	  wprintf(L"%lc", 0x93d);
	  break;
        case 'a':
	  if (cp[1] == 'i')
	  {
	    vowel(0x910, 0x948);
	    cp++;
	  }
	  else if (cp[1] == 'u')
	  {
	    vowel(0x914, 0x94c);
	    cp++;
	  }
	  else
	  {
	    /* In devanagari script, a is the default vowel, so don't need a
	     * combining character for it.
	     */
	    if (state != STATE_CONSONANT)
	    {
              vowel(0x905, 'a');
	    }
	    else
	    {
	      state = STATE_VOWEL;
	    }
	  }
	  break;
	case 0x101: /* a with macron */
	  vowel(0x906, 0x93e);
	  break;
	case 'i':
	  vowel(0x907, 0x93f);
	  break;
	case 0x12b: /* i with macron */
	  vowel(0x908, 0x940);
	  break;
	case 'u':
	  vowel(0x909, 0x941);
	  break;
	case 0x16b: /* u with macron */
	  vowel(0x90a, 0x942);
	  break;
	case 'e':
	  vowel(0x90f, 0x947);
	  break;
	case 'o':
	  vowel(0x913, 0x94b);
	  break;
	case 0x1e5b: /* r with dot below */
	  vowel(0x90b, 0x943);
	  break;
	case 0x1e5d: /* r with dot below and macron */
	  vowel(0x960, 0x944);
	  break;
	case 0x1e37: /* l with dot below */
	  vowel(0x90c, 0x962);
	  break;
	case 0x1e39: /* l with dot below and macron */
	  vowel(0x961, 0x963);
	  break;
	case 0x1e43: /* m with dot below */
	  wprintf(L"%lc", 0x902); /* anusvara */
	  break;
	case 0x1e25: /* h with dot below */
	  wprintf(L"%lc", 0x903); /* visarga */
	  break;
	/* consonants */
	case 'k':
	  if (cp[1] == 'h')
	  {
	    consonant(0x916);
	    cp++;
	  }
	  else
	  {
	    consonant(0x915);
	  }
	  break;
	case 'c':
	  if (cp[1] == 'c')
	  {
	    consonant(0x91b);
	    cp++;
	  }
	  else
	  {
            consonant(0x91a);
	  }
	  break;
	case 0x1e6d: /* t with dot below */
	  if (cp[1] == 'h')
	  {
	    consonant(0x920);
	    cp++;
	  }
	  else
	  {
	    consonant(0x91f);
	  }
	  break;
	case 't':
	  if (cp[1] == 'h')
	  {
	    consonant(0x925);
	    cp++;
	  }
	  else
	  {
	    consonant(0x924);
	  }
	  break;
	case 'p':
	  if (cp[1] == 'h')
	  {
	    consonant(0x92b);
	    cp++;
	  }
	  else
          {
            consonant(0x92a);
	  }
	  break;
	case 'g':
	  if (cp[1] == 'h')
	  {
            consonant(0x918);
	    cp++;
	  }
	  else
          {
            consonant(0x917);
	  }
	  break;
	case 'j':
	  if (cp[1] == 'h')
          {
	    consonant(0x91d);
	    cp++;
	  }
	  else
          {
            consonant(0x91c);
	  }
	  break;
	case 0x1e0d: /* d with dot below */
	  if (cp[1] == 'h')
          {
            consonant(0x922);
	    cp++;
	  }
	  else
          {
	    consonant(0x921);
	  }
	  break;
	case 'd':
	  if (cp[1] == 'h')
	  {
            consonant(0x927);
	    cp++;
	    state = STATE_CONSONANT;
	  }
	  else
	  {
            consonant(0x926);
	  }
	  break;
	case 'b':
	  if (cp[1] == 'h')
	  {
	    consonant(0x92d);
	    cp++;
	  }
	  else
	  {
	    consonant(0x92c);
	  }
	  break;
	case 0x1e45: /* n with dot above */
	  consonant(0x919); /* devanagari letter nga */
	  break;
	case 0xf1: /* n with tilde */
	  consonant(0x91e); /* devanagari letter nya */
	  break;
	case 0x1e47: /* n with dot below */
	  consonant(0x923); /* devanagari letter nna */
	  break;
	case 'n':
	  consonant(0x928);
	  break;
	case 'm':
	  consonant(0x92e);
	  break;
	case 'h':
	  consonant(0x939);
	  break;
	case 'y':
	  consonant(0x92f);
	  break;
	case 'r':
	  consonant(0x930);
	  break;
	case 'l':
	  consonant(0x932);
	  break;
	case 'v':
	  consonant(0x935);
	  break;
	case 0x15b: /* s with acute */
	  consonant(0x936);
	  break;
	case 0x1e63: /* s with dot below */
	  consonant(0x937);
	  break;
	case 's':
	  consonant(0x938);
	  break;
	default:
          wprintf(L"%lc", *cp);
	  if (*cp > 255)
	    wprintf(L"(0x%x)", (int) *cp);
	  state = STATE_SPACE;
	  break;
       }
       cp++;
     }
  }
  return 0;
}


