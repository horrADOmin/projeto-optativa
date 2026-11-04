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

bool bate_em_parede(Rectangle r, Node *walls)
{
    Node *atual = walls->next;
    while (atual != NULL)
    {
        if (CheckCollisionRecs(r, atual->wall))
        {
            return true;
        }
        atual = atual->next;
    }
    return false;
}

void checar_colisao(Rectangle* player, Node *walls, jogador* heroi)
{
    float offset = player->x - heroi->posicao.x;

    // teste do X: x NOVO, y ANTIGO
    Rectangle teste_x = {
        heroi->posicao.x + offset,
        heroi->posicaoAnterior.y + offset,
        player->width,
        player->height
    };

    if (bate_em_parede(teste_x, walls))
    {
        heroi->posicao.x = heroi->posicaoAnterior.x;
    }

    // teste do Y: x JÁ RESOLVIDO, y NOVO
    Rectangle teste_y = {
        heroi->posicao.x + offset,
        heroi->posicao.y + offset,
        player->width,
        player->height
    };

    if (bate_em_parede(teste_y, walls))
    {
        heroi->posicao.y = heroi->posicaoAnterior.y;
    }
}

void movimentacao(jogador* heroi, float delta)
{
    heroi->posicaoAnterior = heroi->posicao;

    int dx = 0;
    int dy = 0;

    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) dx += 1;
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))  dx -= 1;
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))    dy -= 1;
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))  dy += 1;

    
    heroi->andando = (dx != 0 || dy != 0);

    
    if (dx > 0) heroi->animacao_player.linha = 2;
    if (dx < 0) heroi->animacao_player.linha = 1;
    if (dy < 0) heroi->animacao_player.linha = 3;
    if (dy > 0) heroi->animacao_player.linha = 0;

    
    heroi->posicao.x += dx * heroi->speed * delta;
    heroi->posicao.y += dy * heroi->speed * delta;
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

void destruir_jogador(jogador* heroi)
{
    UnloadTexture(heroi->animacao_player.textura);
}

void configurar_camera(Camera2D* camera, jogador* heroi)
{
    camera->target.x = heroi->posicao.x + 20;
    camera->target.y = heroi->posicao.y + 20;

    camera->offset = (Vector2){ GetScreenWidth() / 2.5f, GetScreenHeight() / 2.0f };

    camera->rotation = 0.0f;
    camera->zoom = 1.0f;
}
