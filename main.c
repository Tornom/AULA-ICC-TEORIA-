#include <stdio.h>
#include <stdlib.h>

int main()
{
    int a,b,c,d;//criando as variaveis necessarias
    printf("Digite uma senha\n");// pede a senha
    scanf("%d %d",&a,&b);// configura o valor da senha

    printf("Digite a senha\n"); //pede a senha novamente
    scanf("%d %d",&c,&d); // guarda o valor da senha

    if (a==c && b==d ){ //compara a senha correta com a senha obtida
        printf("Cofre aberto"); //o cofre abriu

    }
    else{
        printf("Senha incorreta");  // o usuario errou a senha
    }
    return 0;
}
