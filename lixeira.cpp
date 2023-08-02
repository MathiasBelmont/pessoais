#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main() {
    char lixeiraPath[256]; // Caminho para a lixeira
    char arquivoParaExcluir[256]; // Arquivo a ser exclu�do
    time_t tempoExpiracao; // Tempo de expira��o (em segundos)

    // Defina o caminho da lixeira com base no sistema operacional (este exemplo � para Windows)
    // Voc� pode precisar de um caminho diferente para outros sistemas operacionais
    strcpy(lixeiraPath, "C:\\$Recycle.Bin\\");

    printf("Digite o caminho do arquivo para excluir: ");
    fgets(arquivoParaExcluir, sizeof(arquivoParaExcluir), stdin);
    arquivoParaExcluir[strcspn(arquivoParaExcluir, "\n")] = '\0'; // Remover o caractere '\n' do final da string

    printf("Digite o tempo de expiracao (em segundos) para excluir o arquivo: ");
    scanf("%ld", &tempoExpiracao);

    // Obter o tempo atual
    time_t tempoAtual;
    time(&tempoAtual);

    // Calcular o tempo de expira��o absoluto
    time_t tempoExpiracaoAbsoluto = tempoAtual + tempoExpiracao;

    // Verificar se o tempo de expira��o foi alcan�ado
    while (tempoAtual < tempoExpiracaoAbsoluto) {
        time(&tempoAtual); // Atualizar o tempo atual
    }

    // Excluir o arquivo da lixeira
    char comando[512];
    sprintf(comando, "move /y \"%s\" \"%s\"", arquivoParaExcluir, lixeiraPath);

    if (system(comando) == 0) {
        printf("Arquivo excluido com sucesso.\n");
    } else {
        printf("Erro ao excluir o arquivo.\n");
    }

    return 0;
}

