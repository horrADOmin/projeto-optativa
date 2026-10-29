#include "funcoes.h"

int main(void)
{
    // INIT
    int const screen_width = 600;
    int const screen_height = 400;
    Camera2D camera = {0};
    InitWindow(screen_width, screen_height, "saBOR Persona");
    SetTargetFPS(60);

    Texture2D fundo = LoadTexture("sprites/fundo.png");
    Texture2D prota = LoadTexture("sprites/Unarmed_Walk_without_shadow.png");
    Texture2D fundo_ubuntu = LoadTexture("sprites/fundo_branco.png");

    Rectangle wall_right = {580, 0, 20, 400};
    Rectangle wall_left = {0, 0, 20, 400};
    Rectangle wall_bottom = {0, 380, 600, 20};
    Rectangle wall_top = {0, 0, 600, 20};
    Rectangle wall_mid = {280, 100, 20, 200};

Node *walls = criar_lista();
    inserir_fim(walls, wall_right);
    inserir_fim(walls, wall_left);
    inserir_fim(walls, wall_bottom);
    inserir_fim(walls, wall_top);
    inserir_fim(walls, wall_mid);

    int Offset = 55;
    int player_width = 40;
    int player_height = 50;

jogador heroi;
    criar_jogador(&heroi, prota);

    configurar_camera(&camera, &heroi);
        
    while (!WindowShouldClose())
    {

        // UPDATE

        float delta = GetFrameTime();

        movimentacao(&heroi, delta);

        rodar_frames(
            heroi.andando,
            &heroi.animacao_player.frameContador,
            &heroi.animacao_player.frameAtual
        );

        heroi.animacao_player.frameRec.x =
            heroi.animacao_player.frameAtual *
            heroi.animacao_player.sprite_width;

        heroi.animacao_player.frameRec.y =
            heroi.animacao_player.linha *
            heroi.animacao_player.sprite_height;

        Rectangle player = {
            heroi.posicao.x + Offset,
            heroi.posicao.y + Offset,
            player_width,
            player_height
        };

        checar_colisao(&player, walls, &heroi); 

        camera.target = (Vector2){heroi.posicao.x + 20, heroi.posicao.y + 20};
        
        BeginDrawing();

        // DRAW

        DrawTexture(fundo_ubuntu, 0, 0, BLUE);

        BeginMode2D(camera);
        DrawTexture(fundo, 0, 0, BLUE);
        DrawRectangleLines(player.x, player.y, player.width, player.height, GREEN);
        
        Node *atual = walls->next;
        while (atual != NULL)
        {
            DrawRectangleRec(atual->wall, RED);
            atual = atual->next;
        }

        DrawTexturePro(
            heroi.animacao_player.textura,
            heroi.animacao_player.frameRec,
            (Rectangle){heroi.posicao.x, heroi.posicao.y, 150, 150},
            (Vector2){0, 0},
            0,
            WHITE
        );
        EndMode2D();
        EndDrawing();
    }

    // DEINIT
    
    UnloadTexture(fundo);
    destruir_jogador(&heroi);
    destruir_lista(walls);
    walls = NULL;

    CloseWindow();

    return 0;
}