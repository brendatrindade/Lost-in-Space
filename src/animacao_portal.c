#include "proc_grafico.h" //retirar
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include "sprite.c"

extern void inicializa_fpga(); //retirar
extern void fecha_dev_mem(); //retirar

int usleep(useconds_t usec);
void gera_sprite_portal_ofst4();
void gera_sprite_portal_ofst5();
void gera_sprite_portal_ofst6();
void gera_sprite_portal_ofst7();
void grava_sprite_portal();
void animacao_portal();
int main();

//Dados da imagem para formar um sprite (20x20) em formato RRRGGGBB - 1 byte por pixel
uint16_t dados_da_imagem_1[altura_sprite][largura_sprite] = {
0xff,0xff,0xff,0xff,0xff,0xff,0x52,0x76,0x76,0x76,0x76,0x76,0x76,0x52,0xff,0xff,0xff,0xff,0xff,0xff
,0xff,0xff,0xff,0xff,0x52,0x52,0x52,0x76,0x76,0x76,0x76,0x76,0x76,0x52,0x52,0x52,0xff,0xff,0xff,0xff
,0xff,0xff,0xff,0x52,0x52,0x52,0x76,0x76,0x52,0x52,0x52,0x52,0x52,0x52,0x52,0x52,0x52,0xff,0xff,0xff
,0xff,0xff,0x29,0x52,0x76,0x76,0x76,0x52,0x52,0x29,0x29,0x29,0x29,0x29,0x29,0x29,0x52,0x29,0xff,0xff
,0xff,0x29,0x29,0x52,0x76,0x52,0x52,0x52,0x29,0x29,0x29,0x25,0x25,0x25,0x29,0x29,0x29,0x29,0x29,0xff
,0xff,0x29,0x29,0x76,0x76,0x52,0x29,0x29,0x29,0x29,0x29,0x00,0x00,0x25,0x25,0x25,0x29,0x29,0x29,0xff
,0x29,0x29,0x76,0x76,0x52,0x52,0x29,0x29,0x00,0x00,0x00,0x00,0x00,0x25,0x25,0x25,0x25,0x29,0x29,0x29
,0x29,0x29,0x76,0x76,0x52,0x52,0x29,0x00,0x00,0x76,0x76,0x76,0x76,0x25,0x25,0x25,0x25,0x29,0x29,0x29
,0x29,0x29,0x29,0x76,0x52,0x52,0x29,0x00,0x76,0x76,0x76,0x00,0x76,0x76,0x25,0x25,0x25,0x25,0x29,0x29
,0x29,0x29,0x29,0x76,0x52,0x52,0x29,0x00,0x00,0x00,0x00,0x00,0x52,0x76,0x76,0x25,0x25,0x25,0x29,0x29
,0x29,0x29,0x29,0x76,0x52,0x52,0x29,0x00,0x00,0x00,0x00,0x00,0x52,0x76,0x76,0x25,0x25,0x25,0x29,0x29
,0x29,0x25,0x29,0x76,0x76,0x52,0x29,0x29,0x00,0x00,0x00,0x29,0x52,0x76,0x25,0x25,0x25,0x25,0x29,0x29
,0x25,0x25,0x25,0x29,0x76,0x52,0x52,0x29,0x29,0x29,0x29,0x29,0x52,0x25,0x25,0x25,0x25,0x25,0x29,0x29
,0x25,0x25,0x25,0x25,0x76,0x52,0x52,0x52,0x52,0x52,0x52,0x52,0x52,0x25,0x25,0x25,0x25,0x25,0x25,0x25
,0xff,0x25,0x25,0x25,0x76,0x76,0x76,0x52,0x52,0x52,0x52,0x76,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0xff
,0xff,0x25,0x25,0x25,0x25,0x76,0x76,0x76,0x76,0x76,0x76,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0xff
,0xff,0xff,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0xff,0xff
,0xff,0xff,0xff,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0xff,0xff,0xff
,0xff,0xff,0xff,0xff,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0xff,0xff,0xff,0xff
,0xff,0xff,0xff,0xff,0xff,0xff,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0xff,0xff,0xff,0xff,0xff,0xff
};

//Dados da imagem para formar um sprite (20x20) em formato RRRGGGBB - 1 byte por pixel
uint16_t dados_da_imagem_2[altura_sprite][largura_sprite] = {
0xff,0xff,0xff,0xff,0xff,0xff,0x29,0x29,0x29,0x29,0x29,0x29,0x29,0x25,0xff,0xff,0xff,0xff,0xff,0xff
,0xff,0xff,0xff,0xff,0x29,0x29,0x29,0x29,0x29,0x29,0x29,0x29,0x29,0x25,0x25,0x25,0xff,0xff,0xff,0xff
,0xff,0xff,0xff,0x29,0x29,0x29,0x29,0x29,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0xff,0xff,0xff
,0xff,0xff,0x52,0x52,0x29,0x29,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0xff,0xff
,0xff,0x52,0x52,0x29,0x29,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0xff
,0xff,0x52,0x52,0x29,0x29,0x25,0x25,0x25,0x25,0x76,0x76,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0xff
,0x52,0x52,0x52,0x29,0x25,0x25,0x25,0x25,0x76,0x76,0x76,0x76,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25
,0x76,0x76,0x52,0x29,0x25,0x00,0x00,0x76,0x76,0x52,0x52,0x52,0x52,0x52,0x25,0x25,0x25,0x25,0x25,0x25
,0x76,0x76,0x52,0x29,0x25,0x00,0x00,0x76,0x00,0x00,0x00,0x29,0x29,0x52,0x76,0x25,0x25,0x25,0x25,0x25
,0x76,0x76,0x52,0x29,0x29,0x29,0x00,0x76,0x76,0x00,0x00,0x00,0x29,0x52,0x52,0x76,0x25,0x25,0x25,0x25
,0x76,0x76,0x52,0x29,0x29,0x29,0x00,0x76,0x76,0x00,0x00,0x00,0x29,0x52,0x52,0x76,0x25,0x25,0x25,0x25
,0x76,0x76,0x52,0x29,0x29,0x29,0x00,0x76,0x76,0x00,0x00,0x00,0x29,0x52,0x52,0x76,0x25,0x25,0x25,0x25
,0x76,0x76,0x52,0x52,0x29,0x29,0x00,0x00,0x76,0x00,0x00,0x00,0x29,0x52,0x52,0x76,0x25,0x25,0x25,0x25
,0x76,0x76,0x76,0x52,0x52,0x29,0x29,0x00,0x00,0x00,0x00,0x29,0x29,0x52,0x52,0x76,0x25,0x25,0x25,0x25
,0x52,0x52,0x76,0x76,0x52,0x29,0x29,0x29,0x29,0x29,0x29,0x29,0x52,0x52,0x76,0x76,0x25,0x25,0x25,0x25
,0xff,0x52,0x52,0x76,0x52,0x52,0x52,0x52,0x52,0x52,0x52,0x52,0x52,0x52,0x76,0x76,0x25,0x25,0x25,0xff
,0xff,0x52,0x52,0x76,0x76,0x76,0x52,0x52,0x52,0x52,0x52,0x76,0x76,0x76,0x76,0x25,0x25,0x25,0x25,0xff
,0xff,0xff,0x52,0x52,0x52,0x76,0x76,0x76,0x76,0x76,0x76,0x76,0x29,0x25,0x25,0x25,0x25,0x25,0xff,0xff
,0xff,0xff,0xff,0x29,0x29,0x29,0x76,0x76,0x29,0x29,0x29,0x29,0x25,0x25,0x25,0x25,0x25,0xff,0xff,0xff
,0xff,0xff,0xff,0xff,0x29,0x29,0x29,0x29,0x29,0x29,0x29,0x25,0x25,0x25,0x25,0x25,0xff,0xff,0xff,0xff
};

//Dados da imagem para formar um sprite (20x20) em formato RRRGGGBB - 1 byte por pixel
uint16_t dados_da_imagem_3[altura_sprite][largura_sprite] = {
0xff,0xff,0xff,0xff,0xff,0xff,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0xff,0xff,0xff,0xff,0xff
,0xff,0xff,0xff,0xff,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0xff,0xff,0xff
,0xff,0xff,0xff,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0xff,0xff
,0xff,0xff,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0xff
,0xff,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x76,0x76,0x76,0x76,0x76,0x76,0x76,0x25,0x25,0x25,0x25
,0xff,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x76,0x52,0x52,0x52,0x52,0x52,0x76,0x76,0x76,0x25,0x25,0x25
,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x52,0x52,0x52,0x52,0x52,0x52,0x52,0x52,0x52,0x76,0x25,0x25,0x25
,0x29,0x29,0x25,0x25,0x25,0x25,0x25,0x52,0x29,0x29,0x29,0x29,0x29,0x29,0x52,0x52,0x76,0x29,0x25,0x25
,0x29,0x29,0x25,0x25,0x25,0x25,0x76,0x52,0x29,0x00,0x00,0x00,0x00,0x29,0x29,0x52,0x76,0x76,0x29,0x25
,0x29,0x29,0x25,0x25,0x25,0x76,0x76,0x52,0x00,0x00,0x00,0x00,0x00,0x00,0x29,0x52,0x52,0x76,0x29,0x29
,0x29,0x29,0x25,0x25,0x25,0x76,0x76,0x52,0x00,0x00,0x00,0x00,0x00,0x00,0x29,0x52,0x52,0x76,0x29,0x29
,0x29,0x29,0x25,0x25,0x25,0x25,0x76,0x76,0x00,0x76,0x76,0x76,0x76,0x00,0x29,0x52,0x52,0x76,0x29,0x29
,0x29,0x29,0x29,0x25,0x25,0x25,0x25,0x76,0x76,0x76,0x76,0x76,0x00,0x00,0x29,0x52,0x52,0x76,0x76,0x29
,0x29,0x29,0x29,0x25,0x25,0x25,0x25,0x00,0x00,0x00,0x00,0x00,0x00,0x29,0x29,0x52,0x52,0x76,0x76,0x29
,0xff,0x29,0x29,0x29,0x25,0x25,0x25,0x00,0x00,0x29,0x29,0x29,0x29,0x29,0x29,0x52,0x76,0x76,0x29,0x29
,0xff,0x29,0x29,0x29,0x29,0x29,0x25,0x25,0x25,0x29,0x29,0x29,0x29,0x52,0x52,0x52,0x76,0x52,0x29,0x29
,0xff,0xff,0x29,0x52,0x29,0x29,0x29,0x29,0x29,0x29,0x29,0x29,0x52,0x52,0x76,0x76,0x76,0x52,0x29,0xff
,0xff,0xff,0xff,0x52,0x52,0x52,0x52,0x52,0x52,0x52,0x52,0x52,0x52,0x76,0x76,0x52,0x52,0x52,0xff,0xff
,0xff,0xff,0xff,0xff,0x52,0x52,0x52,0x76,0x76,0x76,0x76,0x76,0x76,0x76,0x52,0x52,0x52,0xff,0xff,0xff
,0xff,0xff,0xff,0xff,0xff,0xff,0x52,0x76,0x76,0x76,0x76,0x76,0x76,0x76,0x52,0xff,0xff,0xff,0xff,0xff
};

//Dados da imagem para formar um sprite (20x20) em formato RRRGGGBB - 1 byte por pixel
uint16_t dados_da_imagem_4[altura_sprite][largura_sprite] = {
0xff,0xff,0xff,0xff,0x25,0x25,0x25,0x25,0x25,0x29,0x29,0x29,0x29,0x29,0x29,0x29,0xff,0xff,0xff,0xff
,0xff,0xff,0xff,0x25,0x25,0x25,0x25,0x25,0x29,0x29,0x29,0x29,0x76,0x76,0x29,0x29,0x29,0xff,0xff,0xff
,0xff,0xff,0x25,0x25,0x25,0x25,0x25,0x29,0x76,0x76,0x76,0x76,0x76,0x76,0x76,0x52,0x52,0x52,0xff,0xff
,0xff,0x25,0x25,0x25,0x25,0x76,0x76,0x76,0x76,0x52,0x52,0x52,0x52,0x52,0x76,0x76,0x76,0x52,0x52,0xff
,0xff,0x25,0x25,0x25,0x76,0x76,0x52,0x52,0x52,0x52,0x52,0x52,0x52,0x52,0x52,0x52,0x76,0x52,0x52,0xff
,0x25,0x25,0x25,0x25,0x76,0x76,0x52,0x52,0x29,0x29,0x29,0x29,0x29,0x29,0x29,0x52,0x76,0x76,0x52,0x52
,0x25,0x25,0x25,0x25,0x76,0x52,0x52,0x29,0x29,0x00,0x00,0x00,0x00,0x29,0x29,0x52,0x52,0x76,0x76,0x76
,0x25,0x25,0x25,0x25,0x76,0x52,0x52,0x29,0x00,0x00,0x00,0x76,0x00,0x00,0x29,0x29,0x52,0x52,0x76,0x76
,0x25,0x25,0x25,0x25,0x76,0x52,0x52,0x29,0x00,0x00,0x00,0x76,0x76,0x00,0x29,0x29,0x29,0x52,0x76,0x76
,0x25,0x25,0x25,0x25,0x76,0x52,0x52,0x29,0x00,0x00,0x00,0x76,0x76,0x00,0x29,0x29,0x29,0x52,0x76,0x76
,0x25,0x25,0x25,0x25,0x76,0x52,0x52,0x29,0x00,0x00,0x00,0x76,0x76,0x00,0x29,0x29,0x29,0x52,0x76,0x76
,0x25,0x25,0x25,0x25,0x76,0x52,0x52,0x29,0x00,0x00,0x00,0x76,0x76,0x00,0x29,0x29,0x29,0x52,0x76,0x76
,0x25,0x25,0x25,0x25,0x25,0x76,0x52,0x29,0x29,0x00,0x00,0x00,0x76,0x00,0x00,0x25,0x29,0x52,0x76,0x76
,0x25,0x25,0x25,0x25,0x25,0x25,0x52,0x52,0x52,0x52,0x52,0x76,0x76,0x00,0x00,0x25,0x29,0x52,0x76,0x76
,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x76,0x76,0x76,0x76,0x25,0x25,0x25,0x25,0x29,0x52,0x52,0x52
,0xff,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x76,0x76,0x25,0x25,0x25,0x25,0x29,0x29,0x52,0x52,0xff
,0xff,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x29,0x29,0x52,0x52,0xff
,0xff,0xff,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x29,0x29,0x52,0x52,0xff,0xff
,0xff,0xff,0xff,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x25,0x29,0x29,0x29,0x29,0x29,0xff,0xff,0xff
,0xff,0xff,0xff,0xff,0x25,0x25,0x25,0x29,0x29,0x29,0x29,0x29,0x29,0x29,0x29,0x29,0xff,0xff,0xff,0xff

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
                dados_do_sprite[y][x] = APAGA;
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
                dados_do_sprite[y][x] = APAGA;
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
                dados_do_sprite[y][x] = APAGA;
            } else {
                dados_do_sprite[y][x] = converte_em_bgr(dados_da_imagem_3[y][x]);//Converte pixel por pixel do formato RGB para o BGR
            }
            dados_do_sprite[y][x] = converte_em_bgr(dados_da_imagem_3[y][x]);//Converte pixel por pixel do formato RGB para o BGR
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
            cor_temp = dados_da_imagem_3[y][x];
            if (cor_temp == 0xff){
                dados_do_sprite[y][x] = APAGA;
            } else {
                dados_do_sprite[y][x] = converte_em_bgr(dados_da_imagem_4[y][x]);//Converte pixel por pixel do formato RGB para o BGR
            }
            dados_do_sprite[y][x] = converte_em_bgr(dados_da_imagem_4[y][x]);//Converte pixel por pixel do formato RGB para o BGR
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

int main() {
    inicializa_fpga();

    grava_sprite_portal();

    #define mascara_10bits 0b1111111111
    #define limite_superior_eixoY 10
    #define limite_inferior_eixoY 450
    #define limite_esquerdo_eixoX 10
    #define limite_direito_eixoX 610

    uint16_t pos_x = 300;
    uint16_t pos_y = 280;

    pos_x &= mascara_10bits;
    pos_y &= mascara_10bits;
    
    uint32_t pos_xy_20b;
    pos_xy_20b = (pos_x << 10 | pos_y);

    animacao(pos_xy_20b, 1);

    fecha_dev_mem();   
    return 0;
}