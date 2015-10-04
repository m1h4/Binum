/*
 * Arbitrary Precision Integer Arithmetics Library
 * Copyright (C) Marko Mihovilic 2007.
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */

#include "Binum.h"

/*long _ftol2(double);*/
/*long _ftol2_sse(double x) { return _ftol2(x); }*/

#ifndef _DEBUG
void __fastcall __security_check_cookie(uintptr_t _StackCookie) {}
uintptr_t __security_cookie;

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
    return TRUE;
}
#endif

ULONG BINUMAPI BinumPrecision(ULONG precision)
{
	static ULONG binumPrecision = 512 / 8 / sizeof(BIWORD);	/* Default 512 bit precision */

	if(precision)
		binumPrecision = precision;

	return binumPrecision;
}

BOOL BINUMAPI BinumNew(LPBINUMBER number)
{
	*number = (BINUMBER)HeapAlloc(GetProcessHeap(),0,BinumPrecision(0) << 1);

	return *number != NULL;
}

BOOL BINUMAPI BinumDuplicate(LPBINUMBER number,CONST BINUMBER other)
{
	*number = (BINUMBER)HeapAlloc(GetProcessHeap(),0,BinumPrecision(0) << 1);

	if(*number)
		BinumCopy(*number,other);

	return *number != NULL;
}

VOID BINUMAPI BinumDelete(LPBINUMBER number)
{
	HeapFree(GetProcessHeap(),0,*number);

	*number = NULL;
}

VOID BINUMAPI BinumCopy(BINUMBER result,CONST BINUMBER other)
{
	//CopyMemory(result,other,BinumPrecision(0) << 1);

	//ULONG i = BinumPrecision(0);

	//while(i--)
	//	result[i] = other[i];

	__asm
	{
		push	0
		call	BinumPrecision
		add		esp, 4
		mov		ecx, eax
		//shr	ecx, 1	// Not used since eax is the number of elements
		mov		esi, other
		mov		edi, result
		rep		movsw
	}
}

VOID BINUMAPI BinumZero(BINUMBER number)
{
	//ZeroMemory(number,BinumPrecision(0) << 1);

	//ULONG i = BinumPrecision(0);

	//while(i--)
	//	number[i] = 0;

	__asm
	{
		push	0
		call	BinumPrecision
		add		esp, 4
		mov		ecx, eax
		//shr	ecx, 1	// Not used since eax is the number of elements
		xor		eax, eax
		mov		edi, number
		rep		stosw
	}
}

BOOL BINUMAPI BinumIsZero(CONST BINUMBER number)
{
	//ULONG i;

	//for(i = 0; i < BinumPrecision(0); ++i)
	//	if(number[i])
	//		return FALSE;

	__asm
	{
		push	0
		call	BinumPrecision
		add		esp, 4
		mov		esi, number
LLOOP:
		cmp		dword ptr [esi], 0
		jne		LEXITF
		dec		eax
		inc		esi
		test	eax, eax
		je		LEXITT
		jmp		LLOOP
LEXITT:
		inc		eax
		jmp		LEXIT
LEXITF:
		xor		eax, eax
LEXIT:
	}

	//return TRUE;
}

VOID BINUMAPI BinumAdd(BINUMBER result,CONST BINUMBER left,CONST BINUMBER right)
{
	ULONG i;
	BIDWORD carry = 0;

	for(i = 0; i < BinumPrecision(0); ++i)
	{
		carry = left[i] + right[i] + carry;

		result[i] = (BIWORD)(carry & (BIWORD)(-1));

		if(carry > (BIWORD)(-1))
			carry = 1;
		else
			carry = 0;
	}
}

VOID BINUMAPI BinumSubstract(BINUMBER result,CONST BINUMBER left,CONST BINUMBER right)
{
	ULONG i;
	BIDWORD borrow = 0;

	for(i = 0; i < BinumPrecision(0); ++i)
	{
		borrow = ((BIDWORD)(BIWORD)(-1) + (BIDWORD)1) + (BIDWORD)left[i] - (BIDWORD)right[i] - borrow;

		result[i] = (BIWORD)(borrow & (BIWORD)(-1));

		if(borrow <= (BIWORD)(-1))
			borrow = 1;
		else
			borrow = 0;
	}
}

VOID BINUMAPI BinumMultiplySmall(BINUMBER result,CONST BINUMBER left,BIWORD right)
{
	ULONG i;
	BIDWORD carry = 0;

	for(i = 0; i < BinumPrecision(0); ++i)
	{
		carry = (BIDWORD)left[i] * (BIDWORD)right + carry;

		result[i] = (BIWORD)(carry & (BIWORD)(-1));

		carry >>= sizeof(BIWORD) << 3;
	}
}

VOID BINUMAPI BinumDivideSmall(BINUMBER result,CONST BINUMBER left,BIWORD right)
{
	ULONG i;
	BIDWORD dividend = 0;

	for(i = BinumPrecision(0) - 1; i < -1; --i)
	{
		dividend |= (BIDWORD)left[i];
		result[i] = (BIWORD)(dividend / right);
		dividend = (dividend % (BIDWORD)right) << (sizeof(BIWORD) << 3);
	}
}

VOID BINUMAPI BinumModulusSmall(BINUMBER result,CONST BINUMBER left,BIWORD right)
{
	ULONG i;
	ULONG bit;
	BIDWORD power = 1;

	BinumZero(result);

    for(i = 0; i < BinumPrecision(0); ++i)
	{
		for(bit = 0; bit < (sizeof(BIWORD) << 3); ++bit)
		{
            if((left[i] & (1 << bit)))
			{
				result[0] += (BIWORD)power;
                if(result[0] >= right)
                    result[0] -= right;
            }

            power <<= 1;

            if(power >= right)
				power -= right;
        }
    }
}

VOID BINUMAPI BinumMultiply(BINUMBER result,CONST BINUMBER left,CONST BINUMBER right)
{
	ULONG i;
	BINUMBER partial;

	BinumNew(&partial);
	BinumZero(result);

	for(i = 0; i < BinumPrecision(0); ++i)
	{
		BinumMultiplySmall(partial,left,right[i]);
		BinumShiftLeft(partial,partial,(sizeof(BIWORD) << 3) * i);
		BinumAdd(result,result,partial);
	}

	BinumDelete(&partial);
}

VOID BINUMAPI BinumDivide(BINUMBER result,CONST BINUMBER left,CONST BINUMBER right)
{
}

VOID BINUMAPI BinumShiftLeftSingle(BINUMBER result,CONST BINUMBER number)
{
	ULONG i;

	for(i = BinumPrecision(0) - 1; i > 0; --i)
	{
		result[i] = number[i] << 1;
		result[i] |= number[i - 1] >> ((sizeof(BIWORD) << 3) - 1);
	}

	result[i] = number[i] << 1;
}

VOID BINUMAPI BinumShiftRightSingle(BINUMBER result,CONST BINUMBER number)
{
	ULONG i;

	for(i = 0; i < BinumPrecision(0) - 1; ++i)
	{
		result[i] = number[i] >> 1;
		result[i] |= number[i + 1] << ((sizeof(BIWORD) << 3) - 1);
	}

	result[i] = number[i] >> 1;
}

VOID BINUMAPI BinumShiftLeft(BINUMBER result,CONST BINUMBER number,ULONG bits)
{
	BinumCopy(result,number);

	while(bits--)
		BinumShiftLeftSingle(result,result);
}

VOID BINUMAPI BinumShiftRight(BINUMBER result,CONST BINUMBER number,ULONG bits)
{
	BinumCopy(result,number);

	while(bits--)
		BinumShiftRightSingle(result,result);
}

BOOL BINUMAPI BinumSet(BINUMBER result,LPCSTR number)
{
	BINUMBER temp;

	BinumNew(&temp);
	BinumZero(result);

	while(*number)
	{
		BinumMultiplySmall(result,result,10);

		BinumZero(temp);
		temp[0] = *number++ - '0';	/* TODO Should do a sanity check before doing hacks like this */

		BinumAdd(result,result,temp);
	}

	BinumDelete(&temp);

	return TRUE;
}

VOID ReverseString(LPSTR string)
{
	LPSTR end = string;

	/* Have to do some sanity checking, eck */
	if(!*end)
		return;

	while(*++end);

	--end;

	while(end - string > 0)
	{
		*string ^= *end ^= *string ^= *end;

		++string;
		--end;
	}
}

BOOL BINUMAPI BinumGet(LPSTR result,CONST BINUMBER number)
{
	BINUMBER temp;
	BINUMBER modulus;
	LPSTR reverse = result;	/* Need to remember the start of the string for later reversing */

	BinumNew(&modulus);
	BinumDuplicate(&temp,number);

	do
	{
		BinumModulusSmall(modulus,temp,10);

		*result++ = (CHAR)(modulus[0] + '0');

		BinumDivideSmall(temp,temp,10);
	}
	while(!BinumIsZero(temp));

	BinumDelete(&modulus);
	BinumDelete(&temp);

	*result = '\0';

	ReverseString(reverse);	/* TODO It's ugly, I know */

	return TRUE;
}