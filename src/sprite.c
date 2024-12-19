#include "proc_grafico.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

extern void exibe_sprite(uint8_t sp, uint32_t xy, uint16_t offset, uint8_t registrador);
extern void altera_pixel_sprite(uint16_t cor, uint16_t endereco);

#define largura_sprite 20
#define altura_sprite 20

uint16_t converte_em_bgr(uint8_t rgb);
void cria_sprite(uint16_t end_base, uint16_t dados_do_sprite[altura_sprite][largura_sprite]);

//Converte os dados da imagem de RGB para BGR 9 bits - formato da instrucao wsm
uint16_t converte_em_bgr(uint8_t rgb) {
    uint8_t r, g, b;
    uint16_t bgr;

    //Extrai cada tom (R, G e B) do formato RGB
    r = (rgb >> 5) & 0b111; // 3 bits msb do vermelho
    g = (rgb >> 2) & 0b111; // 3 bits do meio do verde
    b = rgb & 0b11; // 2 bits lsb do azul
    //ajusta o azul de 2 para 3 bits
    b = b << 1;

    //Coloca no formato BBB GGG RRR de 9 bits
    bgr = ( (b << 6) | (g << 3) | r );

    return bgr;
}

//Cria e armazena um sprite na memoria de sprites
void cria_sprite(uint16_t end_base, uint16_t dados_do_sprite[altura_sprite][largura_sprite]) {
    uint16_t cor[400]; //20x20 -> 400 pixels por sprite
    int y, x;
    int z = 0;
    for ( y = 0; y < altura_sprite; y++) {
        for ( x = 0; x < largura_sprite; x++) {
            cor[z] = dados_do_sprite[y][x]; //Extrai a cor de cada pixel em 9 bits BGR
            z++;
        }
    }
    //Escreve cada pixel da matriz 20x20 na memoria de sprites
    int i = 0;
    while (i < 400){
        altera_pixel_sprite(cor[i], end_base + i);
        i++;
        usleep(10000);
    }
}

void gera_sprite_ovni_offset1();
void gera_sprite_ovni_offset2();
void gera_sprite_ovni_offset3();
void grava_sprite_ovni();
void gera_sprite_estrela_offset8();
void gera_sprite_estrela_offset9();
void gera_sprite_estrela_offset10();
void grava_sprite_estrela();
void animacao_estrela(uint32_t pos_xy_20b, int sp);
void gera_sprite_portal_ofst4();
void gera_sprite_portal_ofst5();
void gera_sprite_portal_ofst6();
void gera_sprite_portal_ofst7();
void grava_sprite_portal();
void animacao_portal(uint32_t pos_xy_20b, int sp);

//Dados da imagem para formar um sprite (20x20) em formato RRRGGGBB - 1 byte por pixel
uint16_t dados_da_imagem_ovni3[altura_sprite][largura_sprite] = {
{ 0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff },
{ 0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x96,0x76,0x76,0x96,0xdb,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff },
{ 0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0xdb,0x76,0xbf,0x7f,0x7f,0x7f,0x7b,0xbb,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff },
{ 0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x76,0xdf,0x1ff,0x9f,0x7f,0x7f,0x7f,0x76,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff },
{ 0x1ff,0x1ff,0x1ff,0x1ff,0xb6,0x4d,0x7b,0xdf,0xbf,0x7f,0x7f,0x7f,0x7f,0x7f,0x49,0x92,0xdb,0x1ff,0x1ff,0x1ff },
{ 0x1ff,0x1ff,0x96,0x49,0x96,0x29,0x7b,0x9f,0x7f,0x7f,0x7f,0x7f,0x7f,0x7f,0x29,0x96,0x4d,0x71,0x1ff,0x1ff },
{ 0x1ff,0x96,0x72,0x97,0x96,0x4d,0x51,0x56,0x7f,0x7f,0x7f,0x7f,0x57,0x31,0x4d,0x96,0x97,0x72,0x76,0x1ff },
{ 0xdb,0x35,0x97,0x97,0x96,0x4e,0x49,0x2d,0x51,0x2d,0x2d,0x4d,0x29,0x4d,0x49,0x92,0x97,0x97,0x35,0xb6 },
{ 0x96,0x35,0x96,0x72,0x6e,0x96,0x6e,0x49,0x29,0x29,0x29,0x29,0x29,0x6e,0x92,0x72,0x6e,0x96,0x35,0x76 },
{ 0x96,0x35,0x2d,0x92,0x97,0x97,0x97,0x96,0x92,0x72,0x72,0x72,0x92,0x97,0x97,0x97,0x96,0x4d,0x35,0x72 },
{ 0xb6,0x2d,0x35,0x55,0x76,0x76,0x72,0x97,0x97,0x92,0x96,0x97,0x97,0x72,0x76,0x96,0x55,0x35,0x2d,0x92 },
{ 0x1ff,0x29,0x31,0x35,0x35,0x59,0x76,0x96,0x96,0x92,0x96,0x96,0x96,0x76,0x59,0x35,0x35,0x35,0x29,0xdb },
{ 0x1ff,0xba,0x79,0x31,0x35,0x35,0x35,0x35,0x35,0x35,0x35,0x35,0x35,0x35,0x35,0x35,0x31,0x75,0xba,0x1ff },
{ 0x1ff,0xdb,0x99,0x55,0x29,0x31,0x31,0x31,0x31,0x31,0x31,0x31,0x31,0x31,0x31,0x29,0x51,0x99,0xbb,0x1ff },
{ 0x1ff,0x1ff,0x96,0x96,0x72,0x79,0x99,0x2d,0x29,0x55,0x59,0x2d,0x2d,0x79,0x79,0x76,0x96,0x96,0x1ff,0x1ff },
{ 0x1ff,0xb7,0x1ff,0x1ff,0x96,0x96,0x51,0x25,0x25,0x51,0x75,0x25,0x25,0x51,0x96,0x96,0x1ff,0x1ff,0x1ff,0x1ff },
{ 0x1ff,0xdb,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x96,0x96,0x92,0x72,0x96,0x96,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0xdb,0x1ff },
{ 0x1ff,0x1ff,0xdb,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0xbb,0x1ff,0x1ff },
{ 0x1ff,0x1ff,0x1ff,0x1ff,0xdb,0xb7,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0xb7,0xbb,0x1ff,0x1ff,0x1ff,0x1ff },
{ 0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0xdf,0xdf,0xdb,0xdb,0xdf,0xdf,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff }
};

//Dados da imagem para formar um sprite (20x20) em formato RRRGGGBB - 1 byte por pixel
uint16_t dados_da_imagem_ovni2[altura_sprite][largura_sprite] = {
{ 0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff },
{ 0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff },
{ 0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff },
{ 0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff },
{ 0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff },
{ 0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0xfe,0xfe,0x49,0x49,0x49,0x49,0x49,0xfe,0xfe,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff },
{ 0x1ff,0x1ff,0x1ff,0x1ff,0xfe,0xfe,0x49,0x49,0x53,0x53,0x01,0x49,0x49,0xfe,0xfe,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff },
{ 0x1ff,0x1ff,0x1ff,0xfe,0xfe,0x49,0x49,0x29,0x01,0x01,0x01,0x01,0x49,0x49,0xfe,0xfe,0x1ff,0x1ff,0x1ff,0x1ff },
{ 0x1ff,0xfe,0xfe,0xfe,0x49,0x49,0x53,0x01,0x01,0x01,0x01,0x01,0x01,0x49,0x49,0xfe,0xfe,0xfe,0x1ff,0x1ff },
{ 0xfe,0xfe,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0xfe,0xfe,0x1ff },
{ 0xfe,0x49,0x49,0xfd,0x49,0xfd,0x49,0xfd,0x49,0xfd,0x49,0xfd,0x49,0xfd,0x49,0xfd,0x49,0x49,0xfe,0x1ff },
{ 0xfe,0xfe,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0xfe,0xfe,0x1ff },
{ 0x1ff,0xfe,0xfe,0x49,0x49,0xe8,0xc0,0xc0,0xc0,0xdd,0xe1,0xe1,0xe1,0xe8,0x49,0x49,0xfe,0xfe,0x1ff,0x1ff },
{ 0x1ff,0x1ff,0xfe,0xfe,0xfe,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0xfe,0xfe,0xfe,0x1ff,0x1ff,0x1ff },
{ 0x1ff,0x1ff,0x1ff,0x1ff,0xfe,0xfe,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0xfe,0xfe,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff },
{ 0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff },
{ 0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff },
{ 0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff },
{ 0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff },
{ 0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff }
};

//Dados da imagem para formar um sprite (20x20) em formato RRRGGGBB - 1 byte por pixel
uint16_t dados_da_imagem_ovni[altura_sprite][largura_sprite] = {
{ 0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff },
{ 0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff },
{ 0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x76,0x9b,0x9b,0x9b,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff },
{ 0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x76,0x9f,0x9f,0x9f,0x9f,0x9b,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff },
{ 0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x76,0x9f,0x9f,0xc4,0xc4,0x9f,0x9f,0x9b,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff },
{ 0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x76,0x9f,0xc4,0x40,0x40,0xc4,0x9f,0x9b,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff },
{ 0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x76,0x9f,0x9f,0x40,0x80,0x80,0x40,0x9f,0x9f,0x9b,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff },
{ 0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x76,0x9f,0xc4,0x60,0xf2,0xf2,0x60,0xc4,0x9f,0x9b,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff },
{ 0x1ff,0x1ff,0x1ff,0x1ff,0x72,0x76,0x56,0x52,0x52,0x52,0x52,0x52,0x52,0x76,0x7a,0x9b,0x1ff,0x1ff,0x1ff,0x1ff },
{ 0x1ff,0x1ff,0x1ff,0x02,0x53,0x53,0x53,0x53,0x53,0x53,0x53,0x53,0x53,0x53,0x53,0x53,0x02,0x1ff,0x1ff,0x1ff },
{ 0x1ff,0x1ff,0x02,0x77,0x02,0x77,0x77,0x02,0x02,0x77,0x77,0x02,0x02,0x77,0x77,0x02,0x77,0x02,0x1ff,0x1ff },
{ 0x1ff,0x02,0x2f,0x2f,0x2f,0x2f,0x2f,0x2f,0x2f,0x2f,0x2f,0x2f,0x2f,0x2f,0x2f,0x2f,0x2f,0x2f,0x02,0x1ff },
{ 0x02,0xfe,0xf8,0xfe,0xf8,0xfd,0xf8,0xfe,0xf8,0xfe,0xf8,0xfe,0xf8,0xfd,0xf8,0xfe,0xf8,0xfe,0xf8,0x02 },
{ 0x1ff,0x02,0x2f,0x2f,0x2f,0x2f,0x2f,0x2f,0x2f,0x2f,0x2f,0x2f,0x2f,0x2f,0x2f,0x2f,0x2f,0x2f,0x02,0x1ff },
{ 0x1ff,0x1ff,0x02,0x77,0x02,0x77,0x77,0x02,0x02,0x77,0x77,0x02,0x02,0x77,0x77,0x02,0x77,0x02,0x1ff,0x1ff },
{ 0x1ff,0x1ff,0x1ff,0x02,0x53,0x53,0x53,0x53,0x53,0x53,0x53,0x53,0x53,0x53,0x53,0x53,0x02,0x1ff,0x1ff,0x1ff },
{ 0x1ff,0x1ff,0x1ff,0x1ff,0x02,0x02,0x53,0x53,0x53,0x53,0x53,0x53,0x53,0x53,0x02,0x02,0x1ff,0x1ff,0x1ff,0x1ff },
{ 0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x02,0x02,0x53,0x53,0x53,0x53,0x02,0x02,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff },
{ 0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x02,0x02,0x02,0x02,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff },
{ 0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff }
};

void gera_sprite_ovni_offset1(){
    //Dados para formar um sprite 20x20 em formato RRR GGG BBB - 9 bits
    uint16_t dados_do_sprite[altura_sprite][largura_sprite];
    uint16_t cor_temp;
    int y, x;
    for ( y = 0; y < altura_sprite; y++) {
        for ( x = 0; x < largura_sprite; x++) {
            cor_temp = dados_da_imagem_ovni[y][x];
            if (cor_temp == 0x1ff){
                dados_do_sprite[y][x] = APAGA;
            } else {
                dados_do_sprite[y][x] = converte_em_bgr(dados_da_imagem_ovni[y][x]);//Converte pixel por pixel do formato RGB para o BGR
            }
        }
    }
    //Escreve os dados de cada pixel na memoria de sprites. [end_base = offset * 400]
    cria_sprite(400, dados_do_sprite);
}

void gera_sprite_ovni_offset2(){
    //Dados para formar um sprite 20x20 em formato RRR GGG BBB - 9 bits
    uint16_t dados_do_sprite[altura_sprite][largura_sprite];
    uint16_t cor_temp;
    int y, x;
    for ( y = 0; y < altura_sprite; y++) {
        for ( x = 0; x < largura_sprite; x++) {
            cor_temp = dados_da_imagem_ovni2[y][x];
            if (cor_temp == 0x1ff){
                dados_do_sprite[y][x] = APAGA;
            } else {
                dados_do_sprite[y][x] = converte_em_bgr(dados_da_imagem_ovni2[y][x]);//Converte pixel por pixel do formato RGB para o BGR
            }
        }
    }
    //Escreve os dados de cada pixel na memoria de sprites. [end_base = offset * 400]
    cria_sprite(800, dados_do_sprite);
}

void gera_sprite_ovni_offset3(){
    //Dados para formar um sprite 20x20 em formato RRR GGG BBB - 9 bits
    uint16_t dados_do_sprite[altura_sprite][largura_sprite];
    uint16_t cor_temp;
    int y, x;
    for ( y = 0; y < altura_sprite; y++) {
        for ( x = 0; x < largura_sprite; x++) {
            cor_temp = dados_da_imagem_ovni3[y][x];
            if (cor_temp == 0x1ff){
                dados_do_sprite[y][x] = APAGA;
            } else {
                dados_do_sprite[y][x] = converte_em_bgr(dados_da_imagem_ovni3[y][x]);//Converte pixel por pixel do formato RGB para o BGR
            }
        }
    }
    //Escreve os dados de cada pixel na memoria de sprites. [end_base = offset * 400]
    cria_sprite(1200, dados_do_sprite);
}

void grava_sprite_ovni(){
    gera_sprite_ovni_offset1();
    gera_sprite_ovni_offset2();
    gera_sprite_ovni_offset3();
}

//estrela

//Dados da imagem para formar um sprite (20x20) em formato RRRGGGBB - 1 byte por pixel
uint16_t dados_da_imagem_estrela1[altura_sprite][largura_sprite] = {
{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xfe,0xf4,0xfc,0xfe,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xfe,0xf4,0xf4,0xfc,0xfc,0xfe,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{ 0x00,0x00,0x00,0x00,0x00,0x00,0xfe,0xf4,0xf8,0xf8,0xfc,0xfc,0xf8,0xfe,0x00,0x00,0x00,0x00,0x00,0x00},
{ 0x00,0x00,0x00,0x00,0x00,0xfe,0xf4,0xf4,0xf8,0xf8,0xfc,0xfc,0xf8,0xf8,0xfe,0x00,0x00,0x00,0x00,0x00},
{ 0x00,0x00,0x00,0x00,0x00,0xfe,0xf8,0xf8,0xfc,0xfc,0xfe,0xfe,0xfc,0xfc,0xfe,0x00,0x00,0x00,0x00,0x00},
{ 0x00,0x00,0x00,0x00,0xfe,0xfe,0xf8,0xf8,0xfc,0xfc,0xfe,0xfe,0xfc,0xfc,0xfe,0xfe,0x00,0x00,0x00,0x00},
{ 0x00,0x00,0xfe,0xfe,0xf8,0xf8,0xfc,0xfc,0xfc,0xfc,0xfd,0xfd,0xfe,0xfe,0xfc,0xfc,0xfe,0xfe,0x00,0x00},
{ 0x00,0xfe,0xf4,0xf4,0xf8,0xf8,0xfc,0xfc,0xfc,0xfc,0xfd,0xfd,0xfe,0xfe,0xfc,0xfc,0xfc,0xfc,0xfe,0x00},
{ 0xfe,0xf4,0xf8,0xf8,0xf8,0xf8,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfd,0xfd,0xfe,0xfe,0xfd,0xfd,0xfc,0xfe},
{ 0xf4,0xf4,0xf8,0xf8,0xf8,0xf8,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfd,0xfd,0xfe,0xfe,0xfd,0xfd,0xfc,0xfc},
{ 0xf8,0xf8,0xf8,0xf8,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfd,0xfd,0xfd,0xfd,0xfd,0xfd},
{ 0xfe,0xf8,0xf8,0xf8,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfd,0xfd,0xfd,0xfd,0xfd,0xfe},
{ 0x00,0xfe,0xf4,0xf4,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfe,0x00},
{ 0x00,0x00,0xfe,0xf4,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfe,0x00,0x00},
{ 0x00,0x00,0x00,0xfe,0xf8,0xf8,0xfc,0xfc,0xfc,0xfc,0xfd,0xfd,0xfe,0xfe,0xfc,0xfc,0xfe,0x00,0x00,0x00},
{ 0x00,0x00,0x00,0xfe,0xf8,0xf8,0xfc,0xfc,0xfc,0xfc,0xfd,0xfd,0xfe,0xfe,0xfc,0xfc,0xfe,0x00,0x00,0x00},
{ 0x00,0x00,0x00,0xfe,0xf8,0xf8,0xfc,0xfc,0xf4,0xf4,0xf8,0xf8,0xfd,0xfd,0xfc,0xfc,0xfe,0x00,0x00,0x00},
{ 0x00,0x00,0x00,0xfe,0xf8,0xf8,0xfc,0xfc,0xf4,0xf4,0xf8,0xf8,0xfd,0xfd,0xfc,0xfc,0xfe,0x00,0x00,0x00},
{ 0x00,0x00,0x00,0xfe,0xf8,0xf8,0xf4,0xf4,0xfe,0xfe,0xfe,0xfe,0xf8,0xf8,0xfc,0xfc,0xfe,0x00,0x00,0x00},
{ 0x00,0x00,0x00,0x00,0xfe,0xf8,0xf4,0xfe,0x00,0x00,0x00,0x00,0xfe,0xf8,0xfc,0xfe,0x00,0x00,0x00,0x00}
};

//Dados da imagem para formar um sprite (20x20) em formato RRRGGGBB - 1 byte por pixel
uint16_t dados_da_imagem_estrela2[altura_sprite][largura_sprite] = {
{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xf4,0xfc,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xf4,0xf4,0xfc,0xfc,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xf4,0xf8,0xf8,0xfc,0xfc,0xf8,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{ 0x00,0x00,0x00,0x00,0x00,0x00,0xf4,0xf4,0xf8,0xf8,0xfc,0xfc,0xf8,0xf8,0x00,0x00,0x00,0x00,0x00,0x00},
{ 0x00,0x00,0x00,0x00,0x00,0x00,0xf8,0xf8,0xfc,0xfc,0xfe,0xfe,0xfc,0xfc,0x00,0x00,0x00,0x00,0x00,0x00},
{ 0x00,0x00,0x00,0x00,0x00,0x00,0xf8,0xf8,0xfc,0xfc,0xfe,0xfe,0xfc,0xfc,0x00,0x00,0x00,0x00,0x00,0x00},
{ 0x00,0x00,0x00,0x00,0xf8,0xf8,0xfc,0xfc,0xfc,0xfc,0xfd,0xfd,0xfe,0xfe,0xfc,0xfc,0x00,0x00,0x00,0x00},
{ 0x00,0x00,0xf4,0xf4,0xf8,0xf8,0xfc,0xfc,0xfc,0xfc,0xfd,0xfd,0xfe,0xfe,0xfc,0xfc,0xfc,0xfc,0x00,0x00},
{ 0x00,0xf4,0xf8,0xf8,0xf8,0xf8,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfd,0xfd,0xfe,0xfe,0xfd,0xfd,0xfc,0x00},
{ 0xf4,0xf4,0xf8,0xf8,0xf8,0xf8,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfd,0xfd,0xfe,0xfe,0xfd,0xfd,0xfc,0xfc},
{ 0xf8,0xf8,0xf8,0xf8,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfd,0xfd,0xfd,0xfd,0xfd,0xfd},
{ 0x00,0xf8,0xf8,0xf8,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfd,0xfd,0xfd,0xfd,0xfd,0x00},
{ 0x00,0x00,0xf4,0xf4,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0x00,0x00},
{ 0x00,0x00,0x00,0xf4,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0x00,0x00,0x00},
{ 0x00,0x00,0x00,0x00,0xf8,0xf8,0xfc,0xfc,0xfc,0xfc,0xfd,0xfd,0xfe,0xfe,0xfc,0xfc,0x00,0x00,0x00,0x00},
{ 0x00,0x00,0x00,0x00,0xf8,0xf8,0xfc,0xfc,0xfc,0xfc,0xfd,0xfd,0xfe,0xfe,0xfc,0xfc,0x00,0x00,0x00,0x00},
{ 0x00,0x00,0x00,0x00,0xf8,0xf8,0xfc,0xfc,0xf4,0xf4,0xf8,0xf8,0xfd,0xfd,0xfc,0xfc,0x00,0x00,0x00,0x00},
{ 0x00,0x00,0x00,0x00,0xf8,0xf8,0xfc,0xfc,0xf4,0xf4,0xf8,0xf8,0xfd,0xfd,0xfc,0xfc,0x00,0x00,0x00,0x00},
{ 0x00,0x00,0x00,0x00,0xf8,0xf8,0xf4,0xf4,0x00,0x00,0x00,0x00,0xf8,0xf8,0xfc,0xfc,0x00,0x00,0x00,0x00},
{ 0x00,0x00,0x00,0x00,0x00,0xf8,0xf4,0x00,0x00,0x00,0x00,0x00,0x00,0xf8,0xfc,0x00,0x00,0x00,0x00,0x00}
};

//Dados da imagem para formar um sprite (20x20) em formato RRRGGGBB - 1 byte por pixel
uint16_t dados_da_imagem_estrela3[altura_sprite][largura_sprite] = {
{ 0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfc,0xfc,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff },
{ 0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfc,0xfc,0xfc,0xfc,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff },
{ 0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xff,0xff,0xff,0xff,0xff,0xff,0xff },
{ 0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xff,0xff,0xff,0xff,0xff,0xff,0xff },
{ 0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xff,0xff,0xff,0xff,0xff,0xff,0xff },
{ 0xff,0xff,0xff,0xff,0xff,0xff,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xff,0xff,0xff,0xff,0xff,0xff },
{ 0xff,0xff,0xff,0xff,0xff,0xff,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xff,0xff,0xff,0xff,0xff,0xff },
{ 0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc },
{ 0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0x00,0xfc,0xfc,0xfc,0xfc,0x00,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc },
{ 0xff,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0x00,0xfc,0xfc,0xfc,0xfc,0x00,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xff },
{ 0xff,0xff,0xfc,0xfc,0xfc,0xfc,0xfc,0x00,0xfc,0xfc,0xfc,0xfc,0x00,0xfc,0xfc,0xfc,0xfc,0xfc,0xff,0xff },
{ 0xff,0xff,0xff,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xff,0xff,0xff },
{ 0xff,0xff,0xff,0xff,0xfc,0xfc,0x00,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0x00,0xfc,0xfc,0xff,0xff,0xff,0xff },
{ 0xff,0xff,0xff,0xff,0xfc,0xfc,0xfc,0x00,0xfc,0xfc,0xfc,0xfc,0x00,0xfc,0xfc,0xfc,0xff,0xff,0xff,0xff },
{ 0xff,0xff,0xff,0xfc,0xfc,0xfc,0xfc,0xfc,0x00,0x00,0x00,0x00,0xfc,0xfc,0xfc,0xfc,0xfc,0xff,0xff,0xff },
{ 0xff,0xff,0xff,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xff,0xff,0xff },
{ 0xff,0xff,0xff,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xff,0xff,0xff },
{ 0xff,0xff,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xff,0xff,0xff,0xff,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0xff,0xff },
{ 0xff,0xff,0xfc,0xfc,0xfc,0xfc,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfc,0xfc,0xfc,0xfc,0xff,0xff },
{ 0xff,0xff,0xfc,0xfc,0xfc,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfc,0xfc,0xfc,0xff,0xff },
};


void gera_sprite_estrela_offset8(){
    //Dados para formar um sprite 20x20 em formato RRR GGG BBB - 9 bits
    uint16_t dados_do_sprite[altura_sprite][largura_sprite];
    uint16_t cor_temp;
    int y, x;
    for ( y = 0; y < altura_sprite; y++) {
        for ( x = 0; x < largura_sprite; x++) {
            cor_temp = dados_da_imagem_estrela1[y][x];
            if (cor_temp == 0x00){
                dados_do_sprite[y][x] = APAGA;
            } else {
                dados_do_sprite[y][x] = converte_em_bgr(dados_da_imagem_estrela1[y][x]);//Converte pixel por pixel do formato RGB para o BGR
            }
        }
    }
    //Escreve os dados de cada pixel na memoria de sprites. [end_base = offset * 400]
    cria_sprite(3200, dados_do_sprite);
}

void gera_sprite_estrela_offset9(){
    //Dados para formar um sprite 20x20 em formato RRR GGG BBB - 9 bits
    uint16_t dados_do_sprite[altura_sprite][largura_sprite];
    uint16_t cor_temp;
    int y, x;
    for ( y = 0; y < altura_sprite; y++) {
        for ( x = 0; x < largura_sprite; x++) {
            cor_temp = dados_da_imagem_estrela2[y][x];
            if (cor_temp == 0x00){
                dados_do_sprite[y][x] = APAGA;
            } else {
                dados_do_sprite[y][x] = converte_em_bgr(dados_da_imagem_estrela2[y][x]);//Converte pixel por pixel do formato RGB para o BGR
            }
        }
    }
    //Escreve os dados de cada pixel na memoria de sprites. [end_base = offset * 400]
    cria_sprite(3600, dados_do_sprite);
}

void gera_sprite_estrela_offset10(){
    //Dados para formar um sprite 20x20 em formato RRR GGG BBB - 9 bits
    uint16_t dados_do_sprite[altura_sprite][largura_sprite];
    uint16_t cor_temp;
    int y, x;
    for ( y = 0; y < altura_sprite; y++) {
        for ( x = 0; x < largura_sprite; x++) {
            cor_temp = dados_da_imagem_estrela3[y][x];
            if (cor_temp == 0xff){
                dados_do_sprite[y][x] = APAGA;
            } else {
                dados_do_sprite[y][x] = converte_em_bgr(dados_da_imagem_estrela3[y][x]);//Converte pixel por pixel do formato RGB para o BGR
            }
        }
    }
    //Escreve os dados de cada pixel na memoria de sprites. [end_base = offset * 400]
    cria_sprite(4000, dados_do_sprite);
}

void grava_sprite_estrela(){
    gera_sprite_estrela_offset8();
    gera_sprite_estrela_offset9();
    gera_sprite_estrela_offset10();
}

void animacao_estrela(uint32_t pos_xy_20b, int sp){    
    while (1) {    
        int i;
        for (i=8; i<10; i++){
            exibe_sprite(sp, pos_xy_20b, i, 11);
            usleep(100000);
        }
    }
}

//portal


//Dados da imagem para formar um sprite (20x20) em formato RRRGGGBB - 1 byte por pixel
uint16_t dados_da_imagem_1[altura_sprite][largura_sprite] = {
{ 0xff,0xff,0xff,0xff,0xff,0xff,0x52,0x76,0x76,0x76,0x76,0x76,0x76,0x52,0xff,0xff,0xff,0xff,0xff,0xff },
{ 0xff,0xff,0xff,0xff,0x52,0x52,0x52,0x76,0x76,0x76,0x76,0x76,0x76,0x52,0x52,0x52,0xff,0xff,0xff,0xff },
{ 0xff,0xff,0xff,0x52,0x52,0x52,0x76,0x76,0x52,0x52,0x52,0x52,0x52,0x52,0x52,0x52,0x52,0xff,0xff,0xff },
{ 0xff,0xff,0x29,0x52,0x76,0x76,0x76,0x52,0x52,0x29,0x29,0x29,0x29,0x29,0x29,0x29,0x52,0x29,0xff,0xff },
{ 0xff,0x29,0x29,0x52,0x76,0x52,0x52,0x52,0x29,0x29,0x29,0x25,0x25,0x25,0x29,0x29,0x29,0x29,0x29,0xff },
{ 0xff,0x29,0x29,0x76,0x76,0x52,0x29,0x29,0x29,0x29,0x29,0x00,0x00,0x25,0x25,0x25,0x29,0x29,0x29,0xff },
{ 0x29,0x29,0x76,0x76,0x52,0x52,0x29,0x29,0x00,0x00,0x00,0x00,0x00,0x25,0x25,0x25,0x25,0x29,0x29,0x29 },
{ 0x29,0x29,0x76,0x76,0x52,0x52,0x29,0x00,0x00,0x76,0x76,0x76,0x76,0x25,0x25,0x25,0x25,0x29,0x29,0x29 },
{ 0x29,0x29,0x29,0x76,0x52,0x52,0x29,0x00,0x76,0x76,0x76,0x00,0x76,0x76,0x25,0x25,0x25,0x25,0x29,0x29 },
{ 0x29,0x29,0x29,0x76,0x52,0x52,0x29,0x00,0x00,0x00,0x00,0x00,0x52,0x76,0x76,0x25,0x25,0x25,0x29,0x29 },
{ 0x29,0x29,0x29,0x76,0x52,0x52,0x29,0x00,0x00,0x00,0x00,0x00,0x52,0x76,0x76,0x25,0x25,0x25,0x29,0x29 },
{ 0x29,0x25,0x29,0x76,0x76,0x52,0x29,0x29,0x00,0x00,0x00,0x29,0x52,0x76,0x25,0x25,0x25,0x25,0x29,0x29 },
{ 0x25,0x25,0x25,0x29,0x76,0x52,0x52,0x29,0x29,0x29,0x29,0x29,0x52,0x25,0x25,0x25,0x25,0x25,0x29,0x29 },
{ 0x25,0x25,0x25,0x25,0x76,0x52,0x52,0x52,0x52,0x52,0x52,0x52,0x52,0x25,0x25,0x25,0x25,0x25,0x25,0x25 },
{ 0xff,0x25,0x25,0x25,0x76,0x76,0x76,0x52,0x52,0x52,0x52,0x76,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0xff },
{ 0xff,0x25,0x25,0x25,0x25,0x76,0x76,0x76,0x76,0x76,0x76,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0xff },
{ 0xff,0xff,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0xff,0xff },
{ 0xff,0xff,0xff,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0xff,0xff,0xff },
{ 0xff,0xff,0xff,0xff,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0xff,0xff,0xff,0xff },
{ 0xff,0xff,0xff,0xff,0xff,0xff,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0xff,0xff,0xff,0xff,0xff,0xff }
};

//Dados da imagem para formar um sprite (20x20) em formato RRRGGGBB - 1 byte por pixel
uint16_t dados_da_imagem_2[altura_sprite][largura_sprite] = {
{ 0xff,0xff,0xff,0xff,0xff,0xff,0x29,0x29,0x29,0x29,0x29,0x29,0x29,0x25,0xff,0xff,0xff,0xff,0xff,0xff},
{ 0xff,0xff,0xff,0xff,0x29,0x29,0x29,0x29,0x29,0x29,0x29,0x29,0x29,0x25,0x25,0x25,0xff,0xff,0xff,0xff},
{ 0xff,0xff,0xff,0x29,0x29,0x29,0x29,0x29,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0xff,0xff,0xff},
{ 0xff,0xff,0x52,0x52,0x29,0x29,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0xff,0xff},
{ 0xff,0x52,0x52,0x29,0x29,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0xff},
{ 0xff,0x52,0x52,0x29,0x29,0x25,0x25,0x25,0x25,0x76,0x76,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0xff},
{ 0x52,0x52,0x52,0x29,0x25,0x25,0x25,0x25,0x76,0x76,0x76,0x76,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25},
{ 0x76,0x76,0x52,0x29,0x25,0x00,0x00,0x76,0x76,0x52,0x52,0x52,0x52,0x52,0x25,0x25,0x25,0x25,0x25,0x25},
{ 0x76,0x76,0x52,0x29,0x25,0x00,0x00,0x76,0x00,0x00,0x00,0x29,0x29,0x52,0x76,0x25,0x25,0x25,0x25,0x25},
{ 0x76,0x76,0x52,0x29,0x29,0x29,0x00,0x76,0x76,0x00,0x00,0x00,0x29,0x52,0x52,0x76,0x25,0x25,0x25,0x25},
{ 0x76,0x76,0x52,0x29,0x29,0x29,0x00,0x76,0x76,0x00,0x00,0x00,0x29,0x52,0x52,0x76,0x25,0x25,0x25,0x25},
{ 0x76,0x76,0x52,0x29,0x29,0x29,0x00,0x76,0x76,0x00,0x00,0x00,0x29,0x52,0x52,0x76,0x25,0x25,0x25,0x25},
{ 0x76,0x76,0x52,0x52,0x29,0x29,0x00,0x00,0x76,0x00,0x00,0x00,0x29,0x52,0x52,0x76,0x25,0x25,0x25,0x25},
{ 0x76,0x76,0x76,0x52,0x52,0x29,0x29,0x00,0x00,0x00,0x00,0x29,0x29,0x52,0x52,0x76,0x25,0x25,0x25,0x25},
{ 0x52,0x52,0x76,0x76,0x52,0x29,0x29,0x29,0x29,0x29,0x29,0x29,0x52,0x52,0x76,0x76,0x25,0x25,0x25,0x25},
{ 0xff,0x52,0x52,0x76,0x52,0x52,0x52,0x52,0x52,0x52,0x52,0x52,0x52,0x52,0x76,0x76,0x25,0x25,0x25,0xff},
{ 0xff,0x52,0x52,0x76,0x76,0x76,0x52,0x52,0x52,0x52,0x52,0x76,0x76,0x76,0x76,0x25,0x25,0x25,0x25,0xff},
{ 0xff,0xff,0x52,0x52,0x52,0x76,0x76,0x76,0x76,0x76,0x76,0x76,0x29,0x25,0x25,0x25,0x25,0x25,0xff,0xff},
{ 0xff,0xff,0xff,0x29,0x29,0x29,0x76,0x76,0x29,0x29,0x29,0x29,0x25,0x25,0x25,0x25,0x25,0xff,0xff,0xff},
{ 0xff,0xff,0xff,0xff,0x29,0x29,0x29,0x29,0x29,0x29,0x29,0x25,0x25,0x25,0x25,0x25,0xff,0xff,0xff,0xff}
};

//Dados da imagem para formar um sprite (20x20) em formato RRRGGGBB - 1 byte por pixel
uint16_t dados_da_imagem_3[altura_sprite][largura_sprite] = {
{ 0xff,0xff,0xff,0xff,0xff,0xff,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0xff,0xff,0xff,0xff,0xff },
{ 0xff,0xff,0xff,0xff,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0xff,0xff,0xff },
{ 0xff,0xff,0xff,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0xff,0xff },
{ 0xff,0xff,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0xff },
{ 0xff,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x76,0x76,0x76,0x76,0x76,0x76,0x76,0x25,0x25,0x25,0x25 },
{ 0xff,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x76,0x52,0x52,0x52,0x52,0x52,0x76,0x76,0x76,0x25,0x25,0x25 },
{ 0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x52,0x52,0x52,0x52,0x52,0x52,0x52,0x52,0x52,0x76,0x25,0x25,0x25 },
{ 0x29,0x29,0x25,0x25,0x25,0x25,0x25,0x52,0x29,0x29,0x29,0x29,0x29,0x29,0x52,0x52,0x76,0x29,0x25,0x25 },
{ 0x29,0x29,0x25,0x25,0x25,0x25,0x76,0x52,0x29,0x00,0x00,0x00,0x00,0x29,0x29,0x52,0x76,0x76,0x29,0x25 },
{ 0x29,0x29,0x25,0x25,0x25,0x76,0x76,0x52,0x00,0x00,0x00,0x00,0x00,0x00,0x29,0x52,0x52,0x76,0x29,0x29 },
{ 0x29,0x29,0x25,0x25,0x25,0x76,0x76,0x52,0x00,0x00,0x00,0x00,0x00,0x00,0x29,0x52,0x52,0x76,0x29,0x29 },
{ 0x29,0x29,0x25,0x25,0x25,0x25,0x76,0x76,0x00,0x76,0x76,0x76,0x76,0x00,0x29,0x52,0x52,0x76,0x29,0x29 },
{ 0x29,0x29,0x29,0x25,0x25,0x25,0x25,0x76,0x76,0x76,0x76,0x76,0x00,0x00,0x29,0x52,0x52,0x76,0x76,0x29 },
{ 0x29,0x29,0x29,0x25,0x25,0x25,0x25,0x00,0x00,0x00,0x00,0x00,0x00,0x29,0x29,0x52,0x52,0x76,0x76,0x29 },
{ 0xff,0x29,0x29,0x29,0x25,0x25,0x25,0x00,0x00,0x29,0x29,0x29,0x29,0x29,0x29,0x52,0x76,0x76,0x29,0x29 },
{ 0xff,0x29,0x29,0x29,0x29,0x29,0x25,0x25,0x25,0x29,0x29,0x29,0x29,0x52,0x52,0x52,0x76,0x52,0x29,0x29 },
{ 0xff,0xff,0x29,0x52,0x29,0x29,0x29,0x29,0x29,0x29,0x29,0x29,0x52,0x52,0x76,0x76,0x76,0x52,0x29,0xff },
{ 0xff,0xff,0xff,0x52,0x52,0x52,0x52,0x52,0x52,0x52,0x52,0x52,0x52,0x76,0x76,0x52,0x52,0x52,0xff,0xff },
{ 0xff,0xff,0xff,0xff,0x52,0x52,0x52,0x76,0x76,0x76,0x76,0x76,0x76,0x76,0x52,0x52,0x52,0xff,0xff,0xff },
{ 0xff,0xff,0xff,0xff,0xff,0xff,0x52,0x76,0x76,0x76,0x76,0x76,0x76,0x76,0x52,0xff,0xff,0xff,0xff,0xff }
};

//Dados da imagem para formar um sprite (20x20) em formato RRRGGGBB - 1 byte por pixel
uint16_t dados_da_imagem_4[altura_sprite][largura_sprite] = {
{ 0xff,0xff,0xff,0xff,0x25,0x25,0x25,0x25,0x25,0x29,0x29,0x29,0x29,0x29,0x29,0x29,0xff,0xff,0xff,0xff },
{ 0xff,0xff,0xff,0x25,0x25,0x25,0x25,0x25,0x29,0x29,0x29,0x29,0x76,0x76,0x29,0x29,0x29,0xff,0xff,0xff },
{ 0xff,0xff,0x25,0x25,0x25,0x25,0x25,0x29,0x76,0x76,0x76,0x76,0x76,0x76,0x76,0x52,0x52,0x52,0xff,0xff },
{ 0xff,0x25,0x25,0x25,0x25,0x76,0x76,0x76,0x76,0x52,0x52,0x52,0x52,0x52,0x76,0x76,0x76,0x52,0x52,0xff },
{ 0xff,0x25,0x25,0x25,0x76,0x76,0x52,0x52,0x52,0x52,0x52,0x52,0x52,0x52,0x52,0x52,0x76,0x52,0x52,0xff },
{ 0x25,0x25,0x25,0x25,0x76,0x76,0x52,0x52,0x29,0x29,0x29,0x29,0x29,0x29,0x29,0x52,0x76,0x76,0x52,0x52 },
{ 0x25,0x25,0x25,0x25,0x76,0x52,0x52,0x29,0x29,0x00,0x00,0x00,0x00,0x29,0x29,0x52,0x52,0x76,0x76,0x76 },
{ 0x25,0x25,0x25,0x25,0x76,0x52,0x52,0x29,0x00,0x00,0x00,0x76,0x00,0x00,0x29,0x29,0x52,0x52,0x76,0x76 },
{ 0x25,0x25,0x25,0x25,0x76,0x52,0x52,0x29,0x00,0x00,0x00,0x76,0x76,0x00,0x29,0x29,0x29,0x52,0x76,0x76 },
{ 0x25,0x25,0x25,0x25,0x76,0x52,0x52,0x29,0x00,0x00,0x00,0x76,0x76,0x00,0x29,0x29,0x29,0x52,0x76,0x76 },
{ 0x25,0x25,0x25,0x25,0x76,0x52,0x52,0x29,0x00,0x00,0x00,0x76,0x76,0x00,0x29,0x29,0x29,0x52,0x76,0x76 },
{ 0x25,0x25,0x25,0x25,0x76,0x52,0x52,0x29,0x00,0x00,0x00,0x76,0x76,0x00,0x29,0x29,0x29,0x52,0x76,0x76 },
{ 0x25,0x25,0x25,0x25,0x25,0x76,0x52,0x29,0x29,0x00,0x00,0x00,0x76,0x00,0x00,0x25,0x29,0x52,0x76,0x76 },
{ 0x25,0x25,0x25,0x25,0x25,0x25,0x52,0x52,0x52,0x52,0x52,0x76,0x76,0x00,0x00,0x25,0x29,0x52,0x76,0x76 },
{ 0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x76,0x76,0x76,0x76,0x25,0x25,0x25,0x25,0x29,0x52,0x52,0x52 },
{ 0xff,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x76,0x76,0x25,0x25,0x25,0x25,0x29,0x29,0x52,0x52,0xff },
{ 0xff,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x29,0x29,0x52,0x52,0xff },
{ 0xff,0xff,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x29,0x29,0x52,0x52,0xff,0xff },
{ 0xff,0xff,0xff,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x29,0x29,0x29,0x29,0x29,0xff,0xff,0xff },
{ 0xff,0xff,0xff,0xff,0x25,0x25,0x25,0x29,0x29,0x29,0x29,0x29,0x29,0x29,0x29,0x29,0xff,0xff,0xff,0xff }
};

void gera_sprite_portal_ofst4(){
    //Dados para formar um sprite 20x20 em formato RRR GGG BBB - 9 bits
    uint16_t dados_do_sprite[altura_sprite][largura_sprite];
    uint16_t cor_temp;
    int y, x;
    for ( y = 0; y < altura_sprite; y++) {
        for ( x = 0; x < largura_sprite; x++) {
            cor_temp = dados_da_imagem_1[y][x];
            if (cor_temp == 0xff){
                dados_do_sprite[y][x] = 510;
            } else {
                dados_do_sprite[y][x] = converte_em_bgr(dados_da_imagem_1[y][x]);//Converte pixel por pixel do formato RGB para o BGR
            }
        }
    }
    //Escreve os dados de cada pixel na memoria de sprites. [end_base = offset * 400]
    cria_sprite(1600, dados_do_sprite);
}

void gera_sprite_portal_ofst5(){
    //Dados para formar um sprite 20x20 em formato RRR GGG BBB - 9 bits
    uint16_t dados_do_sprite[altura_sprite][largura_sprite];
    uint16_t cor_temp;
    int y, x;
    for ( y = 0; y < altura_sprite; y++) {
        for ( x = 0; x < largura_sprite; x++) {
            cor_temp = dados_da_imagem_2[y][x];
            if (cor_temp == 0xff){
                dados_do_sprite[y][x] = 510;
            } else {
                dados_do_sprite[y][x] = converte_em_bgr(dados_da_imagem_2[y][x]);//Converte pixel por pixel do formato RGB para o BGR
            }
        }
    }
    //Escreve os dados de cada pixel na memoria de sprites. [end_base = offset * 400]
    cria_sprite(2000, dados_do_sprite);
}

void gera_sprite_portal_ofst6(){
    //Dados para formar um sprite 20x20 em formato RRR GGG BBB - 9 bits
    uint16_t dados_do_sprite[altura_sprite][largura_sprite];
    uint16_t cor_temp;
    int y, x;
    for ( y = 0; y < altura_sprite; y++) {
        for ( x = 0; x < largura_sprite; x++) {
            cor_temp = dados_da_imagem_3[y][x];
            if (cor_temp == 0xff){
                dados_do_sprite[y][x] = 510;
            } else {
                dados_do_sprite[y][x] = converte_em_bgr(dados_da_imagem_3[y][x]);//Converte pixel por pixel do formato RGB para o BGR
            }
        }
    }
    //Escreve os dados de cada pixel na memoria de sprites. [end_base = offset * 400]
    cria_sprite(2400, dados_do_sprite);
}

void gera_sprite_portal_ofst7(){
        //Dados para formar um sprite 20x20 em formato RRR GGG BBB - 9 bits
    uint16_t dados_do_sprite[altura_sprite][largura_sprite];
    uint16_t cor_temp;
    int y, x;
    for ( y = 0; y < altura_sprite; y++) {
        for ( x = 0; x < largura_sprite; x++) {
            cor_temp = dados_da_imagem_4[y][x];
            if (cor_temp == 0xff){
                dados_do_sprite[y][x] = 510;
            } else {
                dados_do_sprite[y][x] = converte_em_bgr(dados_da_imagem_4[y][x]);//Converte pixel por pixel do formato RGB para o BGR
            }
        }
    }
    //Escreve os dados de cada pixel na memoria de sprites. [end_base = offset * 400]
    cria_sprite(2800, dados_do_sprite);
}

void grava_sprite_portal(){
    gera_sprite_portal_ofst4();
    gera_sprite_portal_ofst5();
    gera_sprite_portal_ofst6();
    gera_sprite_portal_ofst7();
}

void animacao_portal(uint32_t pos_xy_20b, int sp){    
    while (1) {    
        int i;
        for (i=4; i<8; i++){
            exibe_sprite(sp, pos_xy_20b, i, 10);
            usleep(100000);
        }
    }
}



