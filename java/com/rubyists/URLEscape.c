/**
Copyright © 2009 Evan Phoenix and The Rubyists, LLC
Distributed under the terms of the MIT license.
See the LICENSE file which accompanies this software for the full text
*/

#include <jni.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "URLEscape.h"


static const char space[] = { ' ' };
static const char plus[] =  { '+' };
static const char hex_table[][4] = {
  "%00", "%01", "%02", "%03", "%04", "%05", "%06", "%07",
  "%08", "%09", "%0A", "%0B", "%0C", "%0D", "%0E", "%0F",
  "%10", "%11", "%12", "%13", "%14", "%15", "%16", "%17",
  "%18", "%19", "%1A", "%1B", "%1C", "%1D", "%1E", "%1F",
  "%20", "%21", "%22", "%23", "%24", "%25", "%26", "%27",
  "%28", "%29", "%2A", "%2B", "%2C", "%2D", "%2E", "%2F",
  "%30", "%31", "%32", "%33", "%34", "%35", "%36", "%37",
  "%38", "%39", "%3A", "%3B", "%3C", "%3D", "%3E", "%3F",
  "%40", "%41", "%42", "%43", "%44", "%45", "%46", "%47",
  "%48", "%49", "%4A", "%4B", "%4C", "%4D", "%4E", "%4F",
  "%50", "%51", "%52", "%53", "%54", "%55", "%56", "%57",
  "%58", "%59", "%5A", "%5B", "%5C", "%5D", "%5E", "%5F",
  "%60", "%61", "%62", "%63", "%64", "%65", "%66", "%67",
  "%68", "%69", "%6A", "%6B", "%6C", "%6D", "%6E", "%6F",
  "%70", "%71", "%72", "%73", "%74", "%75", "%76", "%77",
  "%78", "%79", "%7A", "%7B", "%7C", "%7D", "%7E", "%7F",
  "%80", "%81", "%82", "%83", "%84", "%85", "%86", "%87",
  "%88", "%89", "%8A", "%8B", "%8C", "%8D", "%8E", "%8F",
  "%90", "%91", "%92", "%93", "%94", "%95", "%96", "%97",
  "%98", "%99", "%9A", "%9B", "%9C", "%9D", "%9E", "%9F",
  "%A0", "%A1", "%A2", "%A3", "%A4", "%A5", "%A6", "%A7",
  "%A8", "%A9", "%AA", "%AB", "%AC", "%AD", "%AE", "%AF",
  "%B0", "%B1", "%B2", "%B3", "%B4", "%B5", "%B6", "%B7",
  "%B8", "%B9", "%BA", "%BB", "%BC", "%BD", "%BE", "%BF",
  "%C0", "%C1", "%C2", "%C3", "%C4", "%C5", "%C6", "%C7",
  "%C8", "%C9", "%CA", "%CB", "%CC", "%CD", "%CE", "%CF",
  "%D0", "%D1", "%D2", "%D3", "%D4", "%D5", "%D6", "%D7",
  "%D8", "%D9", "%DA", "%DB", "%DC", "%DD", "%DE", "%DF",
  "%E0", "%E1", "%E2", "%E3", "%E4", "%E5", "%E6", "%E7",
  "%E8", "%E9", "%EA", "%EB", "%EC", "%ED", "%EE", "%EF",
  "%F0", "%F1", "%F2", "%F3", "%F4", "%F5", "%F6", "%F7",
  "%F8", "%F9", "%FA", "%FB", "%FC", "%FD", "%FE", "%FF"
};

static inline char hex(char digit) {
  if(digit >= '0' && digit <= '9') {
    return digit - '0';
  } else if(digit >= 'a' && digit <= 'f') {
    return 10 + digit - 'a';
  } else if(digit >= 'A' && digit <= 'F') {
    return 10 + digit - 'A';
  }

  return -1;
}

JNIEXPORT jstring JNICALL Java_URLEscape_unescape(JNIEnv *env, jobject object, jstring str)
{
  jboolean iscopy;
  const char *utf_str;
  char *c_outstr;
  jstring outstr;
  long i, len, bytes;

  utf_str = (*env)->GetStringUTFChars(env, str, &iscopy);
  len = strlen(utf_str);
  c_outstr = (char *) calloc(strlen(utf_str), sizeof(char *));

  for(i = 0, bytes = 0; i < len;) {
    if(utf_str[i] == '%' && i + 2 < len) {
      char high = hex(utf_str[i + 1]);
      char low =  hex(utf_str[i + 2]);

      if(high >= 0 && low >= 0) {
        const char byte = low + (high << 4);
        c_outstr[bytes] = byte;
        bytes++;
        i += 3;
        continue;
      }
    }

    if(utf_str[i] == '+') {
      c_outstr[bytes] = ' ';
    } else {
      c_outstr[bytes] = utf_str[i];
    }
    bytes++;
    i++;
  }
  (*env)->ReleaseStringUTFChars(env, str, utf_str);
  outstr = (*env)->NewStringUTF(env, c_outstr);
  free(c_outstr);

  return outstr;
}

static inline int valid_literal(const char byte)
{
  return ('A' <= byte && byte <= 'Z') ||
         ('a' <= byte && byte <= 'z') ||
         ('0' <= byte && byte <= '9') ||
         byte == '-' ||
         byte == '_' ||
         byte == '.';
}

static inline int valid_two_byte_utf8(const unsigned char byte_one, const unsigned char byte_two)
{
  return (0xc0 <= byte_one && byte_one <= 0xdf &&
          0x80 <= byte_two && byte_two <= 0xbf);
}

static inline int valid_three_byte_utf8(const unsigned char byte_one, const unsigned char byte_two)
{
  return (0xe0 == byte_one &&
          0x80 <= byte_two && byte_two <= 0xbf);
}

static long count_encoded_chars(const char *str, long len)
{
  unsigned char byte_one, byte_two;
  long count = 0;
  long i;

  for(i = 0; i < len;) {
    byte_one = str[i];

    /* (UTF-8 escape, 0x0000-0x007F) */
    if(byte_one < 0x80) {
      if(valid_literal(byte_one)) {
        count += 1;
      } else if(byte_one == ' ') {
        /* a + or %20 replacement (depending on const plus assignment) */
        count += 1;
      } else { /* It's ascii but needs encoding */
        count += 3;
      }
      i++;
      continue;
    }

    /* Ok, there are UTF-8 prefix bytes, so we need to interpret them. */
    /* If we have at least one extra byte to consume */
    if(i + 1 < len) {
      byte_two = str[i + 1];

      /* (UTF-8 escape, 0x0080-0x07FF) */
      if(valid_two_byte_utf8(byte_one, byte_two)) {
        count += 6;
        i += 2;
        continue;

      /* If we have at least two extra bytes to consume */
      } else if(i + 2 < len) {
        /* (UTF-8 escape, 0x0800-0xFFFF) */
        if(valid_three_byte_utf8(byte_one, byte_two)) {
          count += 9;
          i += 3;
          continue;
        }
      }
    }

    /* (ISO Latin-1/2/? escape, 0x0080 - x00FF) */
    count += 3;
    i++;
  }

  return count;
}

JNIEXPORT jstring JNICALL Java_URLEscape_escape(JNIEnv *env, jobject object, jstring str)
{
  jboolean iscopy;
  jstring outstr;
  const char *utf_str;
  char *c_outstr;
  long i, len, bytes;
  unsigned char byte_one, byte_two, byte_three;

  utf_str = (*env)->GetStringUTFChars(env, str, &iscopy);
  len = strlen(utf_str);

  c_outstr = (char *) calloc(count_encoded_chars(utf_str, len) + 1, sizeof(char *));

  for(i = 0, bytes = 0; i < len;) {
    byte_one = utf_str[i];

    /* (UTF-8 escape, 0x0000-0x007F) */
    if(byte_one < 0x80) {
      if(valid_literal(byte_one)) {
        c_outstr[bytes] = byte_one;
        bytes++;
      } else if(byte_one == ' ') {
        /* a + or %20 replacement (depending on const plus assignment) */
        c_outstr[bytes] = plus[0];
        bytes++;
      } else { /* It's ascii but needs encoding */
        c_outstr[bytes] = hex_table[byte_one][0];
        c_outstr[bytes + 1] = hex_table[byte_one][1];
        c_outstr[bytes + 2] = hex_table[byte_one][2];
        bytes += 3;
      }
      i++;
      continue;
    }

    /* Ok, there are UTF-8 prefix bytes, so we need to interpret them. */
    /* If we have at least one extra byte to consume */
    if(i + 1 < len) {
      byte_two = utf_str[i + 1];

      /* (UTF-8 escape, 0x0080-0x07FF) */
      if(valid_two_byte_utf8(byte_one, byte_two)) {
        c_outstr[bytes] = hex_table[byte_one][0];
        c_outstr[bytes + 1] = hex_table[byte_one][1];
        c_outstr[bytes + 2] = hex_table[byte_one][2];
        c_outstr[bytes + 3] = hex_table[byte_two][0];
        c_outstr[bytes + 4] = hex_table[byte_two][1];
        c_outstr[bytes + 5] = hex_table[byte_two][2];
        bytes += 6;
        i += 2;
        continue;

      /* If we have at least two extra bytes to consume */
      } else if(i + 2 < len) {
        byte_two = utf_str[i + 2];

        /* (UTF-8 escape, 0x0800-0xFFFF) */
        if(valid_three_byte_utf8(byte_one, byte_two)) {
          c_outstr[bytes] = hex_table[byte_one][0];
          c_outstr[bytes + 1] = hex_table[byte_one][1];
          c_outstr[bytes + 2] = hex_table[byte_one][2];
          c_outstr[bytes + 3] = hex_table[byte_two][0];
          c_outstr[bytes + 4] = hex_table[byte_two][1];
          c_outstr[bytes + 5] = hex_table[byte_two][2];
          c_outstr[bytes + 6] = hex_table[byte_three][0];
          c_outstr[bytes + 7] = hex_table[byte_three][1];
          c_outstr[bytes + 8] = hex_table[byte_three][2];
          bytes += 9;
          i += 3;
          continue;
        }
      }
    }

    /* (ISO Latin-1/2/? escape, 0x0080 - x00FF) */
    if(0x80 <= byte_one) {
      c_outstr[bytes] = hex_table[byte_one][0];
      c_outstr[bytes + 1] = hex_table[byte_one][1];
      c_outstr[bytes + 2] = hex_table[byte_one][2];
    } else {
      /* Well crap. Just throw it in I guess... */
      c_outstr[bytes] = hex_table[byte_one][0];
      c_outstr[bytes + 1] = hex_table[byte_one][1];
      c_outstr[bytes + 2] = hex_table[byte_one][2];
    }
    bytes += 3;
    i++;
  }
  c_outstr[bytes] = (char)NULL;

  (*env)->ReleaseStringUTFChars(env, str, utf_str);
  outstr = (*env)->NewStringUTF(env, c_outstr);
  free(c_outstr);

  return outstr;
}
