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

void criar_jogador(jogador* heroi, Texture2D prota)
{
    heroi->posicao = (Vector2){175, 100};
    heroi->posicaoAnterior = (Vector2){175, 100};
    heroi->speed = 150.0f;
    heroi->andando = false;
    heroi->animacao_player.textura = prota;
    heroi->animacao_player.frameRec = (Rectangle){0, 0, 64, 64};
    heroi->animacao_player.sprite_width = 64;
    heroi->animacao_player.sprite_height = 64;
    heroi->animacao_player.frameAtual = 0;
    heroi->animacao_player.frameContador = 0;
    heroi->animacao_player.linha = 1;
    heroi->animacao_player.frame_tick = 8;
    heroi->animacao_player.frame_total = 6;
}