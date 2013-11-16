/*
MSVC does not include the isblank function in versions older than VS2013.
This is required in the load_pat.cpp file. To ensure compilation on older MSVC versions this file is added for compatibility.
This file should be force included using compiler directives.
*/

#if _MSC_VER < 1800
static inline int isblank(int ch)
{
    return ch == ' ' || ch == '\t';
}
#endif