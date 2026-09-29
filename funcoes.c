#include "funcoes.h"



void rodar_frames(bool andando, int *frameContador, int *frameAtual)
{
    if (andando)
    {
        (*frameContador)++;

        if (*frameContador >= 8)
        {
            *frameContador = 0;
            (*frameAtual)++;
        }

        if (*frameAtual >= 6)
        {
            *frameAtual = 0;
        }
    }
}

void checar_colisao(Rectangle* player, Node *walls, jogador* heroi)
{
    Node *atual = walls->next;
    while (atual != NULL)
    {
        if (CheckCollisionRecs(*player, atual->wall))
        {
            heroi->posicao = heroi->posicaoAnterior;
        }
        atual = atual->next;
    }
    
}

void movimentacao(jogador* heroi, float delta)
{
    heroi->andando = false;
    heroi->posicaoAnterior = heroi->posicao;
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
    {
        heroi->posicao.x += heroi->speed * delta;
        heroi->andando = true;
        heroi->animacao_player.linha = 2;
    }

    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
    {
        heroi->posicao.x -= heroi->speed * delta;
        heroi->andando = true;
        heroi->animacao_player.linha = 1;
    }

    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
    {
        heroi->posicao.y -= heroi->speed * delta;
        heroi->andando = true;
        heroi->animacao_player.linha = 3;
    }

    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
    {
        heroi->posicao.y += heroi->speed * delta;
        heroi->andando = true;
        heroi->animacao_player.linha = 0;
    }
}