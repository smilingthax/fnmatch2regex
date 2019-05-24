#ifndef FNMATCH2REGEX_H_
#define FNMATCH2REGEX_H_

#include <string>

enum fnflags_t {
  FNF_NONE = 0,
  FNF_NOESCAPE = 0x01,
  FNF_PATHNAME = 0x02,
  FNF_PERIOD = 0x04
};
static inline fnflags_t operator|(fnflags_t a, fnflags_t b) { return (fnflags_t)((int)a|b); }

std::string fnmatch2regex(const char *pattern, fnflags_t flags);

#endif
