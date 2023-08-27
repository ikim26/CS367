/* Fill in your Name and GNumber in the following two comment fields
 * Name: Isaac Kim
 * GNumber: 01201648
 */
#include <stdio.h>
#include <stdlib.h>
#include "fp.h"


int* convertDecimaltoBinary(double val, int length);
double convertBinarytoDecimal(int* array, int length);

//bias = 2^(4-1)-1 = 7

/* Helper method that returns binary representation of
 * double back as an array of 1s and 0s
 */
int *convertDecimaltoBinary(double val, int length){

    int* array = malloc(sizeof(int) * length);				//array for binary vals
	int count = 0;

	//integer value for our double
	int intVal = val;

	//seperate the decimal (< 0) part
	double floatp = val - intVal;
	int intRep;

	while(intVal > 0){
		//get remainder
		array[count] = intVal % 2;
		//keep dividing by 2
		intVal = intVal/2;
		//increment
		count++;
		if(count > length-1){
			break;
		}
	}
	while(count < length){
		//multiply by 2 to see for binary representation
		floatp = floatp*2;
		//get integer representation of this float
		intRep = floatp;
		//add this 1 or 0 to
		array[count] = intRep;
		if(intRep == 1){
			//decrement
			floatp = floatp - intRep;
		}
		count++;	//increment
	}
	return array;
}

/* input: Double value to be represented
 * output: Conversion from Double into our swfp_t
 *
 * Follow the Project Documentation for Instructions
 */
swfp_t double_to_swfp(double val) {
	swfp_t returnVal = 0;
	//method vars
	double E = 0.0;
	double expo = 0;
	double S = 0;
	double M = 0;
	double bias = 7;
	//getting S
	if(val > 0){
		S = 0;
	}
	else{
		S = 1;
	}
	//cancelling potential negative as it is no longer relavent code wise
	if(val < 0){
		val = val * -1;
	}
	//for normalized values
	if(val >= (1.0/64.0) && val <= 255.875){
		double temp;
		//getting E------------------------------------------------------
		temp = val;
		while(temp >= 2){
			temp = temp/2;
			E++;
		}
	    while(temp < 1){
	        temp = temp*2;
	        E--;
	    }
		//calculating M---------------------------------------------------
		while(val >= 2){
			val = val/2;
		}
		while(val < 1){
		    val = val*2;
		}
		//exp calculation
		expo = E + bias;
		//assign Mantissa
		M = val;
		//----------------------------------------------------------------
		//convert our decimal double to binary
		//(frac will equal elements 1-10)
		int* mantissaArray = malloc(sizeof(int) * 11);
		mantissaArray = convertDecimaltoBinary(M,11);
		
		int* expArray = malloc(sizeof(int) * 4);
		expArray = convertDecimaltoBinary(expo, 4);
		
		int binArray[15];
		binArray[0] = S;
		//copy exp bits into one array
		for(int i = 0; i < 4; i++){
			binArray[i + 1] = expArray[3 - i];
		}
		//copy frac bits into one array
		for(int j = 1; j < 11; j++){
			binArray[j + 4] = mantissaArray[j];
		}
		for(int x = 0; x < 15; x++){
		    //printf("%d", binArray[x]);
		}
        //printf("\n");
		returnVal = convertBinarytoDecimal(binArray, 15);
		//printf("%d", returnVal);
		//printf("\n");
		free(mantissaArray);
		free(expArray);

		return returnVal;
	}
	//infinity handling
	else if(val > 255.875){
		if(S == 1){
			return 31744;
		}
		return 15360;
	}
	//for denormalized values
	//if we are here, our val is less than 1/64
	else if (val > 1/65336){
		//E calculation
		E = 1 - bias;
		//exp calculation
		expo = 0;

		int inc = 0;
		while(inc < 6){
			val = val * 2;
			inc++;
		}
		M = val;
		E++;
		M = M/2;
		//convert our decimal double to binary
		//(frac will equal elements 1-10)
		int* mantissaArray = malloc(sizeof(int) * 11);
		mantissaArray = convertDecimaltoBinary(M,11);

		int* expArray = malloc(sizeof(int) * 4);
		expArray = convertDecimaltoBinary(expo, 4);

		int binArray[15];
		binArray[0] = S;
		//copy exp bits into one array
		for(int i = 0; i < 4; i++){
			binArray[i + 1] = expArray[i];
		}
		//copy frac bits into one array
		for(int j = 1; j < 11; j++){
			binArray[j + 4] = mantissaArray[j];
		}

	    returnVal = convertBinarytoDecimal(binArray, 15);

		free(mantissaArray);
		free(expArray);

		return returnVal;
	}
	//zero case
	else if(val == 0){
	    return 0;
	}
	//NaN
	else{
		return 15361;
	}
  return -1;
}

/* Helper method that returns double representation of
 * binary array of 1s and 0s
 */
double convertBinarytoDecimal(int* array, int length){
	int expo = 0;
	int index = 0;
	int expVal = 0;
	double returnVal = 0;

	while(index < length){
		expo = length - index - 1;
		expVal = 1;
		while(expo > 0){
			expVal = expVal * 2;
			expo--;
		}
		returnVal = returnVal + (array[index] * expVal);
		index++;
	}
	//printf("\n");
	return returnVal;
}


/* input: swfp_t representation of our floats
 * output: Conversion back to a Double from our swfp_t
 *
 * Follow the Project Documentation for Instructions
 */
double swfp_to_double(swfp_t val) {
	//infinity check
	if(val >= 32767){
		return DOUBLE_INF;
	}

	//converting to binary-(so we can work with our fp representation)-------------------------------------------------------------
  	int array[15] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};				//array for binary vals
	int count = 0;
	//integer value for our double
	int intVal = val;
	//seperate the decimal (< 0) part

	while(intVal > 0){
		//get remainder
		array[count] = intVal % 2;
		
		//keep dividing by 2
		intVal = intVal/2;

		count++;
		if(count >= 15){
			break;
		}
	}
	//---------------------------------------------------------------------------------------
	//transfer into new array of 16 bits
	int binArray[15] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

	//fill the initial spots, if any, with 0s 
	for(int i = 0; i < 15; i++){
		binArray[i] = array[14-i];
	}
	//split array into sign, exp and frac sections
	int expArray[4] = {0,0,0,0};
	int fracArray[11] = {0,0,0,0,0,0,0,0,0,0,0};
	//for exp array
	for(int j = 1; j < 5; j++){
		expArray[j-1] = binArray[j];
	}
	//for frac array
	if(val >= 1024){			//for normalized values
		fracArray[0] = 1;
	}
	else{						//for denormalized values
		fracArray[0] = 0;
	}
	
	for(int k = 5; k < 15; k++){//setting array elements
		fracArray[k-4] = binArray[k];
	}
	//get S, exp and M------------------------------------------------------------------------
	double S = 0;
	double E = 0;
	double M = 0;
	double expo = 0;
	double returnVal = 1.0;

	S = binArray[0];			//setting S

	if(val >= 1024){			//for normalized values
		expo = convertBinarytoDecimal(expArray, 4);
		E = expo - 7;
	}
	else{						//for denormalized values
		E = -6;
	}

	int ant = 10 - E;			//set counter for dividing by 2
	//set M
	M = convertBinarytoDecimal(fracArray, 11);

	if(expo == 15 && M > 1024){		//NaN check
		return DOUBLE_NAN * -1;
	}
	else if(expo == 15 && M == 1024){	//infinity check
		if(S == 1){
			return DOUBLE_INF * -1;
		}
		return DOUBLE_INF;
	}
	else if(expo == 0){
		if(M == 0){
			return 0;
		}
	}
	
	returnVal = M;

	while(ant > 0){
		returnVal = returnVal/2;
		ant--;
	}
	if(S == 1){				//accounting for sign bit
	    returnVal = returnVal * -1;
	}
    return returnVal;
}

swfp_t swfp_mul(swfp_t val1, swfp_t val2) {
	//special cases

	//+infinity = 15360
	//-infinity = 31744
	//NaN = 15361
	//-0 = 16384
	//0 = 0

	//overflow cases
	if(val1 == 15359 && val2 > 7168){
		return 15360;
	}
	if(val2 == 15359 && val1 > 7168){
		return 15360;
	}
	//+infinity cases
	if(val1 == 15360){
		if(val2 == 15360){
			return 15360;
		}
		else if(val2 == 15361){
			return 15361;
		}
		else if(val2 == 0 || val2 == 16384){
			return 15361;
		}
		else if(val2 == 31744){
			return 31744;
		}
		else if(val2 > 16384){
			return 31744;
		}
		else{
			return 15360;
		}
	}
	if(val2 == 15360){
		if(val1 == 15360){
			return 15360;
		}
		else if(val1 == 15361){
			return 15361;
		}
		else if(val1 == 0 || val1 == 16384){
			return 15361;
		}
		else if(val1 == 31744){
			return 31744;
		}
		else if(val1 > 16384){
			return 31744;
		}
		else{
			return 15360;
		}
	}

	//-infinity cases
	if(val1 == 31744){
		if(val2 == 15361){
			return 15361;
		}
		else if(val2 == 0){
			return 15361;
		}
		else if(val2 == 16384){
			return 15361;
		}
		else if(val2 == 15360){
			return 31744;
		}
		else{
			return 31744;
		}
	}
	if(val2 == 31744){
		if(val1 == 15361){
			return 15361;
		}
		else if(val1 == 0){
			return 15361;
		}
		else if(val1 == 16384){
			return 15361;
		}
		else if(val1 == 15360){
			return 31744;
		}
		else{
			return 31744;
		}
	}
	//NaN cases
	if(val1 == 15361 || val2 == 15361){
		return 15361;
	}
	//0 cases
	if(val1 == 16384){
		if(val2 >= 0){
			return 16384;
		}
		else if(val2 == 16384){
			return 0;
		}

		else{
			return 0;
		}
	}
	if(val1 == 0){
		if(val2 >= 16384){
			return 16384;
		}
		else if(val2 == 0){
			return 0;
		}

		else{
			return 0;
		}
	}
	if(val2 == 16384){
		if(val1 == 0){
			return 16384;
		}
		else if(val1 >= 16384){
			return 0;
		}

		else{
			return 0;
		}
	}
	if(val2 == 0){
		if(val1 >= 16384){
			return 16384;
		}
		else if(val1 == 0){
			return 0;
		}

		else{
			return 0;
		}
	}
	//binarrays for val1 and val2
	//converting to binary---------------------------------------------------------------
  	int array1[15] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};				//array for binary vals
  	int array2[15] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	int count = 0;
	//integer value for our double
	int intVal1 = val1;
	int intVal2 = val2;

	while(intVal1 > 0){
		//get remainder
		array1[count] = intVal1 % 2;
		//printf("%d", array1[count]);

		//keep dividing by 2
		intVal1 = intVal1/2;

		count++;
		if(count >= 15){
			break;
		}
	}
	count = 0;
	while(intVal2 > 0){
	    array2[count] = intVal2 % 2;
	    
	    intVal2 = intVal2/2;
	    
	    count++;
	    if(count >= 15){
	        break;
	    }
	
	}
	//---------------------------------------------------------------------------------------
	//transfer into new array of 16 bits
	int binArray1[15] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	int binArray2[15] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

	//fill the initial spots, if any, with 0s 
	for(int i = 0; i < 15; i++){
		binArray1[i] = array1[14-i];
		binArray2[i] = array2[14-i];
	}

	//split array into sign, exp and frac sections
	int expArray1[4] = {0,0,0,0};
	int fracArray1[11] = {0,0,0,0,0,0,0,0,0,0,0};
	int expArray2[4] = {0,0,0,0};
	int fracArray2[11] = {0,0,0,0,0,0,0,0,0,0,0};

	//for both exp array
	for(int j = 1; j < 5; j++){
		expArray1[j-1] = binArray1[j];
		expArray2[j-1] = binArray2[j];
	}
	//for frac1 array
	if(val1 >= 1024){			//for normalized values
		fracArray1[0] = 1;
	}
	else{						//for denormalized values
		fracArray1[0] = 0;
	}
	//for frac2 array
	if(val2 >= 1024){			//for normalized values
		fracArray2[0] = 1;
	}
	else{						//for denormalized values
		fracArray2[0] = 0;
	}

	for(int k = 5; k < 15; k++){//setting array elements
		fracArray1[k-4] = binArray1[k];
		fracArray2[k-4] = binArray2[k];
	}
	//get S, exp and M------------------------------------------------------------------------
	double S = 0;
	double E = 0;
	double M = 0;
	double expo = 0;

	double S1 = 0;
	double E1 = 0;
	double M1 = 0;
	double expo1 = 0;

	double S2 = 0;
	double E2 = 0;
	double M2 = 0;
	double expo2 = 0;
	//calculating S-----------------------------------------------------------------------
	S1 = binArray1[0];
	S2 = binArray2[0];
    S = S1 + S2;
    if(S == 2){
        S = 0;
    }
	//calculating E-----------------------------------------------------------------------
	if(val1 >= 1024){			//for normalized values
		expo1 = convertBinarytoDecimal(expArray1, 4);
		E1 = expo1 - 7;
	}
	else{						//for denormalized values
		E1 = -6;
	}

	if(val2 >= 1024){			//for normalized values
		expo2 = convertBinarytoDecimal(expArray2, 4);
		E2 = expo2 - 7;
	}
	else{						//for denormalized values
		E2 = -6;
	}

	E = E1 + E2;
	//calculating M-----------------------------------------------------------------------
	M1 = convertBinarytoDecimal(fracArray1, 11);
	M2 = convertBinarytoDecimal(fracArray2, 11);
	//simplifying Mantissas to 1.frac format
	int expCount1 = 10 - E1;
	while(expCount1 > 0){
	    M1 = M1/2;
	    expCount1--;
	}
	int expCount2 = 10 - E2;
	while(expCount2 > 0){
	    M2 = M2/2;
	    expCount2--;
	}
	//multiply
	M = M1 * M2;

	if(val1 < 1024 && val2 < 1024){
		return 0;
	}
    //normalize to 1.frac format
	while(M >= 2){
		M = M/2.0;
	}
	while(M < 1){
	    M = M * 2;
	}
	//---------------------------------------------------------------------------------------
	//Now we convert back into one binary array
	if(E1 == -6 && E2 == -6){
		expo = 0;
	}
	else if(E1 == -6 || E2 == -6){
		expo = E + 6;
	}
	else{
		expo = E + 7;
	}
	//convert our decimal double to binary
	//(frac will equal elements 1-10)
	int* mantissaArray = malloc(sizeof(int) * 11);
	mantissaArray = convertDecimaltoBinary(M,11);

	int* expArray = malloc(sizeof(int) * 4);
	expArray = convertDecimaltoBinary(expo, 4);

	int returnArray[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	returnArray[0] = 0;
	returnArray[1] = S;	

	for(int i = 2; i < 6; i++){
		returnArray[i] = expArray[5-i];
	}
	for(int j = 6; j < 16; j++){
		returnArray[j] = mantissaArray[j-5];
	}

	swfp_t returnVal = convertBinarytoDecimal(returnArray, 16);

	free(mantissaArray);
	free(expArray);

	return returnVal;
}

swfp_t swfp_add(swfp_t val1, swfp_t val2) {
	//special cases

	//+infinity = 15360
	//-infinity = 31744
	//NaN = 15361
	//-0 = 16384
	//0 = 0

	//infinity case
	if(val1 == 15360){
		if(val2 == 15360){
			return 15360;
		}
		else if(val2 == 31744){
			return 15361;
		}
		else if(val2 == 15361){
			return 15361;
		}
		else{
			return 15360;
		}
	}
	if(val2 == 15360){
		if(val1 == 15360){
			return 15360;
		}
		else if(val1 == 31744){
			return 15361;
		}
		else if(val1 == 15361){
			return 15361;
		}
		else{
			return 15360;
		}
	}
	//negative infinity cases
	if(val1 == 31744){
		if(val2 == 31744){
			return 31744;
		}
		else if(val2 == 15361){
			return 15361;
		}
		else{
			return 31744;
		}
	}
	if(val2 == 31744){
		if(val1 == 31744){
			return 31744;
		}
		else if(val1 == 15361){
			return 15361;
		}
		else{
			return 31744;
		}
	}
	//+0 cases
	if(val1 == 0){
		if(val2 == 16384){
			return 0;
		}
		else{
			return val2;
		}
	}
	if(val2 == 0){
		if(val1 == 16384){
			return 0;
		}
		else{
			return val1;
		}
	}
	//-0 cases
	if(val1 == 16384){
		if(val2 == 16384){
			return 16384;
		}
		else{
			return val2;
		}
	}
	if(val2 == 16384){
		if(val1 == 16384){
			return 16384;
		}
		else{
			return val1;
		}
	}
	//NaN cases
	if(val1 == 15361 || val2 == 15361){
		return 15361;
	}

	//binarrays for val1 and val2
	//converting to binary---------------------------------------------------------------
  	int array1[15] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};				//array for binary vals
  	int array2[15] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	int count = 0;

	//integer value for our double
	int intVal1 = val1;
	int intVal2 = val2;

	while(intVal1 > 0){
		//get remainder
		array1[count] = intVal1 % 2;

		//keep dividing by 2
		intVal1 = intVal1/2;

		count++;
		if(count >= 15){
			break;
		}
	}
	count = 0;
	while(intVal2 > 0){
	    array2[count] = intVal2 % 2;
	    
	    intVal2 = intVal2/2;
	    
	    count++;
	    if(count >= 15){
	        break;
	    }
	}
	
	//---------------------------------------------------------------------------------------
	//transfer into new array of 16 bits
	int binArray1[15];
	int binArray2[15];

	//fill the initial spots, if any, with 0s 
	for(int i = 0; i < 15; i++){
		binArray1[i] = array1[14-i];
		binArray2[i] = array2[14-i];
	}

	//split array into sign, exp and frac sections
	int expArray1[4];
	int fracArray1[11];
	int expArray2[4];
	int fracArray2[11];
	//for both exp array
	for(int j = 1; j < 5; j++){
		expArray1[j-1] = binArray1[j];
		expArray2[j-1] = binArray2[j];
	}
	//for frac1 array
	if(val1 >= 1024){			//for normalized values
		fracArray1[0] = 1;
	}
	else{						//for denormalized values
		fracArray1[0] = 0;
	}
	//for frac2 array
	if(val2 >= 1024){			//for normalized values
		fracArray2[0] = 1;
	}
	else{						//for denormalized values
		fracArray2[0] = 0;
	}
	
	for(int k = 5; k < 15; k++){//setting array elements
		fracArray1[k-4] = binArray1[k];
		fracArray2[k-4] = binArray2[k];
	}
	//get S, exp and M------------------------------------------------------------------------
	double S = 0;
	double E = 0;
	double M = 0;
	double expo = 0;

	double S1 = 0;
	double E1 = 0;
	double M1 = 0;
	double expo1 = 0;

	double S2 = 0;
	double E2 = 0;
	double M2 = 0;
	double expo2 = 0;
	//calculating E-----------------------------------------------------------------------
	if(val1 >= 1024){			//for normalized values
		expo1 = convertBinarytoDecimal(expArray1, 4);
		E1 = expo1 - 7;
		//printf("%f", expo1);
	}
	else{						//for denormalized values
		E1 = -6;
	}

	if(val2 >= 1024){			//for normalized values
		expo2 = convertBinarytoDecimal(expArray2, 4);
		E2 = expo2 - 7;
	}
	else{						//for denormalized values
		E2 = -6;
	}

	int e1Greater = 0;
	if(E1 > E2){
		E = E1;
		e1Greater = 1;
	}
	else{
		E = E2;
	}
	//calculating M-----------------------------------------------------------------------
	M1 = convertBinarytoDecimal(fracArray1, 11);
	M2 = convertBinarytoDecimal(fracArray2, 11);
	int M1Greater = 0;
	//normalizing M values
	while(M1 < 1){
	    M1 = M1*2;
	}
	while(M1 >= 2){
	    M1 = M1/2;
	}
	while(M2 < 1){
	    M2 = M2*2;
	}
	while(M2 >= 2){
	    M2 = M2/2;
	}
	//correcting mantissas according to which exponent we are using (the greater one)	
	if(e1Greater == 1){
	    while(E2 < E){
	        M2 = M2/2;
	        E2++;
	    }
	}
	else{
	    while(E1 < E){
	        M1 = M1/2;
	        E1++;
	    }
	}
	//setting M1 is greater or not
	if(M1 >= M2){
	    M1Greater = 1;
	}
	//calculating S-----------------------------------------------------------------------
	S1 = binArray1[0];
	S2 = binArray2[0];
	//determing which sign we use
	if(M1Greater == 1 && S1 == 1){
	    S = 1;
	}
	else if(M1Greater == 0 && S2 == 1){
	    S = 1;
	}
    else{
        S = 0;
    }
	
	if(S1 == 1){
	    M1 = M1 * -1;
	}
	if(S2 == 1){
	    M2 = M2 * -1;
	}
	//addition
	M = M1 + M2;
	//check if negative because negative doesnt matter for binary converison
	if(M < 0){
	    M = M * -1;
	}
	//zero check
	if(M == 0){
	    if(S == 1){
	        return 16384;
	    }
	    return 0;
	}
	//normalizing mantissa to 1.frac format
    while(M >= 2){
        M = M/2;
        E++;
    }
    while(M < 1){
        M = M*2;
        E--;
    }

	//---------------------------------------------------------------------------------------
	//Now we convert back into one binary array

	expo = E + 7;

	//convert our decimal double to binary
	//(frac will equal elements 1-10)
	int* mantissaArray = malloc(sizeof(int) * 11);
	mantissaArray = convertDecimaltoBinary(M,11);

	int* expArray = malloc(sizeof(int) * 4);
	expArray = convertDecimaltoBinary(expo, 4);

	int returnArray[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	returnArray[0] = 0;
	returnArray[1] = S;	

	for(int i = 2; i < 6; i++){
		returnArray[i] = expArray[5-i];
	}
	for(int j = 6; j < 16; j++){
		returnArray[j] = mantissaArray[j-5];
	}

	swfp_t returnVal = convertBinarytoDecimal(returnArray, 16);

	free(mantissaArray);
	free(expArray);

	return returnVal;
}
