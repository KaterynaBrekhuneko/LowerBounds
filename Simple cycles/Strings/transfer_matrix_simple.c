#include "transfer_matrix_simple.h"

long long binary_search(char **valid_signatures, char *signature, long long l, long long r, int w)
{
    if (r >= l) {
        long long m = l + (r - l) / 2;

        for(int i = 0; i<w+1; i++){
            if(valid_signatures[m][i] != signature[i]){
                if(valid_signatures[m][i] > signature[i]){
                    return binary_search(valid_signatures, signature, l, m - 1, w);
                }
                else{
                    return binary_search(valid_signatures, signature, m + 1, r, w);
                }
            }
        }

        return m;
    }
    return -1;
}

long long find_index(char ***valid_signatures, char *signature, long long *counts, int w){
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

bool is_valid_signature(char *signature, int w){

    bool exception = (signature[0] == 'X') && (signature[1] == 'O') && (signature[w] == 'O');
	
	if(exception){
		return false;
	}
	
	int number_a = 0;
	int number_b = 0;
	
	for(int i = 0; i<w+1; i++){
		if(signature[i] == 'A'){
			number_a++;
		}
		else if(signature[i] == 'B'){
			number_b++;
		}
		
		if(number_a < number_b){
			return false;
		}
	}
	
    return number_a != 0 && (number_a == number_b);
}

void construct_signatures(char **signatures, unsigned int dimension, int w, unsigned int n){
    char *states[] = {"A", "B", "O", "X"};	
	unsigned int d = dimension/GROUP_SIZE;
	
	char *c;
	
	if(n < dimension/3){
		c = "A";
	}
	else if(n < 2*(dimension/3)){
		c = "O";
	}
	else{
		c = "X";
	}

    //Set first State
    for(unsigned int i = 0; i<d; i++){
        signatures[i] = (char*)malloc((w+2)* sizeof(char));
		signatures[i][0] = '\0';
		
		strcat(signatures[i], c);
    }

    // Set "middle" states
    int state_number = 0;
    unsigned int period = (dimension/3)/4;
	
	unsigned int current;

    for(int i = 0; i < w-1; i++){
        current = period - (n - period*(n/period));	
		state_number = ((n/period) % 4);
		
        for(unsigned int j = 0; j<d; j++){
            if(current == 0){
                current = period;
                state_number = (state_number+1) % 4;
            }
			strcat(signatures[j], states[state_number]);			
            current--;
        }
        period = period/4;
    }

    //Set last State (last state can not be A)
	int start = 0;
	if(n%3 == 0){
		start = 0;
	}
	else if(n%3 == 1){
		start = 1;
	}
	else{
		start = 2;
	}
	
    for(unsigned int i = 0; i<d; i++){
		if((i+start)%3 == 0){
			strcat(signatures[i], states[1]);
        }
        else if((i+start)%3 == 1){
            strcat(signatures[i], states[2]);
        }
        else{
            strcat(signatures[i], states[3]);
        }
    }
}

void save_valid_signatures(char **valid_signatures, char **signatures, unsigned int dimension, int w){
    unsigned int j = 0;

    for(unsigned int i = 0; i<dimension; i++){
        if(is_valid_signature(signatures[i], w)){
            valid_signatures[j] = (char*)malloc((w+2) * sizeof(char));
            strcpy(valid_signatures[j], signatures[i]);
            j++;
        }
    }
}

unsigned int count_valid_signatures(char **valid_signatures, unsigned int dimension, int w){
    unsigned int count = 0;
	
    for(unsigned int i = 0; i<dimension; i++){
        if(is_valid_signature(valid_signatures[i], w)){
            count++;
        }
    }
    return count;
}

void change_char(char *signature, char *signature_copy, int i, char *c, int w){
    strncpy(signature_copy, signature, i);
    signature_copy[i] = '\0';
    strcat(signature_copy, c);
    strncat(signature_copy, signature+i+1, strlen(signature)-i-1);
}

char* change_to_a(char *signature, char *signature_copy, int number_of_a, int w){
    for(int i = 0; i<strlen(signature); i++){
        char c = signature[i];
        if(c == 'A'){
            number_of_a ++;
        }
        if(c == 'B'){
            number_of_a --;
            if(number_of_a == 0){
                change_char(signature, signature_copy, i, "A", w);
                break;
            }
        }
    }
    return signature_copy;
}

char* change_to_b(char *signature, char *signature_copy, int number_of_b, int w){
    for(int i = strlen(signature)-1; i>=0; i--){
        char c = signature[i];
        if(c == 'B'){
            number_of_b ++;
        }
        if(c == 'A'){
            number_of_b --;
            if(number_of_b == 0){
                change_char(signature, signature_copy, i, "B", w);
                break;
            }
        }
    }
    return signature_copy;
}

bool are_on_the_same_path(char *signature, int index_a, int offset_b){
    int index_a_calculated = 0;

    for(int i = strlen(signature)-1; i>=0; i--){
        char c = signature[i];
        if(c == 'B'){
            offset_b ++;
        }
        if(c == 'A'){
            offset_b --;
            if(offset_b == 0){
                index_a_calculated = i;
                break;
            }
        }
    }
    return (index_a == index_a_calculated);
}

int compute_successor_signatures(char **next_signatures, char *signature, int w){
    int length = 0;

    if(signature[0] == 'X' && signature[w-1] == 'X' && signature[w] == 'X'){
        // keine
        next_signatures[0] = (char *)malloc(w+2);
        strcpy(next_signatures[0], "O");
        strncat(next_signatures[0], signature, w);
        length = 1;
    }
    else if(signature[0] == 'O' && signature[w-1] == 'X' && signature[w] == 'X'){
        // 1
        next_signatures[0] = (char *)malloc(w+2);
        strcpy(next_signatures[0], "AB");
        strncat(next_signatures[0], signature+1, w-1);
        // keine
        next_signatures[1] = (char *)malloc(w+2);
        strcpy(next_signatures[1], "O");
        strncat(next_signatures[1], signature, w);
        length = 2;
    }
    else if(signature[0] == 'X' && signature[w-1]== 'O' && signature[w] == 'X'){
        // 3
        next_signatures[0] = (char *)malloc(w+2);
        strcpy(next_signatures[0], "A");
        strncat(next_signatures[0], signature, w-1);
        strcat(next_signatures[0], "B");
        // keine
        next_signatures[1] = (char *)malloc(w+2);
        strcpy(next_signatures[1], "O");
        strncat(next_signatures[1], signature, w);
        length = 2;
    }
    else if(signature[0] == 'X' && signature[w-1] == 'X' && signature[w] == 'O'){
        // keine
        next_signatures[0] = (char *)malloc(w+2);
        strcpy(next_signatures[0], "O");
        strncat(next_signatures[0], signature, w);
        length = 1;
    }
    else if(signature[0] == 'O' && signature[w-1] == 'O' && signature[w] == 'O'){
        // 1
        next_signatures[0] = (char *)malloc(w+2);
        strcpy(next_signatures[0], "AB");
        strncat(next_signatures[0], signature+1, w-1);
        // 3
        next_signatures[1] = (char *)malloc(w+2);
        strcpy(next_signatures[1], "A");
        strncat(next_signatures[1], signature, w-1);
        strcat(next_signatures[1], "B");
        // 1 3
        next_signatures[2] = (char *)malloc(w+2);
        strcpy(next_signatures[2], "XA");
        strncat(next_signatures[2], signature+1, w-2);
        strcat(next_signatures[2], "B");
        // keine
        next_signatures[3] = (char *)malloc(w+2);
        strcpy(next_signatures[3], "O");
        strncat(next_signatures[3], signature, w);
        length = 4;
    }
    else if(signature[0] == 'X' && signature[w-1] == 'O' && signature[w] == 'O'){
        // 3
        next_signatures[0] = (char *)malloc(w+2);
        strcpy(next_signatures[0], "A");
        strncat(next_signatures[0], signature, w-1);
        strcat(next_signatures[0], "B");
        // keine
        next_signatures[1] = (char *)malloc(w+2);
        strcpy(next_signatures[1], "O");
        strncat(next_signatures[1], signature, w);
        length = 2;
    }
    else if(signature[0] == 'O' && signature[w-1] == 'X' && signature[w] == 'O'){
        // 1
        next_signatures[0] = (char *)malloc(w+2);
        strcpy(next_signatures[0], "AB");
        strncat(next_signatures[0], signature+1, w-1);
        // keine
        next_signatures[1] = (char *)malloc(w+2);
        strcpy(next_signatures[1], "O");
        strncat(next_signatures[1], signature, w);
        length = 2;
    }
    else if(signature[0] == 'O' && signature[w-1] == 'O' && signature[w] == 'X'){
        // 1
        next_signatures[0] = (char *)malloc(w+2);
        strcpy(next_signatures[0], "AB");
        strncat(next_signatures[0], signature+1, w-1);
        // 3
        next_signatures[1] = (char *)malloc(w+2);
        strcpy(next_signatures[1], "A");
        strncat(next_signatures[1], signature, w-1);
        strcat(next_signatures[1], "B");
        // 1 3
        next_signatures[2] = (char *)malloc(w+2);
        strcpy(next_signatures[2], "XA");
        strncat(next_signatures[2], signature+1, w-2);
        strcat(next_signatures[2], "B");
        // keine
        next_signatures[3] = (char *)malloc(w+2);
        strcpy(next_signatures[3], "O");
        strncat(next_signatures[3], signature, w);
        length = 4;
    }
    else if(signature[0] == 'A' && signature[w-1] == 'X' && signature[w] == 'X'){
        // 1
        next_signatures[0] = (char *)malloc(w+2);
        strcpy(next_signatures[0], "AX");
        strncat(next_signatures[0], signature+1, w-1);
        // keine
        next_signatures[1] = (char *)malloc(w+2);
        strcpy(next_signatures[1], "O");
        strncat(next_signatures[1], signature, w);
        length = 2;
    }
    else if(signature[0] == 'A' && signature[w-1] == 'O' && signature[w] == 'O'){
        // 1
        next_signatures[0] = (char *)malloc(w+2);
        strcpy(next_signatures[0], "AX");
        strncat(next_signatures[0], signature+1, w-1);
        // 3
        next_signatures[1] = (char *)malloc(w+2);
        strcpy(next_signatures[1], "A");
        strncat(next_signatures[1], signature, w-1);
        strcat(next_signatures[1], "B");
        // keine
        next_signatures[2] = (char *)malloc(w+2);
        strcpy(next_signatures[2], "O");
        strncat(next_signatures[2], signature, w);
         // 1 3
        char *new_sig = (char*)malloc((strlen(signature)+1) * sizeof(char));
        change_to_a(signature, new_sig, 0, w);
        next_signatures[3] = (char *)malloc(w+2);
        strcpy(next_signatures[3], "XX");
        strncat(next_signatures[3], new_sig+1, w-2);
        strcat(next_signatures[3], "B");
        length = 4;
        
        free(new_sig);
    }
    else if(signature[0] == 'X' && signature[w-1] == 'B' && signature[w] == 'X'){
        // keine
        next_signatures[0] = (char *)malloc(w+2);
        strcpy(next_signatures[0], "O");
        strncat(next_signatures[0], signature, w);
        // 3
        char *new_sig = (char*)malloc((strlen(signature)+1) * sizeof(char));
        change_to_b(signature, new_sig, 0, w);
        next_signatures[1] = (char *)malloc(w+2);
        strcpy(next_signatures[1], "A");
        strncat(next_signatures[1], new_sig, w-1);
        strcat(next_signatures[1], "X");
        length = 2;

        free(new_sig);
    }
    else if(signature[0] == 'X' && signature[w-1] == 'X' && signature[w] == 'B'){
        // 2
        char *new_sig = (char*)malloc((strlen(signature)+1) * sizeof(char));
        change_to_b(signature, new_sig, 0, w);
        next_signatures[0] = (char *)malloc(w+2);
        strcpy(next_signatures[0], "A");
        strncat(next_signatures[0], new_sig, w);
        length = 1;

        free(new_sig);
    }
    else if(signature[0] == 'X' && signature[w-1] == 'B' && signature[w] == 'B'){
        // 2
        char *new_sig = (char*)malloc((strlen(signature)+1) * sizeof(char));
        change_to_b(signature, new_sig, 0, w);
        next_signatures[0] = (char *)malloc(w+2);
        strcpy(next_signatures[0], "A");
        strncat(next_signatures[0], new_sig, w);
        // 2 3
        char *new_sig2 = (char*)malloc((strlen(signature)+1) * sizeof(char));
        change_to_b(signature, new_sig2, -1, w);
        next_signatures[1] = (char *)malloc(w+2);
        strcpy(next_signatures[1], "X");
        strncat(next_signatures[1], new_sig2, w-1);
        strcat(next_signatures[1], "X");
        length = 2;

        free(new_sig);
        free(new_sig2);
    }  
    else if(signature[0] == 'O' && signature[w-1] == 'B' && signature[w] == 'O'){
        // 1
        next_signatures[0] = (char *)malloc(w+2);
        strcpy(next_signatures[0], "AB");
        strncat(next_signatures[0], signature+1, w-1);
        // keine
        next_signatures[1] = (char *)malloc(w+2);
        strcpy(next_signatures[1], "O");
        strncat(next_signatures[1], signature, w);
        // 3
        char *new_sig = (char*)malloc((strlen(signature)+1) * sizeof(char));
        change_to_b(signature, new_sig, 0, w);
        next_signatures[2] = (char *)malloc(w+2);
        strcpy(next_signatures[2], "A");
        strncat(next_signatures[2], new_sig, w-1);
        strcat(next_signatures[2], "X");
        // 1 3
        next_signatures[3] = (char *)malloc(w+2);
        strcpy(next_signatures[3], "XA");
        strncat(next_signatures[3], new_sig+1, w-2);
        strcat(next_signatures[3], "X");
        length = 4;

        free(new_sig);
    }   
    else if(signature[0] == 'O' && signature[w-1] == 'O' && signature[w] == 'B'){
        // 2 3
        next_signatures[0] = (char *)malloc(w+2);
        strcpy(next_signatures[0], "X");
        strncat(next_signatures[0], signature, w-1);
        strcat(next_signatures[0], "B");
        // 2
        char *new_sig = (char*)malloc((strlen(signature)+1) * sizeof(char));
        change_to_b(signature, new_sig, 0, w);
        next_signatures[1] = (char *)malloc(w+2);
        strcpy(next_signatures[1], "A");
        strncat(next_signatures[1], new_sig, w);
        // 1 2
        next_signatures[2] = (char *)malloc(w+2);
        strcpy(next_signatures[2], "XA");
        strncat(next_signatures[2], new_sig+1, w-1);
        length = 3;

        free(new_sig);
    }
    else if(signature[0] == 'O' && signature[w-1] == 'B' && signature[w] == 'B'){  
        // 2
        char *new_sig = (char*)malloc((strlen(signature)+1) * sizeof(char));
        change_to_b(signature, new_sig, 0, w);
        next_signatures[0] = (char *)malloc(w+2);
        strcpy(next_signatures[0], "A");
        strncat(next_signatures[0], new_sig, w);
        // 1 2
        next_signatures[1] = (char *)malloc(w+2);
        strcpy(next_signatures[1], "XA");
        strncat(next_signatures[1], new_sig+1, w-1);
        // 2 3
        char *new_sig2 = (char*)malloc((strlen(signature)+1) * sizeof(char));
        change_to_b(signature, new_sig2, -1, w);
        next_signatures[2] = (char *)malloc(w+2);
        strcpy(next_signatures[2], "X");
        strncat(next_signatures[2], new_sig2, w-1);
        strcat(next_signatures[2], "X");
        length = 3;

        free(new_sig);
        free(new_sig2);
    } 
    else if(signature[0] == 'A' && signature[w-1] == 'X' && signature[w] == 'O'){  
        // 1
        next_signatures[0] = (char *)malloc(w+2);
        strcpy(next_signatures[0], "AX");
        strncat(next_signatures[0], signature+1, w-1);
        // keine
        next_signatures[1] = (char *)malloc(w+2);
        strcpy(next_signatures[1], "O");
        strncat(next_signatures[1], signature, w);
        length = 2;
    }
    else if(signature[0] == 'A' && signature[w-1] == 'O' && signature[w] == 'X'){  
        // 1
        next_signatures[0] = (char *)malloc(w+2);
        strcpy(next_signatures[0], "AX");
        strncat(next_signatures[0], signature+1, w-1);
        // 3
        next_signatures[1] = (char *)malloc(w+2);
        strcpy(next_signatures[1], "A");
        strncat(next_signatures[1], signature, w-1);
        strcat(next_signatures[1], "B");
        // keine
        next_signatures[2] = (char *)malloc(w+2);
        strcpy(next_signatures[2], "O");
        strncat(next_signatures[2], signature, w);
        // 1 3
        char *new_sig = (char*)malloc((strlen(signature)+1) * sizeof(char));
        change_to_a(signature, new_sig, 0, w);
        next_signatures[3] = (char *)malloc(w+2);
        strcpy(next_signatures[3], "XX");
        strncat(next_signatures[3], new_sig+1, w-2);
        strcat(next_signatures[3], "B");
        length = 4;

        free(new_sig);
    }
    else if(signature[0] == 'O' && signature[w-1] == 'B' && signature[w] == 'X'){  
        // 1
        next_signatures[0] = (char *)malloc(w+2);
        strcpy(next_signatures[0], "AB");
        strncat(next_signatures[0], signature+1, w-1);
        // keine
        next_signatures[1] = (char *)malloc(w+2);
        strcpy(next_signatures[1], "O");
        strncat(next_signatures[1], signature, w);
        // 3
        char *new_sig = (char*)malloc((strlen(signature)+1) * sizeof(char));
        change_to_b(signature, new_sig, 0, w);
        next_signatures[2] = (char *)malloc(w+2);
        strcpy(next_signatures[2], "A");
        strncat(next_signatures[2], new_sig, w-1);
        strcat(next_signatures[2], "X");
        // 1 3  
        next_signatures[3] = (char *)malloc(w+2);
        strcpy(next_signatures[3], "XA");
        strncat(next_signatures[3], new_sig+1, w-2);
        strcat(next_signatures[3], "X");     
        length = 4;  

        free(new_sig);
    }
    else if(signature[0] == 'O' && signature[w-1] == 'X' && signature[w] == 'B'){  
         // 2
        char *new_sig = (char*)malloc((strlen(signature)+1) * sizeof(char));
        change_to_b(signature, new_sig, 0, w);
        next_signatures[0] = (char *)malloc(w+2);
        strcpy(next_signatures[0], "A");
        strncat(next_signatures[0], new_sig, w);
        // 1 2
        next_signatures[1] = (char *)malloc(w+2);
        strcpy(next_signatures[1], "XA");
        strncat(next_signatures[1], new_sig+1, w-1);
        length = 2;

        free(new_sig);
    }
    else if(signature[0] == 'X' && signature[w-1] == 'B' && signature[w] == 'O'){  
        // keine
        next_signatures[0] = (char *)malloc(w+2);
        strcpy(next_signatures[0], "O");
        strncat(next_signatures[0], signature, w);
        // 3
        char *new_sig = (char*)malloc((strlen(signature)+1) * sizeof(char));
        change_to_b(signature, new_sig, 0, w);
        next_signatures[1] = (char *)malloc(w+2);
        strcpy(next_signatures[1], "A");
        strncat(next_signatures[1], new_sig, w-1);
        strcat(next_signatures[1], "X");
        length = 2;

        free(new_sig);
    }
    else if(signature[0] == 'X' && signature[w-1] == 'O' && signature[w] == 'B'){  
        // 2 3
        next_signatures[0] = (char *)malloc(w+2);
        strcpy(next_signatures[0], "X");
        strncat(next_signatures[0], signature, w-1);
        strcat(next_signatures[0], "B");
        // 2
        char *new_sig = (char*)malloc((strlen(signature)+1) * sizeof(char));
        change_to_b(signature, new_sig, 0, w);
        next_signatures[1] = (char *)malloc(w+2);
        strcpy(next_signatures[1], "A");
        strncat(next_signatures[1], new_sig, w);
        length = 2;

        free(new_sig);
    }
    //---------------------------
    else if(signature[0] == 'A' && signature[w-1] == 'B' && signature[w] == 'B'){  
        // 2
        char *new_sig = (char*)malloc((strlen(signature)+1) * sizeof(char));
        change_to_b(signature, new_sig, 0, w);
        next_signatures[0] = (char *)malloc(w+2);
        strcpy(next_signatures[0], "A");
        strncat(next_signatures[0], new_sig, w);

        int i = 1;
        if(!are_on_the_same_path(signature, 0, 0)){
            // 1 2
            char *new_sig2 = (char*)malloc((strlen(signature)+1) * sizeof(char));
            change_to_a(new_sig, new_sig2, 0, w);
            next_signatures[i] = (char *)malloc(w+2);
            strcpy(next_signatures[i], "XX");
            strncat(next_signatures[i], new_sig2+1, w-1);
            i++;
            length++;
            free(new_sig2);
        }
        // 2 3
        char *new_sig3 = (char*)malloc((strlen(signature)+1) * sizeof(char));
        change_to_b(signature, new_sig3, -1, w);
        next_signatures[i] = (char *)malloc(w+2);
        strcpy(next_signatures[i], "X");
        strncat(next_signatures[i], new_sig3, w-1);
        strcat(next_signatures[i], "X");
        length +=2;

        free(new_sig);
        free(new_sig3);
    }
    else if(signature[0] == 'A' && signature[w-1] == 'A' && signature[w] == 'B'){  
        // 2
        next_signatures[0] = (char *)malloc(w+2);
        strcpy(next_signatures[0], "A");
        strncat(next_signatures[0], signature, w-1);
        strcat(next_signatures[0], "B");
        // 1 2
        char *new_sig = (char*)malloc((strlen(signature)+1) * sizeof(char));
        change_to_a(signature, new_sig, 0, w);
        next_signatures[1] = (char *)malloc(w+2);
        strcpy(next_signatures[1], "XX");
        strncat(next_signatures[1], new_sig+1, w-2);
        strcat(next_signatures[1], "B");

        length = 2;

        free(new_sig);
    }
    else if(signature[0] == 'A' && signature[w-1] == 'B' && signature[w] == 'X'){  
        // 1
        next_signatures[0] = (char *)malloc(w+2);
        strcpy(next_signatures[0], "AX");
        strncat(next_signatures[0], signature+1, w-2);
        strcat(next_signatures[0], "B");
        // keine
        next_signatures[1] = (char *)malloc(w+2);
        strcpy(next_signatures[1], "O");
        strncat(next_signatures[1], signature, w);
        // 3
        char *new_sig = (char*)malloc((strlen(signature)+1) * sizeof(char));
        change_to_b(signature, new_sig, 0, w);
        next_signatures[2] = (char *)malloc(w+2);
        strcpy(next_signatures[2], "A");
        strncat(next_signatures[2], new_sig, w-1);
        strcat(next_signatures[2], "X");
        length = 3;

        if(!are_on_the_same_path(signature, 0, 0)){
            // 1 2
            char *new_sig2 = (char*)malloc((strlen(signature)+1) * sizeof(char));
            change_to_a(new_sig, new_sig2, 0, w);
            next_signatures[3] = (char *)malloc(w+2);
            strcpy(next_signatures[3], "XX");
            strncat(next_signatures[3], new_sig2+1, w-2);
            strcat(next_signatures[3], "X");
            length++;

            free(new_sig2);
        }
        free(new_sig);
    }
    if(signature[0] == 'A' && signature[w-1] == 'X' && signature[w] == 'B'){ 
        // 2
        char *new_sig = (char*)malloc((strlen(signature)+1) * sizeof(char));
        change_to_b(signature, new_sig, 0, w);
        next_signatures[0] = (char *)malloc(w+2);
        strcpy(next_signatures[0], "A");
        strncat(next_signatures[0], new_sig, w);
        length = 1;

        if(!are_on_the_same_path(signature, 0, 0)){
            // 1 2
            char *new_sig2 = (char*)malloc((strlen(signature)+1) * sizeof(char));
            change_to_a(new_sig, new_sig2, 0, w);
            next_signatures[1] = (char *)malloc(w+2);
            strcpy(next_signatures[1], "XX");
            strncat(next_signatures[1], new_sig2+1, w-1);
            length++;

            free(new_sig2);
        }
        free(new_sig);
    }
    else if(signature[0] == 'X' && signature[w-1] == 'A' && signature[w] == 'B'){ 
        // 2
        next_signatures[0] = (char *)malloc(w+2);
        strcpy(next_signatures[0], "A");
        strncat(next_signatures[0], signature, w-1);
        strcat(next_signatures[0], "B");

        length = 1;
    }
    else if(signature[0] == 'A' && signature[w-1] == 'B' && signature[w] == 'O'){
        // 1
        next_signatures[0] = (char *)malloc(w+2);
        strcpy(next_signatures[0], "AX");
        strncat(next_signatures[0], signature+1, w-1);
        // keine
        next_signatures[1] = (char *)malloc(w+2);
        strcpy(next_signatures[1], "O");
        strncat(next_signatures[1], signature, w);
        // 3
        char *new_sig = (char*)malloc((strlen(signature)+1) * sizeof(char));
        change_to_b(signature, new_sig, 0, w);
        next_signatures[2] = (char *)malloc(w+2);
        strcpy(next_signatures[2], "A");
        strncat(next_signatures[2], new_sig, w-1);
        strcat(next_signatures[2], "X");
        length = 3;

        if(!are_on_the_same_path(signature, 0, 0)){
            // 1 3
            char *new_sig2 = (char*)malloc((strlen(signature)+1) * sizeof(char));
            change_to_a(new_sig, new_sig2, 0, w);
            next_signatures[3] = (char *)malloc(w+2);
            strcpy(next_signatures[3], "XX");
            strncat(next_signatures[3], new_sig2+1, w-2);
            strcat(next_signatures[3], "X");
            length++;

            free(new_sig2);
        }

        free(new_sig);
    }
    else if(signature[0] == 'A' && signature[w-1] == 'O' && signature[w] == 'B'){
        // 2 3
        next_signatures[0] = (char *)malloc(w+2);
        strcpy(next_signatures[0], "XA");
        strncat(next_signatures[0], signature+1, w-2);
        strcat(next_signatures[0], "B");
        // 1 2
        char *new_sig = (char*)malloc((strlen(signature)+1) * sizeof(char));
        change_to_b(signature, new_sig, 0, w);
        next_signatures[1] = (char *)malloc(w+2);
        strcpy(next_signatures[1], "A");
        strncat(next_signatures[1], new_sig, w);
        length = 2;

        if(!are_on_the_same_path(signature, 0, 0)){
            // 1 3
            char *new_sig2 = (char*)malloc((strlen(signature)+1) * sizeof(char));
            change_to_a(new_sig, new_sig2, 0, w);
            next_signatures[2] = (char *)malloc(w+2);
            strcpy(next_signatures[2], "XX");
            strncat(next_signatures[2], new_sig2+1, w-1);
            length++;

            free(new_sig2);
        }
        free(new_sig);
    }
    else if(signature[0] == 'O' && signature[w-1] == 'A' && signature[w] == 'B'){
        // 2
        next_signatures[0] = (char *)malloc(w+2);
        strcpy(next_signatures[0], "A");
        strncat(next_signatures[0], signature, w-1);
        strcat(next_signatures[0], "B");
        // 1 2
        next_signatures[1] = (char *)malloc(w+2);
        strcpy(next_signatures[1], "XA");
        strncat(next_signatures[1], signature+1, w-2);
        strcat(next_signatures[1], "B");

        length = 2;
    }
    return length;
}