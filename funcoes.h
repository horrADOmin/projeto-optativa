#ifndef FUNCOES_H
#define FUNCOES_H

#include "raylib.h"
#include "listaEncadeada.h"


typedef struct Animacao
{
    Texture2D textura;
    Rectangle frameRec;
    int sprite_width;
    int sprite_height;
    int frameAtual;
    int frameContador;
    int linha;
    int frame_tick;
    int frame_total;

} animacione;


typedef struct Player
{
    Vector2 posicao;
    Vector2 posicaoAnterior;
    float speed;
    bool andando;
    animacione animacao_player;
} jogador;


void checar_colisao(Rectangle* player, Node *walls, jogador* heroi);
// ^checa colisao entre player e wall

void movimentacao(jogador* heroi, float delta);
// ^movimentação do player

void rodar_frames(bool andando, int* frameContador, int* frameAtual);
// ^movimentação dos frames


#endif