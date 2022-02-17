#pragma once

#include "stdlib.h"
#include "stdio.h"

#define CC_UNREFERENCED_PARAMETER(X) X

// Folosim logguri doar pentru erori
#define LOG_ERROR(format, ...) printf("ERROR:" "[" __TIME__ "] " __FILE__ "] [" __FUNCTION__ ":" "%d" "]: " format "\n", __LINE__, __VA_ARGS__);

// Template quicksort
void QuickSort( int* Memory, int LeftBorder, int RightBorder );


// Used in QuickSort
// It is advised to use a random partition here, but unfortunaly, I don't think we can use srand. 
// For now, use first element as pivot
// I will do my own random number generator
int Partition( int* Memory, int Left, int Right);


// Template memory interchange
void Swap( int* Value1, int* Value2 );

int RandomNumberGenerator(int MinVal, int MaxVal, char* Seed, int SeedLength);