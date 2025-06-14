#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void * pBuffer;

#define TAMANHO_MAX_NOME 50
#define TAMANHO_MAX_EMAIL 50

// Declaração manual de pseudo-variáveis armazenadas no pBuffer
#define NUMERO_DE_PESSOAS ( *(int *)pBuffer )
#define OPCAO ( *(int *)( ( (char *)pBuffer ) + sizeof(int) ) )
#define AUX_INT1 ( *(int *)( ( (char *)pBuffer ) + 2 * sizeof(int) ) )
#define AUX_INT2 ( *(int *)( ( (char *)pBuffer ) + 3 * sizeof(int) ) )
#define TAMANHO_BPBUFFER ( *(size_t *)( ( (char *)pBuffer ) + 4 * sizeof(int) ) )
#define AUX_PTR1 ( (void *)( ( (char *)pBuffer ) + 4 * sizeof(int) + sizeof(size_t) ) )
#define AUX_PTR2 ( (void *)( ( (char *)pBuffer ) + 4 * sizeof(int) + sizeof(size_t) + sizeof(void *) ) )

#define TAMANHO_VARIAVEIS ( 4 * sizeof(int) + sizeof(size_t) + 2 * sizeof(void *) )

#define INICIO_PESSOAS ( ( (char *)pBuffer ) + TAMANHO_VARIAVEIS )
#define TOPO ( (char *)pBuffer + TAMANHO_BPBUFFER - TAMANHO_MAX_PESSOA )

//                           Nome                              Idade         Email
#define TAMANHO_MAX_PESSOA ( TAMANHO_MAX_NOME * sizeof(char) + sizeof(int) + TAMANHO_MAX_EMAIL * sizeof(char) )

#define TAMANHO_INICIAL_PBUFFER (TAMANHO_VARIAVEIS + TAMANHO_MAX_PESSOA)

void adicionar();
void * buscar();
void * proxima_pessoa();
void buscar_pessoa();
void remover_pessoa();
void imprimir_pessoa();
void listar_todos();

int main() {
    pBuffer = malloc(TAMANHO_INICIAL_PBUFFER);

    NUMERO_DE_PESSOAS = 0;
    TAMANHO_BPBUFFER = TAMANHO_INICIAL_PBUFFER;

    do {
        printf("1) Adicionar Pessoa (Nome, Idade, Email)\n");
        printf("2) Remover Pessoa\n");
        printf("3) Buscar Pessoa\n");
        printf("4) Listar Todos\n");
        printf("5) Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &OPCAO);
        setbuf(stdin, NULL);

        switch (OPCAO) {
            case (1):
                adicionar();
                printf("\n");
                break;
            case (2):
                remover_pessoa();
                printf("\n");
                break;
            case (3):
                buscar_pessoa();
                printf("\n");
                break;
            case (4):
                listar_todos();
                printf("\n");
                break;
            case (5):
                break;
            default:
                printf("Opção inválida, por favor tente novamente.\n");
                break;
        }

    } while (OPCAO != 5);

    free(pBuffer);

    return 0;
}

/*
====================
adicionar
 Adiciona uma nova pessoa à agenda.

 Usa AUX_INT1 e AUX_INT2.
====================
*/
void adicionar() {
    #define TAMANHO1 AUX_INT1
    #define TAMANHO2 AUX_INT2

    NUMERO_DE_PESSOAS++;

    // Escrevendo sobre o TOPO
    printf("Digite o nome da pessoa: ");
    fgets(TOPO, TAMANHO_MAX_NOME, stdin);
    
    // Remove o '\n' do final da string
    TOPO[ strcspn(TOPO, "\n") ] = '\0';
    
    TAMANHO1 = strlen(TOPO);

    // Escrevendo sobre o TOPO + TAMANHO1 + 1 (depois do nome e do '\0')
    printf("Digite a idade da pessoa: ");
    scanf("%d", (int *)(TOPO + TAMANHO1 + 1));
    getchar();

    // Escrevendo sobre o TOPO + TAMANHO1 + 1 + sizeof(int) (depois do nome, do '\0' e da idade)
    printf("Digite o email da pessoa: ");
    fgets(TOPO + TAMANHO1 + 1 + sizeof(int), TAMANHO_MAX_EMAIL, stdin);

    // Remove o '\n' do final da string
    ( TOPO + TAMANHO1 + 1 + sizeof(int) )[ strcspn( TOPO + TAMANHO1 + 1 + sizeof(int), "\n" ) ] = '\0';
    
    TAMANHO2 = strlen(TOPO + TAMANHO1 + 1 + sizeof(int));

    //                  Nome      '\0' Idade         Email     '\0'
    TAMANHO_BPBUFFER += TAMANHO1 + 1 + sizeof(int) + TAMANHO2 + 1;
    pBuffer = realloc(pBuffer, TAMANHO_BPBUFFER);

    #undef TAMANHO1
    #undef TAMANHO2
}

/*
====================
proxima_pessoa
 Retorna o ponteiro para a próxima pessoa na agenda.

 AUX_PTR1 deve ser setado com o ponteiro para a pessoa atual.

 AUX_PRTR1 é atualizado para o ponteiro da próxima pessoa.
====================
*/
void * proxima_pessoa() {
    // Avança para depois do nome
    *(char **)AUX_PTR1 += strlen( *(char **)AUX_PTR1 ) + 1;

    // Avança para depois da idade
    *(int **)AUX_PTR1 += 1;

    // Avança para depois do email
    *(char **)AUX_PTR1 += strlen( *(char **)AUX_PTR1 ) + 1;

    return *(void **)AUX_PTR1;
}

/*
====================
buscar
 Busca uma pessoa na lista de pessoas e retorna o ponteiro para ela. Retorna NULL caso não encontre.

 AUX_PTR1 é setado com o ponteiro para a pessoa encontrada.
 
 TOPO deve ser setado com o nome da pessoa a ser buscada.

 Usa AUX_INT2 e AUX_PTR1.
====================
*/
void * buscar() {
    #define ITERADOR AUX_INT2
    #define PESSOA_ATUAL AUX_PTR1

    *(void **)PESSOA_ATUAL = INICIO_PESSOAS;
    ITERADOR = 0;
    
    while (ITERADOR < NUMERO_DE_PESSOAS) {
        if ( strcmp( *(char **)PESSOA_ATUAL, TOPO ) == 0 ) {
            return *(char **)PESSOA_ATUAL;
        }

        *(void **)PESSOA_ATUAL = proxima_pessoa();

        ITERADOR++;
    }

    *(void **)PESSOA_ATUAL = NULL;
    return NULL;

    #undef ITERADOR
    #undef PESSOA_ATUAL
}

/*
====================
imprimir_pessoa
 Imprime uma pessoa.

 AUX_PTR1 deve ser setado com o ponteiro para a pessoa a ser impressa.

 Usa AUX_PTR2.
====================
*/
void imprimir_pessoa() {
    #define PESSOA AUX_PTR2

    *(void **)PESSOA = *(void **)AUX_PTR1;
    
    printf("Nome: %s\n", *(char **)PESSOA);

    // Avança pra depois do nome
    *(char **)PESSOA += strlen( *(char **)PESSOA ) + 1;

    printf("Idade: %d\n", **(int **)PESSOA);

    // Avança pra depois da idade
    *(int **)PESSOA += 1;

    printf("Email: %s\n", *(char **)PESSOA);

    #undef PESSOA
}

/*
====================
buscar_pessoa
 Busca uma pessoa na agenda e imprime seus dados.

 Se a pessoa não for encontrada, imprime uma mensagem de erro.

 Usa AUX_PTR1.
====================
*/
void buscar_pessoa() {   
    printf("Digite o nome da pessoa a ser buscada: ");
    fgets(TOPO, TAMANHO_MAX_NOME, stdin);

    // Remove o '\n' do final da string
    TOPO[ strcspn(TOPO, "\n") ] = '\0';

    // A buscar seta AUX_PTR1 e imprimir_pessoa usa AUX_PTR1
    if (buscar() == NULL) {
        printf("Pessoa não encontrada.\n");
    } else {
        imprimir_pessoa();
    }
}

/*
====================
listar_todos
 Lista todas as pessoas na agenda.
 
 Se não houver pessoas na agenda, imprime uma mensagem informando.
 
 Usa AUX_INT1 e AUX_PTR1.
====================
*/
void listar_todos() {
    #define ITERADOR AUX_INT1
    #define PESSOA_ATUAL AUX_PTR1

    ITERADOR = 0;
    *(void **)PESSOA_ATUAL = INICIO_PESSOAS;

    if (NUMERO_DE_PESSOAS == 0) {
        printf("Nenhuma pessoa na agenda.\n");
        return;
    }

    while (ITERADOR < NUMERO_DE_PESSOAS) {
        imprimir_pessoa();

        if (ITERADOR < NUMERO_DE_PESSOAS - 1) {
            printf("\n");
        }

        *(void **)PESSOA_ATUAL = proxima_pessoa();

        ITERADOR++;
    }

    #undef ITERADOR
    #undef PESSOA_ATUAL
}

/*
====================
remover_pessoa
 Remove uma pessoa da agenda.

 Se a pessoa não for encontrada, imprime uma mensagem de erro.

 Usa AUX_INT1, AUX_PTR1 e AUX_PTR2.
====================
*/
void remover_pessoa() {
    #define TAMANHO_PESSOA_REMOVIDA AUX_INT1

    printf("Digite o nome da pessoa a ser removida: ");
    fgets(TOPO, TAMANHO_MAX_NOME, stdin);

    // Remove o '\n' do final da string
    TOPO[ strcspn(TOPO, "\n") ] = '\0';

    *(char **)AUX_PTR1 = buscar();

    if ( *(char **)AUX_PTR1 == NULL ) {
        printf("Pessoa não encontrada.\n");
    } else {
        // Seta AUX_PTR2 para a pessoa a ser removida
        *(void **)AUX_PTR2 = *(void **)AUX_PTR1;

        // Encontra a próxima pessoa e seta AUX_PTR1 para ela
        proxima_pessoa();

        #define PESSOA_REMOVIDA AUX_PTR2
        #define PROXIMA_PESSOA AUX_PTR1

        TAMANHO_PESSOA_REMOVIDA = *(char **)PROXIMA_PESSOA - *(char **)PESSOA_REMOVIDA;

        memmove( *(void **)PESSOA_REMOVIDA, *(void **)PROXIMA_PESSOA, (char *)TOPO - *(char **)PROXIMA_PESSOA );

        TAMANHO_BPBUFFER -= TAMANHO_PESSOA_REMOVIDA;

        NUMERO_DE_PESSOAS--;

        pBuffer = realloc(pBuffer, TAMANHO_BPBUFFER);

        printf("Pessoa removida.\n");
        
        #undef PESSOA_REMOVIDA
        #undef PROXIMA_PESSOA
    }

    #undef TAMANHO_PESSOA_REMOVIDA
}