
#ifndef FUNCOES_H
#define FUNCOES_H

#include "raylib.h"
#include "listaEncadeada.h"

typedef enum
{
    ESTADO_EXPLORACAO,
    ESTADO_COMBATE,
    ESTADO_MENU,
    ESTADO_FINAL
} estado_jogo;

typedef enum
{
    TURNO_JOGADOR,
    TURNO_INIMIGO,
    COMBATE_VITORIA,
    COMBATE_DERROTA
} estado_combate;

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
    bool defendendo;
} combatente;

typedef struct Player
{
    Vector2 posicao;
    Vector2 posicaoAnterior;
    float speed;
    bool andando;
    animacione animacao_player;
    combatente combate;
    const char* arma;
    const char* armadura;
} jogador;

void checar_colisao(Rectangle* player, Node *walls, jogador* heroi);
// ^logica da hitbox

void movimentacao(jogador* heroi, float delta);
// ^movimentação do jogador

void rodar_frames(animacione* anim);
// ^logica da animação dos sprites

void criar_jogador(jogador* heroi, Texture2D prota, Texture2D prota_idle);
// ^cria o jogador

void destruir_jogador(jogador* heroi);
// ^libera o jogador da memoria

void configurar_camera(Camera2D* camera, jogador* heroi);
// ^configs da camera

void atualizar_animacao(jogador* heroi, Texture2D prota_run, float delta);
// ^atualiza as animações de parado, andando e correndo

void criar_combatente(combatente* c, const char* nome, int hp, int ataque, int defesa);
// ^cria um combatente com seus atributos

int calcular_dano(combatente *atacante, combatente *alvo);
// ^calcula o dano do ataque

void atacar(combatente *atacante, combatente *alvo);
// ^aplica o dano no alvo

void limitar_camera(Camera2D *camera, int map_width, int map_height);
// ^impede a camera de sair do mapa

void criar_mapa(Node *walls);
// ^cria as hitboxes do mapa

void desenhar_ultimate(Texture2D espadaUlt, float tempoUlt);
// ^desenha a animação das espadas da ultimate

void desenhar_vida(combatente heroi, combatente inimigo);
// ^desenha as barras de vida do jogador e do inimigo

Rectangle criar_hitbox_ataque(Rectangle player, int linhaAtaque);
// ^cria a hitbox do ataque de acordo com a direção

void desenhar_bau(Rectangle bau, bool aberto);
// ^desenha os baus

void desenhar_fonte(Rectangle fonte);
// ^desenha as fontes de pv

void desenhar_menu(const char* titulo, const char* opcao, Rectangle jogar, Rectangle sair);
// ^desenha o menu

void desenhar_mapa(Texture2D mapa, Rectangle player, Rectangle slimes[], bool vivos[], Rectangle baus[], bool abertos[], Rectangle fontes[]);
// ^desenha o mapa


#endif
