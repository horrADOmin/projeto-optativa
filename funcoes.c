#include "funcoes.h"
#include <math.h>
#include <stdio.h>

void rodar_frames(animacione* anim)
{
    anim->frameContador++;

    if (anim->frameContador >= anim->frame_tick)
    {
        anim->frameContador = 0;
        anim->frameAtual++;
    }

    if (anim->frameAtual >= anim->frame_total)
    {
        anim->frameAtual = 0;
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
    heroi->speed = 200.0f;

    if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT))
    {
        heroi->speed = 350.0f;
    }

    heroi->posicaoAnterior = heroi->posicao;

    int dx = 0;
    int dy = 0;

    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
    {
        dx++;
    }

    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
    {
        dx--;
    }

    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
    {
        dy--;
    }

    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
    {
        dy++;
    }

    heroi->andando = false;

    if (dx != 0 || dy != 0)
    {
        heroi->andando = true;
    }

    if (dx > 0)
    {
        heroi->animacao_player.linha = 2;
    }

    if (dx < 0)
    {
        heroi->animacao_player.linha = 1;
    }

    if (dy < 0)
    {
        heroi->animacao_player.linha = 3;
    }

    if (dy > 0)
    {
        heroi->animacao_player.linha = 0;
    }

    heroi->posicao.x += dx * heroi->speed * delta;
    heroi->posicao.y += dy * heroi->speed * delta;
}


void criar_combatente(combatente* c, const char* nome, int hp, int ataque, int defesa)
{
    snprintf(c->nome, sizeof(c->nome), "%s", nome);

    c->hp = hp;
    c->hp_max = hp;
    c->ataque = ataque;
    c->defesa = defesa;
    c->defendendo = false;
}

void criar_jogador(jogador* heroi, Texture2D prota, Texture2D prota_idle)
{
    heroi->posicao = (Vector2){175, 100};
    heroi->posicaoAnterior = (Vector2){175, 100};
    heroi->speed = 150.0f;
    heroi->andando = false;

    heroi->animacao_player.textura = prota;
    heroi->animacao_player.textura_idle = prota_idle;
    heroi->animacao_player.frameRec = (Rectangle){0, 0, 64, 64};
    heroi->animacao_player.sprite_width = 64;
    heroi->animacao_player.sprite_height = 64;
    heroi->animacao_player.frameAtual = 0;
    heroi->animacao_player.frameContador = 0;
    heroi->animacao_player.linha = 1;
    heroi->animacao_player.frame_tick = 8;
    heroi->animacao_player.frame_total = 6;

    criar_combatente(&heroi->combate, "Heroi", 100, 15, 5);
}

void destruir_jogador(jogador* heroi)
{
    UnloadTexture(heroi->animacao_player.textura);
    UnloadTexture(heroi->animacao_player.textura_idle);
}

void configurar_camera(Camera2D* camera, jogador* heroi)
{
    camera->target.x = heroi->posicao.x + 75;
    camera->target.y = heroi->posicao.y + 75;

    camera->offset = (Vector2){
        GetScreenWidth() / 2.0f,
        GetScreenHeight() / 2.0f
    };

    camera->rotation = 0.0f;
    camera->zoom = 1.0f;
}

void atualizar_animacao(jogador* heroi, Texture2D prota_run, float delta)
{
    animacione* anim = &heroi->animacao_player;

    if (heroi->andando)
    {
        if (heroi->speed > 200.0f)
        {
            anim->frame_total = 8;
            anim->frame_tick = 5;
        }
        else
        {
            anim->frame_total = 6;
            anim->frame_tick = 8;
        }
    }
    else
    {
        if (anim->linha == 3)
        {
            anim->frame_total = 4;
            anim->frame_tick = 10;
        }
        else
        {
            anim->frame_total = 12;
            anim->frame_tick = 8;
        }
    }

    if (anim->frameAtual >= anim->frame_total)
    {
        anim->frameAtual = 0;
    }

    if (heroi->andando)
    {
        if (heroi->speed > 200.0f)
        {
            anim->frameRec.width = 64;
            anim->frameRec.height = 64;
        }
    }
}

int calcular_dano(combatente *atacante, combatente *alvo)
{
    int dano = atacante->ataque - alvo->defesa;

    if (dano < 1)
    {
        dano = 1;
    }

    return dano;
}

void atacar(combatente *atacante, combatente *alvo)
{
    int dano = calcular_dano(atacante, alvo);

    if (alvo->defendendo)
    {
        dano = dano / 2;

        if (dano < 1)
        {
            dano = 1;
        }

        alvo->defendendo = false;
    }

    alvo->hp -= dano;

    if (alvo->hp < 0)
    {
        alvo->hp = 0;
    }
}

void limitar_camera(Camera2D *camera, int map_width, int map_height)
{
    float metadeLargura = GetScreenWidth() / (2.0f * camera->zoom);
    float metadeAltura = GetScreenHeight() / (2.0f * camera->zoom);

    if (camera->target.x < metadeLargura)
    {
        camera->target.x = metadeLargura;
    }
    else if (camera->target.x > map_width - metadeLargura)
    {
        camera->target.x = map_width - metadeLargura;
    }

    if (camera->target.y < metadeAltura)
    {
        camera->target.y = metadeAltura;
    }
    else if (camera->target.y > map_height - metadeAltura)
    {
        camera->target.y = map_height - metadeAltura;
    }
}

void criar_mapa(Node *walls)
{
    //bordas do mapa
    inserir_fim(walls, (Rectangle){1780, 0, 20, 1200});
    inserir_fim(walls, (Rectangle){0, 0, 20, 1200});
    inserir_fim(walls, (Rectangle){0, 1180, 1800, 20});
    inserir_fim(walls, (Rectangle){0, 0, 1800, 20});

    //casas
    inserir_fim(walls, (Rectangle){357, 266, 361, 227});
    inserir_fim(walls, (Rectangle){839, 400, 300, 100});
    inserir_fim(walls, (Rectangle){839, 646, 206, 287});
    inserir_fim(walls, (Rectangle){1281, 463, 219, 225});

    inserir_fim(walls, (Rectangle){678, 174, 64, 39});
    inserir_fim(walls, (Rectangle){222, 326, 51, 36});
    inserir_fim(walls, (Rectangle){1652, 248, 44, 33});
    inserir_fim(walls, (Rectangle){570, 928, 54, 48});
    inserir_fim(walls, (Rectangle){1328, 844, 64, 51});
    inserir_fim(walls, (Rectangle){459, 774, 55, 42});

    inserir_fim(walls, (Rectangle){0, 673, 357, 135});
    inserir_fim(walls, (Rectangle){297, 790, 64, 218});
    inserir_fim(walls, (Rectangle){297, 985, 411, 147});
    inserir_fim(walls, (Rectangle){809, 985, 421, 147});
    inserir_fim(walls, (Rectangle){1170, 784, 64, 224});
    inserir_fim(walls, (Rectangle){1173, 673, 627, 135});

    inserir_fim(walls, (Rectangle){1267, 209, 138, 57});
    inserir_fim(walls, (Rectangle){1389, 308, 142, 60});
    inserir_fim(walls, (Rectangle){1588, 305, 142, 63});

    inserir_fim(walls, (Rectangle){1119, 634, 49, 60});
    inserir_fim(walls, (Rectangle){1176, 619, 51, 69});
    inserir_fim(walls, (Rectangle){1122, 463, 64, 66});
}

void desenhar_ultimate(Texture2D espadaUlt, float tempoUlt)
{
    float progresso = 0;

    if (tempoUlt >= 0.65f)
    {
        progresso = (tempoUlt - 0.65f) / 0.50f;
    }

    if (progresso > 1)
    {
        progresso = 1;
    }

    for (int i = 0; i < 3; i++)
    {
        float inicioX = 175;
        float inicioY = 45 + i * 65;
        float destinoX = 420;
        float destinoY = 175;

        float x = inicioX + (destinoX - inicioX) * progresso;
        float y = inicioY + (destinoY - inicioY) * progresso;

        float angulo = atan2f(
            destinoY - inicioY,
            destinoX - inicioX
        ) * RAD2DEG + 45;

        DrawTexturePro(
            espadaUlt,
            (Rectangle){0, 0, espadaUlt.width, espadaUlt.height},
            (Rectangle){x, y, 95, 95},
            (Vector2){47.5f, 47.5f},
            angulo,
            WHITE
        );
    }
}

void desenhar_vida(combatente heroi, combatente inimigo)
{
    float hpHeroi = (float)heroi.hp / heroi.hp_max;
    float hpSlime = (float)inimigo.hp / inimigo.hp_max;

    if (hpHeroi < 0)
    {
        hpHeroi = 0;
    }

    if (hpHeroi > 1)
    {
        hpHeroi = 1;
    }

    if (hpSlime < 0)
    {
        hpSlime = 0;
    }

    if (hpSlime > 1)
    {
        hpSlime = 1;
    }

    DrawRectangle(32, 343, 220, 8, DARKGRAY);
    DrawRectangle(345, 343, 220, 8, DARKGRAY);

    DrawRectangle(
        32, 343, 220 * hpHeroi, 8,
        (Color){111, 170, 143, 255}
    );

    DrawRectangle(
        345, 343, 220 * hpSlime, 8,
        (Color){172, 93, 91, 255}
    );

    DrawText(
        TextFormat("%d / %d", heroi.hp, heroi.hp_max),
        32, 363, 15, (Color){231, 226, 216, 255}
    );

    DrawText(
        TextFormat("%d / %d", inimigo.hp, inimigo.hp_max),
        345, 363, 15, (Color){231, 226, 216, 255}
    );
}

Rectangle criar_hitbox_ataque(Rectangle player, int linhaAtaque)
{
    if (linhaAtaque == 0)
    {
        return (Rectangle){
            player.x - 35,
            player.y + player.height,
            110, 85
        };
    }
    else if (linhaAtaque == 1)
    {
        return (Rectangle){
            player.x - 100,
            player.y - 20,
            100, 90
        };
    }
    else if (linhaAtaque == 2)
    {
        return (Rectangle){
            player.x + player.width,
            player.y - 20,
            100, 90
        };
    }
    else
    {
        return (Rectangle){
            player.x - 35,
            player.y - 85,
            110, 85
        };
    }
}