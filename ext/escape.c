/**
(The MIT License)

Copyright © 2009 Evan Phoenix

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the ‘Software’), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED ‘AS IS’, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/


#include "ruby.h"

#ifndef RSTRING_PTR
#define RSTRING_PTR(obj) RSTRING(obj)->ptr
#endif

#ifndef RSTRING_LEN
#define RSTRING_LEN(obj) RSTRING(obj)->len
#endif

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

static VALUE unescape(VALUE self, VALUE str) {
  const char* buf;
  const char* bufend;
  VALUE outstr;

  StringValue(str);
  buf = RSTRING_PTR(str);
  bufend = buf + RSTRING_LEN(str);
  outstr = rb_str_buf_new(RSTRING_LEN(str));

  while(buf < bufend) {
    if(buf[0] == '%' && buf + 2 <= bufend) {
      char high = hex(buf[1]);
      char low =  hex(buf[2]);

      if(high >= 0 && low >= 0) {
        const char byte = low + (high << 4);
        rb_str_buf_cat(outstr, &byte, 1);
        buf += 3;
        continue;
      }
    }

    if(buf[0] == '+') {
      rb_str_buf_cat(outstr, space, 1);
    } else {
      rb_str_buf_cat(outstr, buf, 1);
    }
    buf++;
  }

  return outstr;
}


static inline int valid_literal(const char byte) {
  return ('A' <= byte && byte <= 'Z') ||
         ('a' <= byte && byte <= 'z') ||
         ('0' <= byte && byte <= '9') ||
         byte == '-' ||
         byte == '_' ||
         byte == '.';

}

static VALUE escape(VALUE self, VALUE str)
{
  char* buf;
  int len;
  VALUE outstr;
  int i;
  unsigned char byte_two, byte_three;

  StringValue(str);
  buf = RSTRING_PTR(str);
  len = RSTRING_LEN(str);
  outstr = rb_str_buf_new(len);

  for(i = 0; i < len;) {
    const unsigned char byte_one = buf[i];

    /* (UTF-8 escape, 0x0000-0x007F) */
    if(byte_one < 0x80) {
      if(valid_literal(byte_one)) {
        rb_str_buf_cat(outstr, buf+i, 1);
      } else if(byte_one == ' ') {
        // a + or %20 replacement (depending on const plus assignment)
        rb_str_buf_cat(outstr, plus, 1);
      } else { // It's ascii but needs encoding
        rb_str_buf_cat(outstr, hex_table[byte_one], 3);
      }
      i++;
      continue;
    }

    // Ok, there are UTF-8 prefix bytes, so we need to interpret
    // them.
    //
    // If we have at least one extra byte to consume
    if(i + 1 < len) {
      byte_two = buf[i + 1];

      /* (UTF-8 escape, 0x0080-0x07FF) */
      if(0xc0 <= byte_one && byte_one <= 0xdf && 0x80 <= byte_two && byte_two <= 0xbf) {
        rb_str_buf_cat(outstr, hex_table[byte_one], 3);
        rb_str_buf_cat(outstr, hex_table[byte_two], 3);
        i += 2;
        continue;

        // If we have at least two extra bytes to consume
      } else if(i + 2 < len) {
        byte_three = buf[i + 2];

        /* (UTF-8 escape, 0x0800-0xFFFF) */
        if(0xe0 == byte_one && 0x80 <= byte_two && byte_two <= 0xbf) {
          rb_str_buf_cat(outstr, hex_table[byte_one], 3);
          rb_str_buf_cat(outstr, hex_table[byte_two], 3);
          rb_str_buf_cat(outstr, hex_table[byte_three], 3);
          i += 3;
          continue;
        }
      }
    }

    /* (ISO Latin-1/2/? escape, 0x0080 - x00FF) */
    if(0x80 <= byte_one) {
      rb_str_buf_cat(outstr, hex_table[byte_one], 3);
    } else {
      // Well crap. Just throw it in I guess...
      rb_str_buf_cat(outstr, hex_table[byte_one], 3);
    }
    i++;
  }

  return outstr;
}

void Init_url_escape() {
  VALUE mod = rb_define_module("URLEscape");
  rb_define_singleton_method(mod, "unescape", unescape, 1);
  rb_define_singleton_method(mod, "escape",   escape,   1);
}
