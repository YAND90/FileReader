#include "wildcard.h"

bool wildcard::match(const char* string, const char* pattern) {
    const char* pString = nullptr;
    const char* pPattern = nullptr;

    bool escape = false;

    while (*string != '\0') {
        if (!escape && (*string == *pattern || *pattern == '?')) {
            string++;
            pattern++;
            escape = false;
        }
        else if (!escape && *pattern == '*') {
            pString = string;
            pPattern = pattern;
            pattern++;
            escape = false;
        }
        else if (*pattern == '\\' && !escape) {
            pattern++;
            escape = true;
        }
        else if (pPattern != nullptr) {
            string = ++pString;
            pattern = pPattern + 1;
            escape = false;
        }
        else {
            return false;
        }
    }

    while (*pattern == '*') {
        pattern++;
    }

    return *pattern == '\0';
}
