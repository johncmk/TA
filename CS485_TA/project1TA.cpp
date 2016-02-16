/*
 * Author: TA
 * Due Date: 11/23 Sunday midnight
 *
 * Note that there is no limit to the number of bits your program can handle. For convenience,
 * your program can assume it is for eight bit binary numbers. Also assume all inputs are valid
 */

#include<iostream>
#include<string>

/**
 * constant global variables for Two's complement
 */
const int lengthBinary = 8;
const char one = '1';
const char zero = '0';
const char *strOne = "1";
const char *strZero = "0";

/**
 * Prepend 0 or 1 to the binary number
 */
char* bitPrepend(const char* head, const char* tail) {
	char* merge = (char*)malloc(std::strlen(head) + std::strlen(tail)+1);
	std::strcpy(merge,head);
	std::strcat(merge,tail);
	return merge;
}

/**
 * Add 0 in front of binary until the length becomes 8
 */
char* lenEight(char* binary) { 
	return (std::strlen(binary) != lengthBinary)? lenEight(bitPrepend(strZero,binary)) : binary;
}

/**
 * convert from decimal to binary
 */
char* toBinary(int num, char* binary = "") {
	int div = num/2;
	if (div == 0) 
		return bitPrepend(strOne,binary);
	else if (num%2 == 0)
		return toBinary(div,bitPrepend(strZero,binary))
	return toBinary(div,bitPrepend(strOne,binary));
}

/**
 * convert the two's complement binary number to negation for negative decimal
 */
char* negation(char* binary, int i = 0) {
	if(i == lengthBinary)
		return binary;
	if(*(binary+i) == zero){
		*(binary+i) = one;
		return negation(binary,++i);
	}
	*(binary+i) = zero;
	return negation(binary,++i);
}

/**
 * OR Truth Table for summation of two numbers of twos complement binary
 * each result updates (carry,remain bit)
 */
const char* truthOR(char top, char bottom, int &carryout) {
	if (top == zero && bottom == zero) {//CASE 00 = 0,0
		if(carryout == 1) {//CASE 001 = 1,0
			carryout = 0;
			return strOne;
		}
		return strZero;
	}
	if(top == one && bottom == one) {//CASE 11 = 1,0
		if(carryout == 1)
			return strOne;//CASE 111 = 1,1
		if(carryout == 0) {
			carryout++;
			return strZero;
		}
	}
	if((top == zero && bottom == one)||(top = one && bottom == zero)) {//CASE 01 OR 10 = 0,1
		if(carryout == 1)
			return strZero;//CASE 011 OR 101 = 1,0
		return strOne;
	}
	return NULL;
}

/**
 * summation of two numbers of twos complement binary
 * if summation with 1bit it convert to 8bits, it becomes from 1 to 00000001
 */
char* summation(char *top,char *bottom = "1",char* binary = "", int size = lengthBinary,int carryout = 0){
	if(size == 0)
		return binary;
	if(std::strlen(bottom) < 8)
		return summation(top,lenEight(bottom),binary,size,carryout);
	binary = bitPrepend(truthOR(top[size-1],bottom[size-1],carryout),binary);
	return summation(top,bottom,binary,--size,carryout);
}

/**
 * convert from decimal to two's complement
 */
char* toTwosCom(const int num){
	if (num == 0)
		return "00000000";
	else if (num > 0)
		return lenEight(toBinary(num));
	return summation(negation(lenEight(toBinary(num))));
}

/**
 * power function (i.e. 2^4 = 16)
 */
int power(int num, int pwr){
	return (pwr == 0)? 1: num *= power(num,--pwr);
}

/**
 * TwosComToDec helper function to multiply 2 to the power if given bit is 1
 */
int _toDec(char* num,int ans = 0, int i = 0,int size = lengthBinary){
	if (i == size)
		return ans;
	else if (*(num+i)==one)
		return ans+= _toDec(num,power(2,size-i),++i)
	return _toDec(num,ans,++i);
}

/**
 * convert from twos complement to decimal
 */
int toDec(char* num){
	return (*num==zero)? _toDec(num) : -1*_toDec(summation(negation(num)));
}

/**
 * check the overflow after summation of two number of twos complement
 */
bool overflow(char*top, char*bottom, char* sum){
	return ((*top == zero && *bottom == zero && *sum == one)||
			(*top == one && *bottom == one && *sum == zero));
}

/**
 * check if the bottom number is negative, if so, then return with parenthesis
 */
std::string checkNegative(int num){
	return (num < 0)? "("+std::to_string(num)+")": std::to_string(num);
}

/**
 * Ask user for two number;
 * number should be between -128 to 127
 */
void inputVar(int &num1, int &num2){
	std::cout<<"1). Enter first decimal number to compute: ";
	std::cin >> num1;
	std::cout<<"2). Enter second decimal number to compute: ";
	std::cin >> num2;
	if((num1 <= 127 && num1 >= -128) && (num2 <= 127 && num2 >= -128))
		return;
	std::cout<<"*)try again\n";
	inputVar(num1,num2);
}

/**
 * convert both decimal to binary and apply binary summation
 */
void setVar(int num1, int num2, char *&top, char *&bottom,char *&sum){
	top = toTwosCom(num1);
	bottom = toTwosCom(num2);
	sum = summation(top,bottom);
}

/**
 * print out summation of two's complement
 */
void outputSummation(char *&top, char *&bottom, char *&sum){
	std::cout<<"top:              "<< top<<std::endl;
	std::cout<<"bottom:         + "<< bottom<<std::endl;
	std::cout<<"                 ----------"<<std::endl;
	std::cout<<"summation answer: "<< sum<<std::endl;
}

/**
 * output message if overflow else output number
 */
void outputOverflow(char*top,char*bottom,char*sum){
	if(overflow(top,bottom,sum)) {
		std::cout<<"\n---There is an overflow between two binary numbers---\n";
		return;
	}
	std::cout<<std::endl<<checkNegative(toDec(top))<< " + "<<checkNegative(toDec(bottom))<<" = "<<toDec(sum)<<std::endl;
}

/**
 * Ask user to exit, press 'C'/'c' to continue
 */
void askExit(char &exit){
	std::cout<<"\nEnter 'c' or 'C' to continue otherwise press any key(i.e. 'q'/'Q'): ";
	std::cin>>exit;
	std::cout<<std::endl;
}

/**
 * Apply two's complement binary addition by given values
 */
void twosComplementBinarryAddiion(int num1 = 0, int num2 = 0,bool flag = true,char exit = NULL,char *top = "",char *bottom = "",char *sum = ""){
	std::cout<<"**)Enter number any integer from -128 to 127\n\n";
	do {
		inputVar(num1,num2);
		setVar(num1,num2,top,bottom,sum);
		outputSummation(top,bottom,sum);
		outputOverflow(top,bottom,sum);
		askExit(exit);
	} while(exit == 'c' || exit == 'C');
}

/**
 * main function
 */
int main() { 
	twosComplementBinarryAddiion();
	return 0;
}
