
#include "funcoes.h"

int main(void)
{
    // INIT
    InitWindow(600, 400, "saBOR Persona");
    SetTargetFPS(60);

    if (!FileExists("sprites/scene.png"))
    {
        ChangeDirectory(GetApplicationDirectory());
    }

    
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

    combatente inimigos[7];
    criar_combatente(&inimigos[0], "Slime do musgo", 48, 8, 1);
    criar_combatente(&inimigos[1], "Slime do musgo", 64, 9, 1);
    criar_combatente(&inimigos[2], "Slime de cobre", 86, 12, 2);
    criar_combatente(&inimigos[3], "Slime de cobre", 92, 12, 2);
    criar_combatente(&inimigos[4], "Slime da bruma", 108, 14, 3);
    criar_combatente(&inimigos[5], "Slime da bruma", 126, 15, 3);
    criar_combatente(&inimigos[6], "Rei do Eco", 228, 16, 4);

    Rectangle slimes[7] = {
        {275, 520, 80, 80}, {670, 682, 80, 80},
        {410, 845, 90, 90}, {750, 90, 90, 90},
        {1190, 390, 100, 100}, {1575, 520, 100, 100},
        {1505, 65, 145, 145}
    };
    Color cores[7] = {
        {210, 255, 155, 255}, {210, 255, 155, 255}, {255, 184, 109, 255}, {255, 184, 109, 255},
        {160, 204, 255, 255}, {160, 204, 255, 255}, {251, 159, 239, 255}
    };
    bool vivos[7] = {true, true, true, true, true, true, true};
    Rectangle baus[4] = {
        {360, 172, 44, 32}, {454, 704, 44, 32},
        {1110, 115, 44, 32}, {1580, 425, 44, 32}
    };
    const char* itens[4] = {"Espada de ferro", "Colete de couro", "Lamina do eco", "Cota de ferro"};
    int poder[4] = {20, 4, 29, 8};
    int selos[4] = {0, 1, 3, 4};
    bool abertos[4] = {false};
    Rectangle fontes[3] = {{145, 249, 38, 44}, {775, 924, 38, 44}, {1248, 350, 38, 44}};
    int derrotados = 0;
    int slimeAtual = 0;
    int bauPerto = -1;
    bool fontePerto = false;
    char recebido[80] = "";
    const char* aviso = "";
    float tempoAviso = 0;
    int rodada = 1;
    int dano = 0;
    bool sair = false;
    Rectangle jogar = {213, 189, 174, 40};
    Rectangle botaoSair = {213, 239, 174, 40};

    estado_jogo estado = ESTADO_MENU;
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

    while (!WindowShouldClose() && !sair)
    {
        float delta = GetFrameTime();

        if (delta > 0.05f)
        {
            delta = 0.05f;
        }

        // UPDATE
        if (estado == ESTADO_MENU || estado == ESTADO_FINAL ||
            (estado == ESTADO_COMBATE && acao == DERROTA))
        {
            bool clicar = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_R) ||
                (clicar && CheckCollisionPointRec(GetMousePosition(), jogar)))
            {
                criar_jogador(&heroi, prota, prota_idle);
                configurar_camera(&camera, &heroi);
                for (int i = 0; i < 7; i++)
                {
                    vivos[i] = true;
                    inimigos[i].hp = inimigos[i].hp_max;
                }
                for (int i = 0; i < 4; i++)
                {
                    abertos[i] = false;
                }
                derrotados = 0;
                hits = 0;
                frameSlime = 0;
                tempoSlime = 0;
                atacando = false;
                texturaAtual = prota_idle;
                bauPerto = -1;
                fontePerto = false;
                aviso = "";
                tempoAviso = 0;
                estado = ESTADO_EXPLORACAO;
            }
            else if (clicar && CheckCollisionPointRec(GetMousePosition(), botaoSair))
            {
                sair = true;
            }
        }
        else if (estado == ESTADO_EXPLORACAO)
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

                if (frameAtaque >= 8)
                {
                    atacando = false;
                    frameAtaque = 0;
                }
            }

            tempoAviso -= delta;
            bauPerto = -1;
            fontePerto = false;
            Rectangle alcance = {player.x - 35, player.y - 35, player.width + 70, player.height + 70};
            for (int i = 0; i < 4; i++)
            {
                if (!abertos[i] && CheckCollisionRecs(alcance, baus[i]))
                {
                    bauPerto = i;
                    if (IsKeyPressed(KEY_E) && derrotados >= selos[i])
                    {
                        abertos[i] = true;
                        aviso = "Baú aberto.";
                        if ((i == 0 || i == 2) && poder[i] > heroi.combate.ataque)
                        {
                            heroi.arma = itens[i];
                            heroi.combate.ataque = poder[i];
                            snprintf(recebido, sizeof(recebido), "Equipado: %s   |   ATQ %d", itens[i], poder[i]);
                            aviso = recebido;
                        }
                        if ((i == 1 || i == 3) && poder[i] > heroi.combate.defesa)
                        {
                            heroi.armadura = itens[i];
                            heroi.combate.defesa = poder[i];
                            snprintf(recebido, sizeof(recebido), "Equipado: %s   |   DEF %d", itens[i], poder[i]);
                            aviso = recebido;
                        }
                        tempoAviso = 3;
                        bauPerto = -1;
                    }
                }
            }
            for (int i = 0; i < 3; i++)
            {
                if (CheckCollisionRecs(alcance, fontes[i]))
                {
                    fontePerto = true;
                    if (IsKeyPressed(KEY_E))
                    {
                        heroi.combate.hp = heroi.combate.hp_max;
                        aviso = "Vida recuperada.";
                        tempoAviso = 3;
                    }
                }
            }

            for (int i = 0; i < 7; i++)
            {
                Rectangle corpo = {slimes[i].x + slimes[i].width / 4, slimes[i].y + slimes[i].height / 3,
                    slimes[i].width / 2, slimes[i].height / 2};
                if (vivos[i] && (CheckCollisionRecs(player, corpo) ||
                    (atacando && frameAtaque >= 2 && frameAtaque <= 5 && CheckCollisionRecs(ataqueHitbox, corpo))))
                {
                    if (i == 6 && derrotados < 6)
                    {
                        aviso = "Chefe selado: derrote os 6 guardas.";
                        tempoAviso = 2;
                        break;
                    }
                    slimeAtual = i;
                    atacando = false;
                    estado = ESTADO_COMBATE;
                    turno = TURNO_JOGADOR;
                    acao = ESCOLHER;
                    frameAcao = 0;
                    tempoAcao = 0;
                    frameIdle = 0;
                    tempoIdle = 0;
                    rodada = 1;
                    dano = 0;
                    break;
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

            if (acao == ESCOLHER && turno == TURNO_JOGADOR)
            {
                bool clicar = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
                if (clicar || IsKeyPressed(KEY_ONE) || IsKeyPressed(KEY_TWO) || IsKeyPressed(KEY_THREE))
                {
                    Vector2 mouse = GetMousePosition();

                    if (IsKeyPressed(KEY_ONE) || (clicar && CheckCollisionPointRec(mouse, botaoAtaque)))
                    {
                        acao = HEROI_ATACA;
                        turno = TURNO_INIMIGO;
                        frameAcao = 0;
                        tempoAcao = 0;
                    }
                    else if (IsKeyPressed(KEY_TWO) || (clicar && CheckCollisionPointRec(mouse, botaoDefesa)))
                    {
                        heroi.combate.defendendo = true;
                        if (hits < 5)
                        {
                            hits++;
                        }
                        acao = SLIME_ATACA;
                        turno = TURNO_INIMIGO;
                        frameAcao = 0;
                        tempoAcao = 0;
                    }
                    else if (hits >= 5)
                    {
                        if (IsKeyPressed(KEY_THREE) || (clicar && CheckCollisionPointRec(mouse, botaoUlt)))
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
                    dano = inimigos[slimeAtual].hp;
                    for (int i = 0; i < 3; i++)
                    {
                        atacar(&heroi.combate, &inimigos[slimeAtual]);
                    }

                    dano -= inimigos[slimeAtual].hp;
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
                    aviso = "Guarda derrotado.";
                    if (slimeAtual == 6)
                    {
                        estado = ESTADO_FINAL;
                    }
                    else
                    {
                        derrotados++;
                        if (derrotados == 6)
                        {
                            aviso = "Rei do Eco liberado no nordeste.";
                        }
                    }
                    tempoAviso = 4;
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
                        dano = inimigos[slimeAtual].hp;
                        atacar(&heroi.combate, &inimigos[slimeAtual]);
                        dano -= inimigos[slimeAtual].hp;

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
                        combatente atacante = inimigos[slimeAtual];
                        if (rodada % 3 == 0)
                        {
                            atacante.ataque += 10;
                        }
                        dano = heroi.combate.hp;
                        atacar(&atacante, &heroi.combate);
                        dano -= heroi.combate.hp;
                        rodada++;
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

        if (estado == ESTADO_MENU)
        {
            desenhar_menu("SaBOR Persona", "Jogar", jogar, botaoSair);
        }
        else if (estado == ESTADO_FINAL)
        {
            desenhar_menu("Você venceu!", "Jogar de novo", jogar, botaoSair);
        }
        else if (estado == ESTADO_COMBATE && acao == DERROTA)
        {
            desenhar_menu("Fim de jogo", "Reiniciar", jogar, botaoSair);
        }
        else if (estado == ESTADO_EXPLORACAO)
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

            for (int i = 0; i < 4; i++)
            {
                desenhar_bau(baus[i], abertos[i]);
            }
            for (int i = 0; i < 3; i++)
            {
                desenhar_fonte(fontes[i]);
            }
            for (int i = 0; i < 7; i++)
            {
                if (vivos[i])
                {
                    DrawTexturePro(
                        slimeSprite,
                        (Rectangle){frameSlime * 64, 0, 64, 64},
                        slimes[i],
                        (Vector2){0, 0},
                        0,
                        cores[i]
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

            DrawRectangle(0, 0, 600, 48, (Color){22, 18, 33, 240});
            DrawText(TextFormat("PV %d/%d", heroi.combate.hp, heroi.combate.hp_max), 12, 7, 15, WHITE);
            DrawRectangle(12, 29, 100, 6, DARKGRAY);
            DrawRectangle(12, 29, 100 * heroi.combate.hp / heroi.combate.hp_max, 6, LIME);
            DrawText(TextFormat("%s   ATQ %d", heroi.arma, heroi.combate.ataque), 135, 7, 14, GOLD);
            DrawText(TextFormat("%s   DEF %d", heroi.armadura, heroi.combate.defesa), 135, 27, 14, SKYBLUE);
            DrawText(TextFormat("GUARDAS  %d/6", derrotados), 458, 7, 15, WHITE);
            DrawText("TAB   Mapa", 458, 27, 14, LIGHTGRAY);

            DrawRectangle(0, 362, 600, 38, (Color){22, 18, 33, 235});
            if (bauPerto >= 0 && derrotados < selos[bauPerto])
            {
                DrawText(TextFormat("Baú fechado   |   Guardas: %d/%d", derrotados, selos[bauPerto]), 14, 374, 15, GOLD);
            }
            else if (bauPerto >= 0)
            {
                DrawText("E   Abrir baú", 14, 374, 15, GOLD);
            }
            else if (tempoAviso > 0)
            {
                DrawText(aviso, 14, 374, 15, WHITE);
            }
            else if (fontePerto)
            {
                DrawText("E   Recuperar vida", 14, 374, 15, SKYBLUE);
            }
            else
            {
                DrawText("WASD / setas: mover    Shift: correr    Espaço: atacar    E: usar", 14, 374, 14, LIGHTGRAY);
            }
            if (IsKeyDown(KEY_TAB))
            {
                desenhar_mapa(mapa, player, slimes, vivos, baus, abertos, fontes);
            }
        }
        else
        {
            Color fundo = {17, 19, 26, 255};
            Color painel = {29, 31, 39, 255};
            Color dourado = {190, 159, 103, 255};
            Color douradoClaro = {232, 205, 151, 255};
            Color texto = {231, 226, 216, 255};

            ClearBackground(fundo);
            DrawText(TextFormat("Rodada %d", rodada), 20, 22, 18, texto);
            if (rodada % 3 == 0 && acao == ESCOLHER)
            {
                DrawText("Golpe forte neste turno", 355, 22, 17, douradoClaro);
            }

            DrawEllipse(230, 250, 35, 6, (Color){9, 10, 15, 180});
            DrawEllipse(445, 250, 39, 7, (Color){9, 10, 15, 180});

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
                (Rectangle){135, 119, 190, 190},
                (Vector2){0, 0},
                0,
                WHITE
            );

            Rectangle destinoSlime = {352, 134, 185, 185};
            if (slimeAtual == 6)
            {
                destinoSlime = (Rectangle){313, 86, 265, 265};
            }
            DrawTexturePro(
                texturaSlime,
                (Rectangle){frameInimigo * 64, 128, 64, 64},
                destinoSlime,
                (Vector2){0, 0},
                0,
                cores[slimeAtual]
            );

            if (slimeAtual == 6 && acao != SLIME_MORRE && acao != VITORIA)
            {
                DrawRectangle(432, 183, 27, 7, GOLD);
                DrawRectangle(432, 179, 5, 6, GOLD);
                DrawRectangle(443, 176, 5, 9, GOLD);
                DrawRectangle(454, 179, 5, 6, GOLD);
            }
            if (acao == ULTIMATE)
            {
                desenhar_ultimate(espadaUlt, tempoUlt);
            }

            if (acao == SLIME_DANO)
            {
                DrawText(TextFormat("-%d", dano), 431, 119 - frameAcao * 3, 26, GOLD);
            }
            else if (acao == HEROI_DANO)
            {
                DrawText(TextFormat("-%d", dano), 215, 119 - frameAcao * 3, 26, PINK);
            }
            else if (heroi.combate.defendendo)
            {
                DrawText("GUARDA", 194, 119, 18, SKYBLUE);
            }
            DrawRectangle(0, 300, 600, 100, painel);
            DrawLine(0, 300, 600, 300, dourado);

            DrawText("HEROI", 32, 315, 17, texto);
            DrawText(inimigos[slimeAtual].nome, 345, 315, 17, texto);

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

                DrawText("1  ATACAR", 29, 121, 16, PINK);
                DrawText("2  DEFESA", 29, 169, 16, SKYBLUE);

                if (hits >= 5)
                {
                    Color corUlt = (Color){85, 65, 35, 255};

                    if (CheckCollisionPointRec(mouse, botaoUlt))
                    {
                        corUlt = dourado;
                    }

                    DrawRectangleRec(botaoUlt, corUlt);
                    DrawRectangleLinesEx(botaoUlt, 2, douradoClaro);
                    DrawText("3  ULTIMATE", 25, 217, 15, WHITE);
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
