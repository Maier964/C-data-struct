#include "common.h"
#include "stdint.h"


// You can do a TEMPLATE SWAP here
void Swap(int* Value1, int* Value2)
{
    // void* aux = NULL;
    // aux = calloc(1,sizeof(Value1)); // Check for errors here

    int aux;

    aux = *Value1;
    *Value1 = *Value2;
    *Value2 = aux;
}


// Generator de numere pseudo-aleatoare
// Accepta un sir de caractere ca seed.
int RandomNumberGenerator(int MinVal, int MaxVal, char* Seed, int SeedLength)
{
    if (MinVal > MaxVal)
    {
        LOG_ERROR(" Bad parameters on random generator ( %d and %d )", MinVal, MaxVal);
    }

    int result = 0;

    // Orice valoare diferita de 0 este valida pentru generare
    uint16_t seed = 0xD011u;

    for ( int i = 0; i < SeedLength; i++ )
        seed ^=  (uint8_t)Seed[i];
    
    // Initializam un Linear Feedback shit register pe 16 biti cu metoda Xorshift
    uint16_t random = seed;

    do
    {
        random ^= random >> 7;
        random ^= random << 9;
        random ^= random >> 13;
        ++result;
    } while (random != seed);

    seed++;

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

int Partition( int* Memory, int Left, int Right) // nu stiu daca avem voie cu sizeof();
{
    // Take first element as pivot for now

    int piv, i, j;

    piv = Memory[Left];
    i = Left;
    j = Right;

    while (i < j)
    {
        i++;
        while (piv > Memory[i])
        {
            i++;
        }

        while (j >= Left && piv < Memory[j])
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