#include "proc_grafico.h"
#include "acelerometro.c"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

extern void inicializa_fpga();
extern void fecha_dev_mem();
extern void exibe_sprite(uint8_t sp, uint32_t xy, uint16_t offset, uint8_t registrador);
extern void altera_pixel_sprite(uint16_t cor, uint16_t endereco);

#define largura_sprite 20
#define altura_sprite 20

//Dados da imagem para formar um sprite (20x20) em formato RRRGGGBB - 1 byte por pixel
uint8_t dados_da_imagem2[altura_sprite][largura_sprite] = {
{ 0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff },
{ 0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x96,0x76,0x76,0x96,0xdb,0xff,0xff,0xff,0xff,0xff,0xff,0xff },
{ 0xff,0xff,0xff,0xff,0xff,0xff,0xdb,0x76,0xbf,0x7f,0x7f,0x7f,0x7b,0xbb,0xff,0xff,0xff,0xff,0xff,0xff },
{ 0xff,0xff,0xff,0xff,0xff,0xff,0x76,0xdf,0xff,0x9f,0x7f,0x7f,0x7f,0x76,0xff,0xff,0xff,0xff,0xff,0xff },
{ 0xff,0xff,0xff,0xff,0xb6,0x4d,0x7b,0xdf,0xbf,0x7f,0x7f,0x7f,0x7f,0x7f,0x49,0x92,0xdb,0xff,0xff,0xff },
{ 0xff,0xff,0x96,0x49,0x96,0x29,0x7b,0x9f,0x7f,0x7f,0x7f,0x7f,0x7f,0x7f,0x29,0x96,0x4d,0x71,0xff,0xff },
{ 0xff,0x96,0x72,0x97,0x96,0x4d,0x51,0x56,0x7f,0x7f,0x7f,0x7f,0x57,0x31,0x4d,0x96,0x97,0x72,0x76,0xff },
{ 0xdb,0x35,0x97,0x97,0x96,0x4e,0x49,0x2d,0x51,0x2d,0x2d,0x4d,0x29,0x4d,0x49,0x92,0x97,0x97,0x35,0xb6 },
{ 0x96,0x35,0x96,0x72,0x6e,0x96,0x6e,0x49,0x29,0x29,0x29,0x29,0x29,0x6e,0x92,0x72,0x6e,0x96,0x35,0x76 },
{ 0x96,0x35,0x2d,0x92,0x97,0x97,0x97,0x96,0x92,0x72,0x72,0x72,0x92,0x97,0x97,0x97,0x96,0x4d,0x35,0x72 },
{ 0xb6,0x2d,0x35,0x55,0x76,0x76,0x72,0x97,0x97,0x92,0x96,0x97,0x97,0x72,0x76,0x96,0x55,0x35,0x2d,0x92 },
{ 0xff,0x29,0x31,0x35,0x35,0x59,0x76,0x96,0x96,0x92,0x96,0x96,0x96,0x76,0x59,0x35,0x35,0x35,0x29,0xdb },
{ 0xff,0xba,0x79,0x31,0x35,0x35,0x35,0x35,0x35,0x35,0x35,0x35,0x35,0x35,0x35,0x35,0x31,0x75,0xba,0xff },
{ 0xff,0xdb,0x99,0x55,0x29,0x31,0x31,0x31,0x31,0x31,0x31,0x31,0x31,0x31,0x31,0x29,0x51,0x99,0xbb,0xff },
{ 0xff,0x97,0x96,0x96,0x72,0x79,0x99,0x2d,0x29,0x55,0x59,0x2d,0x2d,0x79,0x79,0x76,0x96,0x96,0x97,0xff },
{ 0xff,0xb7,0x97,0x97,0x96,0x96,0x51,0x25,0x25,0x51,0x75,0x25,0x25,0x51,0x96,0x96,0x97,0x97,0x97,0xff },
{ 0xff,0xdb,0x97,0x97,0x97,0x97,0x97,0x96,0x96,0x92,0x72,0x96,0x96,0x97,0x97,0x97,0x97,0x97,0xdb,0xff },
{ 0xff,0xff,0xdb,0x97,0x97,0x97,0x97,0x97,0x97,0x97,0x97,0x97,0x97,0x97,0x97,0x97,0x97,0xbb,0xff,0xff },
{ 0xff,0xff,0xff,0xff,0xdb,0xb7,0x97,0x97,0x97,0x97,0x97,0x97,0x97,0x97,0xb7,0xbb,0xff,0xff,0xff,0xff },
{ 0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xdf,0xdf,0xdb,0xdb,0xdf,0xdf,0xff,0xff,0xff,0xff,0xff,0xff,0xff }
};

//Dados da imagem para formar um sprite (20x20) em formato RRRGGGBB - 1 byte por pixel
uint8_t dados_da_imagem1[altura_sprite][largura_sprite] = {
{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },
{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },
{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },
{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },
{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x9b,0x9f,0x9f,0x9f,0x9b,0xdf,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },
{ 0x00,0x00,0x00,0x00,0x00,0x00,0x9b,0xbf,0xff,0xbf,0x9f,0x9f,0x7a,0xdf,0x00,0x00,0x00,0x00,0x00,0x00 },
{ 0x00,0x00,0x00,0x00,0x00,0x76,0x9f,0x9f,0xff,0xff,0x9f,0x9f,0x9b,0x7a,0x00,0x00,0x00,0x00,0x00,0x00 },
{ 0x00,0x00,0x00,0x00,0x00,0x56,0x7a,0x7b,0x7f,0x7f,0x7f,0x7b,0x7a,0x56,0x00,0x00,0x00,0x00,0x00,0x00 },
{ 0x00,0x00,0x00,0x24,0x00,0x49,0x51,0x76,0x7a,0x7b,0x7a,0x7a,0x51,0x28,0x00,0x24,0x00,0x00,0x00,0x00 },
{ 0x00,0x00,0x54,0x92,0x92,0x49,0x28,0x55,0x76,0x7b,0x76,0x55,0x24,0x49,0x92,0x92,0x54,0x00,0x00,0x00 },
{ 0x2c,0x54,0x56,0x92,0xb6,0x24,0xb6,0x72,0x71,0x49,0x71,0x72,0xb6,0x24,0xb6,0x92,0x56,0x54,0x2c,0x00 },
{ 0xf8,0x56,0x92,0x49,0x24,0x49,0x92,0x92,0x6d,0x6d,0x6d,0x92,0x92,0x49,0x24,0x49,0x92,0x56,0xf8,0x00 },
{ 0x2c,0x54,0x00,0x24,0xb6,0x72,0x72,0x92,0x6d,0x49,0x6d,0x92,0x72,0x72,0xb6,0x24,0x00,0x54,0x2c,0x00 },
{ 0x00,0x2c,0xfd,0x00,0x34,0x55,0x6d,0x92,0x6d,0x49,0x6d,0x92,0x4d,0x51,0x54,0x00,0xfd,0x2c,0x00,0x00 },
{ 0x00,0x00,0x59,0x08,0x2c,0x34,0x00,0x49,0x24,0x49,0x24,0x49,0x24,0x30,0x2c,0x08,0x59,0x00,0x00,0x00 },
{ 0x00,0x00,0x00,0x00,0x00,0x2c,0x08,0x00,0x2c,0xf8,0x2c,0x00,0x08,0x2c,0x00,0x00,0x00,0x00,0x00,0x00 },
{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },
{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },
{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },
{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 }
};

//Dados da imagem para formar um sprite (20x20) em formato RRRGGGBB - 1 byte por pixel
uint8_t dados_da_imagem[altura_sprite][largura_sprite] = {
{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },
{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },
{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },
{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },
{ 0x00,0x00,0x00,0x00,0x00,0x00,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },
{ 0x00,0x00,0x00,0x00,0x00,0xfe,0xfe,0x00,0x00,0x00,0x00,0x00,0xfe,0xfe,0x00,0x00,0x00,0x00,0x00,0x00 },
{ 0x00,0x00,0x00,0x00,0xfe,0xfe,0x00,0x00,0x53,0x53,0x01,0x00,0x00,0xfe,0xfe,0x00,0x00,0x00,0x00,0x00 },
{ 0x00,0x00,0x00,0xfe,0xfe,0x00,0x00,0x29,0x01,0x01,0x01,0x01,0x00,0x00,0xfe,0xfe,0x00,0x00,0x00,0x00 },
{ 0x00,0xfe,0xfe,0xfe,0x00,0x00,0x53,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0xfe,0xfe,0xfe,0x00,0x00 },
{ 0xfe,0xfe,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xfe,0xfe,0x00 },
{ 0xfe,0x00,0x00,0xfd,0x00,0xfd,0x00,0xfd,0x00,0xfd,0x00,0xfd,0x00,0xfd,0x00,0xfd,0x00,0x00,0xfe,0x00 },
{ 0xfe,0xfe,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xfe,0xfe,0x00 },
{ 0x00,0xfe,0xfe,0x00,0x00,0xe8,0xc0,0xc0,0xc0,0xdd,0xe1,0xe1,0xe1,0xe8,0x00,0x00,0xfe,0xfe,0x00,0x00 },
{ 0x00,0x00,0xfe,0xfe,0xfe,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xfe,0xfe,0xfe,0x00,0x00,0x00 },
{ 0x00,0x00,0x00,0x00,0xfe,0xfe,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xfe,0xfe,0x00,0x00,0x00,0x00,0x00 },
{ 0x00,0x00,0x00,0x00,0x00,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0x00,0x00,0x00,0x00,0x00,0x00 },
{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },
{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },
{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },
{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 }
};

//Dados da imagem para formar um sprite (20x20) em formato RRRGGGBB - 1 byte por pixel
uint8_t dados_da_imagem_ovni[altura_sprite][largura_sprite] = {
{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },
{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },
{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x76,0x9b,0x9b,0x9b,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },
{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x76,0x9f,0x9f,0x9f,0x9f,0x9b,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },
{ 0x00,0x00,0x00,0x00,0x00,0x00,0x76,0x9f,0x9f,0xc4,0xc4,0x9f,0x9f,0x9b,0x00,0x00,0x00,0x00,0x00,0x00 },
{ 0x00,0x00,0x00,0x00,0x00,0x00,0x76,0x9f,0xc4,0x40,0x40,0xc4,0x9f,0x9b,0x00,0x00,0x00,0x00,0x00,0x00 },
{ 0x00,0x00,0x00,0x00,0x00,0x76,0x9f,0x9f,0xc4,0x80,0x80,0xc4,0x9f,0x9f,0x9b,0x00,0x00,0x00,0x00,0x00 },
{ 0x00,0x00,0x00,0x00,0x00,0x76,0x9f,0xc4,0x40,0xf2,0xf2,0x40,0xc4,0x9f,0x9b,0x00,0x00,0x00,0x00,0x00 },
{ 0x00,0x00,0x00,0x00,0x72,0x76,0x56,0x52,0x52,0x52,0x52,0x52,0x52,0x76,0x7a,0x9b,0x00,0x00,0x00,0x00 },
{ 0x00,0x00,0x00,0x00,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x00,0x00,0x00,0x00 },
{ 0x00,0x00,0x00,0x92,0x00,0x92,0x92,0x00,0x00,0x92,0x92,0x00,0x00,0x92,0x92,0x00,0x92,0x00,0x00,0x00 },
{ 0x00,0x00,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x00,0x00 },
{ 0x00,0xfd,0xf8,0xfd,0xf8,0xfd,0xf8,0xfd,0xf8,0xfd,0xf8,0xfd,0xf8,0xfd,0xf8,0xfd,0xf8,0xfd,0xf8,0x00 },
{ 0x00,0x00,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x00,0x00 },
{ 0x00,0x00,0x00,0x92,0x00,0x92,0x92,0x00,0x00,0x92,0x92,0x00,0x00,0x92,0x92,0x00,0x92,0x00,0x00,0x00 },
{ 0x00,0x00,0x00,0x00,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x00,0x00,0x00,0x00 },
{ 0x00,0x00,0x00,0x00,0x00,0x00,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x00,0x00,0x00,0x00,0x00,0x00 },
{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x49,0x49,0x49,0x49,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },
{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },
{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 }
};

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

void gera_sprite_ovni(){
    //Dados para formar um sprite 20x20 em formato RRR GGG BBB - 9 bits
    uint16_t dados_do_sprite[altura_sprite][largura_sprite];

    int y, x;
    for ( y = 0; y < altura_sprite; y++) {
        for ( x = 0; x < largura_sprite; x++) {
            dados_do_sprite[y][x] = converte_em_bgr(dados_da_imagem[y][x]);//Converte pixel por pixel do formato RGB para o BGR
        }
    }

    //Escreve os dados de cada pixel na memoria de sprites. [end_base = offset * 400]
    cria_sprite(1600, dados_do_sprite);
}

//Exibe e move um sprite armazenado na memoria de sprites pela tela
void move_sprite() {
    #define mascara_10bits 0b1111111111
    uint16_t pos_x = 350;
    uint16_t pos_y = 240;

    pos_x &= mascara_10bits;
    pos_y &= mascara_10bits;
    
    uint32_t pos_xy_20b;
    pos_xy_20b = (pos_x << 10 | pos_y);
    uint32_t pos_xy_20b_ant = (pos_xy_20b); //inicia com posicao anterior igual a posicao atual

    int direcao_sprite = 1; //1 descendo e -1 subindo
    int i = 0;

    while (i < 1000) {
        //apaga o sprite exibido na posicao anterior
        exibe_sprite(0, pos_xy_20b_ant, 4, 1);//sp = 0 - desabilita sprite
        pos_xy_20b_ant = pos_xy_20b;
    
        //exibe o sprite na posicao atual
        exibe_sprite(1, pos_xy_20b, 4, 1);//sp = 1 - habilita sprite

        //verifica os limites da tela para ajustar a direcao
        //tela 640 x 480
        if (direcao_sprite == 1 && (pos_xy_20b < 358850) ){
            pos_xy_20b+= 10;//posicao atual + 10
            if(pos_xy_20b == 358850){
                //101011110 0111000010 -> x = 0101011110 = 350, y = 0111000010 = 450
                direcao_sprite = -1;
            }
        }

        else if (direcao_sprite == -1 && (pos_xy_20b > 358410) ){
            pos_xy_20b-=10;//posicao atual - 10
            if(pos_xy_20b == 358410){
                //0101011110 0000001010 -> x = 0101011110 = 350, y = 0000001010 = 10
                direcao_sprite = 1;
            }
        }
        usleep(10000);
        i++;
    }
}

//Move 1 eixo por vez
//Exibe e move um sprite armazenado na memoria de sprites pela tela controlado pelo acelerometro
void move_sprite_acel() {
    // tela 640 x 480 
    #define mascara_10bits 0b1111111111
    #define limite_superior_eixoY 10
    #define limite_inferior_eixoY 450
    #define limite_esquerdo_eixoX 10
    #define limite_direito_eixoX 610

    uint16_t pos_x = 350;
    uint16_t pos_y = 240;

    pos_x &= mascara_10bits;
    pos_y &= mascara_10bits;
    
    uint32_t pos_xy_20b;
    pos_xy_20b = (pos_x << 10 | pos_y);
    
    uint32_t pos_xy_20b_ant = (pos_xy_20b); //inicia com posicao anterior igual a posicao atual

    int direcao_sprite;
    int velocidade = 1;

    gera_sprite_ovni();

    while (1) {
        pos_y = (pos_xy_20b & mascara_10bits);
        pos_x = ((pos_xy_20b >> 10) & mascara_10bits);

        direcao_sprite = get_movimento(&velocidade); //8 cima, 2 baixo, 6 direita, 4 esquerda, 0 sem movimento

        //apaga o sprite exibido na posicao anterior
        exibe_sprite(0, pos_xy_20b_ant, 4, 1);//sp = 0 - desabilita sprite
        pos_xy_20b_ant = pos_xy_20b;
    
        //exibe o sprite na posicao atual
        exibe_sprite(1, pos_xy_20b, 4, 1);//sp = 1 - habilita sprite

        //descendo
        if ( direcao_sprite == 2 ){
            if (pos_y < limite_inferior_eixoY) {
                pos_y += (5*velocidade);//posicao atual + 10 * (1,2 ou 3)
            } else {
                pos_y == limite_inferior_eixoY; //fica no limite da tela
            }
        }
        //subindo
        else if ( direcao_sprite == 8 ){
            if (pos_y > limite_superior_eixoY) {
                pos_y -= (5*velocidade);//posicao atual + 10 * (1,2 ou 3)
            } else {
                pos_y == limite_superior_eixoY; //fica no limite da tela
            }
        }
        //direita
        else if ( direcao_sprite == 6 ){
            if (pos_x < limite_direito_eixoX) {
                pos_x += (5*velocidade);//posicao atual + 10 * (1,2 ou 3)
            } else {
                pos_x == limite_direito_eixoX; //fica no limite da tela
            }
        }
        //esquerda
        else if ( direcao_sprite == 4 ){
            if (pos_x > limite_esquerdo_eixoX) {
                pos_x -= (5*velocidade);//posicao atual + 10 * (1,2 ou 3)
            } else {
                pos_x == limite_direito_eixoX; //fica no limite da tela
            }
        }

        pos_xy_20b = (pos_x << 10 | pos_y);
        usleep(10000);
    }
}

int main(){
    inicializa_fpga();

    move_sprite_acel();

    fecha_dev_mem();   

   return 0;
}

/*
//Move 2 eixos por vez
//Exibe e move um sprite armazenado na memoria de sprites pela tela controlado pelo acelerometro
void move_sprite_acel() {
    // tela 640 x 480 
    #define mascara_10bits 0b1111111111
    #define limite_superior_eixoY 10
    #define limite_inferior_eixoY 450
    #define limite_esquerdo_eixoX 10
    #define limite_direito_eixoX 610

    uint16_t pos_x = 350;
    uint16_t pos_y = 240;

    pos_x &= mascara_10bits;
    pos_y &= mascara_10bits;
    
    uint32_t pos_xy_20b;
    pos_xy_20b = (pos_x << 10 | pos_y);
    
    uint32_t pos_xy_20b_ant = (pos_xy_20b); //inicia com posicao anterior igual a posicao atual

    int direcao_y_sprite, direcao_x_sprite;
    int velocidade_y, velocidade_x = 1;

    int i = 0;

    while (i != -1) {
        pos_y = (pos_xy_20b & mascara_10bits);
        pos_x = ((pos_xy_20b >> 10) & mascara_10bits);

        direcao_y_sprite = -get_direcao_movimento_y(&velocidade_y); //1 descendo e -1 subindo
        direcao_x_sprite = get_direcao_movimento_x(&velocidade_x); //1 direita e -1 esquerda

        //apaga o sprite exibido na posicao anterior
        exibe_sprite(0, pos_xy_20b_ant, 5, 1);//sp = 0 - desabilita sprite
        pos_xy_20b_ant = pos_xy_20b;
    
        //exibe o sprite na posicao atual
        exibe_sprite(1, pos_xy_20b, 5, 1);//sp = 1 - habilita sprite

        //descendo
        if ( direcao_y_sprite == 1 ){
            if (pos_y < limite_inferior_eixoY) {
                pos_y += (10*velocidade_y);//posicao atual + 10 * (1,2 ou 3)
            } else {
                pos_y == limite_inferior_eixoY; //fica no limite da tela
            }
        }
        //subindo
        else if ( direcao_y_sprite == -1 ){
            if (pos_y > limite_superior_eixoY) {
                pos_y -= (10*velocidade_y);//posicao atual + 10 * (1,2 ou 3)
            } else {
                pos_y == limite_superior_eixoY; //fica no limite da tela
            }
        }
        //direita
        if ( direcao_x_sprite == 1 ){
            if (pos_x < limite_direito_eixoX) {
                pos_x += (10*velocidade_x);//posicao atual + 10 * (1,2 ou 3)
            } else {
                pos_x == limite_direito_eixoX; //fica no limite da tela
            }
        }
        //esquerda
        else if ( direcao_x_sprite == -1 ){
            if (pos_x > limite_esquerdo_eixoX) {
                pos_x -= (10*velocidade_x);//posicao atual + 10 * (1,2 ou 3)
            } else {
                pos_x == limite_direito_eixoX; //fica no limite da tela
            }
        }

        pos_xy_20b = (pos_x << 10 | pos_y);

        usleep(10000);
        i++;
    }
}
*/

/*
//Exibe e move um sprite armazenado na memoria de sprites pela tela controlado pelo acelerometro
void move_sprite_acel() {
    #define mascara_10bits 0b1111111111
    #define limite_superior_eixoY 450
    #define limite_inferior_eixoY 10

    uint16_t pos_x = 350;
    uint16_t pos_y = 240;

    pos_x &= mascara_10bits;
    pos_y &= mascara_10bits;
    
    uint32_t pos_xy_20b;
    pos_xy_20b = (pos_x << 10 | pos_y);

    uint32_t pos_xy_20b_ant = (pos_xy_20b); //inicia com posicao anterior igual a posicao atual

    int velocidade = 1;
    int direcao_y_sprite = -get_direcao_movimento_y(&velocidade); //1 descendo e -1 subindo

    int i = 0;

    while (i != -1) {
        //apaga o sprite exibido na posicao anterior
        exibe_sprite(0, pos_xy_20b_ant, 5, 1);//sp = 0 - desabilita sprite
        pos_xy_20b_ant = pos_xy_20b;
    
        //exibe o sprite na posicao atual
        exibe_sprite(1, pos_xy_20b, 5, 1);//sp = 1 - habilita sprite

        //verifica os limites da tela para ajustar a direcao
        //tela 640 x 480
        if (direcao_y_sprite == 1 && (pos_xy_20b < 358850) ){
            pos_xy_20b+= (10*velocidade);//posicao atual + 10 * (1,2 ou 3)
            if(pos_xy_20b == 358850){
                //101011110 0111000010 -> x = 0101011110 = 350, y = 0111000010 = 450
                //direcao_y_sprite = -1;
                pos_xy_20b == 358850; //fica no limite da tela
            }
        }
        else if (direcao_y_sprite == -1 && (pos_xy_20b > 358410) ){
            pos_xy_20b-= (10*velocidade);//posicao atual - 10 * (1,2 ou 3)
            if(pos_xy_20b == 358410){
                //0101011110 0000001010 -> x = 0101011110 = 350, y = 0000001010 = 10
                //direcao_y_sprite = 1;
                pos_xy_20b == 358410; //fica no limite da tela
            }
        }
        usleep(10000);
        i++;
    }
}*/