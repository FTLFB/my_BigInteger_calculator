# my_BigInteger_calculator
here you may find lab1, lab2 from course "спецiальнi роздiли обчислювальноi математики", but I advice you to use GMP

BigInt calculator for numbers 1-2048 bit legth.
It uses base-2^32 positional numeral system (each digit of BigInt contains values from 0 to ((2^32)-1), 
for example in base-10 pns each digit contains values from 0 to 9)

BigInt is array of 64 unsigned integers (32 bits) 

   [63]         [62]          ...          [2]       [1]        [0]      array that contains BigInt 
	
2047-2016    2015-1984        ...         95-64     63-32      31-0      bits positions of BigInt 





1) переведення малих констант у формат великого числа (зокрема, 0 та 1);
2) додавання чисел;
3) віднімання чисел;
4) множення чисел, піднесення чисел до квадрату;
5) ділення чисел, знаходження остачі від ділення;
6) піднесення числа до багаторозрядного степеня;
7) конвертування (переведення) числа в символьну строку та обернене перетворення
символьної строки у число; обов’язкова підтримка шістнадцяткового представлення, бажана –
десяткового та двійкового.
8) обчислення НСД та НСК двох чисел;
9) додавання чисел за модулем;
10) віднімання чисел за модулем;
11) множення чисел та піднесення чисел до квадрату за модулем;
12) піднесення числа до багаторозрядного степеня d по модулю n .









In this code next functions are present (most func are given w/o parametrs) : 
setNull() // BigInt assignment to 0

outputHexBI() // shows BI in console in hexadecimal

hexStrToDecInt() // frankly, it is not "to Decimal Integer" but anyway, it gets string variable with BigInt in hex, and writes it to array or 32bit unsigned integers

comparison() // accepts two BigInts and compares them, return 0, 1, -1 as result

addition(BigInt Res, BigInt A, BigInt B) // Res = A + B

substraction() // Res = A - B if A >= B; otherwise Res = B - A

setValue() // assignment "operator"

smallMul(BigInt Res, BigInt A, int b) // Res = A * b

longShiftToHigh() // shifts left digits of BigInt (one digit of BigInt contains 32 bit)

multiplication() // Res = A * B                                                    (((mod 2048 bit)))

powSquare() // Res = A * A

getBitLength() // returns amount of bits of given BigInt

getBit() // returns bit which is setted in the certain position

powBI() // Res = A ^ B                                                             (((mod 2048 bit)))

shiftRight() 

shiftLeft()

shiftBitLeftK()

gcd() // greatest common divisor

insertBit() 

divisionBImodQ() 

shift()

meow()

redBurito() // Barett modular reduction

powModBI(Res, A, B, N) // Res = A^B mod N

NSKoplimpiyskiy() // least common multiple of A,B

additionModN() // Res = A+B mod N

substractionModN() // Res = A - B mod N

multiplicationModN() // Res = A * B mod N  			



