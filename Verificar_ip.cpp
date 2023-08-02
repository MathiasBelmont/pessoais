#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <windows.h>

// Estrutura para armazenar as informações de IP
typedef struct {
    int i;
    int j;
} IPInfo;

#ifdef _WIN32 // Compilação para Windows
#include <conio.h>
#else // Compilação para Linux
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

int kbhit() {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}
#endif

// Função para verificar se o IP responde ao comando ping
int isPingable(const char* ip) {
    char ping_cmd[100];
    sprintf(ping_cmd, "ping %s ", ip); // No caso do Windows, usar "ping -c 1 -W 1 %s > /dev/null" para Linux

    int exit_code = system(ping_cmd);
    return exit_code == 0 ? 1 : 0; // Retorna 1 se o IP respondeu ao ping, 0 caso contrário
}

// Função para a thread de verificação de IP
void* checkIP(void* arg) {
    IPInfo* ip_info = (IPInfo*)arg;
    char ip[16]; // Espaço para armazenar o IP no formato xxx.xxx.xxx.xxx
    sprintf(ip, "192.168.%d.%d", ip_info->i, ip_info->j);

    if (!isPingable(ip)) {
        printf("IP %s nao responde ao ping.\n", ip);

        FILE* not_responding_file = fopen("ips_nao_respondidos.txt", "a");
        if (not_responding_file != NULL) {
            fprintf(not_responding_file, "%s\n", ip); // Armazena o IP não respondido no arquivo
            fclose(not_responding_file);
        } else {
            perror("Erro ao abrir o arquivo");
        }
    }

    free(ip_info);
    return NULL;
}

int main() {
    int i, j;
    pthread_t threads[4 * 256];

    // Criação do arquivo para armazenar os IPs não respondidos
    FILE* not_responding_file = fopen("ips_nao_respondidos.txt", "w");
    if (not_responding_file == NULL) {
        perror("Erro ao criar o arquivo");
        return 1;
    }
    fclose(not_responding_file); // Fecha o arquivo, será aberto novamente durante a execução

    for (i = 0; i <= 3; i++) {
        for (j = 0; j <= 255; j++) {
            // Criação de uma estrutura para armazenar os dados do IP para a thread
            IPInfo* ip_info = (IPInfo*)malloc(sizeof(IPInfo));
            ip_info->i = i;
            ip_info->j = j;

            // Criação da thread para verificar o IP
            if (pthread_create(&threads[i * 256 + j], NULL, checkIP, ip_info) != 0) {
                perror("Erro ao criar a thread");
                return 1;
            }

            // Aguarda 100 microssegundos antes de criar a próxima thread
          // Sleep(100);
        }
    }

    // Verifica se o usuário pressionou Enter para interromper
    while (!kbhit()) {
        Sleep(1000); // Aguarda 1 milissegundo antes de verificar novamente
    }

    printf("Programa interrompido pelo usuario.\n");

    // Cancela as threads não finalizadas
    for (i = 0; i < 4 * 256; i++) {
        pthread_cancel(threads[i]);
    }

    return 0;
}

