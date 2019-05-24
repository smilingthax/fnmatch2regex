#ifndef FNMATCH_H_
#define FNMATCH_H_

#ifdef __cplusplus
extern "C" {
#endif

#define FNM_NOESCAPE (1<<0)
#define FNM_PATHNAME (1<<1)
#define FNM_PERIOD (1<<2)
#define FNM_CASEFOLD (1<<4)

#define FNM_NOMATCH 1

int fnmatch(const char *pattern, const char *string, int flags);

#ifdef __cplusplus
}
#endif

#endif
