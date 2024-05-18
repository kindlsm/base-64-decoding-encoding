#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static const char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static const unsigned char base64_dec_table[256] = {
    ['A'] = 0,['B'] = 1,['C'] = 2,['D'] = 3,['E'] = 4,['F'] = 5,['G'] = 6,['H'] = 7,
    ['I'] = 8,['J'] = 9,['K'] = 10,['L'] = 11,['M'] = 12,['N'] = 13,['O'] = 14,['P'] = 15,
    ['Q'] = 16,['R'] = 17,['S'] = 18,['T'] = 19,['U'] = 20,['V'] = 21,['W'] = 22,['X'] = 23,
    ['Y'] = 24,['Z'] = 25,['a'] = 26,['b'] = 27,['c'] = 28,['d'] = 29,['e'] = 30,['f'] = 31,
    ['g'] = 32,['h'] = 33,['i'] = 34,['j'] = 35,['k'] = 36,['l'] = 37,['m'] = 38,['n'] = 39,
    ['o'] = 40,['p'] = 41,['q'] = 42,['r'] = 43,['s'] = 44,['t'] = 45,['u'] = 46,['v'] = 47,
    ['w'] = 48,['x'] = 49,['y'] = 50,['z'] = 51,['0'] = 52,['1'] = 53,['2'] = 54,['3'] = 55,
    ['4'] = 56,['5'] = 57,['6'] = 58,['7'] = 59,['8'] = 60,['9'] = 61,['+'] = 62,['/'] = 63
};

char* base64_encode(const unsigned char* data, size_t input_length, size_t* output_length) {
    *output_length = 4 * ((input_length + 2) / 3);
    char* encoded_data = (char*)malloc(*output_length + 1); // Null terminator Æ÷ÇÔ
    if (encoded_data == NULL) return NULL;

    for (size_t i = 0, j = 0; i < input_length;) {
        uint32_t octet_a = i < input_length ? (unsigned char)data[i++] : 0;
        uint32_t octet_b = i < input_length ? (unsigned char)data[i++] : 0;
        uint32_t octet_c = i < input_length ? (unsigned char)data[i++] : 0;
        uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;
        encoded_data[j++] = base64_table[(triple >> 3 * 6) & 0x3F];
        encoded_data[j++] = base64_table[(triple >> 2 * 6) & 0x3F];
        encoded_data[j++] = base64_table[(triple >> 1 * 6) & 0x3F];
        encoded_data[j++] = base64_table[(triple >> 0 * 6) & 0x3F];
    
    }

    for (size_t i = 0; i < (input_length % 3 ? 3 - (input_length % 3) : 0); i++)
        encoded_data[*output_length - 1 - i] = '=';

    encoded_data[*output_length] = '\0';
    return encoded_data;
}

unsigned char* base64_decode(const char* data, size_t input_length, size_t* output_length) {
    if (input_length % 4 != 0) return NULL;

    *output_length = input_length / 4 * 3;
    if (data[input_length - 1] == '=') (*output_length)--;
    if (data[input_length - 2] == '=') (*output_length)--;

    unsigned char* decoded_data = (unsigned char*)malloc(*output_length + 1);
    if (decoded_data == NULL) return NULL;

    for (size_t i = 0, j = 0; i < input_length;) {
        uint32_t sextet_a = data[i] == '=' ? 0 & i++ : base64_dec_table[(unsigned char)data[i++]];
        uint32_t sextet_b = data[i] == '=' ? 0 & i++ : base64_dec_table[(unsigned char)data[i++]];
        uint32_t sextet_c = data[i] == '=' ? 0 & i++ : base64_dec_table[(unsigned char)data[i++]];
        uint32_t sextet_d = data[i] == '=' ? 0 & i++ : base64_dec_table[(unsigned char)data[i++]];

        uint32_t triple = (sextet_a << 3 * 6)
            + (sextet_b << 2 * 6)
            + (sextet_c << 1 * 6)
            + (sextet_d << 0 * 6);

        if (j < *output_length) decoded_data[j++] = (triple >> 2 * 8) & 0xFF;
        if (j < *output_length) decoded_data[j++] = (triple >> 1 * 8) & 0xFF;
        if (j < *output_length) decoded_data[j++] = (triple >> 0 * 8) & 0xFF;
    }

    decoded_data[*output_length] = '\0';
    return decoded_data;
}

int main() {
    char input[256];
    int choice;

    printf("1. Base64 incoding\n");
    printf("2. Base64 decoding\n");
    printf("#input 1 ");
    if (scanf("%d", &choice) != 1) {
        fprintf(stderr, "error\n");
        return 1;
    }
    getchar();

    if (choice == 1) {
        printf("#input 2 ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            fprintf(stderr, "error\n");
            return 1;
        }
        input[strcspn(input, "\n")] = 0;

        size_t encoded_length;
        char* encoded_text = base64_encode((const unsigned char*)input, strlen(input), &encoded_length);
        if (encoded_text == NULL) {
            fprintf(stderr, "failed\n");
            return 1;
        }
        printf("\t>>> %s\n", encoded_text);
        free(encoded_text);
    }
    else if (choice == 2) {
        printf("#input 2 ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            fprintf(stderr, "error\n");
            return 1;
        }
        input[strcspn(input, "\n")] = 0; 

        size_t decoded_length;
        unsigned char* decoded_text = base64_decode(input, strlen(input), &decoded_length);
        if (decoded_text == NULL) {
            fprintf(stderr, "error\n");
            return 1;
        }
        printf("\t>>> % s\n", decoded_text);
        free(decoded_text);
    }
    else {
        printf("error.\n");
    }

    return 0;
}
