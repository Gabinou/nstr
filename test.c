#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <string.h>

#include "nstr.h"

/* MINCTEST - Minimal C Test Library - 0.2.0
*  ---------> MODIFIED FOR TNECS <----------
* Copyright (c) 2014-2017 Lewis Van Winkle
*
* http://CodePlea.com
*
* This software is provided 'as-is', without any express or implied
* warranty. In no event will the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented; you must not
*    claim that you wrote the original software. If you use this software
*    in a product, an acknowledgement in the product documentation would be
*    appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
*    misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

#ifndef __MINCTEST_H__
#define __MINCTEST_H__
/* --- TEST GLOBALS --- */
FILE * globalf;
/* NB all tests should be in one file. */
static int ltests = 0;
static int lfails = 0;

/* Display the test results. */
#define lresults() do {\
    if (lfails == 0) {\
        dupprintf(globalf,"ALL TESTS PASSED (%d/%d)\n", ltests, ltests);\
    } else {\
        dupprintf(globalf,"SOME TESTS FAILED (%d/%d)\n", ltests-lfails, ltests);\
    }\
} while (0)

/* Run a test. Name can be any string to print out, test is the function name to call. */
#define lrun(name, test) do {\
    const int ts = ltests;\
    const int fs = lfails;\
    const clock_t start = clock();\
    dupprintf(globalf,"\t%-14s", name);\
    test();\
    dupprintf(globalf,"pass:%2d   fail:%2d   %4dms\n",\
            (ltests-ts)-(lfails-fs), lfails-fs,\
            (int)((clock() - start) * 1000 / CLOCKS_PER_SEC));\
} while (0)

/* Assert a true statement. */
#define lok(test) do {\
    ++ltests;\
    if (!(test)) {\
        ++lfails;\
        dupprintf(globalf,"%s:%d error \n", __FILE__, __LINE__);\
    }} while (0)

#endif /*__MINCTEST_H__*/

void dupprintf(FILE * f, char const * fmt, ...) { // duplicate printf
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
    va_start(ap, fmt);
    vfprintf(f, fmt, ap);
    va_end(ap);
}

#define DEFAULT_BUFFER_SIZE 108

int test_nstr() {
    printf("test_nstr");

    char * temp_str = (char *)malloc(DEFAULT_BUFFER_SIZE);
    strncpy(temp_str, "Hello_Anna", DEFAULT_BUFFER_SIZE);
    lok(strcmp(str_replaceSingle(temp_str, '_', ' '), "Hello Anna") == 0);
    strncpy(temp_str, "Hello=Anna", DEFAULT_BUFFER_SIZE);
    lok(strcmp(str_replaceSingle(temp_str, '=', ' '), "Hello Anna") == 0);
    strncpy(temp_str, "Hello Anna", DEFAULT_BUFFER_SIZE);
    lok(strcmp(str_replaceSingle(temp_str, ' ', '_'), "Hello_Anna") == 0);

    strncpy(temp_str, "Hello_Anna", DEFAULT_BUFFER_SIZE);
    lok(strcmp(str_toLower(temp_str), "hello_anna") == 0);
    strncpy(temp_str, "My Mama Told Me So", DEFAULT_BUFFER_SIZE);
    lok(strcmp(str_toLower(temp_str), "my mama told me so") == 0);
    strncpy(temp_str, "My Mama Told Me So", DEFAULT_BUFFER_SIZE);
    lok(strcmp(str_toUpper(temp_str), "MY MAMA TOLD ME SO") == 0);

    strncpy(temp_str, "my mama told me so", DEFAULT_BUFFER_SIZE);
    lok(strcmp(str_camelCase(temp_str, ' ', 1), "My Mama Told Me So") == 0);
    strncpy(temp_str, "my mama told me so", DEFAULT_BUFFER_SIZE);
    lok(strcmp(str_camelCase(temp_str, ' ', 2), "my Mama Told me so") == 0);
    strncpy(temp_str, "my mama told me so", DEFAULT_BUFFER_SIZE);
    lok(strcmp(str_camelCase(temp_str, '_', 3), "My mama told me so") == 0);

    strncpy(temp_str, "This is a beautiful day", DEFAULT_BUFFER_SIZE);
    lok(strcmp("is a beautiful day", str_slicefromStart(temp_str, 5)) == 0);
    strncpy(temp_str, "This is a beautiful day", DEFAULT_BUFFER_SIZE);
    lok(strcmp("This is a beautifu", str_slicefromEnd(temp_str, 5)) == 0);
}

int main() {
    globalf = fopen("nstr_test_results.txt", "w+");
    dupprintf(globalf, "\nHello, World! I am testing nstr.\n");
    lrun("test_nstr", test_nstr);
    lresults();

    dupprintf(globalf, "tnecs Test End \n \n");
    fclose(globalf);
    return (0);
}