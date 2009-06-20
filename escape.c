/**
(The MIT License)

Copyright © 2009 Evan Phoenix

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the ‘Software’), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED ‘AS IS’, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/


#include "ruby.h"

#ifndef RSTRING_PTR(obj)
#define RSTRING_PTR(obj) RSTRING(obj)->ptr
#endif

#ifndef RSTRING_LEN(obj)
#define RSTRING_LEN(obj) RSTRING(obj)->len
#endif

const char space[] = { ' ' };
const char plus[] =  { '+' };

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
  StringValue(str);

  const char* buf = RSTRING_PTR(str);
  const char* bufend = buf + RSTRING_LEN(str);

  VALUE outstr = rb_str_buf_new(RSTRING_LEN(str));

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

const char hex_table[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                           'a', 'b', 'c', 'd', 'e', 'f' };

static VALUE escape(VALUE self, VALUE str) {
  StringValue(str);

  const char* buf = RSTRING_PTR(str);
  const char* bufend = buf + RSTRING_LEN(str);

  VALUE outstr = rb_str_buf_new(RSTRING_LEN(str));

  while(buf < bufend) {
    const char byte = buf[0];
    if(byte == ' ') {
      rb_str_buf_cat(outstr, plus, 1);
    } else if(byte >= 'A' && byte <= 'Z') {
      rb_str_buf_cat(outstr, buf, 1);
    } else if(byte >= 'a' && byte <= 'z') {
      rb_str_buf_cat(outstr, buf, 1);
    } else if(byte >= '0' && byte <= '9') {
      rb_str_buf_cat(outstr, buf, 1);
    } else {
      const char char_one = hex_table[byte >> 4];
      const char char_two = hex_table[byte % 16];
      const char tmp[] = { '%', char_one, char_two };
      rb_str_buf_cat(outstr, tmp, 3);
    }
    buf++;
  }

  return outstr;
}

void Init_unescape() {
  VALUE mod = rb_define_module("URLEscape");
  rb_define_singleton_method(mod, "unescape", unescape, 1);
  rb_define_singleton_method(mod, "escape",   escape,   1);
}
