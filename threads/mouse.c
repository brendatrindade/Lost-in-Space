#include <stdio.h>
#include <fcntl.h>
#include <linux/input.h>
#include <unistd.h>
#include <stdint.h>

int abre_mouse();
void le_mouse(int fd);
int le_mouse_direcao(int fd, uint16_t *x, uint16_t *y);
int teste_leitura(int fd);
int teste();

int abre_mouse() {
    int fd = open("/dev/input/event0", O_RDONLY);
    if (fd == -1) {
        perror("Erro");
        return -1;
    }
    return fd;
}

// muito sensivel
int le_mouse_orientacao(int fd) {
    struct input_event ie;
    int x = 0, y = 0;
    
    while (1) {
        if (read(fd, &ie, sizeof(ie)) != sizeof(ie)) {
            perror("Erro ao ler o evento");
            break;
        }

        if (ie.type == EV_REL) {
            if (ie.code == REL_X) { 
                x = ie.value; // direita = 1; esquerda = -1
                return x;
            } else if (ie.code == REL_Y) {
                y = ie.value; // baixo = 1; cima = -1
                return y;
            }
        }
    }
}

int le_mouse_direcao(int fd, uint16_t *x, uint16_t *y) {
    struct input_event ie;
    //int x = 0, y = 0;
    
    while (1) {
        if (read(fd, &ie, sizeof(ie)) != sizeof(ie)) {
            perror("Erro ao ler o evento");
            break;
        }

        if (ie.type == EV_REL) {
            if (ie.code == REL_X) {
                return 0; //horizontal
            } else if (ie.code == REL_Y) {
                return 1; //vertical
            }
        }
    }
}

int teste_leitura(int fd) {
    struct input_event ie;
    int x = 0, y = 0;
    
    while (1) {
        if (read(fd, &ie, sizeof(ie)) != sizeof(ie)) {
            perror("Erro ao ler o evento");
            break;
        }

        if (ie.type == EV_REL) {
            if (ie.code == REL_X) {
                x = ie.value;
                return x, y;
            } else if (ie.code == REL_Y) {
                y = ie.value;
                return x, y;
            }
        }
    }
}

int teste() {
    int fd = open("/dev/input/event0", O_RDONLY);
    if (fd == -1) {
        perror("Erro");
        return 1;
    }

    struct input_event ie; // struct que recebe as informações do mouse
    while (read(fd, &ie, sizeof(struct input_event)) > 0) {
        printf("Type: %d, Code: %d, Value: %d\n", ie.type, ie.code, ie.value);
    }

    close(fd);
    return 0;
}

// int main() {
//     int fd = abre_mouse();
//     if (fd != -1) {
//         le_mouse(fd);
//         close(fd);
//     }
    
//     return 0;
// }

/*
struct input_event {
struct timeval time;
__u16 type;
__u16 code;
__s32 value;
};
*/