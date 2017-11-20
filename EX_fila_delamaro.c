#include "header.h"
#include "Normal.c"
#define tempo_chega_normal 4
#define tempo_chega_desvio 1.5
#define tempo_atende_normal 2
#define tempo_atende_desvio 1



//Função para alocar memória para guardar uma fila
FILA* cria_fila()
{
    FILA* fila_n;

    fila_n = (FILA*) malloc(sizeof(FILA*));
    if(fila_n != NULL)              //verifica se foi possível alocar memória,caso sim retorna o ponteiro;
    {
        fila_n->tamanho = 0;        //Inicia a fila vazia;
        fila_n->primeiro = NULL;
        fila_n->ultimo = NULL;
        return fila_n;
    }
    else                            //Caso contrário libera a variavel e retorna NULL
    {
        free(fila_n);
        printf("Nao foi possivel alocar memoria para a fila\n");
        return NULL;
    }

}

//Posiciona o cliente no final da fila
int enfileirar_cliente(FILA* fila_n,CLIENTE* cliente)
{
    if(fila_n != NULL && cliente != NULL)
    {
        if(fila_n->ultimo != NULL && fila_n->tamanho != 0)  //Verifica se a fila está vazia e se o ultimo ponteiro é diferente de NULL
        {
            fila_n->ultimo->proximo = cliente;              //Coloca o novo cliente como proximo depois do ultimo
            fila_n->ultimo = cliente;                       //Define o cliente como ultimo da fila
            cliente->proximo = NULL;                        //Não existe alguem alem do novo cliente, portanto o proximo é NULL
            fila_n->tamanho++;                              //Incrementa o tamanho da fila para refletir seu novo valor real
            return true;
        }

        else                                                //No caso em que a fila está vazia o cliente é o primeiro e o ultimo da fila
        {
            cliente->proximo = NULL;
            fila_n->primeiro = cliente;
            fila_n->ultimo = cliente;
            fila_n->tamanho++;
            return true;
        }
    }
    else
    {
        if(fila_n == NULL)printf("Fila invalida\n");
        else printf("Cliente invalido\n");
        return false;
    }
}

//Remove o primeiro cliente da fila, retornando ela para quem o chamou
CLIENTE* desenfileirar_cliente(FILA* fila_n)
{
    if(fila_n->tamanho <= 0 || fila_n == NULL)
    {
        if(fila_n == NULL) printf("A fila não existe\n");
        else printf("A fila_n está vazia\n");
        return NULL;                                        //Caso a fila esteja vazia não há o que retornar ou que não seja um a fila válida
    }
    else
    {
        CLIENTE* cliente_retorno;                           //Um cliente auxiliar é criado para guardar quem será removido da fila
        cliente_retorno = fila_n->primeiro;
        fila_n->primeiro = fila_n->primeiro->proximo;       //O primeiro da fila é trocado
        fila_n->tamanho--;                                  //O tamanho da fila é alterado para refletir o novo valor
        return cliente_retorno;

    }
}

//Aloca memoria para um ponteiro guardar a informação de um cliente
CLIENTE* cria_cliente(double tempo_atendimento,double tempo_chegada)
{
    CLIENTE* novo_cliente;
    novo_cliente = (CLIENTE*)malloc(sizeof(CLIENTE*));
    if(novo_cliente != NULL)                                        //Verifica se foi possivel alocar memoria para o novo cliente
    {
        novo_cliente->tempo_chegada = tempo_chegada;                //Utiliza o tempo de chegada como uma distribuição normal
        novo_cliente->tempo_atendimento = tempo_atendimento;        //Utiliza o tempo de atendimento como uma distribuição normal
        novo_cliente->proximo = NULL;
        novo_cliente->tempo_no_caixa = 0;
        return novo_cliente;
    }
    else                                                            //Caso não foi possível alocar memoria libera a variavel e retorna NULL
    {
        free(novo_cliente);
        printf("Nao ha espaco para armazenar mais clientes\n");
        return NULL;
    }
}

//cria vetores para filas e chama a função para a simulação
int inicializacao(int N_caixa,int N_pessoas,int N_fila)
{

    //Ponteiros para guardar as filas em vetores e criar o proximo cliente para entrar
    FILA** vetor_fila;
    int i;



    if(N_fila != 1 && N_caixa != N_fila) return false;

    if(N_caixa <= 0 || N_pessoas <= 0 || N_fila <= 0) return false;     //se algum parametro passado for igual a 0, não é necessario realizar o programa
    else
    {
        vetor_fila = malloc(N_fila*sizeof(FILA**));                     //Cria um vetor de fila
        if(vetor_fila == NULL)
        {
            free(vetor_fila);
            printf("Nao foi possivel alocar o vetor de filas\n");
            return false;
        }
        else
        {
            for(i = 0; i < N_fila; i++)                                 //Cria o N de filas indicado pela função
            {
                vetor_fila[i] = cria_fila();
                if(vetor_fila[i] == NULL) return false;
            }
        }

    }


    if(simulacao(vetor_fila,N_pessoas,N_caixa,N_fila))                      //ao final da simulação informa se esta foi completa ou nao
    {
  //      printf("Simulacao completa\n");
    }
  //  else printf("Simulacao falhou\n");

    destroi_fila(*vetor_fila);

    return true;
}

//Faz a simulação da agencia, obtem o tempo maximo e a media de tempo gasto pelos clientes no banco;
int simulacao(FILA** vetor_fila,int N_pessoas, int N_caixa,int N_fila)
{
    /*                                          Variaveis
        tempo_maximo: o tempo maximo registrado,
        tempo_medio: o tempo medio,
        tempo_acumulado: tempo acumulado por todos para gerar o tempo medio,
        tempo_atendido: tempo que cada cliente levou para ser atendido para comparar com o tempo maximo e adicionar ao tempo acumulado,
        tempo_fim_atendimento: tempo para o termino do atendimento do primeiro cliente da fila,
        N_na_fila: o numero de pessoas em todas as filas
    */
    double maior_tempo,tempo_medio,tempo_acumulado,tempo_atendido_total,N_na_fila;
    //numero de pessoas atendidas e a menor fila para inserir um novo cliente

    int N_atendido, menor_fila,i,j,k,boolean;
    CLIENTE* proximo_cliente,*vetor_caixa[N_caixa],*aux;           //o proximo cliente a entrar na fila,auxiliar para busca
    proximo_cliente = NULL;

    //inicia as variaveis com valor nulo
    maior_tempo = 0;
    tempo_acumulado = 0;
    tempo_medio = 0;
    boolean = false;
    double twobool = 0;
    N_na_fila = 0;
    N_atendido = 0;
    constante = false;
    for(i = 0;i < N_caixa;i++)
    vetor_caixa[i] = NULL;
    i = j = 0;


    //começa a simulação

    while(N_atendido < N_pessoas)                                   //ate que o numero de atendimentos equipare o numero de clientes
    {
        tempo_atendido_total = 0;
        if(N_na_fila + N_atendido < N_pessoas)
        {
            proximo_cliente = cria_cliente(2,1);                    //Cria um cliente com distribuição normal
            if(proximo_cliente == NULL) return false;               //A criação de um novo cliente falhou
            N_na_fila++;                                            //incrementa a fila
            menor_fila = procura_menor_fila(vetor_fila,N_fila);     //encontra a menor fila para o novo cliente entrar
            if(!enfileirar_cliente(vetor_fila[menor_fila],proximo_cliente)) return false;   //enfileira o cliente na fila menor
            if(N_fila == 1)                                         //se for uma fila unica
            {

                boolean = true;
                for(i = 0; i < N_caixa; i++)                        //procura um caixa vazio
                {
                    if(vetor_caixa[i]!= NULL) vetor_caixa[i]->tempo_no_caixa += proximo_cliente->tempo_chegada; //incrementa o tempo de permanencia dos clientes no caixa
                    else                                            //caso o tempo passou do tempo de atendimento
                    {

                        if(proximo_cliente != NULL)                 //e o proximo cliente é nao nulo
                        {
                            proximo_cliente = NULL;
                            vetor_caixa[i] = desenfileirar_cliente(vetor_fila[0]);          //o primeiro da fila vai para um caixa vazio
                        }
                        continue;
                    }
                    if(vetor_caixa[i]->tempo_no_caixa >= vetor_caixa[i]->tempo_atendimento) //se o tempo de atendimento for menor que o de permanencia
                    {
                        if(vetor_fila[0]->primeiro != NULL && boolean)                      //o primeiro cliente é nao nulo e é a primeira vez que entra nesse if
                        {
                            vetor_fila[0]->primeiro->tempo_no_caixa = vetor_caixa[i]->tempo_no_caixa - vetor_caixa[i]->tempo_atendimento;  //o tempo no caixa da pessoa que acabou de entrar e calculado
                            free(vetor_caixa[i]);
                            vetor_caixa[i] = desenfileirar_cliente(vetor_fila[0]);          //remove o cliente da fila
                            N_na_fila--;                                                    //um cliente sai da agencia
                            N_atendido++;                                                   //aumenta o numero de clientes atendidos
                            boolean = false;
                        }
                        else if(!boolean) vetor_caixa[i] = NULL;                            //se não for a primeira vez o caixa fica vazio
                    }
                }
            }
            else                                //no caso de varias filas
            {
                for(i = 0; i < N_caixa; i++)    //procura um caixa ocupado
                {
                    if(vetor_caixa[i]!= NULL)   //e nao nulo
                    {
                        vetor_caixa[i]->tempo_no_caixa += proximo_cliente->tempo_chegada;   //e incrementa seu tempo de permanencia
                        if(vetor_caixa[i]->tempo_no_caixa >= vetor_caixa[i]->tempo_atendimento) //se a permanencia for maior que o tempo de atendimento
                        {
                            vetor_fila[i]->primeiro->tempo_no_caixa = vetor_caixa[i]->tempo_no_caixa - vetor_caixa[i]->tempo_atendimento;
                            free(vetor_caixa[i]);
                            vetor_caixa[i] = desenfileirar_cliente(vetor_fila[i]);      //o cliente sai do banco
                            N_na_fila--;
                            N_atendido++;
                        }
                    }
                    else if(vetor_fila[i]->primeiro != NULL)                            //se a fila não esta vazia
                    {
                        vetor_caixa[i] = desenfileirar_cliente(vetor_fila[i]);          //coloca um cliente no proximo caixa
                    }
                }
            }

            tempo_atendido_total = calcula_tempo(vetor_fila[menor_fila]);               //o tempo gasto por um cliente na fila
            tempo_atendido_total += (vetor_caixa[menor_fila]->tempo_atendimento - vetor_caixa[menor_fila]->tempo_no_caixa); //e calculado nessas duas linhas

            tempo_acumulado += tempo_atendido_total;                                    //reune os tempos para fazer a media ao final

            if(maior_tempo < tempo_atendido_total) maior_tempo = tempo_atendido_total;  //compara com o maior atual para a possivel troca

        }

        if(N_fila == 1)                                 //No caso de somente uma fila
        {
            if(N_na_fila + N_atendido >= N_pessoas)     //Se todos os clientes ja entraram na agencia
            {
                if(vetor_fila[0]->primeiro == NULL)     //e a fila esta vazia
                {
                    for(i = 0; i < N_caixa;i++)         // procura por caixas ocupados
                    {
                        if(vetor_caixa[i] != NULL)
                        {

                           // free(vetor_caixa[i]);
                            vetor_caixa[i] = NULL;      // e limpa eles
                            N_na_fila--;
                            N_atendido++;
                        }
                    }
                }


                aux = vetor_fila[0]->primeiro;          //Inicia o auxiliar de busca no primeiro elemento da fila
                k = 0;
                while(k < N_caixa)                      //busca por um caixa nao nulo
                {
                    if(vetor_caixa[k]!= NULL) break;    //Se encontrar sai do loop
                    k++;
                }

                //coloca um valor inicial em twobool para procurar pelo caixa mais proximo de liberar
                if(k < N_caixa)twobool = vetor_caixa[k]->tempo_atendimento - vetor_caixa[k]->tempo_no_caixa;
                else if(N_atendido >= N_pessoas) break; //se todos os caixas estao livres sai do loop principal

                for(k = 0; k < N_caixa; k++)            //procura pelo caixa nao nulo com menor tempo para o cliente sair
                {
                    if(vetor_caixa[k] != NULL)
                    {
                        if(vetor_caixa[k]->tempo_atendimento - vetor_caixa[k]->tempo_no_caixa < twobool)
                        {
                            twobool = vetor_caixa[k]->tempo_atendimento - vetor_caixa[k]->tempo_no_caixa;
                            j = k;                      //guarda a posição do elemento
                        }
                    }
                }


                for(i = 0; i < vetor_fila[0]->tamanho; i++) //Percorre a fila para contabilizar os tempos dos clientes na fila
                {
                    vetor_caixa[j]->tempo_no_caixa += aux->tempo_chegada;
                    if(vetor_caixa[j]->tempo_atendimento <= vetor_caixa[j]->tempo_no_caixa)   //se o tempo no caixa for maior que o tempo de atendimento, o cliente sai do caixa
                    {
                        //o resto do tempo que passou do atendimento e' passado adiante
                        vetor_fila[0]->primeiro->tempo_no_caixa = vetor_caixa[j]->tempo_no_caixa - vetor_caixa[j]->tempo_atendimento;
                        free(vetor_caixa[j]);
                        vetor_caixa[j] = desenfileirar_cliente(vetor_fila[0]);  //remove o cliente da fila e o coloca no caixa
                        N_na_fila--;
                        N_atendido++;
                    }
                    aux = aux->proximo;
                }
            }

        }

        else                                                    //caso a quantidade de filas for maior que um
        {
            if(N_na_fila + N_atendido >= N_pessoas)             //e não há mais clientes para entrar no banco
            {
                for(i = 0; i < N_fila;i++)
                {
                    if(vetor_fila[i]->primeiro == NULL)         //procura por filas vazias com caixas ocupados
                    {
                        if(vetor_caixa[i] != NULL)
                        {
                            free(vetor_caixa[i]);
                            vetor_caixa[i] = NULL;              //entao remove o caixa ocupado
                            N_na_fila--;
                            N_atendido++;
                        }

                    }

                    aux = vetor_fila[i]->primeiro;

                    for(j = 0; j < vetor_fila[i]->tamanho; j++) //Percorre a fila para contabilizar os tempos dos clientes na fila
                    {
                        vetor_caixa[i]->tempo_no_caixa += aux->tempo_chegada;
                        if(vetor_caixa[i]->tempo_atendimento <= vetor_caixa[i]->tempo_no_caixa)   //se o tempo no caixa for maior que o tempo de atendimento, o cliente sai do caixa
                        {
                            //o resto do tempo que passou do atendimento e' passado adiante
                            vetor_fila[i]->primeiro->tempo_no_caixa = vetor_caixa[i]->tempo_no_caixa - vetor_caixa[i]->tempo_atendimento;
                            free(vetor_caixa[i]);
                            vetor_caixa[i] = desenfileirar_cliente(vetor_fila[i]);  //remove o cliente da fila e o coloca no caixa
                            N_na_fila--;
                            N_atendido++;
                        }
                        aux = aux->proximo;
                    }
                }
            }
        }

    }



    tempo_medio = tempo_acumulado/N_pessoas;

    printf("                                                                                 \n");
    printf("\n------  Tempo medio = %f | Tempo maximo = %f  ------\n",tempo_medio,maior_tempo);
    return true;
}

//Calcula quanto tempo os clientes na fila levaram sendo atendidos
double calcula_tempo(FILA* afila)
{
    CLIENTE* aux;
    double valor = 0;                       //inicia com zero

    aux = afila->primeiro;
    while(aux != NULL)
    {
        valor += aux->tempo_atendimento;    //e incrementa a cada nova pessoa que passa pela fila
        aux = aux->proximo;
    }


    aux = NULL;

    return  valor;
}


//encontra a menor fila para o cliente entrar
int procura_menor_fila(FILA** vetor_fila,int N_fila)
{
    int i;
    int menor_fila = 0;
    int menor_tamanho = vetor_fila[0]->tamanho;         //compara o primeiro tamanho com outros
    for(i = 1; i < N_fila; i++)
    {
        if(vetor_fila[i]->tamanho < menor_tamanho)
        {
            menor_tamanho = vetor_fila[i]->tamanho;     //guarda o menor tamanho
            menor_fila = i;
        }
    }
    return menor_fila;
}

//libera espaço de memória
int destroi_fila(FILA* fi)
{
    CLIENTE*aux2;
    CLIENTE* aux = fi->primeiro;
    int boolean = true;
    if(fi == NULL) boolean = false;
    for(int i = 0; i < fi->tamanho; i++)                //libera todos os clientes da fila antes de liberar a fila
    {
        aux2 = aux->proximo;
        if(!destroi_cliente(aux)) boolean =  false;
        aux = aux2;
    }

    free(fi);                                           //libera a fila
    fi = NULL;
    return boolean;
}

//libera a memoria reservada para os clientes
int destroi_cliente(CLIENTE* mor)
{
    if(mor == NULL) return false;
    free(mor);
    mor = NULL;
    return true;
}
