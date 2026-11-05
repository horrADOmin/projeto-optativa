#ifndef FUNCOES_H
#define FUNCOES_H

#include "raylib.h"
#include "listaEncadeada.h"

typedef enum
{
    ESTADO_EXPLORACAO,
    ESTADO_COMBATE
} estado_jogo;

typedef struct Animacao
{
    Texture2D textura;
    Texture2D textura_idle;
    Rectangle frameRec;
    int sprite_width;
    int sprite_height;
    int frameAtual;
    int frameContador;
    int linha;
    int frame_tick;
    int frame_total;

} animacione;

typedef struct
{
    char nome[20];
    int hp;
    int hp_max;
    int ataque;
    int defesa;
} combatente;

typedef struct Player
{
    Vector2 posicao;
    Vector2 posicaoAnterior;
    float speed;
    bool andando;
    animacione animacao_player;
    combatente combate;
} jogador;


void checar_colisao(Rectangle* player, Node *walls, jogador* heroi);
// ^checa colisao entre player e wall

void movimentacao(jogador* heroi, float delta);
// ^movimentação do player

void rodar_frames(animacione* anim);
// ^movimentação dos frames

void criar_jogador(jogador* heroi, Texture2D prota, Texture2D prota_idle);
// ^cria o jogador

void destruir_jogador(jogador* heroi);
// ^destrói o jogador e a textura

void configurar_camera(Camera2D* camera, jogador* heroi);
// ^configura a camera para seguir o jogador

void atualizar_animacao(jogador* heroi);
// ^atualiza a animação do jogador com base no estado de movimento

void criar_combatente(combatente* c, const char* nome, int hp, int ataque, int defesa);
// ^cria um combatente com os atributos

#endif /* FUNCOES_H */