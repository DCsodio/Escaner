#include "convert.h"

void int_to_str(uint16_t numero, char *p) {
    uint16_t divisor = 1;
    uint8_t cantDigitos = 0;  // uint8_t suficiente (máx 5 dígitos)

    if(numero != 0) {
        while (numero / divisor != 0) {
            divisor *= 10;
            cantDigitos++;
        }
        divisor /= 10;

        for (uint8_t i = 0; i < cantDigitos; i++) {
            p[i] = numero / divisor + '0';  // Más legible que 48
            numero %= divisor;
            divisor /= 10;
        }
        p[cantDigitos] = '\0';
    } else {
        p[0] = '0';
        p[1] = '\0';
    }
}

/*
void int_to_str(uint16_t numero, char *p) {
    char *inicio = p;

    // Generar dígitos en orden inverso
    do {
        *p++ = (numero % 10) + '0';
        numero /= 10;
    } while (numero > 0);
    *p = '\0';

    // Invertir la cadena
    char *fin = p - 1;
    while (inicio < fin) {
        char temp = *inicio;
        *inicio = *fin;
        *fin = temp;
        inicio++;
        fin--;
    }
}*/
