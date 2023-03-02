#include "transfer_matrix_simple1.h"


void get_string_rep(unsigned long long signature, int w){
	unsigned long long base = 3*pow(4, w);
	
	for(int i = 0; i<w+1; i++){
		if((signature & base) == 0){
			printf("A");
		}
		else if((signature & base) == pow(4,w-i)){
			printf("B");
		}
		else if((signature & base) == 2*pow(4,w-i)){
			printf("O");
		}
		else{
			printf("X");
		}
		
		base = base >> 2;
	}
}

long long binary_search(unsigned long long *valid_signatures, unsigned long long signature, long long l, long long r, int w)
{
    if (r >= l) {
        long long m = l + (r - l) / 2;

		if(valid_signatures[m] != signature){
			if(valid_signatures[m] > signature){
				return binary_search(valid_signatures, signature, l, m - 1, w);
			}
			else{
				return binary_search(valid_signatures, signature, m + 1, r, w);
			}
		}

        return m;
    }
    return -1;
}

long long find_index(unsigned long long **valid_signatures, unsigned long long signature, long long *counts, int w){
	long long count = 0;
	
	for(int i = 0; i<GROUP_SIZE; i++){	
		long long index = binary_search(valid_signatures[i], signature, 0, counts[i]-1, w);
		if(index != -1){
			return count + index;
		}
		count += counts[i];
	}
	return -1;
}


bool is_valid_signature(unsigned long long signature, int w){
	
	unsigned long long base = 3*pow(4, w);

    bool exception = ((signature & base)  == base) && ((signature & base/4) == 2*pow(4, w-1)) && ((signature & 3) == 2); // XO...O
	
	int number_a = 0;
	int number_b = 0;
	unsigned long long sig_copy = signature;
	
	for(int i = 0; i<w+1; i++){
		if((sig_copy & base) == 0){
			number_a++;
		}
		else if((sig_copy & base) == pow(4, w)){
			number_b++;
		}
		
		sig_copy = sig_copy << 2;
		
		if(number_a < number_b){
			return false;
		}
	}
	
    return number_a != 0 && (number_a == number_b) && !exception;
}

void construct_signatures(unsigned long long *signatures, unsigned long long dimension, int w, unsigned long long n){
	//              A, B, O, X
    int states[] = {0, 1, 2, 3};	
	unsigned long long d = dimension/GROUP_SIZE;
	
	int start = 0;
	if(n < dimension/3){
		start = 0;
	}
	else if(n < 2*(dimension/3)){
		start = 2;
	}
	else{
		start = 3;
	}

    //Set first State
    for(unsigned long long i = 0; i<d; i++){
        signatures[i] = start * pow(4, w);
    }

    // Set "middle" states
    int state_number = 0;
    unsigned long long period = (dimension/3)/4;
	
	unsigned long long current;

    for(int i = 0; i < w-1; i++){
        current = period - (n - period*(n/period));	
		state_number = ((n/period) % 4);
		
        for(unsigned long long j = 0; j<d; j++){
            if(current == 0){
                current = period;
                state_number = (state_number+1) % 4;
            }
			signatures[j] = signatures[j] + states[state_number] * pow(4, w-i-1);			
            current--;
        }
        period = period/4;
    }

    //Set last State (last state can not be A)
	if(n%3 == 0){
		start = 0;
	}
	else if(n%3 == 1){
		start = 1;
	}
	else{
		start = 2;
	}
	
    for(unsigned long long i = 0; i<d; i++){
		if((i+start)%3 == 0){
			signatures[i] = signatures[i] + states[1];
        }
        else if((i+start)%3 == 1){
            signatures[i] = signatures[i] + states[2];
        }
        else{
            signatures[i] = signatures[i] + states[3];
        }
    }
}

void save_valid_signatures(unsigned long long *valid_signatures, unsigned long long *signatures, unsigned long long dimension, int w){
    unsigned long long j = 0;

    for(unsigned long long i = 0; i<dimension; i++){
        if(is_valid_signature(signatures[i], w)){
            valid_signatures[j] = signatures[i];
            j++;
        }
    }
}

unsigned long long count_valid_signatures(unsigned long long *valid_signatures, unsigned long long dimension, int w){
    unsigned long long count = 0;
	
    for(unsigned long long i = 0; i<dimension; i++){
        if(is_valid_signature(valid_signatures[i], w)){
            count++;
        }
    }
    return count;
}

unsigned long long change_to_a(unsigned long long signature, int number_of_a, int w){
	unsigned long long sig_copy = signature;
	unsigned long long result = signature;
	
	unsigned long long base = 3*pow(4,w);
	
	for(int i = 0; i<w+1; i++){
		if((sig_copy & base) == 0){
			number_of_a++;
		}
		else if((sig_copy & base) == (unsigned long long)(base/3)){
			number_of_a --;
            if(number_of_a == 0){
				result -= (unsigned long long)pow(4, w-i);
                break;
            }
		}
		
		sig_copy = sig_copy << 2;
	}
    return result;
}

unsigned long long change_to_b(unsigned long long signature, int number_of_b, int w){
	unsigned long long base = 3;
	unsigned long long result = signature;
	
	for(int i = 0; i<w+1; i++){
		if((signature & base) == (unsigned long long)(base/3)){
			number_of_b++;
		}
		else if((signature & base) == 0){
			number_of_b --;
            if(number_of_b == 0){
				result += (unsigned long long)base/3;
                break;
            }
		}
		
		base = base << 2;
	}
    return result;
}

bool are_on_the_same_path(unsigned long long signature, int index_a, int offset_b, int w){
	unsigned long long base = 3;
    int index_a_calculated = 0;
	
	for(int i = 0; i<w+1; i++){
		if((signature & base) == (unsigned long long)(base/3)){
			offset_b++;
		}
		else if((signature & base) == 0){
			offset_b--;
            if(offset_b == 0){
				index_a_calculated = w-i;
                break;
            }
		}
		
		base = base << 2;
	}
    return (index_a == index_a_calculated);
}

int compute_successor_signatures(unsigned long long *next_signatures, unsigned long long signature, int w){
    int length = 0;
	
	unsigned long long base_X = 3*pow(4, w);
	unsigned long long base_O = 2*pow(4, w);
	unsigned long long base_B = pow(4, w);

    if(((signature & base_X) == base_X) && ((signature & 12) == 12) && ((signature & 3) == 3)){ //X...XX
        // keine
		next_signatures[0] = signature >> 2;
		next_signatures[0] += base_O; // 10... ->  O...
		
        length = 1;
    }
    else if(((signature & base_X) == base_O) && ((signature & 12) == 12) && ((signature & 3) == 3)){ //O...XX
        // 1
		next_signatures[0] = signature >> 2;
		next_signatures[0] -= (base_B/4); // 0001... -> AB...	
        // keine
		next_signatures[1] = signature >> 2;
		next_signatures[1] += base_O; // 10... ->  O...

        length = 2;
    }
    else if(((signature & base_X) == base_X) && ((signature & 12) == 8) && ((signature & 3) == 3)){ //X...OX
        // 3
		next_signatures[0] = signature >> 2;
		next_signatures[0] -= 1; // 00...01 -> A...B
        // keine
		next_signatures[1] = signature >> 2;
		next_signatures[1] += base_O; // 10... ->  O...
        
        length = 2;
    }
    else if(((signature & base_X) == base_X) && ((signature & 12) == 12) && ((signature & 3) == 2)){ // X...XO
        // keine
		next_signatures[0] = signature >> 2;
		next_signatures[0] += base_O; // 10... ->  O...
		
        length = 1;
    }
    else if(((signature & base_X) == base_O) && ((signature & 12) == 8) && ((signature & 3) == 2)){// O...OO
        // 1
		next_signatures[0] = signature >> 2;
		next_signatures[0] -= (base_B/4); // 0001... -> AB...
        // 3	
		next_signatures[1] = signature >> 2;
		next_signatures[1] -= 1; // 00...01 -> A...B
        // 1 3
		next_signatures[2] = signature >> 2;
		next_signatures[2] += (base_X - (base_O/4) - 1); // 1100...01 -> XA...B
        // keine
		next_signatures[3] = signature >> 2;
		next_signatures[3] += base_O; // 10... ->  O...
		
        length = 4;
    }
    else if(((signature & base_X) == base_X) && ((signature & 12) == 8) && ((signature & 3) == 2)){// X...OO
        // 3
		next_signatures[0] = signature >> 2;
		next_signatures[0] -= 1; // 00...01 -> A...B
        // keine
        next_signatures[1] = signature >> 2;
		next_signatures[1] += base_O; // 10... ->  O...
		
        length = 2;
    }
    else if(((signature & base_X) == base_O) && ((signature & 12) == 12) && ((signature & 3) == 2)){// O... XO
        // 1
        next_signatures[0] = signature >> 2;
		next_signatures[0] -= (base_B/4); // 0001... -> AB...
        // keine
        next_signatures[1] = signature >> 2;
		next_signatures[1] += base_O; // 10... ->  O...
		
        length = 2;
    }
    else if(((signature & base_X) == base_O) && ((signature & 12) == 8) && ((signature & 3) == 3)){ // O...OX
        // 1
        next_signatures[0] = signature >> 2;
		next_signatures[0] -= (base_B/4); // 0001... -> AB...
        // 3
        next_signatures[1] = signature >> 2;
		next_signatures[1] -= 1; // 00...01 -> A...B
        // 1 3
        next_signatures[2] = signature >> 2;
		next_signatures[2] += (base_X - (base_O/4) - 1); // 1100...01 -> XA...B
        // keine
        next_signatures[3] = signature >> 2;
		next_signatures[3] += base_O; // 10... ->  O...
		
        length = 4;
    }
    else if(((signature & base_X) == 0) && ((signature & 12) == 12) && ((signature & 3) == 3)){ // A...XX
        // 1	
		next_signatures[0] = signature >> 2;
		next_signatures[0] += (base_X/4); // 0011... -> AX...
        // keine
        next_signatures[1] = signature >> 2;
		next_signatures[1] += base_O; // 10... ->  O...
		
        length = 2;
    }
    else if(((signature & base_X) == 0) && ((signature & 12) == 8) && ((signature & 3) == 2)){ // A...OO
        // 1
		next_signatures[0] = signature >> 2;
		next_signatures[0] += (base_X/4); // 0011... -> AX...
        // 3
		next_signatures[1] = signature >> 2;
		next_signatures[1] -= 1; // 00...01 -> A...B
        // keine
		next_signatures[2] = signature >> 2;
		next_signatures[2] += base_O; // 10... ->  O...
        // 1 3
		next_signatures[3] = change_to_a(signature, 0, w);
		next_signatures[3] = next_signatures[3] >> 2;
		next_signatures[3] += (base_X + (base_X/4)-1); // 1111...01 -> XX...B
		
        length = 4;
    }
    else if(((signature & base_X) == base_X) && ((signature & 12) == 4) && ((signature & 3) == 3)){ // X...BX
        // keine
        next_signatures[0] = signature >> 2;
		next_signatures[0] += base_O; // 10... ->  O...
        // 3
		next_signatures[1] = change_to_b(signature, 0, w);
		next_signatures[1] = next_signatures[1] >> 2; 
		next_signatures[1] += 2; // 00...11 -> A...X
		
        length = 2;
    }
    else if(((signature & base_X) == base_X) && ((signature & 12) == 12) && ((signature & 3) == 1)){//X...XB
        // 2
		next_signatures[0] = change_to_b(signature, 0, w);
		next_signatures[0] = next_signatures[0] >> 2; 
		next_signatures[0] += 0; // 00... -> A...

        length = 1;
    }
    else if(((signature & base_X) == base_X) && ((signature & 12) == 4) && ((signature & 3) == 1)){  //X...BB
        // 2
		next_signatures[0] = change_to_b(signature, 0, w);
		next_signatures[0] = next_signatures[0] >> 2; 
		next_signatures[0] += 0; // 00... -> A...
        // 2 3
		next_signatures[1] = change_to_b(signature, -1, w);
		next_signatures[1] = next_signatures[1] >> 2; 
		next_signatures[1] += (base_X + 2); // 11...11 -> X...X
		
        length = 2;
    }  
    else if(((signature & base_X) == base_O) && ((signature & 12) == 4) && ((signature & 3) == 2)){ //O...BO
        // 1
		next_signatures[0] = signature >> 2;
		next_signatures[0] -= (base_B/4); // 0001... -> AB...
        // keine
        next_signatures[1] = signature >> 2;
		next_signatures[1] += base_O; // 10... ->  O...
        // 3
		unsigned long long new_sig = change_to_b(signature, 0, w);
		next_signatures[2] = new_sig >> 2;
		next_signatures[2] += 2; // 00...11 -> A...X
        // 1 3
		next_signatures[3] = new_sig >> 2; 
		next_signatures[3] += (base_X - (base_O/4) + 2); // 1100...11 -> XA...X
		
        length = 4;
    }  	
    else if(((signature & base_X) == base_O) && ((signature & 12) == 8) && ((signature & 3) == 1)){ //O...OB
        // 2 3
		next_signatures[0] = signature >> 2;
		next_signatures[0] += (base_X - 1); // 11...01 -> X...B
        // 2
		unsigned long long new_sig = change_to_b(signature, 0, w);
		next_signatures[1] = new_sig >> 2;
		next_signatures[1] += 0; // 00... -> A...
        // 1 2
		next_signatures[2] = new_sig >> 2;
		next_signatures[2] += (base_X - (base_O/4)); // 1100... -> XA...
		
        length = 3;
    }
    else if(((signature & base_X) == base_O) && ((signature & 12) == 4) && ((signature & 3) == 1)){ // O...BB 
        // 2
		unsigned long long new_sig = change_to_b(signature, 0, w);
		next_signatures[0] = new_sig >> 2;
		next_signatures[0] += 0; // 00... -> A...
        // 1 2
		next_signatures[1] = new_sig >> 2;
		next_signatures[1] += (base_X - (base_O/4)); // 1100... -> XA...
        // 2 3
		new_sig = change_to_b(signature, -1, w);
		next_signatures[2] = new_sig >> 2;
		next_signatures[2] += (base_X + 2); // 11...11 -> X...X
		
        length = 3;
    } 
    else if(((signature & base_X) == 0) && ((signature & 12) == 12) && ((signature & 3) == 2)){ // A...XO
        // 1
		next_signatures[0] = signature >> 2;
		next_signatures[0] += (base_X/4); // 0011... -> AX...
        // keine
        next_signatures[1] = signature >> 2;
		next_signatures[1] += base_O; // 10... ->  O...
		
        length = 2;
    }
    else if(((signature & base_X) == 0) && ((signature & 12) == 8) && ((signature & 3) == 3)){ // A...OX
        // 1
		next_signatures[0] = signature >> 2;
		next_signatures[0] += (base_X/4); // 0011... -> AX...
        // 3
		next_signatures[1] = signature >> 2;
		next_signatures[1] -= 1; // 00...01 -> A...B
        // keine
        next_signatures[2] = signature >> 2;
		next_signatures[2] += base_O; // 10... ->  O...
        // 1 3
		next_signatures[3] = change_to_a(signature, 0, w);
		next_signatures[3] = next_signatures[3] >> 2;
		next_signatures[3] += (base_X + (base_X/4) - 1); // 1111...01 -> XX...B

        length = 4;
    }
    else if(((signature & base_X) == base_O) && ((signature & 12) == 4) && ((signature & 3) == 3)){ // O...BX
        // 1
		next_signatures[0] = signature >> 2;
		next_signatures[0] -= (base_B/4); // 0001... -> AB...
        // keine
		next_signatures[1] = signature >> 2;
		next_signatures[1] += base_O; // 10... ->  O...
        // 3
		unsigned long long new_sig = change_to_b(signature, 0, w);
		next_signatures[2] = new_sig >> 2;
		next_signatures[2] += 2; // 00...11 -> A...X
        // 1 3  
		next_signatures[3] = new_sig >> 2;
		next_signatures[3] += (base_X - (base_O/4) + 2); // 1100...11 -> XA...X
	  
        length = 4;  
    }
    else if(((signature & base_X) == base_O) && ((signature & 12) == 12) && ((signature & 3) == 1)){ // O...XB
        // 2
		unsigned long long new_sig = change_to_b(signature, 0, w);
		next_signatures[0] = new_sig >> 2;
		next_signatures[0] += 0; // 00... -> A...
        // 1 2
		next_signatures[1] = new_sig >> 2;
		next_signatures[1] += (base_X - (base_O/4)); // 1100... -> XA...
		
        length = 2;
    }
    else if(((signature & base_X) == base_X) && ((signature & 12) == 4) && ((signature & 3) == 2)){ // X...BO  
        // keine
		next_signatures[0] = signature >> 2;
		next_signatures[0] += base_O; // 10... ->  O...
        // 3
		unsigned long long new_sig = change_to_b(signature, 0, w);
		next_signatures[1] = new_sig >> 2;
		next_signatures[1] += 2; // 00...11 -> A...X

        length = 2;
    }
    else if(((signature & base_X) == base_X) && ((signature & 12) == 8) && ((signature & 3) == 1)){ // X...OB 
        // 2 3
		next_signatures[0] = signature >> 2;
		next_signatures[0] += (base_X - 1); // 11...01 -> X...B
        // 2
		unsigned long long new_sig = change_to_b(signature, 0, w);
		next_signatures[1] = new_sig >> 2;
		next_signatures[1] += 0; // 00... -> A...
		
        length = 2;
    }
    //---------------------------
    else if(((signature & base_X) == 0) && ((signature & 12) == 4) && ((signature & 3) == 1)){ // A...BB 
        // 2
		unsigned long long new_sig = change_to_b(signature, 0, w);
		next_signatures[0] = new_sig >> 2;
		next_signatures[0] += 0; // 00... -> A...	

        int i = 1;
        if(!are_on_the_same_path(signature, 0, 0, w)){
            // 1 2
			new_sig = change_to_a(new_sig, 0, w);
			next_signatures[i] = new_sig >> 2;
			next_signatures[i] += (base_X + (base_X/4)); // 1111... -> XX...
            i++;
            length++;
        }
        // 2 3
		new_sig = change_to_b(signature, -1, w);
		next_signatures[i] = new_sig >> 2;
		next_signatures[i] += (base_X + 2); // 11...11 -> X...X
	
        length +=2;
    }
    else if(((signature & base_X) == 0) && ((signature & 12) == 0) && ((signature & 3) == 1)){ // A...AB
        // 2
		next_signatures[0] = signature >> 2;
		next_signatures[0] += 1; // 00...01 -> A...B
        // 1 2
		next_signatures[1] = change_to_a(signature, 0, w);
		next_signatures[1] = next_signatures[1] >> 2;
		next_signatures[1] += (base_X + (base_X/4) + 1); // 1111...01 -> XX...B
		
        length = 2;
    }
    else if(((signature & base_X) == 0) && ((signature & 12) == 4) && ((signature & 3) == 3)){ // A...BX
        // 1
		next_signatures[0] = signature >> 2;
		next_signatures[0] += (base_X/4); // 0011...01 -> AX...B
        // keine
		next_signatures[1] = signature >> 2;
		next_signatures[1] += base_O; // 10... ->  O...
        // 3
		unsigned long long new_sig = change_to_b(signature, 0, w);
		next_signatures[2] = new_sig >> 2;
		next_signatures[2] += 2; // 00...11 -> A...X
		
        length = 3;

        if(!are_on_the_same_path(signature, 0, 0, w)){
            // 1 2
			new_sig = change_to_a(new_sig, 0, w);
			next_signatures[3] = new_sig >> 2;
			next_signatures[3] += (base_X + (base_X/4) + 2); // 1111...11 -> XX...X

            length++;
        }
    }
    if(((signature & base_X) == 0) && ((signature & 12) == 12) && ((signature & 3) == 1)){ // A...XB
        // 2
		unsigned long long new_sig = change_to_b(signature, 0, w);
		next_signatures[0] = new_sig >> 2;
		next_signatures[0] += 0; // 00... -> A...
		
        length = 1;

        if(!are_on_the_same_path(signature, 0, 0, w)){
            // 1 2
			new_sig = change_to_a(new_sig, 0, w);
			next_signatures[1] = new_sig >> 2;
			next_signatures[1] += (base_X + (base_X/4)); // 1111... -> XX...

            length++;
        }
    }
    else if(((signature & base_X) == base_X) && ((signature & 12) == 0) && ((signature & 3) == 1)){ // X...AB
        // 2
		next_signatures[0] = signature >> 2;
		next_signatures[0] += 1; // 00...01 -> A...B
		
        length = 1;
    }
    else if(((signature & base_X) == 0) && ((signature & 12) == 4) && ((signature & 3) == 2)){ // A...BO
        // 1
		next_signatures[0] = signature >> 2;
		next_signatures[0] += (base_X/4); // 0011... -> AX...
        // keine
		next_signatures[1] = signature >> 2;
		next_signatures[1] += base_O; // 10... ->  O...
        // 3
		unsigned long long new_sig = change_to_b(signature, 0, w);
		next_signatures[2] = new_sig >> 2;
		next_signatures[2] += 2; // 00...11 -> A...X

        length = 3;

        if(!are_on_the_same_path(signature, 0, 0, w)){
            // 1 3
			new_sig = change_to_a(new_sig, 0, w);
			next_signatures[3] = new_sig >> 2;
			next_signatures[3] += (base_X + (base_X/4) + 2); // 1111...11 -> XX...X
			
            length++;
        }
    }
    else if(((signature & base_X) == 0) && ((signature & 12) == 8) && ((signature & 3) == 1)){ // A...OB
        // 2 3
		next_signatures[0] = signature >> 2;
		next_signatures[0] += (base_X - 1); // 1100...01 -> XA...B
        // 2
		unsigned long long new_sig = change_to_b(signature, 0, w);
		next_signatures[1] = new_sig >> 2;
		next_signatures[1] += 0; // 00... -> A...
	
        length = 2;

        if(!are_on_the_same_path(signature, 0, 0, w)){
            // 1 2
			new_sig = change_to_a(new_sig, 0, w);
			next_signatures[2] = new_sig >> 2;
			next_signatures[2] += (base_X + (base_X/4)); // 1111... -> XX...
			
            length++;
        }
    }
    else if(((signature & base_X) == base_O) && ((signature & 12) == 0) && ((signature & 3) == 1)){ // O...AB
        // 2
		next_signatures[0] = signature >> 2;
		next_signatures[0] += 1; // 00...01 -> A...B
        // 1 2
		next_signatures[1] = signature >> 2;
		next_signatures[1] += (base_X - (base_O/4) + 1); // 1100...01 -> XA...B
		
        length = 2;
    }
    return length;
}