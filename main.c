
#include "funcoes.h"

int main(void)
{
    // INIT
    InitWindow(600, 400, "saBOR Persona");
    SetTargetFPS(60);

    Texture2D prota = LoadTexture("sprites/Unarmed_Walk_without_shadow.png");
    Texture2D prota_idle = LoadTexture("sprites/Unarmed_Idle_without_shadow.png");
    Texture2D prota_run = LoadTexture("sprites/Unarmed_Run_without_shadow.png");
    Texture2D ataqueSprite = LoadTexture("sprites/Sword_attack_without_shadow.png");
    Texture2D mapa = LoadTexture("sprites/scene.png");

    Texture2D slimeSprite = LoadTexture("sprites/Slime1_Idle_without_shadow.png");
    Texture2D heroiHurt = LoadTexture("sprites/Unarmed_Hurt_without_shadow.png");
    Texture2D heroiDeath = LoadTexture("sprites/Unarmed_Death_without_shadow.png");
    Texture2D slimeHurt = LoadTexture("sprites/Slime1_Hurt_without_shadow.png");
    Texture2D slimeAttack = LoadTexture("sprites/Slime1_Attack_without_shadow.png");
    Texture2D slimeDeath = LoadTexture("sprites/Slime1_Death_without_shadow.png");

    Image imagemEspada = LoadImage("sprites/espada.png");
    ImageColorReplace(&imagemEspada, BLACK, BLANK);
    Texture2D espadaUlt = LoadTextureFromImage(imagemEspada);
    UnloadImage(imagemEspada);

    Node *walls = criar_lista();
    criar_mapa(walls);

    jogador heroi;
    criar_jogador(&heroi, prota, prota_idle);

    Camera2D camera = {0};
    configurar_camera(&camera, &heroi);

    combatente inimigos[2];
    criar_combatente(&inimigos[0], "Slime", 200, 10, 2);
    criar_combatente(&inimigos[1], "Slime", 200, 10, 2);

    Rectangle slimes[2] = {
        {750, 550, 60, 60},
        {1050, 550, 60, 60}
    };

    bool vivos[2] = {true, true};
    int slimeAtual = 0;

    estado_jogo estado = ESTADO_EXPLORACAO;
    estado_combate turno = TURNO_JOGADOR;

    enum {
        ESCOLHER,
        HEROI_ATACA,
        SLIME_DANO,
        SLIME_MORRE,
        SLIME_ATACA,
        HEROI_DANO,
        HEROI_MORRE,
        ULTIMATE,
        VITORIA,
        DERROTA
    };

    int acao = ESCOLHER;
    int frameAcao = 0;
    int frameIdle = 0;
    int frameSlime = 0;
    int frameAtaque = 0;
    int linhaAtaque = 0;
    int hits = 0;

    float tempoAcao = 0;
    float tempoIdle = 0;
    float tempoSlime = 0;
    float tempoAtaque = 0;
    float tempoUlt = 0;

    bool atacando = false;

    Rectangle player = {0};
    Rectangle ataqueHitbox = {0};

    Rectangle botaoAtaque = {20, 110, 115, 39};
    Rectangle botaoDefesa = {20, 158, 115, 39};
    Rectangle botaoUlt = {20, 206, 115, 39};

    Texture2D texturaAtual = prota;

    while (!WindowShouldClose())
    {
        float delta = GetFrameTime();

        // UPDATE
        if (estado == ESTADO_EXPLORACAO)
        {
            if (!atacando)
            {
                movimentacao(&heroi, delta);
                atualizar_animacao(&heroi, prota_run, delta);
                rodar_frames(&heroi.animacao_player);
            }

            if (heroi.andando)
            {
                if (heroi.speed > 200.0f)
                {
                    texturaAtual = prota_run;
                }
                else
                {
                    texturaAtual = prota;
                }
            }
            else
            {
                texturaAtual = prota_idle;
            }

            heroi.animacao_player.frameRec.x =
                heroi.animacao_player.frameAtual *
                heroi.animacao_player.sprite_width;

            heroi.animacao_player.frameRec.y =
                heroi.animacao_player.linha *
                heroi.animacao_player.sprite_height;

            player.x = heroi.posicao.x + 55;
            player.y = heroi.posicao.y + 55;
            player.width = 40;
            player.height = 50;

            checar_colisao(&player, walls, &heroi);

            player.x = heroi.posicao.x + 55;
            player.y = heroi.posicao.y + 55;

            camera.target.x = player.x + 20;
            camera.target.y = player.y + 25;

            limitar_camera(&camera, 1800, 1200);

            tempoSlime += delta;

            if (tempoSlime >= 0.15f)
            {
                frameSlime++;

                if (frameSlime >= 6)
                {
                    frameSlime = 0;
                }

                tempoSlime = 0;
            }

            if (IsKeyPressed(KEY_SPACE) && !atacando)
            {
                atacando = true;
                frameAtaque = 0;
                tempoAtaque = 0;
                linhaAtaque = heroi.animacao_player.linha;

                ataqueHitbox = criar_hitbox_ataque(player, linhaAtaque);
            }

            if (atacando)
            {
                tempoAtaque += delta;

                if (tempoAtaque >= 0.06f)
                {
                    frameAtaque++;
                    tempoAtaque = 0;
                }

                if (frameAtaque >= 2 && frameAtaque <= 5)
                {
                    for (int i = 0; i < 2; i++)
                    {
                        if (vivos[i])
                        {
                            if (CheckCollisionRecs(ataqueHitbox, slimes[i]))
                            {
                                slimeAtual = i;
                                atacando = false;
                                estado = ESTADO_COMBATE;
                                turno = TURNO_JOGADOR;
                                acao = ESCOLHER;
                                frameAcao = 0;
                                tempoAcao = 0;
                                frameIdle = 0;
                                tempoIdle = 0;
                                break;
                            }
                        }
                    }
                }

                if (frameAtaque >= 8)
                {
                    atacando = false;
                    frameAtaque = 0;
                }
            }
        }
        else
        {
            tempoIdle += delta;
            tempoSlime += delta;

            int totalIdle = prota_idle.width / 64;

            if (totalIdle < 1)
            {
                totalIdle = 1;
            }

            if (tempoIdle >= 0.12f)
            {
                frameIdle++;

                if (frameIdle >= totalIdle)
                {
                    frameIdle = 0;
                }

                tempoIdle = 0;
            }

            if (tempoSlime >= 0.15f)
            {
                frameSlime++;

                if (frameSlime >= 6)
                {
                    frameSlime = 0;
                }

                tempoSlime = 0;
            }

            if (acao == ESCOLHER)
            {
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    Vector2 mouse = GetMousePosition();

                    if (CheckCollisionPointRec(mouse, botaoAtaque))
                    {
                        acao = HEROI_ATACA;
                        turno = TURNO_INIMIGO;
                        frameAcao = 0;
                        tempoAcao = 0;
                    }
                    else if (CheckCollisionPointRec(mouse, botaoDefesa))
                    {
                        heroi.combate.defendendo = true;
                        acao = SLIME_ATACA;
                        turno = TURNO_INIMIGO;
                        frameAcao = 0;
                        tempoAcao = 0;
                    }
                    else if (hits >= 5)
                    {
                        if (CheckCollisionPointRec(mouse, botaoUlt))
                        {
                            hits = 0;
                            tempoUlt = 0;
                            acao = ULTIMATE;
                            turno = TURNO_INIMIGO;
                        }
                    }
                }
            }
            else if (acao == ULTIMATE)
            {
                tempoUlt += delta;

                if (tempoUlt >= 1.15f)
                {
                    for (int i = 0; i < 3; i++)
                    {
                        atacar(&heroi.combate, &inimigos[slimeAtual]);
                    }

                    acao = SLIME_DANO;
                    frameAcao = 0;
                    tempoAcao = 0;
                }
            }
            else if (acao == VITORIA)
            {
                tempoAcao += delta;

                if (tempoAcao >= 1.5f)
                {
                    vivos[slimeAtual] = false;
                    estado = ESTADO_EXPLORACAO;
                    turno = TURNO_JOGADOR;
                    acao = ESCOLHER;
                    heroi.combate.defendendo = false;
                    hits = 0;
                }
            }
            else if (acao == DERROTA)
            {
                if (IsKeyPressed(KEY_R))
                {
                    heroi.combate.hp = heroi.combate.hp_max;
                    heroi.combate.defendendo = false;

                    inimigos[slimeAtual].hp = inimigos[slimeAtual].hp_max;
                    inimigos[slimeAtual].defendendo = false;

                    acao = ESCOLHER;
                    turno = TURNO_JOGADOR;
                    frameAcao = 0;
                    tempoAcao = 0;
                    hits = 0;
                }
            }
            else
            {
                tempoAcao += delta;

                if (tempoAcao >= 0.09f)
                {
                    frameAcao++;
                    tempoAcao = 0;

                    if (acao == HEROI_ATACA && frameAcao >= 8)
                    {
                        atacar(&heroi.combate, &inimigos[slimeAtual]);

                        if (hits < 5)
                        {
                            hits++;
                        }

                        acao = SLIME_DANO;
                        frameAcao = 0;
                    }
                    else if (acao == SLIME_DANO && frameAcao >= 5)
                    {
                        if (inimigos[slimeAtual].hp <= 0)
                        {
                            acao = SLIME_MORRE;
                        }
                        else
                        {
                            acao = SLIME_ATACA;
                        }

                        frameAcao = 0;
                    }
                    else if (acao == SLIME_MORRE && frameAcao >= 10)
                    {
                        acao = VITORIA;
                        frameAcao = 9;
                        tempoAcao = 0;
                    }
                    else if (acao == SLIME_ATACA && frameAcao >= 10)
                    {
                        atacar(&inimigos[slimeAtual], &heroi.combate);
                        heroi.combate.defendendo = false;

                        acao = HEROI_DANO;
                        frameAcao = 0;
                    }
                    else if (acao == HEROI_DANO && frameAcao >= 5)
                    {
                        if (heroi.combate.hp <= 0)
                        {
                            acao = HEROI_MORRE;
                        }
                        else
                        {
                            acao = ESCOLHER;
                            turno = TURNO_JOGADOR;
                        }

                        frameAcao = 0;
                    }
                    else if (acao == HEROI_MORRE && frameAcao >= 7)
                    {
                        acao = DERROTA;
                        frameAcao = 6;
                    }
                }
            }
        }

        // DRAW
        BeginDrawing();
        ClearBackground(BLACK);

        if (estado == ESTADO_EXPLORACAO)
        {
            BeginMode2D(camera);

            DrawTexturePro(
                mapa,
                (Rectangle){0, 0, mapa.width, mapa.height},
                (Rectangle){0, 0, 1800, 1200},
                (Vector2){0, 0},
                0,
                WHITE
            );

            for (int i = 0; i < 2; i++)
            {
                if (vivos[i])
                {
                    DrawTexturePro(
                        slimeSprite,
                        (Rectangle){frameSlime * 64, 0, 64, 64},
                        slimes[i],
                        (Vector2){0, 0},
                        0,
                        WHITE
                    );
                }
            }

            if (atacando)
            {
                DrawTexturePro(
                    ataqueSprite,
                    (Rectangle){frameAtaque * 64, linhaAtaque * 64, 64, 64},
                    (Rectangle){heroi.posicao.x, heroi.posicao.y, 150, 150},
                    (Vector2){0, 0},
                    0,
                    WHITE
                );
            }
            else
            {
                Rectangle frame = heroi.animacao_player.frameRec;

                DrawTexturePro(
                    texturaAtual,
                    frame,
                    (Rectangle){heroi.posicao.x, heroi.posicao.y, 150, 150},
                    (Vector2){0, 0},
                    0,
                    WHITE
                );
            }

            EndMode2D();

            DrawText(
                "SHIFT: correr | ESPACO: atacar",
                10, 370, 14, WHITE
            );
        }
        else
        {
            Color fundo = {17, 19, 26, 255};
            Color painel = {29, 31, 39, 255};
            Color dourado = {190, 159, 103, 255};
            Color douradoClaro = {232, 205, 151, 255};
            Color texto = {231, 226, 216, 255};

            DrawRectangleGradientV(
                0, 0, 600, 400,
                (Color){30, 31, 41, 255},
                fundo
            );

            DrawEllipse(160, 273, 94, 13, (Color){9, 10, 15, 180});
            DrawEllipse(440, 273, 94, 13, (Color){9, 10, 15, 180});
            DrawLine(40, 286, 560, 286, (Color){93, 82, 68, 255});

            Texture2D texturaHeroi = prota_idle;
            Texture2D texturaSlime = slimeSprite;

            int frameHeroi = frameIdle;
            int frameInimigo = frameSlime;

            if (acao == HEROI_ATACA)
            {
                texturaHeroi = ataqueSprite;
                frameHeroi = frameAcao;
            }
            else if (acao == HEROI_DANO)
            {
                texturaHeroi = heroiHurt;
                frameHeroi = frameAcao;
            }
            else if (acao == HEROI_MORRE)
            {
                texturaHeroi = heroiDeath;
                frameHeroi = frameAcao;
            }
            else if (acao == DERROTA)
            {
                texturaHeroi = heroiDeath;
                frameHeroi = 6;
            }

            if (acao == SLIME_ATACA)
            {
                texturaSlime = slimeAttack;
                frameInimigo = frameAcao;
            }
            else if (acao == SLIME_DANO)
            {
                texturaSlime = slimeHurt;
                frameInimigo = frameAcao;
            }
            else if (acao == SLIME_MORRE)
            {
                texturaSlime = slimeDeath;
                frameInimigo = frameAcao;
            }
            else if (acao == VITORIA)
            {
                texturaSlime = slimeDeath;
                frameInimigo = 9;
            }

            DrawTexturePro(
                texturaHeroi,
                (Rectangle){frameHeroi * 64, 128, 64, 64},
                (Rectangle){100, 115, 145, 145},
                (Vector2){0, 0},
                0,
                WHITE
            );

            DrawTexturePro(
                texturaSlime,
                (Rectangle){frameInimigo * 64, 128, 64, 64},
                (Rectangle){360, 115, 145, 145},
                (Vector2){0, 0},
                0,
                WHITE
            );

            if (acao == ULTIMATE)
            {
                desenhar_ultimate(espadaUlt, tempoUlt);
            }

            DrawRectangle(0, 300, 600, 100, painel);
            DrawLine(0, 300, 600, 300, dourado);

            DrawText("HEROI", 32, 315, 17, texto);
            DrawText("SLIME", 345, 315, 17, texto);

            desenhar_vida(heroi.combate, inimigos[slimeAtual]);

            if (acao == ESCOLHER)
            {
                Vector2 mouse = GetMousePosition();

                Color corAtaque = painel;
                Color corDefesa = painel;

                if (CheckCollisionPointRec(mouse, botaoAtaque))
                {
                    corAtaque = (Color){99, 59, 48, 255};
                }

                if (CheckCollisionPointRec(mouse, botaoDefesa))
                {
                    corDefesa = (Color){69, 59, 88, 255};
                }

                DrawRectangleRec(botaoAtaque, corAtaque);
                DrawRectangleRec(botaoDefesa, corDefesa);

                DrawRectangleLinesEx(botaoAtaque, 1.5f, dourado);
                DrawRectangleLinesEx(botaoDefesa, 1.5f, dourado);

                DrawText("ATACAR", 38, 121, 17, RED);
                DrawText("DEFENDER", 29, 169, 17, BLUE);

                if (hits >= 5)
                {
                    Color corUlt = (Color){85, 65, 35, 255};

                    if (CheckCollisionPointRec(mouse, botaoUlt))
                    {
                        corUlt = dourado;
                    }

                    DrawRectangleRec(botaoUlt, corUlt);
                    DrawRectangleLinesEx(botaoUlt, 2, douradoClaro);
                    DrawText("ULTIMATE", 29, 217, 16, WHITE);
                }
                else
                {
                    DrawText(
                        TextFormat("ULT: %d/5", hits),
                        27, 215, 17, dourado
                    );
                }
            }
            else if (acao == VITORIA)
            {
                DrawRectangle(0, 0, 600, 300, (Color){0, 0, 0, 150});
                DrawText("VITORIA", 205, 130, 34, douradoClaro);
            }
            else if (acao == DERROTA)
            {
                DrawRectangle(0, 0, 600, 300, (Color){0, 0, 0, 150});
                DrawText("DERROTA", 205, 120, 34, texto);
                DrawText(
                    "Pressione R para tentar novamente",
                    130, 170, 17, texto
                );
            }
        }

        EndDrawing();
    }

    // DEINIT
    UnloadTexture(mapa);
    UnloadTexture(slimeSprite);
    UnloadTexture(ataqueSprite);
    UnloadTexture(prota_run);
    UnloadTexture(espadaUlt);
    UnloadTexture(heroiHurt);
    UnloadTexture(heroiDeath);
    UnloadTexture(slimeHurt);
    UnloadTexture(slimeAttack);
    UnloadTexture(slimeDeath);

    destruir_jogador(&heroi);
    destruir_lista(walls);

    CloseWindow();

    return 0;
}
