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
        return NULL;             //Caso a fila esteja vazia não há o que retornar ou que não seja um a fila válida
    }
    else
    {
        CLIENTE* cliente_retorno;                   //Um cliente auxiliar é criado para guardar quem será removido da fila
        cliente_retorno = fila_n->primeiro;
        fila_n->primeiro = fila_n->primeiro->proximo;   //O primeiro da fila é trocado
        fila_n->tamanho--;                            //O tamanho da fila é alterado para refletir o novo valor
        return cliente_retorno;

    }
}

//Aloca memoria para um ponteiro guardar a informação de um cliente
CLIENTE* cria_cliente(double tempo_atendimento,double tempo_chegada)
{
    CLIENTE* novo_cliente;
    novo_cliente = (CLIENTE*)malloc(sizeof(CLIENTE*));
    if(novo_cliente != NULL)   //Verifica se foi possivel alocar memoria para o novo cliente
    {
        novo_cliente->tempo_chegada = tempo_chegada;                //Utiliza o tempo de chegada como uma distribuição normal
        novo_cliente->tempo_atendimento = tempo_atendimento;        //Utiliza o tempo de atendimento como uma distribuição normal
        novo_cliente->proximo = NULL;
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

    if(N_caixa == 0 || N_pessoas == 0 || N_fila == 0) return false;     //se algum parametro passado for igual a 0, não é necessario realizar o programa
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
            for(i = 0; i < N_fila; i++)
            {
                vetor_fila[i] = cria_fila();
                if(vetor_fila[i] == NULL) return false;
            }
        }

    }

    if(simulacao(vetor_fila,N_pessoas,N_caixa,N_fila))
    {
        printf("Simulacao completa\n");
    }
    else printf("Simulacao falhou\n");

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
    double tempo_maximo,tempo_medio,tempo_acumulado,tempo_atendido,tempo_fim_atendimento,N_na_fila;
    //numero de pessoas atendidas e a menor fila para inserir um novo cliente
    int N_atendido, menor_fila,i,j;
    CLIENTE* proximo_cliente,*p_auxiliar,*vetor_caixa[N_caixa];           //o proximo cliente a entrar na fila,auxiliar para busca
    proximo_cliente = NULL;

    //inicia as variaveis com valor nulo
    tempo_maximo = 0;
    tempo_acumulado = 0;
    tempo_medio = 0;
    N_na_fila = 0;
    tempo_fim_atendimento = 0;
    double tempo_decorrido = 0;
    N_atendido = 0;
    for(i = 0;i < N_caixa;i++)
    vetor_caixa[i] = NULL;
    i = j = 0;


    //começa a simulação
    while(N_atendido < N_pessoas)                                   //Enquanto houver clientes para serem atendidos
    {
        tempo_atendido = 0;

        if(N_na_fila+N_atendido < N_pessoas)                        //Verifica se não há mais clientes para entrarem na fila
        {
            //Caso existam mais clientes para entrar na fila
            //Cria um cliente com a distribuiçao normal
            proximo_cliente = cria_cliente(Normal(tempo_chega_normal,tempo_chega_desvio),Normal(tempo_atende_normal,tempo_atende_desvio));
            if(proximo_cliente == NULL) return false;               //A criação de um novo cliente falhou
            N_na_fila++;


            menor_fila = procura_menor_fila(vetor_fila,N_fila);     //Procura em qual fila o cliente entrará;
            if(!enfileirar_cliente(vetor_fila[menor_fila],proximo_cliente)) return false;   //Enfilera o cliente, caso a ação não foi realizada interrompe o programa
            if(vetor_fila[menor_fila]->tamanho == 1)                            //Quando ha somente uma pessoa na fila
            {
                tempo_atendido = proximo_cliente->tempo_atendimento;            //O tempo atendido é igual ao tempo de atendimento do cliente
               //tempo_acumulado += tempo_atendido;                              //Aumenta o tempo aculmulado
                if(tempo_maximo < tempo_atendido) tempo_maximo = tempo_atendido;//E troca o tempo maximo se for necessario
            }

            else                                                                //Se nao for o primeiro cliente
            {
                p_auxiliar = vetor_fila[menor_fila]->primeiro->proximo;         //Inicia uma varredura para calcular o tempo atendido do cliente

                while(p_auxiliar != NULL)                                       //Até que chegue no fim da fila
                {
                    tempo_atendido += p_auxiliar->tempo_atendimento;            //O tempo atendido será a somatória dos tempos de atendimento
                    tempo_decorrido += p_auxiliar->tempo_chegada;               //O tempo decorrido será a somatória dos tempos de chegada
                    if(tempo_decorrido >= vetor_fila[menor_fila]->primeiro->tempo_atendimento)   //Caso o tempo decorrido for maior que o tempo de atendimento do primeiro, o primeiro sai do caixa
                    {
                        desenfileirar_cliente(vetor_fila[menor_fila]);          //Remove o primeiro cliente da fila
                        N_na_fila--;                                            //Decrementa o tamanho da fila
                        N_atendido++;                                           //Incrementa o numero de pessoas atendidas
                        p_auxiliar = vetor_fila[menor_fila]->primeiro->proximo; //Reinicia a Busca
                        tempo_atendido = 0;
                    }
                    p_auxiliar = p_auxiliar->proximo;
                }

                //Calcula o tempo que falta para o primeiro terminar o atendimento
                tempo_fim_atendimento = calcula_tempo(vetor_fila[menor_fila],vetor_fila[menor_fila]->primeiro->tempo_atendimento,vetor_fila[menor_fila]->tamanho);
                if(tempo_fim_atendimento < 0) return false;     //Informa que houve um problema na simulação

                tempo_atendido += tempo_fim_atendimento;        //Para terminar o calculo do tempo perdido soma o valor tempo_fim_atendimento

                if(tempo_atendido > tempo_maximo) tempo_maximo = tempo_atendido;    //Verifica se o cliente atual levou o maior tempo

                tempo_acumulado += tempo_atendido;              //Adiciona o tempo do cliente ao tempo acumulado para calcular a media
            }
        }
        else
        {
            while(N_na_fila > 0)
            {
                for(i = 0; i < N_fila; i++)
                {
                    if(vetor_fila[i]->tamanho == 0) continue;
                    if(vetor_fila[i]->tamanho == 1)                                     //Quando ha somente uma pessoa na fila
                    {
                        tempo_atendido = proximo_cliente->tempo_atendimento;            //O tempo atendido é igual ao tempo de atendimento do cliente
                        tempo_acumulado += tempo_atendido;                              //Aumenta o tempo aculmulado
                        if(tempo_maximo < tempo_atendido) tempo_maximo = tempo_atendido;//E troca o tempo maximo se for necessario
                        desenfileirar_cliente(vetor_fila[i]);
                        N_na_fila--;
                        N_atendido++;
                    }

                    else                                                                //Se nao for o primeiro cliente
                    {
                        p_auxiliar = vetor_fila[i]->primeiro->proximo;                  //Inicia uma varredura para calcular o tempo atendido do cliente

                        while(p_auxiliar != NULL)                                       //Até que chegue no fim da fila
                        {
                            tempo_atendido += p_auxiliar->tempo_atendimento;            //O tempo atendido será a somatória dos tempos de atendimento
                            tempo_decorrido += p_auxiliar->tempo_chegada;               //O tempo decorrido será a somatória dos tempos de chegada
                            if(N_fila == 1)
                            {
                                for(j = 0; j < N_caixa;j++)
                                {
                                    if(caixa(j,vetor_fila[i]->primeiro->proximo,tempo_decorrido,vetor_caixa))
                                    {
                                        desenfileirar_cliente(vetor_fila[i]);                   //Remove o primeiro cliente da fila
                                        N_na_fila--;                                            //Decrementa o tamanho da fila
                                        N_atendido++;                                           //Incrementa o numero de pessoas atendidas
                                        p_auxiliar = vetor_fila[i]->primeiro->proximo;          //Reinicia a Busca
                                        tempo_atendido = 0;
                                    }
                                }
                                if(p_auxiliar == NULL) break;
                            }
                            else
                            {
                                for(j = 0; j < N_caixa;j++)
                                {

                                }
                            }
                            p_auxiliar = p_auxiliar->proximo;
                        }

                        //Calcula o tempo que falta para o primeiro terminar o atendimento
                        tempo_fim_atendimento = calcula_tempo(vetor_fila[i],vetor_fila[i]->primeiro->tempo_atendimento,vetor_fila[i]->tamanho);
                        if(tempo_fim_atendimento < 0) return false;     //Informa que houve um problema na simulação

                        tempo_atendido += tempo_fim_atendimento;        //Para terminar o calculo do tempo perdido soma o valor tempo_fim_atendimento

                        if(tempo_atendido > tempo_maximo) tempo_maximo = tempo_atendido;    //Verifica se o cliente atual levou o maior tempo

                        tempo_acumulado += tempo_atendido;
                    }
                }


            }
        }
    }
    tempo_medio = tempo_acumulado/N_pessoas;

    printf("\n------  Tempo medio = %f | Tempo maximo = %f  ------\n",tempo_medio,tempo_maximo);

    return true;
}

//Funcao que calcula quanto deverá ser subtraido do tempo de atendimento do cliente no caixa(quanto tempo ja passou desde o inicio do atendimento)
double calcula_tempo(FILA* fila_n, double tempo_atendimento,double tamanho_fila)
{
    double valor;                                   //Valor que ira retornar
    int i,j;
    CLIENTE* auxiliar,*o_auxiliar;                   //Auxiliar para buscar pelo cliente

    auxiliar = fila_n->primeiro->proximo;
    valor = 0;                      //Este valor será o tempo que o primeiro cliente já foi atendido

    while(auxiliar != NULL )
    {
        valor = valor + auxiliar->tempo_chegada;
        auxiliar = auxiliar->proximo;
    }
    valor = tempo_atendimento - valor;

    return  valor;
}

int procura_menor_fila(FILA** vetor_fila,int N_fila)
{
    int i;
    int menor_fila = 0;
    int menor_tamanho = vetor_fila[0]->tamanho;
    for(i = 1; i < N_fila; i++)
    {
        if(vetor_fila[i]->tamanho < menor_tamanho)
        {
            menor_tamanho = vetor_fila[i]->tamanho;
            menor_fila = i;
        }
    }
    return menor_fila;
}

int caixa(int qual_caixa,CLIENTE* client,double tempo_decorrido,CLIENTE* vetor_caixa[])
{
    if(vetor_caixa[qual_caixa] == NULL)
    {
        vetor_caixa[qual_caixa] = client;
        return true;
    }
    else if(vetor_caixa[qual_caixa]->tempo_atendimento <= tempo_decorrido)
    {
        vetor_caixa[qual_caixa] = client;
        return true;
    }
    else return false;
}
