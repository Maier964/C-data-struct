#include "common.h"
#include "stdint.h"

// This seed variable will be incremented to ensure a certain level of "randomness"
// for the shift register
static char gDefaultSeed = 'a';

void Swap(int* Value1, int* Value2)
{
    if (NULL == Value1 || NULL == Value2) 
    {
        return;
    }

    int aux;

    aux = *Value1;
    *Value1 = *Value2;
    *Value2 = aux;
}


// Pseudo-random number generator
// Needs a seed character
int RandomNumberGenerator(int MinVal, int MaxVal, char Seed)
{
    if (MinVal > MaxVal)
    {
        LOG_ERROR(" Bad parameters on random generator ( %d and %d )", MinVal, MaxVal);
    }

    int result = 0;

    uint16_t val = 0xD011u;
    
    // Linear Feedback shit register on 16 bits (Xorshift method)
    uint16_t random = val ^ Seed;

    do
    {
        random ^= random >> 7;
        random ^= random << 9;
        random ^= random >> 13;
        ++result;
    } while (random != val);


    return (result % ( MaxVal - MinVal + 1 )) + MinVal;
}

void QuickSort(int* Memory, int LeftBorder, int RightBorder)
{

    if (LeftBorder < RightBorder)
    {
        int pivot = Partition(Memory, LeftBorder, RightBorder);

        QuickSort(Memory, LeftBorder, pivot - 1);

        QuickSort(Memory, pivot + 1, RightBorder);
    }

}

int Partition( int* Memory, int Left, int Right) 
{
   
    int piv, i, j, rand, aux;

    rand = RandomNumberGenerator(Left, Right, gDefaultSeed);

    piv = Memory[rand];

    gDefaultSeed += '1';

    aux = piv;

    piv = Memory[Left];

    Memory[Left] = piv;

    i = Left;
    j = Right;

    while (i < j)
    {
        i++;
        while (piv < Memory[i])
        {
            i++;
        }

        while (j >= Left && piv > Memory[j])
        {
            j--;
        }

        if (i < j && i <= Right)
        {
            Swap(&Memory[i], &Memory[j]);
        }
    }


    Swap(&Memory[Left], &Memory[j]);

    return j;
}


int Max( int a, int b ){ return ( (a) > (b) ) ? a : b; }

void CustomMemcpy( void* Destination, void* Source, size_t Count )
{
    for( size_t i = 0; i < Count; i++ )
    {
        ((int*)Destination)[i] = ((int*)Source)[i];
    }
}

int GetStringLen( char* Str )
{
    int len = 0;

    while( Str[len] != '\0' )
    {
        len++;
    }

    return len;
}

int CustomStrCmp( char*s1, char*s2 )
{
    const unsigned char *p1 = ( const unsigned char * )s1;
    const unsigned char *p2 = ( const unsigned char * )s2;

    while ( *p1 && *p1 == *p2 ) ++p1, ++p2;

    return ( *p1 > *p2 ) - ( *p2  > *p1 );
}


int CustomIntMemSet( int* Source, int Value, size_t Count )
{

    if ( NULL == Source )
    {
        return -1;
    }

    for( size_t i = 0; i < Count; i++ )
    {
        Source[i] = Value;
    }

    return 0;
}
