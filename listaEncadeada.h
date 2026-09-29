#ifndef LISTAENCADEADA_H
#define LISTAENCADEADA_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "raylib.h"

/* Definição da estrutura de um nó da lista encadeada */
typedef struct Node {
    Rectangle wall;  // Estrutura Rectangle para armazenar as dimensões da parede
    struct Node *next;
} Node;

/** nao sendo utilizado, mas mantido para o futuro caso seja necessário
typedef struct {
    struct Node *inicio;
    struct Node *fim;
    int len;
} Cabecalho; 
*/ 

/**
 * Aloca e inicializa o nó cabeçalho (sentinela).
 * O nó cabeçalho atua como referência fixa e não armazena dados de domínio.
 */
Node* criar_lista(void);

/**
 * Insere um novo valor no início da lista (imediatamente após o nó cabeçalho).
 */
bool inserir_inicio(Node *cabecalho, Rectangle wall);

/**
 * Insere um novo valor ao final da lista encadeada.
 */
bool inserir_fim(Node *cabecalho, Rectangle wall);

/**
Busca a primeira ocorrência de um determinado valor na lista encadeada.
Retorna o ponteiro para o nó correspondente ou NULL caso não seja localizado.
 */
/**
Node* buscar(Node *cabecalho, Rectangle wall); -> nao está sendo utilizado, mas mantido para o futuro caso seja necessário (1)
*/
/**
Remove a primeira ocorrência de um determinado valor na lista.
Retorna true em caso de remoção bem-sucedida ou false se o valor for inexistente.
 
bool remover(Node *cabecalho, Rectangle wall); -> nao está sendo utilizado, mas mantido para o futuro caso seja necessário (2)
*/

/**
Percorre e imprime os elementos da lista encadeada.

void exibir_lista(Node *cabecalho); -> nao esta sendo utilizado, mas mantido para o futuro caso seja necessário (3)
*/

/**
 * Libera sistematicamente toda a memória alocada dinamicamente, incluindo o nó sentinela.
 */
void destruir_lista(Node *cabecalho);

#endif /* LISTAENCADEADA_H */