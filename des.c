//
// Created by dong on 22-4-16.
//
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define BLOCKSIZE 64
#define KEYSIZE 56
#define HALF_KEYSIZE 28
#define FAKE_KEYSIZE 64
#define ROUND_KEYSIZE 48
#define ROUND 16

// char ? '0','1'
typedef unsigned int bit_t;

const int IP_table[8][8] = {
        {58, 50, 42, 34, 26, 18, 10, 2},
        {60, 52, 44, 36, 28, 20, 12, 4},
        {62, 54, 46, 38, 30, 22, 14, 6},
        {64, 56, 48, 40, 32, 24, 16, 8},
        {57, 49, 41, 33, 25, 17, 9,  1},
        {59, 51, 43, 35, 27, 19, 11, 3},
        {61, 53, 45, 37, 29, 21, 13, 5},
        {63, 55, 47, 39, 31, 23, 15, 7}
};
const int Perm_table[8][4] = {

        {16, 7,  20, 21},
        {29, 12, 28, 17},
        {1,  15, 23, 26},
        {5,  18, 31, 10},
        {2,  8,  24, 14},
        {32, 27, 3,  9},
        {19, 13, 30, 6},
        {22, 11, 4,  25}
};
const int PC1_table[8][7] = {
        {57, 49, 41, 33, 25, 17, 9},
        {1,  58, 50, 42, 34, 26, 18},
        {10, 2,  59, 51, 43, 35, 27},
        {19, 11, 3,  60, 52, 44, 36},
        {63, 55, 47, 39, 31, 23, 15},
        {7,  62, 54, 46, 38, 30, 22},
        {14, 6,  61, 53, 45, 37, 29},
        {21, 13, 5,  28, 20, 12, 4}
};
const int PC2_table[8][6] = {
        {14, 17, 11, 24, 1,  5},
        {3,  28, 15, 6,  21, 10},
        {23, 19, 12, 4,  26, 8},
        {16, 7,  27, 20, 13, 2},
        {41, 52, 31, 37, 47, 55},
        {30, 40, 51, 45, 33, 48},
        {44, 49, 39, 56, 34, 53},
        {46, 42, 50, 36, 29, 32}
};
const int Expension_table[8][6] = {
        {32, 1,  2,  3,  4,  5},
        {4,  5,  6,  7,  8,  9},
        {8,  9,  10, 11, 12, 13},
        {12, 13, 14, 15, 16, 17},
        {16, 17, 18, 19, 20, 21},
        {20, 21, 22, 23, 24, 25},
        {24, 25, 26, 27, 28, 29},
        {28, 29, 30, 31, 32, 1}
};
const int IP_invert_table[8][8] = {
        {40, 8, 48, 16, 56, 24, 64, 32},
        {39, 7, 47, 15, 55, 23, 63, 31},
        {38, 6, 46, 14, 54, 22, 62, 30},
        {37, 5, 45, 13, 53, 21, 61, 29},
        {36, 4, 44, 12, 52, 20, 60, 28},
        {35, 3, 43, 11, 51, 19, 59, 27},
        {34, 2, 42, 10, 50, 18, 58, 26},
        {33, 1, 41, 9,  49, 17, 57, 25}
};
const int Left_shift_round_table[16] = {
        1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
};

const int S_boxes[8][4][16] = {
        {
                {14, 4,  13, 1,  2,  15, 11, 8,  3,  10, 6,  12, 5,  9,  0,  7},
                {0,  15, 7,  4,  14, 2,  13, 1,  10, 6, 12, 11, 9,  5,  3,  8},
                {4,  1,  14, 8,  13, 6,  2,  11, 15, 12, 9,  7,  3,  10, 5,  0},
                {15, 12, 8,  2,  4,  9,  1,  7,  5,  11, 3,  14, 10, 0, 6,  13}
        },
        {
                {15, 1,  8,  14, 6,  11, 3,  4,  9,  7,  2,  13, 12, 0,  5,  10},
                {3,  13, 4,  7,  15, 2,  8,  14, 12, 0, 1,  10, 6,  9,  11, 5},
                {0,  14, 7,  11, 10, 4,  13, 1,  5,  8,  12, 6,  9,  3,  2,  15},
                {13, 8,  10, 1,  3,  15, 4,  2,  11, 6,  7,  12, 0,  5, 14, 9}

        },
        {
                {10, 0,  9,  14, 6,  3,  15, 5,  1,  13, 12, 7,  11, 4,  2,  8},
                {13, 7,  0,  9,  3,  4,  6,  10, 2,  8, 5,  14, 12, 11, 15, 1},
                {13, 6,  4,  9,  8,  15, 3,  0,  11, 1,  2,  12, 5,  10, 14, 7},
                {1,  10, 13, 0,  6,  9,  8,  7,  4,  15, 14, 3,  11, 5, 2,  12}
        },
        {
                {7,  13, 14, 3,  0,  6,  9,  10, 1,  2,  8,  5,  11, 12, 4,  15},
                {13, 8,  11, 5,  6,  15, 0,  3,  4,  7, 2,  12, 1,  10, 14, 9},
                {10, 6,  9,  0,  12, 11, 7,  13, 15, 1,  3,  14, 5,  2,  8,  4},
                {3,  15, 0,  6,  10, 1,  13, 8,  9,  4,  5,  11, 12, 7, 2,  14}
        },
        {
                {2,  12, 4,  1,  7,  10, 11, 6,  8,  5,  3,  15, 13, 0,  14, 9},
                {14, 11, 2,  12, 4,  7,  13, 1,  5,  0, 15, 10, 3,  9,  8,  6},
                {4,  2,  1,  11, 10, 13, 7,  8,  15, 9,  12, 5,  6,  3,  0,  14},
                {11, 8,  12, 7,  1,  14, 2,  13, 6,  15, 0,  9,  10, 4, 5,  3}
        },
        {
                {12, 1,  10, 15, 9,  2,  6,  8,  0,  13, 3,  4,  14, 7,  5,  11},
                {10, 15, 4,  2,  7,  12, 9,  5,  6,  1, 13, 14, 0,  11, 3,  8},
                {9,  14, 15, 5,  2,  8,  12, 3,  7,  0,  4,  10, 1,  13, 11, 6},
                {4,  3,  2,  12, 9,  5,  15, 10, 11, 14, 1,  7,  6,  0, 8,  13}
        },
        {
                {4,  11, 2,  14, 15, 0,  8,  13, 3,  12, 9,  7,  5,  10, 6,  1},
                {13, 0,  11, 7,  4,  9,  1,  10, 14, 3, 5,  12, 2,  15, 8,  6},
                {1,  4,  11, 13, 12, 3,  7,  14, 10, 15, 6,  8,  0,  5,  9,  2},
                {6,  11, 13, 8,  1,  4,  10, 7,  9,  5,  0,  15, 14, 2, 3,  12}
        },
        {
                {13, 2,  8,  4,  6,  15, 11, 1,  10, 9,  3,  14, 5,  0,  12, 7},
                {1,  15, 13, 8,  10, 3,  7,  4,  12, 5, 6,  11, 0,  14, 9,  2},
                {7,  11, 4,  1,  9,  12, 14, 2,  0,  6,  10, 13, 15, 3,  5,  8},
                {2,  1,  14, 7,  4,  10, 8,  13, 15, 12, 9,  0,  3,  5, 6,  11}
        }
};
void ulong_to_bit(unsigned long x,bit_t a[]);

void ip(bit_t bit_arr[]);

void feistel_network(bit_t key_bit_arr[], bit_t bit_arr[]);

void left_right_perm(bit_t bit_arr[]);

void ip_invert(bit_t bit_arr[]);

void gen_round_key(bit_t last_left_shift[], bit_t round_key[], int current_round);

void round_func(bit_t round_key[], bit_t right_bit_arr[], bit_t result_bit_arr[]);

void expension(bit_t right_bit_arr[], bit_t result_bit_arr[]);

void xor(bit_t bit_arr1[], bit_t bit_arr2[], bit_t res[], int n);

void s_box(bit_t interm_bit_arr[], bit_t result_bit_arr[]);

void perm(bit_t result_bit_arr[]);

void joint(bit_t a[], int n1, bit_t b[], int n2, bit_t result[]);

void bitarr_to_chararr(bit_t a[], char c[]);

void pc1(bit_t key[], bit_t last_left_shift[]);

void left_cycle_shift(bit_t a[], int n, int current_round);

void pc2(const bit_t last_left_shift[], bit_t round_key[]);

int bit_to_byte(const bit_t a[]);

void ulong_to_bit(unsigned long x,bit_t a[])
{
    int l=63;
    unsigned long tmp;
    do {
        tmp=x%2;
        x=x/2;
        a[l--]=tmp;
    } while (x!=0);
}



void des_encrypt(unsigned long key, unsigned long plain_text,char res[]) {
    bit_t inter_bit_arr[BLOCKSIZE]={0}, key_bit_arr[FAKE_KEYSIZE]={0};

    ulong_to_bit(key,key_bit_arr);
    ulong_to_bit(plain_text,inter_bit_arr);

    ip(inter_bit_arr);
    feistel_network(key_bit_arr, inter_bit_arr);
    left_right_perm(inter_bit_arr);
    ip_invert(inter_bit_arr);
    bitarr_to_chararr(inter_bit_arr, res);
}

void ip(bit_t bit_arr[]) {
    int i;
    bit_t tmp[BLOCKSIZE];
    
    //memcpy(tmp,bit_arr,BLOCKSIZE);

    for (i=0;i<BLOCKSIZE;i++){
        tmp[i]=bit_arr[i];
    }

    for (i = 0; i < BLOCKSIZE; i++) {
        bit_arr[i] = tmp[IP_table[i / 8][i % 8]-1];
    }
}

void feistel_network(bit_t key_bit_arr[], bit_t result_bit_arr[]) {
    int i;
    bit_t round_key[48];
    bit_t left_bit_arr[2][32], right_bit_arr[2][32];
    bit_t tmp_bit_arr[32];

    int current_round = 0;
    bit_t last_left_shift[56];

    for (i=0;i<32;i++){
        left_bit_arr[0][i]=result_bit_arr[i];
    }
    for (;i<64;i++){
        right_bit_arr[0][i]=result_bit_arr[i];
    }


    pc1(key_bit_arr, last_left_shift);

    for (i = 0; i < ROUND; i++) {
        current_round = i;
        memcpy(left_bit_arr[1], right_bit_arr[0], 32* sizeof(bit_t));
        gen_round_key(last_left_shift, round_key, current_round);
        round_func(round_key, right_bit_arr[0], tmp_bit_arr);
        xor(left_bit_arr[0], tmp_bit_arr, right_bit_arr[1], 32);

        memcpy(right_bit_arr[0],right_bit_arr[1],32* sizeof(bit_t));
        memcpy(left_bit_arr[0],left_bit_arr[1],32* sizeof(bit_t));
    }
    joint(left_bit_arr[1], 32, right_bit_arr[1], 32, result_bit_arr);
}

void left_right_perm(bit_t bit_arr[]) {
    bit_t left[32], right[32];
    int i;
    for (i = 0; i < 32; i++) {
        right[i] = bit_arr[i];
    }
    for (; i < BLOCKSIZE; i++) {
        left[i - 32] = bit_arr[i];
    }
    joint(left, 32, right, 32, bit_arr);
}

void ip_invert(bit_t bit_arr[]) {
    int i;
    bit_t tmp[BLOCKSIZE];
    for (i = 0; i < BLOCKSIZE; i++) {
        tmp[i] = bit_arr[IP_invert_table[i / 8][i % 8]-1];
    }
    memcpy(bit_arr, tmp, 64* sizeof(bit_t));
}

void gen_round_key(bit_t last_left_shift[], bit_t round_key[], int current_round) {
    bit_t left_bit_arr[28], right_bit_arr[28];

    int i;
    for (i = 0; i < 28; i++) {
        left_bit_arr[i] = last_left_shift[i];
    }
    for (; i < 56; i++) {
        right_bit_arr[i-28] = last_left_shift[i];
    }

    left_cycle_shift(left_bit_arr, 28, current_round);
    left_cycle_shift(right_bit_arr, 28, current_round);
    joint(left_bit_arr, 28, right_bit_arr, 28, last_left_shift);
    pc2(last_left_shift, round_key);

}

void round_func(bit_t round_key[], bit_t right_bit_arr[], bit_t result_bit_arr[]) {
    bit_t interm_bit_arr[48];
    expension(right_bit_arr, interm_bit_arr);
    xor(interm_bit_arr, round_key, interm_bit_arr, 48);
    s_box(interm_bit_arr, result_bit_arr);
    perm(result_bit_arr);

}

void expension(bit_t right_bit_arr[], bit_t result_bit_arr[]) {
    int i;
    for (i = 0; i < 48; i++) {
        result_bit_arr[i] = right_bit_arr[Expension_table[i / 8][i % 6]-1];
    }
}

void xor(bit_t bit_arr1[], bit_t bit_arr2[], bit_t res[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        res[i] = bit_arr1[i] ^ bit_arr2[i];
    }
}

void s_box(bit_t interm_bit_arr[], bit_t result_bit_arr[]) {

    int i,j,k=0,l;
    int tmp;
    int row,colum;
    int num;
    bit_t sbox_input[6];
    bit_t sbox_output[4]={0,0,0,0};
    for(i=0;i<8;i++){
        for (j=0;j<6;j++){
            sbox_input[j]=interm_bit_arr[i*6+j];
        }
        row=sbox_input[0]*2+sbox_input[5]*1;
        colum=sbox_input[1]*8+sbox_input[2]*4+sbox_input[3]*2+sbox_input[4]*1;
        num=S_boxes[i][row][colum];

        l=3;
        do{
            tmp=num%2;
            sbox_output[l--]=tmp;
            num=num/2;
        } while (num!=0);

        for (l=0;l<4;l++,k++){
            result_bit_arr[k]=sbox_output[l];
        }

    }

}

void perm(bit_t result_bit_arr[]) {
    bit_t tmp_bit_arr[32];
    int i;
    for (i = 0; i < 32; i++) {
        tmp_bit_arr[i] = result_bit_arr[Perm_table[i / 8][i % 4]-1];
    }
    memcpy(result_bit_arr, tmp_bit_arr, 32* sizeof(bit_t));

}

void joint(bit_t a[], int n1, bit_t b[], int n2, bit_t result[]) {
    int i,k=0;
    for (i = 0; i < n1; i++,k++) {
        result[k] = a[i];
    }
    for (i = 0; i < n2; i++,k++) {
        result[k] = b[i];
    }
}

void bitarr_to_chararr(bit_t a[], char res[]) {
    int d;
    int i;
    bit_t tmp[8];
    for (i = 0; i < BLOCKSIZE; i = i + 8) {

        for (int j = 0; j < 8; j++) {
            tmp[j] = a[i + j];
        }
        res[i / 8] = (char)bit_to_byte(tmp);
    }

}

void pc1(bit_t key[], bit_t last_left_shift[]) {
    int i;
    for (i = 0; i < KEYSIZE; i++) {
        last_left_shift[i] = key[PC1_table[i / 8][i % 7]-1];
    }

}

void left_cycle_shift(bit_t a[], int n, int current_round) {
    int num = Left_shift_round_table[current_round];
    bit_t tmp[num];
    int i, j;
    for (i = 0; i < num; ++i) {
        tmp[i] = a[i];
    }
    for (; i < n; i++) {
        a[i - num] = a[i];
    }
    for (i = i - num, j = 0; i < n; i++, j++) {
        a[i] = tmp[j];
    }
}

void pc2(const bit_t last_left_shift[], bit_t round_key[]) {
    int i;
    for (i = 0; i < 48; i++) {
        round_key[i] = last_left_shift[PC2_table[i / 8][i % 6]-1];
    }
}

int bit_to_byte(const bit_t a[]) {
    int i = 0;
    int d;
    for (i = 0; i < 8; i++) {
        d += (int) (a[i] * pow(2, i));
    }
    return d;
}
