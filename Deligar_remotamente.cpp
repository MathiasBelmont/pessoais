#include <stdio.h>
#include <windows.h>

int main()
{
    char NomePc[50];
    printf("Digite o nome do computador: ");
    gets(NomePc);

    int Opc;
    do
    {
        printf("Digite o que quer fazer com o computador: [1]-Desligar remotamente [2]-Cancelar o desligamento remoto: ");
        scanf("%i", &Opc);
        switch (Opc)
        {
        case 1:
        {
            // Para desligar remotamente, use o comando "shutdown" com as opções adequadas.
            // Utilize o sprintf para montar o comando corretamente com o nome do computador.
            char comando[100];
            sprintf(comando, "shutdown /m \\\\%s /s /f /c \"Salve todo o seu trabalho, o computador será desligado\" /t 6000", NomePc);
            system(comando);
            break;
        }
        case 2:
        {
            // Para cancelar o desligamento remoto, utilize o mesmo comando, mas com a opção "/a".
            char comando[100];
            sprintf(comando, "shutdown /m \\\\%s /a", NomePc);
            system(comando);
            break;
        }
        default:
            printf("encerrando.....\n");
        }
    } while (Opc != 1 && Opc != 2);

    return 0;
}

