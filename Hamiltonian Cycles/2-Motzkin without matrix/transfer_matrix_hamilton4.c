#include "transfer_matrix_hamilton4.h"

unsigned long long number_of_motzkin(int w){
	unsigned long long *C = (unsigned long long *)malloc((w+1)*sizeof(unsigned long long *));
	
	unsigned long long C_n = 0;	
	C[0] = 1;
	C[1] = 1;
	
	for(int i = 2; i<=w+1; i++){
		for(int j = 0; j<i; j++){
			C_n += C[j]*C[i-j-1];
		}	
		C[i] = C_n;
		C_n = 0;
	}
	
	unsigned long long n = C[w+1];
	free(C);
	
	return n;
}

void construct_S(unsigned long long **S, int w){
	//assign 1s to the rightmost diagonal
	int j = 0;
	for(int i = w; i>=(w - (w)/2); i--){
		S[i][j] = 1;
		j++;
	}
	
	// calculate the height of every column
	unsigned long long *heights = (unsigned long long *)malloc((w+1) * sizeof(unsigned long long));
	j = 1;
	for(int i = 0; i<w+1; i++){
		if(i<(w+1)/2){
			heights[i] = j;
			j++;
		}
		else{
			if(((w+1)%2) == 1){
				heights[i] = j;	
			}			
			j--;
			if(((w+1)%2) == 0){
				heights[i] = j;	
			}
		}
	}
	
	//calculate the rest of the values
	unsigned long long n = 0;
	for(int i = w; i>=0; i--){
		for(j = 0; j<heights[i]; j++){
			if(S[i][j]==1){
				break;
			}
			else{
				n = S[i+1][j];
				
				if(i != w-1){
					n = n + S[i+1][j];
				}
				
				if(j != (heights[i+1]-1)){
					n = n + S[i+1][j+1];
				}
				
				if(j!=0){
					n = n + S[i+1][j-1];
				}
				
				S[i][j] = n;
				n = 0;
			}
		}
	}
	
	free(heights);
}

unsigned long long rank(unsigned long long path, unsigned long long **S, int w){
	unsigned long long n = 1;
	int i = 0;
	int j = 0;
	
	unsigned long long base = 3*pow(4, w);
	
	for(int a = 0; a<w+1; a++){
		if((path & base) == 0){ // A
			n += 2*S[i+1][j];
			if(j!=0){
				n += S[i+1][j-1];
			}
			j++;
		}
		else if((path & base) == (base/3)){ // B
			n += 2*S[i+1][j];
			j--;
		}
		else if((path & base) == 2*(base/3)){ //O
			n += S[i+1][j];
		}
		else{
			
		}
		
		i++;
		base = (base >> 2);
	}
	
	return n;
}

unsigned long long unrank(unsigned long long n, unsigned long long **S, int w){
	unsigned long long path = 0;
	int i = 0;
	int j = 0;
	
	unsigned long long base = 3*pow(4, w);
	
	for(int a = 0; a<w+1; a++){
		if(n <= S[i+1][j]){ //X
			path += base;
		}
		else if(n <= (2*S[i+1][j])){//O
			path += 2*(base/3);
			n -= S[i+1][j];
		}
		else if(j!= 0 && n <= (S[i+1][j-1] + 2*S[i+1][j])){//B
			path += base/3;
			n -= 2*S[i+1][j];
			j--;
		}
		else{//A
			n -= 2*S[i+1][j];
			if(j!=0){
				n -= S[i+1][j-1];
			}
			j++;
		}
		
		i++;
		base = (base >> 2);
	}
	
	return path;
}

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

bool is_valid_signature(unsigned long long signature, int w){
	
	unsigned long long base = 3*pow(4, w);
	
	if((signature & 3) == 2){
		return false;
	}
	
	for(int i = 0; i<w+1; i++){
		if((signature & base) == 0 || (signature & base) == base/3){
			return true;
		}
		
		base = base >> 2;
	}
	
    return false;
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
	
	
	if(is_valid_signature(signature, w)){
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
			
			length = 1;
		}
		else if(((signature & base_X) == base_O) && ((signature & 12) == 8) && ((signature & 3) == 3)){ // O...OX
			// 3
			next_signatures[0] = signature >> 2;
			next_signatures[0] -= 1; // 00...01 -> A...B
			// 1 3
			next_signatures[1] = signature >> 2;
			next_signatures[1] += (base_X - (base_O/4) - 1); // 1100...01 -> XA...B
			
			length = 2;
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
		else if(((signature & base_X) == base_O) && ((signature & 12) == 8) && ((signature & 3) == 1)){ //O...OB
			// 2 3
			next_signatures[0] = signature >> 2;
			next_signatures[0] += (base_X - 1); // 11...01 -> X...B
			
			length = 1;
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
		else if(((signature & base_X) == 0) && ((signature & 12) == 8) && ((signature & 3) == 3)){ // A...OX

			// 3
			next_signatures[0] = signature >> 2;
			next_signatures[0] -= 1; // 00...01 -> A...B
			// 1 3
			next_signatures[1] = change_to_a(signature, 0, w);
			next_signatures[1] = next_signatures[1] >> 2;
			next_signatures[1] += (base_X + (base_X/4) - 1); // 1111...01 -> XX...B

			length = 2;
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
		else if(((signature & base_X) == base_X) && ((signature & 12) == 8) && ((signature & 3) == 1)){ // X...OB 
			// 2 3
			next_signatures[0] = signature >> 2;
			next_signatures[0] += (base_X - 1); // 11...01 -> X...B
			
			length = 1;
		}
		//---------------------------
		else if(((signature & base_X) == 0) && ((signature & 12) == 4) && ((signature & 3) == 1)){ // A...BB 
			// 2
			unsigned long long new_sig = change_to_b(signature, 0, w);
			next_signatures[0] = new_sig >> 2;
			next_signatures[0] += 0; // 00... -> A...	

			int i = 1;
			if(are_on_the_same_path(signature, 0, 0, w)){
				//cycles++;
			}
			else{
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
			
			//cycles++;
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

			if(are_on_the_same_path(signature, 0, 0, w)){
				//cycles++;
			}
			else{
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

			if(are_on_the_same_path(signature, 0, 0, w)){
				//cycles++;
			}
			else{
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
			
			//cycles++;
			length = 1;
		}
		else if(((signature & base_X) == 0) && ((signature & 12) == 8) && ((signature & 3) == 1)){ // A...OB
			// 2 3
			next_signatures[0] = signature >> 2;
			next_signatures[0] += (base_X - 1); // 1100...01 -> XA...B

			length = 1;
		}
		else if(((signature & base_X) == base_O) && ((signature & 12) == 0) && ((signature & 3) == 1)){ // O...AB
			// 2
			next_signatures[0] = signature >> 2;
			next_signatures[0] += 1; // 00...01 -> A...B
			// 1 2
			next_signatures[1] = signature >> 2;
			next_signatures[1] += (base_X - (base_O/4) + 1); // 1100...01 -> XA...B
			
			//cycles++;
			length = 2;
		}
	}	
    return length;
}
