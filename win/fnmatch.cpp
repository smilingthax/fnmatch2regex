#include "fnmatch.h"
#include "../fnmatch2regex.h"
#include <regex>

int fnmatch(const char *pattern, const char *string, int flags)
{
  fnflags_t fl =
    ((flags&FNM_NOESCAPE) ? FNF_NOESCAPE : 0) |
    ((flags&FNM_PATHNAME) ? FNF_PATHNAME : 0) |
    ((flags&FNM_PERIOD)   ? FNF_PERIOD   : 0);
  std::string restr = fnmatch2regexp(pat, fl);

  using std::regex_constants;
  std::regex re(restr, nosubs | ((flags&FNM_CASEFOLD) ? icase));
  if (!std::regex_search(string, re)) {
    return FNM_NOMATCH;
  }
  return 0;
}

