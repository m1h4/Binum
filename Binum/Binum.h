#pragma once

#include <windows.h>

/* Represents the basic number entity */
typedef unsigned short BIWORD,*LPBIWORD;
typedef unsigned long BIDWORD,*LPBIDWORD;
typedef LPBIWORD BINUMBER,*LPBINUMBER;

#ifdef BINUM_EXPORTS
#define BINUMAPI __declspec(dllexport)
#else
#define BINUMAPI __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/* Sets the precision of the library in multiples of the word size */
/* Valid to call only when there are no instances of integers */
ULONG BINUMAPI BinumPrecision(ULONG precision);

/* Creates a new instance of a number */
BOOL BINUMAPI BinumNew(LPBINUMBER number);
/* Creates a new instance of a number from a existing number */
BOOL BINUMAPI BinumDuplicate(LPBINUMBER number,CONST BINUMBER other);
/* Deletes a instance of a existing number, automatically terminates the pointer */
VOID BINUMAPI BinumDelete(LPBINUMBER number);

/* Copies the value of one number to the other */
VOID BINUMAPI BinumCopy(BINUMBER result,CONST BINUMBER other);
/* Sets the value of a number to zero */
VOID BINUMAPI BinumZero(BINUMBER number);
/* Tests the value of a number for equality with zero */
BOOL BINUMAPI BinumIsZero(CONST BINUMBER number);

/* Adds one number to the other storing the result in a third */
VOID BINUMAPI BinumAdd(BINUMBER result,CONST BINUMBER left,CONST BINUMBER right);
/* Substracts one number from the other storing the result in a third */
VOID BINUMAPI BinumSubstract(BINUMBER result,CONST BINUMBER left,CONST BINUMBER right);

/* Multiplies a number with a nother number of the word size storing the result */
VOID BINUMAPI BinumMultiplySmall(BINUMBER result,CONST BINUMBER left,BIWORD right);
/* Divides a number with a nother number of the word size storing the result */
VOID BINUMAPI BinumDivideSmall(BINUMBER result,CONST BINUMBER left,BIWORD right);
/* Computes the modulus of a integer division of the word size storing the result, overlap is not permitted */
VOID BINUMAPI BinumModulusSmall(BINUMBER result,CONST BINUMBER left,BIWORD right);

/* Multiplies a number with a nother number storing the result, overlap is not permitted */
VOID BINUMAPI BinumMultiply(BINUMBER result,CONST BINUMBER left,CONST BINUMBER right);
/* Divides a number with a nother number storing the result */
VOID BINUMAPI BinumDivide(BINUMBER result,CONST BINUMBER left,CONST BINUMBER right);

/* Shifts the bits of a number to the left by one */
VOID BINUMAPI BinumShiftLeftSingle(BINUMBER result,CONST BINUMBER number);
/* Shifts the bits of a number to the right by one */
VOID BINUMAPI BinumShiftRightSingle(BINUMBER result,CONST BINUMBER number);

/* Shifts the bits of a number to the left by the specified count */
VOID BINUMAPI BinumShiftLeft(BINUMBER result,CONST BINUMBER number,ULONG bits);
/* Shifts the bits of a number to the right by the specified count */
VOID BINUMAPI BinumShiftRight(BINUMBER result,CONST BINUMBER number,ULONG bits);

/* Costructs a number from the specified string, the number must be instanced */
BOOL BINUMAPI BinumSet(BINUMBER result,LPCSTR number);
/* Costructs a string from the specified number, the string must have sufficient storage space to accommodate the number */
BOOL BINUMAPI BinumGet(LPSTR result,CONST BINUMBER number);

#ifdef __cplusplus
}
#endif