#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#define HPS_PHYS_BASE 0xFF000000
#define HPS_SPAN 0x01000000   
#define SYSMGR_BASE 0xFFD08000
#define I2C0_BASE 0xFFC04000 

#define SYSMGR_I2C0USEFPGA (SYSMGR_BASE + 0x704)
#define SYSMGR_GENERALIO7 (SYSMGR_BASE + 0x49C)
#define SYSMGR_GENERALIO8 (SYSMGR_BASE + 0x4A0)

#define I2C0_CON (I2C0_BASE + 0x00) 
#define I2C0_TAR (I2C0_BASE + 0x04)
#define I2C0_DATA_CMD (I2C0_BASE + 0x10)
#define I2C0_FS_SCL_HCNT (I2C0_BASE + 0x1C)
#define I2C0_FS_SCL_LCNT (I2C0_BASE + 0x20)
#define I2C0_ENABLE (I2C0_BASE + 0x6C) 
#define I2C0_RXFLR (I2C0_BASE + 0x78)   
#define I2C0_TXFLR (I2C0_BASE + 0x74)
#define I2C0_CLR_INTR (I2C0_BASE + 0x40)

#define ADXL345_ADDR 0x53   
#define ADXL345_DEVID 0x00 
#define ADXL345_POWER_CTL 0x2D
#define ADXL345_DATA_FORMAT 0x31
#define ADXL345_DATAX0 0x32 
#define ADXL345_DATAX1 0x33 
#define ADXL345_DATAY0 0x34
#define ADXL345_DATAY1 0x35
#define ADXL345_DATAZ0 0x36
#define ADXL345_DATAZ1 0x37
#define ADXL345_INT_ENABLE 0x2E
#define ADXL345_INT_MAP 0x2F
#define ADXL345_INT_SOURCE 0x30
#define ADXL345_BW_RATE 0x2C

#define AMOSTRAS_CALIBRACAO 100
#define FILTRO_MOVIMENTO 30

volatile uint32_t *base_hps; //ponteiro para a memoria HPS mapeada
int mg_por_lsb = 2; //mg por bit menos significativo para conversão
int16_t offset_x, offset_y; //offset dos eixos X, Y e Z para calibracao
int fd;

void escrever_registrador(uint32_t endereco, uint32_t valor);
uint32_t ler_registrador(uint32_t endereco);
void inicializar_i2c();
void verificar_status_i2c();
void ler_reg_acel(uint8_t endereco, uint8_t *valor);
void escrever_reg_acel(uint8_t endereco, uint8_t valor);
void escrever_i2c(uint8_t endereco_reg, uint8_t valor);
uint8_t ler_i2c(uint8_t endereco_reg);
void inicializar_acelerometro();
uint8_t ler_devid_acelerometro();
void ler_aceleracao_xy(int16_t *x, int16_t *y);
int dados_prontos();
void calibrar_acelerometro(int16_t *offset_x, int16_t *offset_y);
int configurar_acelerometro();
int desmapear_memoria();
int define_velocidade(float valor_g);
int get_movimento(int *velocidade);

void escrever_registrador(uint32_t endereco, uint32_t valor) {
   *(volatile uint32_t*)(base_hps + (endereco - HPS_PHYS_BASE) / 4) = valor; //endereco inicial da regiao mapeada na memoria virtual + ( endereco do registrador na memoria fisica - endereco inicial da regiao mapeada na memoria fisica )
}

uint32_t ler_registrador(uint32_t endereco) {
   return *(volatile uint32_t*)(base_hps + (endereco - HPS_PHYS_BASE) / 4);
}

void inicializar_i2c() {
   escrever_registrador(SYSMGR_I2C0USEFPGA, 0); //HPS
   escrever_registrador(SYSMGR_GENERALIO7, 1); //I2C
   escrever_registrador(SYSMGR_GENERALIO8, 1); //I2C

   escrever_registrador(I2C0_ENABLE, 0x0); // Desabilita
   escrever_registrador(I2C0_CON, 0x65); //Mestre, modo rapido 400kbit/s, enderecamento 7 bits
   escrever_registrador(I2C0_TAR, ADXL345_ADDR);//destino acelerometro

   escrever_registrador(I2C0_FS_SCL_HCNT, 60 + 30);
   escrever_registrador(I2C0_FS_SCL_LCNT, 130 + 30);
  
   escrever_registrador(I2C0_ENABLE, 0x1); //Habilita
   usleep(10000);
}

void verificar_status_i2c() {
   uint32_t status = ler_registrador(I2C0_ENABLE);
   printf("Status I2C: 0x%08X\n", status);
   if (status & 0x1) {
       printf("I2C habilitado\n");
   } else {
       printf("I2C nao habilitado\n");
   }
}

void ler_reg_acel(uint8_t endereco, uint8_t *valor) {
   escrever_registrador(I2C0_DATA_CMD, endereco + 0x400); //endereco e sinal Start
   escrever_registrador(I2C0_DATA_CMD, 0x100); //sinal de leitura
   while (ler_registrador(I2C0_RXFLR) == 0) {} //aguarda até que o buffer de recepcao nao esteja vazio
   *valor = ler_registrador(I2C0_DATA_CMD) & 0xFF; //0b11111111 - le os bits LSB do reg 7-0 escritos em dat 
}

void escrever_reg_acel(uint8_t endereco, uint8_t valor) {
   escrever_registrador(I2C0_DATA_CMD, endereco + 0x400);// endereco e sinal start
   escrever_registrador(I2C0_DATA_CMD, valor); //valor para ser escrito
}

void escrever_i2c(uint8_t endereco_reg, uint8_t valor) {
   escrever_reg_acel(endereco_reg, valor);
}

uint8_t ler_i2c(uint8_t endereco_reg) {
   uint8_t valor;
   ler_reg_acel(endereco_reg, &valor);
   return valor;
}

void inicializar_acelerometro() {
   escrever_i2c(ADXL345_POWER_CTL, 0x00); //standby
   usleep(10000);
   escrever_i2c(ADXL345_POWER_CTL, 0x08); //modo medicao
   escrever_i2c(ADXL345_DATA_FORMAT, 0x00); // ±2g
   escrever_i2c(ADXL345_BW_RATE, 0x0A); // 100 Hz
   escrever_i2c(ADXL345_INT_ENABLE, 0x80); //habilita interrupcao de dados prontos
   escrever_i2c(ADXL345_INT_MAP, 0x00); //mapeia interrupcao 
}

uint8_t ler_devid_acelerometro() {
   return ler_i2c(ADXL345_DEVID);
}

void ler_aceleracao_x(int16_t *x) {
   while(!dados_prontos()); //aguarda novos dados
   *x = (ler_i2c(ADXL345_DATAX1) << 8) | ler_i2c(ADXL345_DATAX0); //le e combina em 16bits os dados MSB e LSB do eixo x
   ler_i2c(ADXL345_INT_SOURCE); //limpa interrupcoes detectadas
}

void ler_aceleracao_y(int16_t *y) {
   while(!dados_prontos()); //aguarda novos dados
   *y = (ler_i2c(ADXL345_DATAY1) << 8) | ler_i2c(ADXL345_DATAY0); //le e combina em 16bits os dados MSB e LSB do eixo y
   ler_i2c(ADXL345_INT_SOURCE); //limpa interrupcoes detectadas
}

void ler_aceleracao_xy(int16_t *x, int16_t *y){
   while(!dados_prontos()); //aguarda novos dados
   *x = (ler_i2c(ADXL345_DATAX1) << 8) | ler_i2c(ADXL345_DATAX0); //le e combina em 16bits os dados MSB e LSB do eixo x
   *y = (ler_i2c(ADXL345_DATAY1) << 8) | ler_i2c(ADXL345_DATAY0); //le e combina em 16bits os dados MSB e LSB do eixo y
   ler_i2c(ADXL345_INT_SOURCE); //limpa interrupcoes detectadas
}

int dados_prontos() {
   return (ler_i2c(ADXL345_INT_SOURCE) & 0x80) != 0; //0b10000000 - verifica o bit 7 data_ready
}

void calibrar_acelerometro(int16_t *offset_x, int16_t *offset_y) {
   int32_t soma_x = 0;
   int32_t soma_y = 0;
   int16_t x, y;
   int i;

   for (i = 0; i < AMOSTRAS_CALIBRACAO; i++) {
      ler_aceleracao_xy(&x, &y);
      soma_x += x;
      soma_y += y;
   }

   *offset_x = soma_x / AMOSTRAS_CALIBRACAO; //calcula a media de leituras obtidas por x
   *offset_y = soma_y / AMOSTRAS_CALIBRACAO; //calcula a media de leituras obtidas por y

   printf("Calibracao completa \n Offset: X=%d \n Offset: Y=%d \n", *offset_x, *offset_y);
}

int configurar_acelerometro(){
   uint8_t devid;

   fd = open("/dev/mem", O_RDWR | O_SYNC); // Abre o /dev/mem para o mapeamento de memoria
   if (fd < 0) {
       printf("Erro ao abrir /dev/mem\n");
       return -1;
   }

   base_hps = mmap(NULL, HPS_SPAN, PROT_READ | PROT_WRITE, MAP_SHARED, fd, HPS_PHYS_BASE); // Mapeia a memoria HPS
   if (base_hps == MAP_FAILED) {
       printf("Erro ao mapear memoria\n");
       close(fd);
       return -1;
   }

   inicializar_i2c();
   verificar_status_i2c();

   int i;
   for (i = 0; i < 5; i++) {
       devid = ler_devid_acelerometro();
       printf("Tentativa %d - ADXL345 DEVID: 0x%02X\n", i+1, devid);
       if (devid == 0xE5) {
           break;
       }
       usleep(100000);
   }
   inicializar_acelerometro();

   printf("Calibrando acelerometro...\n");
   calibrar_acelerometro(&offset_x, &offset_y);

   printf("Calibrado!\n");
   return 0;
}

int desmapear_memoria(){
   munmap((void*)base_hps, HPS_SPAN);
   close(fd);
   return 0;
}

// Define a velocidade com base na aceleração
int define_velocidade(float valor_g) {
   if (valor_g < ( FILTRO_MOVIMENTO * 3)) {
      //printf("velocidade 1");
      return 1; //velocidade baixa
   } else if (valor_g < ( FILTRO_MOVIMENTO * 4)) {
      //printf("velocidade 2");
      return 2; //velocidade media
   } else {
      //printf("velocidade 3");
      return 3; //velocidade alta
   }
}

//8 cima, 2 baixo, 6 direita, 4 esquerda, 0 sem movimento
int get_movimento(int *velocidade){
   inicializar_i2c();
   inicializar_acelerometro();
   int16_t x_bruto, y_bruto;
   
   usleep(10000);

   ler_aceleracao_xy(&x_bruto, &y_bruto);

   if ( (x_bruto > FILTRO_MOVIMENTO) && (x_bruto > y_bruto)) { //movimento no eixo x+
      //printf("movimento x+\n");
      *velocidade = define_velocidade(x_bruto);
      return 6; // x - direcao para direita
   } else if ( (y_bruto > FILTRO_MOVIMENTO) && (y_bruto > x_bruto) ) { //movimento no eixo y+
      //printf("movimento y+\n");
      *velocidade = define_velocidade(y_bruto);
      return 8; // y - direcao para cima
   
   } else if ( (x_bruto < -FILTRO_MOVIMENTO) && (x_bruto < y_bruto)) { //movimento no eixo x-
      //printf("movimento x-\n");
      *velocidade = define_velocidade(-x_bruto);
      return 4; // x - direcao para esquerda

   } else if ( (y_bruto < -FILTRO_MOVIMENTO) && (y_bruto < x_bruto)) { //movimento no eixo y-
      //printf("movimento y-\n");
      *velocidade = define_velocidade(-y_bruto);
      return 2; //y - direcao para baixo
   }

   //printf("sem movimento\n");
   return 0; //sem movimento   
}