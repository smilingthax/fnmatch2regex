#include "fnmatch2regex.h"
#include <regex>
#include <stdio.h>

#define PARSE_RANGE

// TODO? inside bracket vs outside bracket ?! (".+*?" is also safe)
static bool _re_must_escape(char ch) // {{{
{
  switch (ch) {
  case '^': case '$': case '-': case '.': case '|':
  case '?': case '*': case '+': case '\\':
  case '(': case ')': case '[': case ']': case '{': case '}':
    return true;
  }
  return false;
}
// }}}

std::string fnmatch2regex(const char *pattern, fnflags_t flags)
{
  // assert(pattern);
  std::string out;
  out.push_back('^');
  bool first=true;
  for (const char *p=pattern; *p; p++) {
    char ch=*p;
    if (ch=='\\' && !(flags&FNF_NOESCAPE)) {
      p++; // trick: do not change ch yet
      if (!*p) {
        out.append("[]", 2); // no match
        return out;
      }
    }
    if (flags&FNF_PERIOD) {
      if (flags&FNF_PATHNAME && *p=='/') {
        first=true;
        // out.push_back('/'); continue;
      } else if (first) {
        if (*p!='.') {
          out.append("(?!\\.)",6);
        }
        first=false;
      }
    }

    switch (ch) {
    case '?':
      if (flags&FNF_PATHNAME) {
        out.append("[^/]",4);
      } else {
        out.push_back('.');
      }
      break;

    case '*':
      if (flags&FNF_PATHNAME) {
        out.append("[^/]*",5);
      } else {
        out.append(".*",2);
      }
      break;

    case '[': {
      if (flags&FNF_PATHNAME) {
        out.append("(?!/)",5);
      }
      out.push_back('[');
      ch=*++p;

#ifdef PARSE_RANGE
      int from=-1;
#endif
      if (ch=='!' || ch=='^') { // not (allow both)
        out.push_back('^');
        ch=*++p;
      }
      if (ch==']') { // special case
        out.append("\\]", 2);
        ch=*++p;
#ifdef PARSE_RANGE
        from=ch;
#endif
      }
      for (; ch!=']'; ch=*++p) {
        if (!ch) { // not terminated -> no match
          out.append("][]", 3);
          return out;
        }
        switch (ch) {
        case '-':
#ifdef PARSE_RANGE
          if (from==-1 || p[1]==']') {
            // (note: "[-" is also equal to "[\\-" in regexp)
            // (note: "-]" is also equal to "\\-]" in regexp)
            out.append("\\-",2);
          } else {
            if (p[1]=='\\' && !(flags&FNF_NOESCAPE)) {
              p++;
            }
            if (!p[1]) { // NOT: else if!
              continue; // -> no match
            }
            ch=*++p;
if (ch<from) { // reverse range
  out.pop_back(); // HACK!!!
  from=-1;
  break;
}
            out.push_back('-');
            if (_re_must_escape(ch)) {
              out.push_back('\\');
            }
            out.push_back(ch);
          }
          from=-1;
#else
          out.push_back('-');
#endif
          break;

        case '\\':
          if (!(flags&FNF_NOESCAPE)) {
            if (!p[1]) {
              continue; // -> no match
            }
            ch=*++p;
          }
        default:
          if (_re_must_escape(ch)) {
            out.push_back('\\');
          }
          out.push_back(ch);
#ifdef PARSE_RANGE
          from=ch;
#endif
          break;
        }
      }
      out.push_back(']');
      break;
    }

    case '\\':
      ch=*p;
    default:
      if (_re_must_escape(ch)) {
        out.push_back('\\');
      }
      out.push_back(ch);
      break;
    }
  }
  out.push_back('$');
  return out;
}

