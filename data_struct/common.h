#pragma once

#include "stdlib.h"
#include "stdio.h"

#define CC_UNREFERENCED_PARAMETER(X) X

#define STR(x) #x


// Folosim logguri doar pentru erori
#define LOG_ERROR(format, ...) printf("ERROR:" "[" __TIME__ "] " __FILE__ "] [" __FUNCTION__ ":" "%d" "]: " format "\n", __LINE__, __VA_ARGS__);
