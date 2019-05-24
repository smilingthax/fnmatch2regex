# fnmatch(3) to regex converter

* e.g. for C++11 <regex>
* `win/fnmatch.h` can be used as drop-in replacement on systems without fnmatch support
* the transformation is utf-8 safe (but see below), i.e. to get correct results with utf-8, use a regex engine which supports utf8

Known issues:

* Reverse ranges must be filtered out (at least for `libstdc++`), but this uses a non-utf8-safe hack for now...
* Older `libstdc++` (e.g. version 5) does not recognize that the dash in `[a\-b]` is escaped...

Copyright (c) 2019 Tobias Hoffmann

License: http://opensource.org/licenses/MIT

