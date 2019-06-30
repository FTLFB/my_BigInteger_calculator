#include <iostream>
#include <string>
#include "math.h"
#include <algorithm>
#include <stdlib.h>
#include <iomanip>

using namespace std;

//BigInteger arythmetics for numbers 0-2048 bits length

//positional numeral system that is used in this code is base-2^32 (that means that each digit contains values from 0 to 2^32)
//note that it isn't decimal, hexadecimal or any other!


void setNull(uint32_t* op)
{
    for(int i = 0; i < 64; i++)
    {
        op[i] = 0;
    }
}

void outputHexBI(uint32_t* masBI)//shows BigInt in console
{
    int notNullPos = 0;
    while(masBI[notNullPos] == 0)
    {
        ++notNullPos;
        if(notNullPos == 64)
        {
            cout << 0 << endl ;
            return;
        }
    }
    for(int i = notNullPos; i < 64; ++i)
    {

        cout << hex <<  masBI[i];
    }
    cout << endl << endl;
}

void hexStrToDecInt(string input, uint32_t* masBI)//as we get number in hexadecimal numeral system (that is written in string) -> 
// ->we transform it into base-2^32 positional numeral system
{
     string temp = "00000000";
    int sizeOfBI = 63;
    int sizeTemp = 7;


    for (int i = input.length() - 1; i > -1; i--)
    {
        if( sizeTemp > 0)
        {
            temp[sizeTemp] = input[i];
            sizeTemp--;
            masBI[sizeOfBI] = strtoul(temp.c_str(), nullptr, 16);
        }
        else
        {
            temp[sizeTemp] = input[i];
            masBI[sizeOfBI] = strtoul(temp.c_str(), nullptr, 16);
            sizeOfBI--;
            sizeTemp = 7;
            temp = "00000000";
        }


    }
}

int comparison(uint32_t* operand1, uint32_t* operand2)// if op1 = op2 -> return 0; if op1 > op2 -> return 1; if op1 < op2 -> return -1.
{
    int iter = 0;
    while (operand1[iter] == operand2[iter])
    {
        iter++;
        if(iter == 64)
        {
            return 0;
        }
    }
    if(operand1[iter] > operand2[iter])
    {
        return 1;
    }
    else
    {
        return -1;
    }

}

void addition(uint32_t* resultBI, uint32_t* operand1, uint32_t* operand2)
{
    uint64_t temp, a, b, carry = 0;
    for(int i = 63; i> -1; i--)
    {
        a = operand1[i];
        b = operand2[i];
        temp = a +b + carry;//performed simle digit-by-digit sum of 2 numbers, with carry bit
        resultBI[i] = temp & 0xFFFFFFFFUL;//resultBI is 32 bits length, temp is 64, so we need this '&' operation to avoid overflow
        carry = temp >> 32;//now temp is 1 or 0
    }

}

void substraction(uint32_t* result, uint32_t* operand1, uint32_t* operand2)
{

    int cmpRes = comparison(operand1, operand2);
    int64_t r = 0, temp = 0;
    uint32_t borrow = 0;
    if(cmpRes == 0)
    {
        for(int i = 0; i < 64; i++)
        {
            result[i] = 0;
        }
    }
    else if(cmpRes == 1 )
    {
     for(int i = 63; i > -1; i--)
     {
         temp = (uint64_t)operand1[i] - operand2[i] - borrow;
         if(temp >= 0)
         {
             r = temp;
             borrow = 0;
         }
         else
         {
             r = temp + 4294967296UL;
             borrow = 1;
         }
         result[i] = r;
     }
    }
    else if( cmpRes == -1)
    {
          for(int i = 63; i > -1; i--)
     {
         temp = (uint64_t)operand2[i] - operand1[i] - borrow;
         if(temp >= 0)
         {
             r = temp;
             borrow = 0;
         }
         else
         {
             r = temp + 4294967296UL;
             borrow = 1;
         }
         result[i] = r;
     }
    }
    else
    {
        cout << "COMPARISON ERROR!" << endl;
    }
}

void setValue(uint32_t* operand1, uint32_t* operand2)//assignment "operator"
{
    for(int i = 0; i < 64; i++)
    {
        operand1[i] = operand2[i];
    }
}

void smallMul(uint32_t* opBI, uint32_t* operand1, uint32_t op)
{
    uint64_t temp, t, c = op;
    uint64_t carry = 0;

    for(int i = 63; i > -1; i--)
    {
        t = operand1[i];
        temp = t*c + carry;
        opBI[i] = temp & 0xFFFFFFFFUL;
        carry = temp >> 32;
    }

}

void longShiftToHigh(uint32_t* opBI, int shift)
{
    uint32_t newBI[64] = {};
    for(int i = 0; i < 64-shift; i++)
    {
        newBI[i] = opBI[i+shift];
    }
    for(int i = 0; i < 64; i++)
    {
        opBI[i] = newBI[i];
    }
}

void multiplication(uint32_t* result, uint32_t* operand1, uint32_t* operand2)
{


    for(int i = 63; i > -1; i--)
    {
        uint32_t tempOpBI[64] = {};
        smallMul(tempOpBI,operand1, operand2[i]);
        longShiftToHigh(tempOpBI, 63-i);
        addition(result, result, tempOpBI);

    }

}

void powSquare(uint32_t* result, uint32_t* operand1)// do not pass the same BigInt as parametrs (result != operandBI), otherwise it would crash (:
{
    multiplication(result, operand1, operand1);
}

int getBitLength(uint32_t* opBI)
{
    uint32_t temp = 0;

    int notEmptyPos = 0;
    int bitLen = 0;

    for ( notEmptyPos; notEmptyPos < 64; notEmptyPos++)
    {
        if(opBI[notEmptyPos] != 0)
        {
            break;
        }
    }

    temp = opBI[notEmptyPos];

    for(int j = 31; j > -1; j--)
    {
        if( ( ( temp >> j ) & 1 ) == 1 )
        {
            j++;
            break;
        }
        bitLen = j;
    }
    bitLen += 32*(63 - notEmptyPos);

    return bitLen;
}

int getBit(uint32_t* opBI, int bitPos)
{
    int temp = getBitLength(opBI);
    int i = 0;
    i = 63 - bitPos/32;
    if( bitPos < temp)
    {
        temp = bitPos%32;
        return ((opBI[i] >> temp ) & 1);
    }
    return 2;
}

void powBI(uint32_t* result, uint32_t* operand1, uint32_t* operand2)
{
    for(int i =0; i< 64; i++)
    {
        result[i] = 0;
    }
    hexStrToDecInt("1", result);
    uint32_t kostil[64] ={};
    int t = 0;
    int i = getBitLength(operand2);

    for( i; i > -1; i--)
    {
        t = getBit(operand2, i);
        if(t == 1)
        {
            multiplication(result, result, operand1);

        }
        if(i != 0)
        {
            powSquare(kostil, result);
            setValue(result, kostil);
           setNull(kostil);
        }
    }
}

///////////////////////////////////////////laba2/////////////////////////////////////////////////////////////////////////////

void shiftRight(uint32_t* op)// /2
{

    for(int i = 63; i > 0; i--)
    {
       op[i]= (op[i]>>1)| (op[i-1] << 31);
    }
    op[0]=op[0]>>1;

}

void shiftLeft(uint32_t* op)// *2 mod 2048bit
{
    for(int i = 0; i < 63; i++)
    {
        op[i] = ((op[i] << 1) | (op[i+1] >> 31));
    }
    op[63] = op[63] << 1;
}

void shiftBitLeftK(uint32_t* op, int k)
{
    while(k!=0)
    {
       for(int i = 0; i < 63; i++)
    {
        op[i] = ((op[i] << 1) | (op[i+1] >> 31));
    }
    op[63] = op[63] << 1;
    k--;
    }

}
void gcd(uint32_t* result, uint32_t* operand1, uint32_t* operand2)//gcd from "Handbook of applied cryptography 14.4.1"
{
    uint32_t op1Temp[64] ={};
    uint32_t op2Temp[64] ={};
    setValue(op1Temp, operand1);
    setValue(op2Temp, operand2);

    uint32_t testZero[64] ={};
    uint32_t t[64] = {};

    int whoIsBigger = 0;

    uint32_t gcd[64] = {};
    gcd[63] = 1;

    while( (comparison(op1Temp, testZero) && comparison(op2Temp, testZero)) && !(op1Temp[63] & 1 ) && !(op2Temp[63] & 1))
    {

        shiftRight(op1Temp);
        shiftRight(op2Temp);
        shiftLeft(gcd);
    }
    while( comparison(op1Temp,testZero))
    {
        while((comparison(op1Temp, testZero)) && (!(op1Temp[63] & 1 )) )
        {
            shiftRight(op1Temp);
        }
        while((comparison(op2Temp, testZero)) && (!(op2Temp[63] & 1)))
        {
            shiftRight(op2Temp);
        }
        whoIsBigger = comparison(op1Temp, op2Temp);
        substraction(t, op1Temp, op2Temp);
        shiftRight(t);
        if((whoIsBigger == 1) || (whoIsBigger == 0))
        {
            setValue(op1Temp, t);
        }
        else if(whoIsBigger == -1)
        {
            setValue(op2Temp, t);
        }
        else{cout << "YOU ARE IN BIG TROUBLE" << endl;}

    }
    multiplication(testZero, gcd, op2Temp);// testZero  = g*y
    setValue(result, testZero);
}

void insertBit(uint32_t* op, int bitPos) // remember : in BigInt we have bit positions from 0 to 2047
{
    uint32_t bit[64] = {};
    uint32_t b  = 1; 
    b = b << (bitPos%32); 
    bit[63 - bitPos/32] = b;

    uint32_t uncertainMoment[64] = {}; // not sure if  addition(A, A, B) is correct (:

    addition(uncertainMoment, op, bit);
    setValue(op, uncertainMoment);
}

void divisionBImodQ(uint32_t* result, uint32_t* operand1, uint32_t* operand2) // result = Q ; operand1 = R
{
    uint32_t R[64] ={};
    uint32_t Q[64] ={};
    uint32_t C[64] ={};

    uint32_t uncertainMoment[64] = {};
    int k = getBitLength(operand2);
    int temp = 0;
    setValue(R, operand1);
    while(comparison(R, operand2) != -1) // (comparison(R, operand2) == 1) || (comparison(R, operand2) == 0)
    {
        temp = getBitLength(R);
        setValue(C, operand2);
        shiftBitLeftK(C, temp - k);
        if(comparison(R, C) == -1)
        {
            temp--;
            setValue(C, operand2);
            shiftBitLeftK(C, temp - k);
        }
        substraction(uncertainMoment, R, C);
        setValue(R, uncertainMoment);

        insertBit(Q, temp-k);

    }
    setValue(result, Q);
}
void shift(uint32_t* op, int shift)//not cyclic shift
{
    uint32_t temp[64] = {};

    for(int i = 0; i+shift < 64; i++)
    {
        temp[i+shift] = op[i];
    }
    for(int i = 0; i < 64; i++)
    {
        op[i] = temp[i];
    }
}

void meow(uint32_t* op, uint32_t* n, int k) //greek letter "myu"
{
    uint32_t temp[64] = {};
    uint32_t uncertainMoment[64] = {};

    if(k == 32 )
    {
      temp[0] = 1;
    }
    else{    temp[63-2*k] = 1; }

    divisionBImodQ(uncertainMoment, temp, n);

    setValue(op, uncertainMoment);

}

void redBurito(uint32_t* op, uint32_t* modN, uint32_t* meowBI)//Barett modular reduction
{

    uint32_t Q[64] = {};
    uint32_t temp[64]= {};
    setValue(temp, op);
    setValue(Q, op);
    int len = 0;

    for(int i = 0; i < 64; i++)
    {
        if(modN[i]!=0)
        {
            len = 64 - i;
            break;
        }
    }

    shift(Q,(len-1));

    uint32_t uncertainMoment[64] = {};

    multiplication(uncertainMoment, Q, meowBI);
    setValue(Q, uncertainMoment);

    shift(Q, len+1);

    setNull(uncertainMoment);
    multiplication(uncertainMoment, Q, modN);
    setValue(Q, uncertainMoment);
    setNull(uncertainMoment);
    substraction(op, temp, Q);

    while(comparison(op, modN) != -1) // (comparison(op, modN) == 1) || (comparison(op, modN) == 0)
    {
        //cout << "point " ;//debug this for 1024
        substraction(uncertainMoment, op, modN);
        setValue(op, uncertainMoment);
        setNull(uncertainMoment);
    }

}

void powModBI(uint32_t* result, uint32_t* operand1, uint32_t* operand2, uint32_t* modN)
{
    uint32_t uncertainMoment[64] ={};
    uint32_t temp[64] = {};
    temp[63] = 1;
    setValue(result, temp);
    setNull(temp);


   int len = 0;
    for(int i = 0; i < 64; i++)
    {
        if(modN[i]!=0)
        {
            len = 64 - i;
            break;
        }
    }
    meow(temp, modN, len);
    len = getBitLength(operand2);
    for(int i = len-1; i > -1; i--)
    {
        if(getBit(operand2, i) == 1)
        {
            multiplication(uncertainMoment, result, operand1);
            setValue(result, uncertainMoment);
            setNull(uncertainMoment);
redBurito(result, modN, temp);

        }
        if(i != 0)
        {
            multiplication(uncertainMoment, result, result);

            setValue(result, uncertainMoment);
            setNull(uncertainMoment);

            redBurito(result, modN, temp);


        }
    }
}
void NSKolimpiyskiy(uint32_t* result, uint32_t* operand1, uint32_t* operand2)
{
     uint32_t uncertainMoment[64] = {};
    multiplication(uncertainMoment, operand1, operand2);
    uint32_t gcd1[64] ={};
    gcd(gcd1, operand1, operand2);
    divisionBImodQ(result, uncertainMoment, gcd1);
}

void additionModN(uint32_t* result, uint32_t* operand1, uint32_t* operand2, uint32_t* modN)
{
    int len = 0;
     for(int i = 0; i < 64; i++)
    {
        if(modN[i]!=0)
        {
            len = 64 - i;
            break;
        }
    }
    uint32_t mw[64] = {};
    meow(mw, modN, len);
    addition(result, operand1, operand2);
    redBurito(result, modN, mw);
}

void substractionModN(uint32_t* result, uint32_t* operand1, uint32_t* operand2, uint32_t* modN)
{
    int len = 0;
     for(int i = 0; i < 64; i++)
    {
        if(modN[i]!=0)
        {
            len = 64 - i;
            break;
        }
    }
    uint32_t mw[64] = {};
    meow(mw, modN, len);
    substraction(result, operand1, operand2);
    redBurito(result, modN, mw);
}

void multiplicationModN(uint32_t* result, uint32_t* operand1, uint32_t* operand2, uint32_t* modN)
{
    int len = 0;
     for(int i = 0; i < 64; i++)
    {
        if(modN[i]!=0)
        {
            len = 64 - i;
            break;
        }
    }
    uint32_t mw[64] = {};
    meow(mw, modN, len);
    multiplication(result, operand1, operand2);
    redBurito(result, modN, mw);
}


 int main()
{
    uint32_t masBI[64] = {};
    uint32_t masBI2[64] = {};
    uint32_t resultBI[64] = {};
    uint32_t modN[64] = {};


    string strNum = "A5DEACF574901CECC4F2DA11FD5229359E98CD65FAB60C4FC755E2859A7A9CD8C64087204417DD513A2094604351FF6D8C24ABED52B5B9FF263BAEE0F9C8ACEBEE80C2FC74991A700439461DA775F57719A7D7639928FB29566AC41F96BC735A";//"21E8D922A2A123DD";
    hexStrToDecInt(strNum, masBI);

    strNum = "AC7F97F90990D9CFAB65F61A78C548202DA142DA8A04B9A58EDCF89B753D2D1452462060E38E378D54A4F30243620F28788EFF23BFDC09CA083327D3680B120B87261643C22AFB63851D21234B57568FB5187C480AAB31C0E0E4B4AF88FFBF87";//"21E8D922A";
    hexStrToDecInt(strNum, masBI2);

    strNum = "2EEC5C84037ED4CC6A7758DC64702E474D3E854150DEE70A6932BF69165BD272BCF4907480AA2953D63CA8B938F78FA038D3BA4D85197EF4272790D5CE7D15163E23AE3AA5A00CB1AE0E61713C501CE689C7D8C2B6F397FBED6B5E4F8E29AFF2";
    hexStrToDecInt(strNum, modN);

    outputHexBI(masBI);
    outputHexBI(masBI2);
    outputHexBI(modN);
    outputHexBI(resultBI);

    //int res = comparison(masBI, masBI2);
    //cout << endl << "this is result of comparison : " << dec <<  res << endl;//why should we put 'dec' into stream for good output?

    //+
   // addition(resultBI, masBI, masBI2);
   // outputHexBI(resultBI);

    //setNull(resultBI);
    //outputHexBI(resultBI);

      //-
   // substraction(resultBI, masBI, masBI2);
   // outputHexBI(resultBI);

    //*
//    multiplication(resultBI, masBI, masBI2);
//    outputHexBI(resultBI);

    //^2
//    powSquare(resultBI, masBI);
//    outputHexBI(resultBI);

    //^
   // powBI(resultBI, masBI, masBI2);
   // outputHexBI(resultBI);

// (operand/2)
//     shiftRight(resultBI);
//     outputHexBI(resultBI);

    //* 2 mod 2048bit
//    shiftLeft(masBI2);

    //gcd
//    gcd(resultBI, masBI, masBI2);
//    outputHexBI(resultBI);

    //pow modN
//   powModBI(resultBI, masBI, masBI2, modN);
//    outputHexBI(resultBI);

    //NSK
//    NSKolimpiyskiy(resultBI, masBI, masBI2);
//    outputHexBI(resultBI);

    //+ modN
//    additionModN(resultBI, masBI, masBI2, modN);
//    outputHexBI(resultBI);

    //- modN
//    substractionModN(resultBI, masBI, masBI2, modN);
//    outputHexBI(resultBI);

    // * modN
    //multiplicationModN(resultBI, masBI, masBI2, modN);
   //outputHexBI(resultBI);


    cout << endl << endl;

    cout << "Aftermath check of the input values " << endl;
    outputHexBI(masBI);
    outputHexBI(masBI2);
    outputHexBI(modN);



    return 0;
}



