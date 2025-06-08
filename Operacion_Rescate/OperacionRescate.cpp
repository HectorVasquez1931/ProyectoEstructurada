#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct
{
    int objetivo;
    int numA;
    int numB;
    int numC;
    int numD;
    int numE;
    int numF;

    int resultadoJugador;
    int clicksHechos;

    bool mostrarResultado;
    bool resultadoCorrecto;

    bool botonASeleccionado;
    bool botonBSeleccionado;
    bool botonCSeleccionado;
    bool botonDSeleccionado;
    bool botonESeleccionado;
    bool botonFSeleccionado;

    Rectangle botonA;
    Rectangle botonB;
    Rectangle botonC;
    Rectangle botonD;
    Rectangle botonE;
    Rectangle botonF;

    int aciertos = 0;

    bool modoResta = false;
    bool modoMultiplicacion = false;
    bool modoDivision = false;

    // tiempo de preguntas
    float tiempoRestante;
    float tiempoMaximo;
    bool tiempoAgotado;
} Juego;

typedef struct button
{
    Rectangle rectan;
    Color color;
} Ts_button;

typedef struct button_circle
{
    int x;
    int y;
    int radious;
    Color color;
} Ts_button_circle;

typedef enum
{
    MENU,
    NIVEL1,
    NIVEL2,
    NIVE3,
    MAPA
} SCREEN_MODE;

Ts_button button_0 = {0};
Ts_button button_1 = {0};
Ts_button button_2 = {0};
// CAMBIO
Ts_button button_cerrar = {0};
Ts_button button_play = {0};
Ts_button button_lava = {0};
Ts_button button_nieve = {0};
Ts_button button_final = {0};
Ts_button button_reiniciar = {0};

// funciones logica de los niveles
Juego GenerarNuevaPreguntaSuma(Juego estructura);
Juego GenerarNuevaPreguntaResta(Juego estructura);
void funcion_pausa();
Juego GenerarNuevaPreguntaMultiplicacion(Juego estructura);
Juego GenerarNuevaPreguntaDivision(Juego estructura);

int main()
{
    const int screenWidth = 1920;
    const int screenHeight = 1080;
    int vidas;
    int aciertos = 0;

    int pausa = 0;
    int bandera_vidas = 0;
    InitWindow(screenWidth, screenHeight, "One piece");
    ToggleFullscreen();
    SetExitKey(0);
    InitAudioDevice();

    int bandera_nivel2 = 0;
    int bandera_nivel3 = 0;

    Texture2D background = LoadTexture("bosque.jpg");
    Texture2D background2 = LoadTexture("fondo2.png");
    // CAMBIO
    Texture2D fondo_nieve = LoadTexture("fondo_nieve.jpg");
    Texture2D fondo_volcan = LoadTexture("fondo_volcan.jpg");
    Texture2D fondo_final = LoadTexture("fondo_final.jpg");

    Texture2D bomba = LoadTexture("bomba.png");
    Texture2D bombaexplotando = LoadTexture("bombaexplosion.png");
    Texture2D op_logo = LoadTexture("OP_logo.png");

    // CAMBIO
    Texture2D cerrar = LoadTexture("boton_salir.png");
    Texture2D play = LoadTexture("boton_play.png");
    Texture2D islas = LoadTexture("islas.jpg");
    Texture2D imagen_pausa = LoadTexture("en_pausa.png");
    Texture2D reiniciar = LoadTexture("boton_reiniciar.png");
    Texture2D vidas_agotas = LoadTexture("imagen_vidas_agotadas.png");
    Texture2D texto_nivel1 = LoadTexture("imagentextonivel1.png");
    Texture2D texto_nivel2 = LoadTexture("imagentextonivel2.png");
    Texture2D texto_nivel3 = LoadTexture("imagentextonivel3.png");
    Texture2D texto_bloqueado_nivel2 = LoadTexture("bloqueado_nivel2.png");
    Texture2D texto_bloqueado_nivel3 = LoadTexture("bloqueado_nivel3.png");
    Texture2D imagen_nivel_superado = LoadTexture("imagen_nivel_superado.png");

    // animacion del hielo
    Texture2D hielo_completo = LoadTexture("hielo_completo.jpg");
    Texture2D hielo_parcial = LoadTexture("hielo_parcial.jpg");
    Texture2D hielo_derretido = LoadTexture("hielo_derretido.jpg");

    Font fuente_letra = LoadFont("fuente.ttf");

    ////////////////////////////

    Music cancion_principal = LoadMusicStream("principal.mp3");
    Music cancion_navidad = LoadMusicStream("navidad.mp3");
    Music cancion_medieval = LoadMusicStream("medieval.mp3");
    Music cancion_final = LoadMusicStream("musica_final.mp3");

    SetTargetFPS(60);

    button_0.rectan = {screenWidth / 2.0f - 100, screenHeight / 2.0f - 50, 200, 50};
    button_1.rectan = {screenWidth / 2.0f - 100, screenHeight / 2.0f + 50, 200, 50};
    button_2.rectan = {screenWidth / 2.0f - 100, screenHeight / 2.0f + 200, 200, 50};

    // CAMBIO
    button_cerrar.rectan = {screenWidth / 2.0f + 200, screenHeight / 2.0f, 300, 300};
    button_play.rectan = {screenWidth / 2.0f - 200, screenHeight / 2.0f, 300, 300};
    button_reiniciar.rectan = {screenWidth / 2.0f - 200, screenHeight / 2.0f, 300, 300};

    ///////////////////////////////////

    /******************************************* funciones logica de los niveles ****************************************************/

    srand(time(NULL));

    Juego estructura;

    estructura.botonA = (Rectangle){1150, 350, 250, 100};
    estructura.botonB = (Rectangle){1500, 350, 250, 100};
    estructura.botonC = (Rectangle){1150, 600, 250, 100};
    estructura.botonD = (Rectangle){1500, 600, 250, 100};
    estructura.botonE = (Rectangle){1150, 850, 250, 100};
    estructura.botonF = (Rectangle){1500, 850, 250, 100};

    Rectangle botonContinuar = {1330, 250, 250, 60};

    estructura = GenerarNuevaPreguntaSuma(estructura);

    int validacion = 0;
    int validacionNivel3 = 0;

    /********************************************************************************************************************************/

    // Main game loop

    SCREEN_MODE screen = MENU;
    int last_screen = -1;

    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        UpdateMusicStream(cancion_principal);
        UpdateMusicStream(cancion_medieval);
        UpdateMusicStream(cancion_navidad);
        UpdateMusicStream(cancion_final);

        if (screen != last_screen)
        {
            StopMusicStream(cancion_principal);
            StopMusicStream(cancion_navidad);
            StopMusicStream(cancion_medieval);
            StopMusicStream(cancion_final);

            switch (screen)
            {
            case MENU:
                PlayMusicStream(cancion_principal);
                break;
            case NIVEL1:
                PlayMusicStream(cancion_navidad);
                break;
            case NIVEL2:
                PlayMusicStream(cancion_medieval);
                break;
            case NIVE3:
                PlayMusicStream(cancion_final);
                break;
            case MAPA:
                PlayMusicStream(cancion_principal);
                break;
            }
            last_screen = screen;
        }

        BeginDrawing();
        ClearBackground(WHITE);
        Vector2 mouse = GetMousePosition();

        switch (screen)
        {
        case MENU:
        {
            DrawTexture(background, 0, 0, WHITE); // Dibuja la imagen en (0,0)
            float escala = 1.5f;
            int op_logo_ancho = op_logo.width * escala;
            int op_logo_alto = op_logo.height * escala;

            Vector2 posicion_logo = {
                screenWidth / 2.0f - op_logo_ancho / 2.0f,
                screenHeight / 2.0f - op_logo_alto / 2.0f - 300};

            DrawTextureEx(op_logo, posicion_logo, 0.0f, escala, WHITE);
            DrawRectangleRec(button_0.rectan, button_0.color);
            DrawRectangleRec(button_2.rectan, button_2.color);

            // CAMBIO
            Vector2 centro_cerrar = {
                button_cerrar.rectan.x + button_cerrar.rectan.width / 2 - 150,
                button_cerrar.rectan.y + button_cerrar.rectan.height / 2};
            float radio_cerrar = button_cerrar.rectan.width / 2;

            Vector2 centro_play = {
                button_play.rectan.x + button_play.rectan.width / 2 - 150,
                button_play.rectan.y + button_play.rectan.height / 2};
            float radio_play = button_play.rectan.width / 2;

            DrawTexture(cerrar, button_cerrar.rectan.x - 150, button_cerrar.rectan.y, WHITE);
            if (CheckCollisionPointCircle(GetMousePosition(), centro_cerrar, radio_cerrar))
            {
                DrawCircleLines(centro_cerrar.x, centro_cerrar.y, radio_cerrar, RED); //  resaltar
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    CloseWindow();
                }
            }

            DrawTexture(play, button_play.rectan.x - 150, button_play.rectan.y, WHITE);
            if (CheckCollisionPointCircle(GetMousePosition(), centro_play, radio_play))
            {
                DrawCircleLines(centro_play.x, centro_play.y, radio_play, RED); // resaltar
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    screen = MAPA;
                    aciertos = 0;
                }
            }
        };
        break;

        case NIVEL1:
            DrawTexture(fondo_nieve, 0, 0, WHITE);
            if (vidas > 0)
            {
                char texto_vidas[13];
                sprintf(texto_vidas, "Vidas: %d", vidas);
                Vector2 posicionTexto = {20, 20};
                DrawTextEx(fuente_letra, texto_vidas, posicionTexto, 34, 1, BLACK);

                if (pausa == 0)
                {
                    if (aciertos < 10)
                    {

                        if (!estructura.mostrarResultado && !estructura.tiempoAgotado)
                        {
                            estructura.tiempoRestante -= GetFrameTime();
                            if (estructura.tiempoRestante <= 0)
                            {
                                estructura.tiempoRestante = 0;
                                estructura.tiempoAgotado = true;
                                estructura.mostrarResultado = true;
                                estructura.resultadoCorrecto = false;
                            }
                        }

                        if (!estructura.mostrarResultado && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                        {
                            if (CheckCollisionPointRec(mouse, estructura.botonA) && !estructura.botonASeleccionado)
                            {
                                if (!estructura.modoResta)
                                {
                                    estructura.resultadoJugador += estructura.numA;
                                }
                                else
                                {
                                    // modo resta
                                    if (estructura.clicksHechos == 0)
                                    {
                                        estructura.resultadoJugador += estructura.numA;
                                    }
                                    else
                                    {
                                        estructura.resultadoJugador -= estructura.numA;
                                    }
                                }

                                estructura.clicksHechos++;
                                estructura.botonASeleccionado = true;
                            }
                            else
                            {
                                if (CheckCollisionPointRec(mouse, estructura.botonB) && !estructura.botonBSeleccionado)
                                {
                                    if (!estructura.modoResta)
                                    {
                                        estructura.resultadoJugador += estructura.numB;
                                    }
                                    else
                                    {
                                        // modo resta
                                        if (estructura.clicksHechos == 0)
                                        {
                                            estructura.resultadoJugador += estructura.numB;
                                        }
                                        else
                                        {
                                            estructura.resultadoJugador -= estructura.numB;
                                        }
                                    }

                                    estructura.clicksHechos++;
                                    estructura.botonBSeleccionado = true;
                                }
                                else
                                {
                                    if (CheckCollisionPointRec(mouse, estructura.botonC) && !estructura.botonCSeleccionado)
                                    {
                                        if (!estructura.modoResta)
                                        {
                                            estructura.resultadoJugador += estructura.numC;
                                        }
                                        else
                                        {
                                            // modo resta
                                            if (estructura.clicksHechos == 0)
                                            {
                                                estructura.resultadoJugador += estructura.numC;
                                            }
                                            else
                                            {
                                                estructura.resultadoJugador -= estructura.numC;
                                            }
                                        }

                                        estructura.clicksHechos++;
                                        estructura.botonCSeleccionado = true;
                                    }
                                    else
                                    {
                                        if (CheckCollisionPointRec(mouse, estructura.botonD) && !estructura.botonDSeleccionado)
                                        {
                                            if (!estructura.modoResta)
                                            {
                                                estructura.resultadoJugador += estructura.numD;
                                            }
                                            else
                                            {
                                                // modo resta
                                                if (estructura.clicksHechos == 0)
                                                {
                                                    estructura.resultadoJugador += estructura.numD;
                                                }
                                                else
                                                {
                                                    estructura.resultadoJugador -= estructura.numD;
                                                }
                                            }

                                            estructura.clicksHechos++;
                                            estructura.botonDSeleccionado = true;
                                        }
                                        else
                                        {
                                            if (CheckCollisionPointRec(mouse, estructura.botonE) && !estructura.botonESeleccionado)
                                            {
                                                if (!estructura.modoResta)
                                                {
                                                    estructura.resultadoJugador += estructura.numE;
                                                }
                                                else
                                                {
                                                    // modo resta
                                                    if (estructura.clicksHechos == 0)
                                                    {
                                                        estructura.resultadoJugador += estructura.numE;
                                                    }
                                                    else
                                                    {
                                                        estructura.resultadoJugador -= estructura.numE;
                                                    }
                                                }

                                                estructura.clicksHechos++;
                                                estructura.botonESeleccionado = true;
                                            }
                                            else
                                            {
                                                if (CheckCollisionPointRec(mouse, estructura.botonF) && !estructura.botonFSeleccionado)
                                                {
                                                    if (!estructura.modoResta)
                                                    {
                                                        estructura.resultadoJugador += estructura.numF;
                                                    }
                                                    else
                                                    {
                                                        // modo resta
                                                        if (estructura.clicksHechos == 0)
                                                        {
                                                            estructura.resultadoJugador += estructura.numF;
                                                        }
                                                        else
                                                        {
                                                            estructura.resultadoJugador -= estructura.numF;
                                                        }
                                                    }
                                                    estructura.clicksHechos++;
                                                    estructura.botonFSeleccionado = true;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }

                        if (estructura.clicksHechos == 2 && !estructura.mostrarResultado)
                        {
                            if (estructura.resultadoJugador < 0)
                            {
                                estructura.resultadoJugador *= -1;
                            }

                            estructura.resultadoCorrecto = (estructura.resultadoJugador == estructura.objetivo);
                            estructura.mostrarResultado = true;

                            if (estructura.resultadoCorrecto)
                            {
                                estructura.aciertos++;
                                aciertos++;

                                if (estructura.aciertos >= 5 && !estructura.modoResta)
                                {
                                    estructura.modoResta = true;
                                    estructura.aciertos = 0; // reinicia para poder usarlo también con futuras validaciones
                                }
                            }
                        }

                        if (!estructura.modoResta)
                        {
                            Vector2 posicion = {1000, 150};
                            char texto[100];
                            sprintf(texto, "Suma dos botones para llegar a: %d", estructura.objetivo);
                            DrawTextEx(fuente_letra, texto, posicion, 50, 1, BLACK);
                        }
                        else
                        {
                            Vector2 posicion = {1000, 150};
                            char texto[100];
                            sprintf(texto, "Resta dos botones para llegar a: %d", estructura.objetivo);
                            DrawTextEx(fuente_letra, texto, posicion, 50, 1, BLACK);
                        }

                        {
                            int fontSize = 30;
                            float spacing = 1;

                            // Botón A
                            DrawRectangleRec(estructura.botonA, estructura.botonASeleccionado ? SKYBLUE : LIGHTGRAY);
                            char textoA[10];
                            sprintf(textoA, "%d", estructura.numA);
                            Vector2 medidaTextoA = MeasureTextEx(fuente_letra, textoA, fontSize, spacing);
                            Vector2 posicionTextoA = {
                                estructura.botonA.x + (estructura.botonA.width - medidaTextoA.x) / 2,
                                estructura.botonA.y + (estructura.botonA.height - medidaTextoA.y) / 2};
                            DrawTextEx(fuente_letra, textoA, posicionTextoA, fontSize, spacing, BLACK);

                            // Botón B
                            DrawRectangleRec(estructura.botonB, estructura.botonBSeleccionado ? SKYBLUE : LIGHTGRAY);
                            char textoB[10];
                            sprintf(textoB, "%d", estructura.numB);
                            Vector2 medidaTextoB = MeasureTextEx(fuente_letra, textoB, fontSize, spacing);
                            Vector2 posicionTextoB = {
                                estructura.botonB.x + (estructura.botonB.width - medidaTextoB.x) / 2,
                                estructura.botonB.y + (estructura.botonB.height - medidaTextoB.y) / 2};
                            DrawTextEx(fuente_letra, textoB, posicionTextoB, fontSize, spacing, BLACK);

                            // Botón C
                            DrawRectangleRec(estructura.botonC, estructura.botonCSeleccionado ? SKYBLUE : LIGHTGRAY);
                            char textoC[10];
                            sprintf(textoC, "%d", estructura.numC);
                            Vector2 medidaTextoC = MeasureTextEx(fuente_letra, textoC, fontSize, spacing);
                            Vector2 posicionTextoC = {
                                estructura.botonC.x + (estructura.botonC.width - medidaTextoC.x) / 2,
                                estructura.botonC.y + (estructura.botonC.height - medidaTextoC.y) / 2};
                            DrawTextEx(fuente_letra, textoC, posicionTextoC, fontSize, spacing, BLACK);

                            // Botón D
                            DrawRectangleRec(estructura.botonD, estructura.botonDSeleccionado ? SKYBLUE : LIGHTGRAY);
                            char textoD[10];
                            sprintf(textoD, "%d", estructura.numD);
                            Vector2 medidaTextoD = MeasureTextEx(fuente_letra, textoD, fontSize, spacing);
                            Vector2 posicionTextoD = {
                                estructura.botonD.x + (estructura.botonD.width - medidaTextoD.x) / 2,
                                estructura.botonD.y + (estructura.botonD.height - medidaTextoD.y) / 2};
                            DrawTextEx(fuente_letra, textoD, posicionTextoD, fontSize, spacing, BLACK);

                            // Botón E
                            DrawRectangleRec(estructura.botonE, estructura.botonESeleccionado ? SKYBLUE : LIGHTGRAY);
                            char textoE[10];
                            sprintf(textoE, "%d", estructura.numE);
                            Vector2 medidaTextoE = MeasureTextEx(fuente_letra, textoE, fontSize, spacing);
                            Vector2 posicionTextoE = {
                                estructura.botonE.x + (estructura.botonE.width - medidaTextoE.x) / 2,
                                estructura.botonE.y + (estructura.botonE.height - medidaTextoE.y) / 2};
                            DrawTextEx(fuente_letra, textoE, posicionTextoE, fontSize, spacing, BLACK);

                            // Botón F
                            DrawRectangleRec(estructura.botonF, estructura.botonFSeleccionado ? SKYBLUE : LIGHTGRAY);
                            char textoF[10];
                            sprintf(textoF, "%d", estructura.numF);
                            Vector2 medidaTextoF = MeasureTextEx(fuente_letra, textoF, fontSize, spacing);
                            Vector2 posicionTextoF = {
                                estructura.botonF.x + (estructura.botonF.width - medidaTextoF.x) / 2,
                                estructura.botonF.y + (estructura.botonF.height - medidaTextoF.y) / 2};
                            DrawTextEx(fuente_letra, textoF, posicionTextoF, fontSize, spacing, BLACK);
                        };

                        // boton continuar
                        if (estructura.mostrarResultado)
                        {
                            DrawRectangleRec(botonContinuar, LIGHTGRAY);
                            char texto_continuar[100] = "Continuar";
                            Vector2 posicionTexto = {
                                botonContinuar.x + 40,
                                botonContinuar.y + 20};

                            DrawTextEx(fuente_letra, texto_continuar, posicionTexto, 34, 1, BLACK);
                        }

                        if (estructura.mostrarResultado)
                        {
                            if (estructura.resultadoCorrecto)
                            {
                                DrawRectangleRec(botonContinuar, LIGHTGRAY);
                                char texto_continuar[100] = "Continuar";
                                Vector2 posicionTexto = {
                                    botonContinuar.x + 40,
                                    botonContinuar.y + 20};

                                DrawTextEx(fuente_letra, texto_continuar, posicionTexto, 34, 1, BLACK);

                                Vector2 posicionTexto2 = {
                                    screenWidth / 2 - 800,
                                    screenHeight / 2};
                                DrawTextEx(fuente_letra, "Correcto, presione continuar :)", posicionTexto2, 34, 1, GREEN);
                            }
                            else
                            {
                                Vector2 posicionTexto = {
                                    screenWidth / 2 - 800,
                                    screenHeight / 2};

                                if (estructura.tiempoAgotado)
                                {
                                    DrawTextEx(fuente_letra, "Se acabo el tiempo", posicionTexto, 34, 1, RED);
                                }
                                else
                                {
                                    DrawTextEx(fuente_letra, "Incorrecto :( Presiona para intentarlo de nuevo", posicionTexto, 34, 1, RED);
                                }

                                if (bandera_vidas == 0)
                                {
                                    vidas--;
                                }
                                bandera_vidas = 1;
                            }
                        }

                        if (estructura.mostrarResultado && CheckCollisionPointRec(mouse, botonContinuar))
                        {

                            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                            {
                                bandera_vidas = 0;
                                if (estructura.modoResta)
                                {
                                    estructura = GenerarNuevaPreguntaResta(estructura);
                                }
                                else
                                {
                                    estructura = GenerarNuevaPreguntaSuma(estructura);
                                }
                            }
                        }

                        // barra de tiempo
                        float barraMaxAncho = 600;
                        float barraAncho = (estructura.tiempoRestante / estructura.tiempoMaximo) * barraMaxAncho;

                        DrawRectangle(160, 700, barraMaxAncho, 20, GRAY);       // fondo
                        DrawRectangle(160, 700, barraAncho, 20, GREEN);         // tiempo restante
                        DrawRectangleLines(160, 700, barraMaxAncho, 20, BLACK); // borde

                        /*if (estructura.tiempoRestante > (estructura.tiempoMaximo / 2))
                        {
                            DrawTexture(hielo_completo, 80, 300, WHITE); // imagen de hielo completo
                        }
                        else
                        {
                            if (estructura.tiempoRestante == 0)
                            {
                                DrawTexture(hielo_derretido, 80, 300, WHITE); // imagen de hielo derretido
                            }
                            else
                            {
                                DrawTexture(hielo_parcial, 80, 300, WHITE); // imagen de hielo parcial
                            }
                        }*/

                        if (IsKeyPressed(KEY_ESCAPE))
                        {
                            pausa = 1;
                        }
                    }
                    else
                    {

                        Vector2 centra_imagen = {
                            (screenWidth - imagen_nivel_superado.width) / 2.0f,
                            (screenHeight - imagen_nivel_superado.height) / 2.0f};
                        DrawTexture(imagen_nivel_superado, centra_imagen.x, centra_imagen.y - 300, WHITE);

                        DrawTexture(play, button_play.rectan.x, button_play.rectan.y, WHITE);
                        Vector2 centro_play = {
                            button_play.rectan.x + button_play.rectan.width / 2,
                            button_play.rectan.y + button_play.rectan.height / 2};
                        float radio_play = button_play.rectan.width / 2;

                        DrawTexture(cerrar, button_cerrar.rectan.x, button_cerrar.rectan.y, WHITE);
                        Vector2 centro_cerrar = {
                            button_cerrar.rectan.x + button_cerrar.rectan.width / 2,
                            button_cerrar.rectan.y + button_cerrar.rectan.height / 2};
                        float radio_cerrar = button_cerrar.rectan.width / 2;

                        float reiniciar_x = button_reiniciar.rectan.x - 400;
                        DrawTexture(reiniciar, reiniciar_x, button_reiniciar.rectan.y, WHITE);

                        Vector2 centro_reiniciar = {
                            reiniciar_x + button_reiniciar.rectan.width / 2,
                            button_reiniciar.rectan.y + button_reiniciar.rectan.height / 2};
                        float radio_reiniciar = button_reiniciar.rectan.width / 2;

                        if (CheckCollisionPointCircle(GetMousePosition(), centro_play, radio_play))
                        {
                            DrawCircleLines(centro_play.x, centro_play.y, radio_play, RED); // resaltar
                            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                            {
                                screen = NIVEL2;
                                vidas = 3;
                                bandera_nivel2 = 1;
                                aciertos = 0;
                            }
                        }

                        if (CheckCollisionPointCircle(GetMousePosition(), centro_cerrar, radio_cerrar))
                        {
                            DrawCircleLines(centro_cerrar.x, centro_cerrar.y, radio_cerrar, RED); //  resaltar
                            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                            {
                                screen = MAPA;
                                bandera_nivel2 = 1;
                                aciertos = 0;

                                estructura.aciertos = 0;
                                estructura.modoResta = false;                      // reiniciar modo resta
                                estructura = GenerarNuevaPreguntaSuma(estructura); // o Resta si quieres
                            }
                        }
                        if (CheckCollisionPointCircle(GetMousePosition(), centro_reiniciar, radio_reiniciar))
                        {
                            DrawCircleLines(centro_reiniciar.x, centro_reiniciar.y, radio_reiniciar, RED); // resaltar
                            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                            {
                                bandera_nivel2 = 1;
                                aciertos = 0;
                                vidas = 3;
                                bandera_vidas = 0;

                                estructura.aciertos = 0;
                                estructura.modoResta = false;                      // reiniciar modo resta
                                estructura = GenerarNuevaPreguntaSuma(estructura); // o Resta si quieres
                            }
                        }
                    }
                }
                else
                {
                    float x_pausa = (screenWidth - 777) / 2.0f;
                    float y_pausa = (screenHeight - 321) / 2.0f;
                    DrawTexture(imagen_pausa, x_pausa - 20, y_pausa - 333, WHITE);

                    Vector2 centro_play = {
                        button_play.rectan.x + button_play.rectan.width / 2 - 150,
                        button_play.rectan.y + button_play.rectan.height / 2};
                    float radio_play = button_play.rectan.width / 2;

                    Vector2 centro_cerrar = {
                        button_cerrar.rectan.x + button_cerrar.rectan.width / 2 - 150,
                        button_cerrar.rectan.y + button_cerrar.rectan.height / 2};
                    float radio_cerrar = button_cerrar.rectan.width / 2;

                    DrawTexture(play, button_play.rectan.x - 150, button_play.rectan.y, WHITE);
                    if (CheckCollisionPointCircle(GetMousePosition(), centro_play, radio_play))
                    {
                        DrawCircleLines(centro_play.x, centro_play.y, radio_play, RED); // resaltar
                        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                        {
                            pausa = 0;
                        }
                    }

                    DrawTexture(cerrar, button_cerrar.rectan.x - 150, button_cerrar.rectan.y, WHITE);
                    if (CheckCollisionPointCircle(GetMousePosition(), centro_cerrar, radio_cerrar))
                    {
                        DrawCircleLines(centro_cerrar.x, centro_cerrar.y, radio_cerrar, RED); //  resaltar
                        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                        {
                            screen = MAPA;
                            pausa = 0;
                            aciertos = 0;

                            estructura.aciertos = 0;
                            estructura.modoResta = false;                      // reiniciar modo resta
                            estructura = GenerarNuevaPreguntaSuma(estructura); // o Resta si quieres
                        }
                    }
                }
            }
            else
            {
                DrawTexture(vidas_agotas, (screenWidth - vidas_agotas.width) / 2, (screenHeight - vidas_agotas.height) / 2 - 300, WHITE);
                Vector2 centro_reiniciar = {
                    button_reiniciar.rectan.x + button_reiniciar.rectan.width / 2 - 150,
                    button_reiniciar.rectan.y + button_reiniciar.rectan.height / 2};
                float radio_reiniciar = button_reiniciar.rectan.width / 2;

                Vector2 centro_cerrar = {
                    button_cerrar.rectan.x + button_cerrar.rectan.width / 2 - 150,
                    button_cerrar.rectan.y + button_cerrar.rectan.height / 2};
                float radio_cerrar = button_cerrar.rectan.width / 2;

                DrawTexture(reiniciar, button_reiniciar.rectan.x - 150, button_reiniciar.rectan.y, WHITE);
                if (CheckCollisionPointCircle(GetMousePosition(), centro_reiniciar, radio_reiniciar))
                {
                    DrawCircleLines(centro_reiniciar.x, centro_reiniciar.y, radio_reiniciar, RED); // resaltar
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    {
                        vidas = 3;
                        bandera_vidas = 0;
                        aciertos = 0;

                        estructura.aciertos = 0;
                        estructura.modoResta = false;                      // reiniciar modo resta
                        estructura = GenerarNuevaPreguntaSuma(estructura); // o Resta si quieres
                    }
                }

                DrawTexture(cerrar, button_cerrar.rectan.x - 150, button_cerrar.rectan.y, WHITE);
                if (CheckCollisionPointCircle(GetMousePosition(), centro_cerrar, radio_cerrar))
                {
                    DrawCircleLines(centro_cerrar.x, centro_cerrar.y, radio_cerrar, RED); //  resaltar
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    {
                        screen = MAPA;
                        aciertos = 0;

                        estructura.aciertos = 0;
                        estructura.modoResta = false;                      // reiniciar modo resta
                        estructura = GenerarNuevaPreguntaSuma(estructura); // o Resta si quieres
                    }
                }

                DrawTexture(reiniciar, button_reiniciar.rectan.x - 150, button_reiniciar.rectan.y, WHITE);
                if (CheckCollisionPointCircle(GetMousePosition(), centro_reiniciar, radio_reiniciar))
                {
                    DrawCircleLines(centro_reiniciar.x, centro_reiniciar.y, radio_reiniciar, RED); // resaltar
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    {
                        vidas = 3;
                        aciertos = 0;
                        bandera_vidas = 0;

                        estructura.aciertos = 0;
                        estructura.modoResta = false;                      // reiniciar modo resta
                        estructura = GenerarNuevaPreguntaSuma(estructura); // o Resta si quieres
                    }
                }
            }
            break;
        case NIVEL2:
            DrawTexture(fondo_volcan, 0, 0, WHITE);
            if (vidas > 0)
            {
                char texto_continuar[100];
                sprintf(texto_continuar, "Vidas: %d", vidas);
                Vector2 posicionTexto = {20, 20};
                DrawTextEx(fuente_letra, texto_continuar, posicionTexto, 34, 1, BLACK);
                if (pausa == 0)
                {
                    if (aciertos < 10)
                    {

                        if (!validacion)
                        {
                            // solo se genera una pregunta al iniciar el nivel
                            estructura.aciertos = 0;
                            estructura = GenerarNuevaPreguntaMultiplicacion(estructura);
                            validacion = 1;
                        }

                        if (!estructura.mostrarResultado && !estructura.tiempoAgotado)
                        {
                            estructura.tiempoRestante -= GetFrameTime();
                            if (estructura.tiempoRestante <= 0)
                            {
                                estructura.tiempoRestante = 0;
                                estructura.tiempoAgotado = true;
                                estructura.mostrarResultado = true;
                                estructura.resultadoCorrecto = false;
                            }
                        }

                        if (!estructura.mostrarResultado && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                        {
                            if (CheckCollisionPointRec(mouse, estructura.botonA) && !estructura.botonASeleccionado)
                            {
                                if (!estructura.modoDivision)
                                {
                                    // MODO MULTIPLICACION
                                    if (estructura.clicksHechos == 0)
                                    {
                                        estructura.resultadoJugador += estructura.numA;
                                    }
                                    else
                                    {
                                        estructura.resultadoJugador *= estructura.numA;
                                    }
                                }
                                else
                                {
                                    // modo division
                                    if (estructura.clicksHechos == 0)
                                    {
                                        estructura.resultadoJugador += estructura.numA;
                                    }
                                    else
                                    {
                                        estructura.resultadoJugador /= estructura.numA;
                                    }
                                }

                                estructura.clicksHechos++;
                                estructura.botonASeleccionado = true;
                            }
                            else
                            {
                                if (CheckCollisionPointRec(mouse, estructura.botonB) && !estructura.botonBSeleccionado)
                                {
                                    if (!estructura.modoDivision)
                                    {
                                        // MODO MULTIPLICACION
                                        if (estructura.clicksHechos == 0)
                                        {
                                            estructura.resultadoJugador += estructura.numB;
                                        }
                                        else
                                        {
                                            estructura.resultadoJugador *= estructura.numB;
                                        }
                                    }
                                    else
                                    {
                                        // modo division
                                        if (estructura.clicksHechos == 0)
                                        {
                                            estructura.resultadoJugador += estructura.numB;
                                        }
                                        else
                                        {
                                            estructura.resultadoJugador /= estructura.numB;
                                        }
                                    }

                                    estructura.clicksHechos++;
                                    estructura.botonBSeleccionado = true;
                                }
                                else
                                {
                                    if (CheckCollisionPointRec(mouse, estructura.botonC) && !estructura.botonCSeleccionado)
                                    {
                                        if (!estructura.modoDivision)
                                        {
                                            // MODO MULTIPLICACION
                                            if (estructura.clicksHechos == 0)
                                            {
                                                estructura.resultadoJugador += estructura.numC;
                                            }
                                            else
                                            {
                                                estructura.resultadoJugador *= estructura.numC;
                                            }
                                        }
                                        else
                                        {
                                            // modo division
                                            if (estructura.clicksHechos == 0)
                                            {
                                                estructura.resultadoJugador += estructura.numC;
                                            }
                                            else
                                            {
                                                estructura.resultadoJugador /= estructura.numC;
                                            }
                                        }

                                        estructura.clicksHechos++;
                                        estructura.botonCSeleccionado = true;
                                    }
                                    else
                                    {
                                        if (CheckCollisionPointRec(mouse, estructura.botonD) && !estructura.botonDSeleccionado)
                                        {
                                            if (!estructura.modoDivision)
                                            {
                                                // MODO MULTIPLICACION
                                                if (estructura.clicksHechos == 0)
                                                {
                                                    estructura.resultadoJugador += estructura.numD;
                                                }
                                                else
                                                {
                                                    estructura.resultadoJugador *= estructura.numD;
                                                }
                                            }
                                            else
                                            {
                                                // modo division
                                                if (estructura.clicksHechos == 0)
                                                {
                                                    estructura.resultadoJugador += estructura.numD;
                                                }
                                                else
                                                {
                                                    estructura.resultadoJugador /= estructura.numD;
                                                }
                                            }

                                            estructura.clicksHechos++;
                                            estructura.botonDSeleccionado = true;
                                        }
                                        else
                                        {
                                            if (CheckCollisionPointRec(mouse, estructura.botonE) && !estructura.botonESeleccionado)
                                            {
                                                if (!estructura.modoDivision)
                                                {
                                                    // MODO MULTIPLICACION
                                                    if (estructura.clicksHechos == 0)
                                                    {
                                                        estructura.resultadoJugador += estructura.numE;
                                                    }
                                                    else
                                                    {
                                                        estructura.resultadoJugador *= estructura.numE;
                                                    }
                                                }
                                                else
                                                {
                                                    // modo division
                                                    if (estructura.clicksHechos == 0)
                                                    {
                                                        estructura.resultadoJugador += estructura.numE;
                                                    }
                                                    else
                                                    {
                                                        estructura.resultadoJugador /= estructura.numE;
                                                    }
                                                }

                                                estructura.clicksHechos++;
                                                estructura.botonESeleccionado = true;
                                            }
                                            else
                                            {
                                                if (CheckCollisionPointRec(mouse, estructura.botonF) && !estructura.botonFSeleccionado)
                                                {
                                                    if (!estructura.modoDivision)
                                                    {
                                                        // MODO MULTIPLICACION
                                                        if (estructura.clicksHechos == 0)
                                                        {
                                                            estructura.resultadoJugador += estructura.numF;
                                                        }
                                                        else
                                                        {
                                                            estructura.resultadoJugador *= estructura.numF;
                                                        }
                                                    }
                                                    else
                                                    {
                                                        // modo division
                                                        if (estructura.clicksHechos == 0)
                                                        {
                                                            estructura.resultadoJugador += estructura.numF;
                                                        }
                                                        else
                                                        {
                                                            estructura.resultadoJugador /= estructura.numF;
                                                        }
                                                    }
                                                    estructura.clicksHechos++;
                                                    estructura.botonFSeleccionado = true;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }

                        if (estructura.clicksHechos == 2 && !estructura.mostrarResultado)
                        {
                            if (estructura.resultadoJugador < 0)
                            {
                                estructura.resultadoJugador *= -1;
                            }

                            estructura.resultadoCorrecto = (estructura.resultadoJugador == estructura.objetivo);
                            estructura.mostrarResultado = true;

                            if (estructura.resultadoCorrecto)
                            {
                                estructura.aciertos++;
                                aciertos++;

                                if (estructura.aciertos >= 5 && !estructura.modoDivision)
                                {
                                    estructura.modoDivision = true;
                                    estructura.aciertos = 0; // reinicia para poder usarlo también con futuras validaciones
                                }
                            }
                        }

                        if (!estructura.modoDivision)
                        {
                            Vector2 posicion = {920, 150};
                            char texto[100];
                            sprintf(texto, "Multiplica dos botones para llegar a: %d", estructura.objetivo);
                            DrawTextEx(fuente_letra, texto, posicion, 50, 1, BLACK);
                        }
                        else
                        {
                            Vector2 posicion = {920, 150};
                            char texto[100];
                            sprintf(texto, "Divide dos botones para llegar a: %d", estructura.objetivo);
                            DrawTextEx(fuente_letra, texto, posicion, 50, 1, BLACK);
                        }

                        {
                            int fontSize = 30;
                            float spacing = 1;

                            // Botón A
                            DrawRectangleRec(estructura.botonA, estructura.botonASeleccionado ? SKYBLUE : LIGHTGRAY);
                            char textoA[10];
                            sprintf(textoA, "%d", estructura.numA);
                            Vector2 medidaTextoA = MeasureTextEx(fuente_letra, textoA, fontSize, spacing);
                            Vector2 posicionTextoA = {
                                estructura.botonA.x + (estructura.botonA.width - medidaTextoA.x) / 2,
                                estructura.botonA.y + (estructura.botonA.height - medidaTextoA.y) / 2};
                            DrawTextEx(fuente_letra, textoA, posicionTextoA, fontSize, spacing, BLACK);

                            // Botón B
                            DrawRectangleRec(estructura.botonB, estructura.botonBSeleccionado ? SKYBLUE : LIGHTGRAY);
                            char textoB[10];
                            sprintf(textoB, "%d", estructura.numB);
                            Vector2 medidaTextoB = MeasureTextEx(fuente_letra, textoB, fontSize, spacing);
                            Vector2 posicionTextoB = {
                                estructura.botonB.x + (estructura.botonB.width - medidaTextoB.x) / 2,
                                estructura.botonB.y + (estructura.botonB.height - medidaTextoB.y) / 2};
                            DrawTextEx(fuente_letra, textoB, posicionTextoB, fontSize, spacing, BLACK);

                            // Botón C
                            DrawRectangleRec(estructura.botonC, estructura.botonCSeleccionado ? SKYBLUE : LIGHTGRAY);
                            char textoC[10];
                            sprintf(textoC, "%d", estructura.numC);
                            Vector2 medidaTextoC = MeasureTextEx(fuente_letra, textoC, fontSize, spacing);
                            Vector2 posicionTextoC = {
                                estructura.botonC.x + (estructura.botonC.width - medidaTextoC.x) / 2,
                                estructura.botonC.y + (estructura.botonC.height - medidaTextoC.y) / 2};
                            DrawTextEx(fuente_letra, textoC, posicionTextoC, fontSize, spacing, BLACK);

                            // Botón D
                            DrawRectangleRec(estructura.botonD, estructura.botonDSeleccionado ? SKYBLUE : LIGHTGRAY);
                            char textoD[10];
                            sprintf(textoD, "%d", estructura.numD);
                            Vector2 medidaTextoD = MeasureTextEx(fuente_letra, textoD, fontSize, spacing);
                            Vector2 posicionTextoD = {
                                estructura.botonD.x + (estructura.botonD.width - medidaTextoD.x) / 2,
                                estructura.botonD.y + (estructura.botonD.height - medidaTextoD.y) / 2};
                            DrawTextEx(fuente_letra, textoD, posicionTextoD, fontSize, spacing, BLACK);

                            // Botón E
                            DrawRectangleRec(estructura.botonE, estructura.botonESeleccionado ? SKYBLUE : LIGHTGRAY);
                            char textoE[10];
                            sprintf(textoE, "%d", estructura.numE);
                            Vector2 medidaTextoE = MeasureTextEx(fuente_letra, textoE, fontSize, spacing);
                            Vector2 posicionTextoE = {
                                estructura.botonE.x + (estructura.botonE.width - medidaTextoE.x) / 2,
                                estructura.botonE.y + (estructura.botonE.height - medidaTextoE.y) / 2};
                            DrawTextEx(fuente_letra, textoE, posicionTextoE, fontSize, spacing, BLACK);

                            // Botón F
                            DrawRectangleRec(estructura.botonF, estructura.botonFSeleccionado ? SKYBLUE : LIGHTGRAY);
                            char textoF[10];
                            sprintf(textoF, "%d", estructura.numF);
                            Vector2 medidaTextoF = MeasureTextEx(fuente_letra, textoF, fontSize, spacing);
                            Vector2 posicionTextoF = {
                                estructura.botonF.x + (estructura.botonF.width - medidaTextoF.x) / 2,
                                estructura.botonF.y + (estructura.botonF.height - medidaTextoF.y) / 2};
                            DrawTextEx(fuente_letra, textoF, posicionTextoF, fontSize, spacing, BLACK);
                        };

                        // boton continuar
                        if (estructura.mostrarResultado) // Esto no qeuda todavia
                        {
                            DrawRectangleRec(botonContinuar, LIGHTGRAY);
                            char texto_continuar[100] = "Continuar";
                            Vector2 posicionTexto = {
                                botonContinuar.x + 40,
                                botonContinuar.y + 20};

                            DrawTextEx(fuente_letra, texto_continuar, posicionTexto, 34, 1, BLACK);
                        }

                        if (estructura.mostrarResultado)
                        {
                            if (estructura.resultadoCorrecto)
                            {
                                DrawRectangleRec(botonContinuar, LIGHTGRAY);
                                char texto_continuar[100] = "Continuar";
                                Vector2 posicionTexto = {
                                    botonContinuar.x + 40,
                                    botonContinuar.y + 20};

                                DrawTextEx(fuente_letra, texto_continuar, posicionTexto, 34, 1, BLACK);
                                Vector2 posicionTexto2 = {
                                    screenWidth / 2 - 800,
                                    screenHeight / 2};
                                DrawTextEx(fuente_letra, "Correcto, presione continuar :)", posicionTexto2, 34, 1, GREEN);
                            }
                            else
                            {
                                Vector2 posicionTexto = {
                                    screenWidth / 2 - 800,
                                    screenHeight / 2};

                                if (estructura.tiempoAgotado)
                                {
                                    DrawTextEx(fuente_letra, "Se acabo el tiempo", posicionTexto, 34, 1, RED);
                                }
                                else
                                {
                                    DrawTextEx(fuente_letra, "Incorrecto :( Presiona para intentarlo de nuevo", posicionTexto, 34, 1, RED);
                                }

                                if (bandera_vidas == 0)
                                {
                                    vidas--;
                                }
                                bandera_vidas = 1;
                            }
                        }

                        if (estructura.mostrarResultado && CheckCollisionPointRec(mouse, botonContinuar))
                        {
                            if (estructura.mostrarResultado && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                            {
                                bandera_vidas = 0;
                                if (estructura.modoDivision)
                                {
                                    estructura = GenerarNuevaPreguntaDivision(estructura);
                                }
                                else
                                {
                                    estructura = GenerarNuevaPreguntaMultiplicacion(estructura);
                                }
                            }
                        }

                        // barra de tiempo
                        float barraMaxAncho = 600;
                        float barraAncho = (estructura.tiempoRestante / estructura.tiempoMaximo) * barraMaxAncho;

                        DrawRectangle(160, 700, barraMaxAncho, 20, GRAY);       // fondo
                        DrawRectangle(160, 700, barraAncho, 20, GREEN);         // tiempo restante
                        DrawRectangleLines(160, 700, barraMaxAncho, 20, BLACK); // borde

                        if (IsKeyPressed(KEY_ESCAPE))
                        {
                            pausa = 1;
                        }
                    }
                    else
                    {

                        Vector2 centra_imagen = {
                            (screenWidth - imagen_nivel_superado.width) / 2.0f,
                            (screenHeight - imagen_nivel_superado.height) / 2.0f};
                        DrawTexture(imagen_nivel_superado, centra_imagen.x, centra_imagen.y - 300, WHITE);

                        DrawTexture(play, button_play.rectan.x, button_play.rectan.y, WHITE);
                        Vector2 centro_play = {
                            button_play.rectan.x + button_play.rectan.width / 2,
                            button_play.rectan.y + button_play.rectan.height / 2};
                        float radio_play = button_play.rectan.width / 2;

                        DrawTexture(cerrar, button_cerrar.rectan.x, button_cerrar.rectan.y, WHITE);
                        Vector2 centro_cerrar = {
                            button_cerrar.rectan.x + button_cerrar.rectan.width / 2,
                            button_cerrar.rectan.y + button_cerrar.rectan.height / 2};
                        float radio_cerrar = button_cerrar.rectan.width / 2;

                        float reiniciar_x = button_reiniciar.rectan.x - 400;
                        DrawTexture(reiniciar, reiniciar_x, button_reiniciar.rectan.y, WHITE);

                        Vector2 centro_reiniciar = {
                            reiniciar_x + button_reiniciar.rectan.width / 2,
                            button_reiniciar.rectan.y + button_reiniciar.rectan.height / 2};
                        float radio_reiniciar = button_reiniciar.rectan.width / 2;

                        if (CheckCollisionPointCircle(GetMousePosition(), centro_play, radio_play))
                        {
                            DrawCircleLines(centro_play.x, centro_play.y, radio_play, RED); // resaltar
                            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                            {
                                screen = NIVE3;
                                bandera_nivel3 = 1;
                                vidas = 3;
                                aciertos = 0;
                            }
                        }

                        if (CheckCollisionPointCircle(GetMousePosition(), centro_cerrar, radio_cerrar))
                        {
                            DrawCircleLines(centro_cerrar.x, centro_cerrar.y, radio_cerrar, RED); //  resaltar
                            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                            {
                                screen = MAPA;
                                aciertos = 0;
                                vidas = 3;

                                bandera_nivel3 = 1;

                                estructura.aciertos = 0;         // reiniciar aciertos
                                validacion = 0;                  // reiniciar validacion para que se genere una nueva pregunta
                                estructura.modoDivision = false; // reiniciar modo division
                            }
                        }
                        if (CheckCollisionPointCircle(GetMousePosition(), centro_reiniciar, radio_reiniciar))
                        {
                            DrawCircleLines(centro_reiniciar.x, centro_reiniciar.y, radio_reiniciar, RED); // resaltar
                            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                            {
                                bandera_nivel3 = 1;
                                aciertos = 0;

                                vidas = 3;
                                bandera_vidas = 0;

                                validacion = 0;                  // reiniciar validacion para que se genere una nueva pregunta
                                estructura.aciertos = 0;         // reiniciar aciertos
                                estructura.modoDivision = false; // reiniciar modo division
                            }
                        }
                    }
                }
                else
                {
                    float x_pausa = (screenWidth - 777) / 2.0f;
                    float y_pausa = (screenHeight - 321) / 2.0f;
                    DrawTexture(imagen_pausa, x_pausa - 20, y_pausa - 333, WHITE);

                    Vector2 centro_play = {
                        button_play.rectan.x + button_play.rectan.width / 2 - 150,
                        button_play.rectan.y + button_play.rectan.height / 2};
                    float radio_play = button_play.rectan.width / 2;

                    Vector2 centro_cerrar = {
                        button_cerrar.rectan.x + button_cerrar.rectan.width / 2 - 150,
                        button_cerrar.rectan.y + button_cerrar.rectan.height / 2};
                    float radio_cerrar = button_cerrar.rectan.width / 2;

                    DrawTexture(play, button_play.rectan.x - 150, button_play.rectan.y, WHITE);
                    if (CheckCollisionPointCircle(GetMousePosition(), centro_play, radio_play))
                    {
                        DrawCircleLines(centro_play.x, centro_play.y, radio_play, RED); // resaltar
                        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                        {
                            pausa = 0;
                        }
                    }

                    DrawTexture(cerrar, button_cerrar.rectan.x - 150, button_cerrar.rectan.y, WHITE);
                    if (CheckCollisionPointCircle(GetMousePosition(), centro_cerrar, radio_cerrar))
                    {
                        DrawCircleLines(centro_cerrar.x, centro_cerrar.y, radio_cerrar, RED); //  resaltar
                        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                        {
                            screen = MAPA;
                            aciertos = 0;
                            pausa = 0;

                            estructura.aciertos = 0;
                            validacion = 0;
                            estructura.modoDivision = false; // reiniciar modo division
                        }
                    }
                }
            }
            else
            {
                DrawTexture(vidas_agotas, (screenWidth - vidas_agotas.width) / 2, (screenHeight - vidas_agotas.height) / 2 - 300, WHITE);
                Vector2 centro_reiniciar = {
                    button_reiniciar.rectan.x + button_reiniciar.rectan.width / 2 - 150,
                    button_reiniciar.rectan.y + button_reiniciar.rectan.height / 2};
                float radio_reiniciar = button_reiniciar.rectan.width / 2;

                Vector2 centro_cerrar = {
                    button_cerrar.rectan.x + button_cerrar.rectan.width / 2 - 150,
                    button_cerrar.rectan.y + button_cerrar.rectan.height / 2};
                float radio_cerrar = button_cerrar.rectan.width / 2;

                DrawTexture(reiniciar, button_reiniciar.rectan.x - 150, button_reiniciar.rectan.y, WHITE);
                if (CheckCollisionPointCircle(GetMousePosition(), centro_reiniciar, radio_reiniciar))
                {
                    DrawCircleLines(centro_reiniciar.x, centro_reiniciar.y, radio_reiniciar, RED); // resaltar
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    {
                        vidas = 3;
                        aciertos = 0;
                        bandera_vidas = 0;

                        estructura.aciertos = 0;
                        estructura.modoDivision = false; // reiniciar modo division
                        validacion = 0;                  // reiniciar validacion para que se genere una nueva pregunta
                    }
                }

                DrawTexture(cerrar, button_cerrar.rectan.x - 150, button_cerrar.rectan.y, WHITE);
                if (CheckCollisionPointCircle(GetMousePosition(), centro_cerrar, radio_cerrar))
                {
                    DrawCircleLines(centro_cerrar.x, centro_cerrar.y, radio_cerrar, RED); //  resaltar
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    {
                        screen = MAPA;
                        aciertos = 0;

                        estructura.aciertos = 0;
                        estructura.modoDivision = false; // reiniciar modo division
                        validacion = 0;                  // reiniciar validacion para que se genere una nueva pregunta
                    }
                }
            }
            break;

        case NIVE3:
            DrawTexture(fondo_final, 0, 0, WHITE);
            if (vidas > 0)
            {
                char texto_continuar[100];
                sprintf(texto_continuar, "Vidas: %d", vidas);
                Vector2 posicionTexto = {20, 20};
                DrawTextEx(fuente_letra, texto_continuar, posicionTexto, 34, 1, BLACK);
                if (pausa == 0)
                {
                    if (aciertos < 10)
                    {
                        if (!validacionNivel3)
                        {
                            estructura.modoResta = false;
                            estructura.modoDivision = false;
                            estructura.modoMultiplicacion = false;

                            int operacion = rand() % 4;

                            if (operacion == 0)
                            {
                                estructura = GenerarNuevaPreguntaSuma(estructura);
                            }
                            else
                            {
                                if (operacion == 1)
                                {
                                    estructura = GenerarNuevaPreguntaResta(estructura);
                                    estructura.modoResta = true;
                                }
                                else
                                {
                                    if (operacion == 2)
                                    {
                                        estructura = GenerarNuevaPreguntaMultiplicacion(estructura);
                                        estructura.modoMultiplicacion = true;
                                    }
                                    else
                                    {
                                        estructura = GenerarNuevaPreguntaDivision(estructura);
                                        estructura.modoDivision = true;
                                    }
                                }
                            }

                            validacionNivel3 = 1;
                        }

                        if (!estructura.mostrarResultado && !estructura.tiempoAgotado)
                        {
                            estructura.tiempoRestante -= GetFrameTime();
                            if (estructura.tiempoRestante <= 0)
                            {
                                estructura.tiempoRestante = 0;
                                estructura.tiempoAgotado = true;
                                estructura.mostrarResultado = true;
                                estructura.resultadoCorrecto = false;
                            }
                        }

                        if (!estructura.mostrarResultado && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                        {
                            if (CheckCollisionPointRec(mouse, estructura.botonA) && !estructura.botonASeleccionado)
                            {
                                if (estructura.modoResta)
                                {
                                    // modo resta
                                    if (estructura.clicksHechos == 0)
                                    {
                                        estructura.resultadoJugador += estructura.numA;
                                    }
                                    else
                                    {
                                        estructura.resultadoJugador -= estructura.numA;
                                    }
                                }
                                else
                                {
                                    if (estructura.modoMultiplicacion)
                                    {
                                        // MODO MULTIPLICACION
                                        if (estructura.clicksHechos == 0)
                                        {
                                            estructura.resultadoJugador += estructura.numA;
                                        }
                                        else
                                        {
                                            estructura.resultadoJugador *= estructura.numA;
                                        }
                                    }
                                    else
                                    {
                                        if (estructura.modoDivision)
                                        {
                                            // modo division
                                            if (estructura.clicksHechos == 0)
                                            {
                                                estructura.resultadoJugador += estructura.numA;
                                            }
                                            else
                                            {
                                                estructura.resultadoJugador /= estructura.numA;
                                            }
                                        }
                                        else
                                        {
                                            estructura.resultadoJugador += estructura.numA;
                                        }
                                    }
                                }

                                estructura.clicksHechos++;
                                estructura.botonASeleccionado = true;
                            }
                            else
                            {
                                if (CheckCollisionPointRec(mouse, estructura.botonB) && !estructura.botonBSeleccionado)
                                {
                                    if (estructura.modoResta)
                                    {
                                        // modo resta
                                        if (estructura.clicksHechos == 0)
                                        {
                                            estructura.resultadoJugador += estructura.numB;
                                        }
                                        else
                                        {
                                            estructura.resultadoJugador -= estructura.numB;
                                        }
                                    }
                                    else
                                    {
                                        if (estructura.modoMultiplicacion)
                                        {
                                            // MODO MULTIPLICACION
                                            if (estructura.clicksHechos == 0)
                                            {
                                                estructura.resultadoJugador += estructura.numB;
                                            }
                                            else
                                            {
                                                estructura.resultadoJugador *= estructura.numB;
                                            }
                                        }
                                        else
                                        {
                                            if (estructura.modoDivision)
                                            {
                                                // modo division
                                                if (estructura.clicksHechos == 0)
                                                {
                                                    estructura.resultadoJugador += estructura.numB;
                                                }
                                                else
                                                {
                                                    estructura.resultadoJugador /= estructura.numB;
                                                }
                                            }
                                            else
                                            {
                                                estructura.resultadoJugador += estructura.numB;
                                            }
                                        }
                                    }

                                    estructura.clicksHechos++;
                                    estructura.botonBSeleccionado = true;
                                }
                                else
                                {
                                    if (CheckCollisionPointRec(mouse, estructura.botonC) && !estructura.botonCSeleccionado)
                                    {
                                        if (estructura.modoResta)
                                        {
                                            // modo resta
                                            if (estructura.clicksHechos == 0)
                                            {
                                                estructura.resultadoJugador += estructura.numC;
                                            }
                                            else
                                            {
                                                estructura.resultadoJugador -= estructura.numC;
                                            }
                                        }
                                        else
                                        {
                                            if (estructura.modoMultiplicacion)
                                            {
                                                // MODO MULTIPLICACION
                                                if (estructura.clicksHechos == 0)
                                                {
                                                    estructura.resultadoJugador += estructura.numC;
                                                }
                                                else
                                                {
                                                    estructura.resultadoJugador *= estructura.numC;
                                                }
                                            }
                                            else
                                            {
                                                if (estructura.modoDivision)
                                                {
                                                    // modo division
                                                    if (estructura.clicksHechos == 0)
                                                    {
                                                        estructura.resultadoJugador += estructura.numC;
                                                    }
                                                    else
                                                    {
                                                        estructura.resultadoJugador /= estructura.numC;
                                                    }
                                                }
                                                else
                                                {
                                                    estructura.resultadoJugador += estructura.numC;
                                                }
                                            }
                                        }

                                        estructura.clicksHechos++;
                                        estructura.botonCSeleccionado = true;
                                    }
                                    else
                                    {
                                        if (CheckCollisionPointRec(mouse, estructura.botonD) && !estructura.botonDSeleccionado)
                                        {
                                            if (estructura.modoResta)
                                            {
                                                // modo resta
                                                if (estructura.clicksHechos == 0)
                                                {
                                                    estructura.resultadoJugador += estructura.numD;
                                                }
                                                else
                                                {
                                                    estructura.resultadoJugador -= estructura.numD;
                                                }
                                            }
                                            else
                                            {
                                                if (estructura.modoMultiplicacion)
                                                {
                                                    // MODO MULTIPLICACION
                                                    if (estructura.clicksHechos == 0)
                                                    {
                                                        estructura.resultadoJugador += estructura.numD;
                                                    }
                                                    else
                                                    {
                                                        estructura.resultadoJugador *= estructura.numD;
                                                    }
                                                }
                                                else
                                                {
                                                    if (estructura.modoDivision)
                                                    {
                                                        // modo division
                                                        if (estructura.clicksHechos == 0)
                                                        {
                                                            estructura.resultadoJugador += estructura.numD;
                                                        }
                                                        else
                                                        {
                                                            estructura.resultadoJugador /= estructura.numD;
                                                        }
                                                    }
                                                    else
                                                    {
                                                        estructura.resultadoJugador += estructura.numD;
                                                    }
                                                }
                                            }

                                            estructura.clicksHechos++;
                                            estructura.botonDSeleccionado = true;
                                        }
                                        else
                                        {
                                            if (CheckCollisionPointRec(mouse, estructura.botonE) && !estructura.botonESeleccionado)
                                            {
                                                if (estructura.modoResta)
                                                {
                                                    // modo resta
                                                    if (estructura.clicksHechos == 0)
                                                    {
                                                        estructura.resultadoJugador += estructura.numE;
                                                    }
                                                    else
                                                    {
                                                        estructura.resultadoJugador -= estructura.numE;
                                                    }
                                                }
                                                else
                                                {
                                                    if (estructura.modoMultiplicacion)
                                                    {
                                                        // MODO MULTIPLICACION
                                                        if (estructura.clicksHechos == 0)
                                                        {
                                                            estructura.resultadoJugador += estructura.numE;
                                                        }
                                                        else
                                                        {
                                                            estructura.resultadoJugador *= estructura.numE;
                                                        }
                                                    }
                                                    else
                                                    {
                                                        if (estructura.modoDivision)
                                                        {
                                                            // modo division
                                                            if (estructura.clicksHechos == 0)
                                                            {
                                                                estructura.resultadoJugador += estructura.numE;
                                                            }
                                                            else
                                                            {
                                                                estructura.resultadoJugador /= estructura.numE;
                                                            }
                                                        }
                                                        else
                                                        {
                                                            estructura.resultadoJugador += estructura.numE;
                                                        }
                                                    }
                                                }

                                                estructura.clicksHechos++;
                                                estructura.botonESeleccionado = true;
                                            }
                                            else
                                            {
                                                if (CheckCollisionPointRec(mouse, estructura.botonF) && !estructura.botonFSeleccionado)
                                                {
                                                    if (estructura.modoResta)
                                                    {
                                                        // modo resta
                                                        if (estructura.clicksHechos == 0)
                                                        {
                                                            estructura.resultadoJugador += estructura.numF;
                                                        }
                                                        else
                                                        {
                                                            estructura.resultadoJugador -= estructura.numF;
                                                        }
                                                    }
                                                    else
                                                    {
                                                        if (estructura.modoMultiplicacion)
                                                        {
                                                            // MODO MULTIPLICACION
                                                            if (estructura.clicksHechos == 0)
                                                            {
                                                                estructura.resultadoJugador += estructura.numF;
                                                            }
                                                            else
                                                            {
                                                                estructura.resultadoJugador *= estructura.numF;
                                                            }
                                                        }
                                                        else
                                                        {
                                                            if (estructura.modoDivision)
                                                            {
                                                                // modo division
                                                                if (estructura.clicksHechos == 0)
                                                                {
                                                                    estructura.resultadoJugador += estructura.numF;
                                                                }
                                                                else
                                                                {
                                                                    estructura.resultadoJugador /= estructura.numF;
                                                                }
                                                            }
                                                            else
                                                            {
                                                                estructura.resultadoJugador += estructura.numF;
                                                            }
                                                        }
                                                    }

                                                    estructura.clicksHechos++;
                                                    estructura.botonFSeleccionado = true;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }

                        if (estructura.clicksHechos == 2 && !estructura.mostrarResultado)
                        {
                            if (estructura.resultadoJugador < 0)
                            {
                                estructura.resultadoJugador *= -1;
                            }

                            estructura.resultadoCorrecto = (estructura.resultadoJugador == estructura.objetivo);
                            estructura.mostrarResultado = true;

                            if (estructura.resultadoCorrecto)
                            {
                                estructura.aciertos++;
                                aciertos++;
                            }
                        }

                        if (estructura.modoResta)
                        {
                            Vector2 posicion = {1050, 150};
                            char texto[100];
                            sprintf(texto, "Resta dos botones para llegar a: %d", estructura.objetivo);
                            DrawTextEx(fuente_letra, texto, posicion, 50, 1, BLACK);
                        }
                        else
                        {
                            if (estructura.modoMultiplicacion)
                            {
                                Vector2 posicion = {920, 150};
                                char texto[100];
                                sprintf(texto, "Multiplica dos botones para llegar a: %d", estructura.objetivo);
                                DrawTextEx(fuente_letra, texto, posicion, 50, 1, BLACK);
                            }
                            else
                            {
                                if (estructura.modoDivision)
                                {
                                    Vector2 posicion = {920, 150};
                                    char texto[100];
                                    sprintf(texto, "Divide dos botones para llegar a: %d", estructura.objetivo);
                                    DrawTextEx(fuente_letra, texto, posicion, 50, 1, BLACK);
                                }
                                else
                                {
                                    Vector2 posicion = {1050, 150};
                                    char texto[100];
                                    sprintf(texto, "Suma dos botones para llegar a: %d", estructura.objetivo);
                                    DrawTextEx(fuente_letra, texto, posicion, 50, 1, BLACK);
                                }
                            }
                        }

                        {
                            int fontSize = 30;
                            float spacing = 1;

                            // Botón A
                            DrawRectangleRec(estructura.botonA, estructura.botonASeleccionado ? SKYBLUE : LIGHTGRAY);
                            char textoA[10];
                            sprintf(textoA, "%d", estructura.numA);
                            Vector2 medidaTextoA = MeasureTextEx(fuente_letra, textoA, fontSize, spacing);
                            Vector2 posicionTextoA = {
                                estructura.botonA.x + (estructura.botonA.width - medidaTextoA.x) / 2,
                                estructura.botonA.y + (estructura.botonA.height - medidaTextoA.y) / 2};
                            DrawTextEx(fuente_letra, textoA, posicionTextoA, fontSize, spacing, BLACK);

                            // Botón B
                            DrawRectangleRec(estructura.botonB, estructura.botonBSeleccionado ? SKYBLUE : LIGHTGRAY);
                            char textoB[10];
                            sprintf(textoB, "%d", estructura.numB);
                            Vector2 medidaTextoB = MeasureTextEx(fuente_letra, textoB, fontSize, spacing);
                            Vector2 posicionTextoB = {
                                estructura.botonB.x + (estructura.botonB.width - medidaTextoB.x) / 2,
                                estructura.botonB.y + (estructura.botonB.height - medidaTextoB.y) / 2};
                            DrawTextEx(fuente_letra, textoB, posicionTextoB, fontSize, spacing, BLACK);

                            // Botón C
                            DrawRectangleRec(estructura.botonC, estructura.botonCSeleccionado ? SKYBLUE : LIGHTGRAY);
                            char textoC[10];
                            sprintf(textoC, "%d", estructura.numC);
                            Vector2 medidaTextoC = MeasureTextEx(fuente_letra, textoC, fontSize, spacing);
                            Vector2 posicionTextoC = {
                                estructura.botonC.x + (estructura.botonC.width - medidaTextoC.x) / 2,
                                estructura.botonC.y + (estructura.botonC.height - medidaTextoC.y) / 2};
                            DrawTextEx(fuente_letra, textoC, posicionTextoC, fontSize, spacing, BLACK);

                            // Botón D
                            DrawRectangleRec(estructura.botonD, estructura.botonDSeleccionado ? SKYBLUE : LIGHTGRAY);
                            char textoD[10];
                            sprintf(textoD, "%d", estructura.numD);
                            Vector2 medidaTextoD = MeasureTextEx(fuente_letra, textoD, fontSize, spacing);
                            Vector2 posicionTextoD = {
                                estructura.botonD.x + (estructura.botonD.width - medidaTextoD.x) / 2,
                                estructura.botonD.y + (estructura.botonD.height - medidaTextoD.y) / 2};
                            DrawTextEx(fuente_letra, textoD, posicionTextoD, fontSize, spacing, BLACK);

                            // Botón E
                            DrawRectangleRec(estructura.botonE, estructura.botonESeleccionado ? SKYBLUE : LIGHTGRAY);
                            char textoE[10];
                            sprintf(textoE, "%d", estructura.numE);
                            Vector2 medidaTextoE = MeasureTextEx(fuente_letra, textoE, fontSize, spacing);
                            Vector2 posicionTextoE = {
                                estructura.botonE.x + (estructura.botonE.width - medidaTextoE.x) / 2,
                                estructura.botonE.y + (estructura.botonE.height - medidaTextoE.y) / 2};
                            DrawTextEx(fuente_letra, textoE, posicionTextoE, fontSize, spacing, BLACK);

                            // Botón F
                            DrawRectangleRec(estructura.botonF, estructura.botonFSeleccionado ? SKYBLUE : LIGHTGRAY);
                            char textoF[10];
                            sprintf(textoF, "%d", estructura.numF);
                            Vector2 medidaTextoF = MeasureTextEx(fuente_letra, textoF, fontSize, spacing);
                            Vector2 posicionTextoF = {
                                estructura.botonF.x + (estructura.botonF.width - medidaTextoF.x) / 2,
                                estructura.botonF.y + (estructura.botonF.height - medidaTextoF.y) / 2};
                            DrawTextEx(fuente_letra, textoF, posicionTextoF, fontSize, spacing, BLACK);
                        };

                        // boton continuar
                        if (estructura.mostrarResultado) // Esto no qeuda todavia
                        {
                            DrawRectangleRec(botonContinuar, LIGHTGRAY);
                            char texto_continuar[100] = "Continuar";
                            Vector2 posicionTexto = {
                                botonContinuar.x + 40,
                                botonContinuar.y + 20};

                            DrawTextEx(fuente_letra, texto_continuar, posicionTexto, 34, 1, BLACK);
                        }

                        if (estructura.mostrarResultado)
                        {
                            if (estructura.resultadoCorrecto)
                            {
                                DrawRectangleRec(botonContinuar, LIGHTGRAY);
                                char texto_continuar[100] = "Continuar";
                                Vector2 posicionTexto = {
                                    botonContinuar.x + 40,
                                    botonContinuar.y + 20};

                                DrawTextEx(fuente_letra, texto_continuar, posicionTexto, 34, 1, BLACK);
                                Vector2 posicionTexto2 = {
                                    screenWidth / 2 - 800,
                                    screenHeight / 2};
                                DrawTextEx(fuente_letra, "Correcto, presione continuar :)", posicionTexto2, 34, 1, GREEN);
                            }
                            else
                            {
                                Vector2 posicionTexto = {
                                    screenWidth / 2 - 800,
                                    screenHeight / 2};

                                if (estructura.tiempoAgotado)
                                {
                                    DrawTextEx(fuente_letra, "Se acabo el tiempo", posicionTexto, 34, 1, RED);
                                }
                                else
                                {
                                    DrawTextEx(fuente_letra, "Incorrecto :( Presiona para intentarlo de nuevo", posicionTexto, 34, 1, RED);
                                }

                                if (bandera_vidas == 0)
                                {
                                    vidas--;
                                }
                                bandera_vidas = 1;
                            }
                        }

                        if (estructura.mostrarResultado && CheckCollisionPointRec(mouse, botonContinuar))
                        {

                            if (estructura.mostrarResultado && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                            {
                                bandera_vidas = 0;
                                validacionNivel3 = 0;
                            }
                        }

                        // barra de tiempo
                        float barraMaxAncho = 600;
                        float barraAncho = (estructura.tiempoRestante / estructura.tiempoMaximo) * barraMaxAncho;

                        DrawRectangle(160, 700, barraMaxAncho, 20, GRAY);       // fondo
                        DrawRectangle(160, 700, barraAncho, 20, GREEN);         // tiempo restante
                        DrawRectangleLines(160, 700, barraMaxAncho, 20, BLACK); // borde

                        if (IsKeyPressed(KEY_ESCAPE))
                        {
                            pausa = 1;
                        }
                    }
                    else
                    {

                        Vector2 centra_imagen = {
                            (screenWidth - imagen_nivel_superado.width) / 2.0f,
                            (screenHeight - imagen_nivel_superado.height) / 2.0f};
                        DrawTexture(imagen_nivel_superado, centra_imagen.x, centra_imagen.y - 300, WHITE);

                        DrawTexture(cerrar, button_cerrar.rectan.x, button_cerrar.rectan.y, WHITE);
                        Vector2 centro_cerrar = {
                            button_cerrar.rectan.x + button_cerrar.rectan.width / 2,
                            button_cerrar.rectan.y + button_cerrar.rectan.height / 2};
                        float radio_cerrar = button_cerrar.rectan.width / 2;

                        float reiniciar_x = button_reiniciar.rectan.x - 400;
                        DrawTexture(reiniciar, reiniciar_x, button_reiniciar.rectan.y, WHITE);

                        Vector2 centro_reiniciar = {
                            reiniciar_x + button_reiniciar.rectan.width / 2,
                            button_reiniciar.rectan.y + button_reiniciar.rectan.height / 2};
                        float radio_reiniciar = button_reiniciar.rectan.width / 2;

                        if (CheckCollisionPointCircle(GetMousePosition(), centro_cerrar, radio_cerrar))
                        {
                            DrawCircleLines(centro_cerrar.x, centro_cerrar.y, radio_cerrar, RED); //  resaltar
                            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                            {
                                screen = MAPA;
                                aciertos = 0;

                                bandera_nivel2 = 1;

                                estructura.aciertos = 0; // reiniciar aciertos
                                validacionNivel3 = 0;    // reiniciar validacion nivel 3
                            }
                        }
                        if (CheckCollisionPointCircle(GetMousePosition(), centro_reiniciar, radio_reiniciar))
                        {
                            DrawCircleLines(centro_reiniciar.x, centro_reiniciar.y, radio_reiniciar, RED); // resaltar
                            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                            {
                                bandera_nivel2 = 1;
                                vidas = 3;
                                bandera_vidas = 0;
                                aciertos = 0;

                                estructura.aciertos = 0; // reiniciar aciertos
                                validacionNivel3 = 0;    // reiniciar validacion nivel 3
                            }
                        }
                    }
                }
                else
                {
                    float x_pausa = (screenWidth - 777) / 2.0f;
                    float y_pausa = (screenHeight - 321) / 2.0f;
                    DrawTexture(imagen_pausa, x_pausa - 20, y_pausa - 333, WHITE);

                    Vector2 centro_play = {
                        button_play.rectan.x + button_play.rectan.width / 2 - 150,
                        button_play.rectan.y + button_play.rectan.height / 2};
                    float radio_play = button_play.rectan.width / 2;

                    Vector2 centro_cerrar = {
                        button_cerrar.rectan.x + button_cerrar.rectan.width / 2 - 150,
                        button_cerrar.rectan.y + button_cerrar.rectan.height / 2};
                    float radio_cerrar = button_cerrar.rectan.width / 2;

                    DrawTexture(play, button_play.rectan.x - 150, button_play.rectan.y, WHITE);
                    if (CheckCollisionPointCircle(GetMousePosition(), centro_play, radio_play))
                    {
                        DrawCircleLines(centro_play.x, centro_play.y, radio_play, RED); // resaltar
                        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                        {
                            pausa = 0;
                        }
                    }

                    DrawTexture(cerrar, button_cerrar.rectan.x - 150, button_cerrar.rectan.y, WHITE);
                    if (CheckCollisionPointCircle(GetMousePosition(), centro_cerrar, radio_cerrar))
                    {
                        DrawCircleLines(centro_cerrar.x, centro_cerrar.y, radio_cerrar, RED); //  resaltar
                        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                        {
                            screen = MAPA;
                            aciertos = 0;
                            pausa = 0;

                            validacionNivel3 = 0;    // reiniciar validacion nivel 3
                            estructura.aciertos = 0; // reiniciar aciertos
                        }
                    }
                }
            }
            else
            {
                DrawTexture(vidas_agotas, (screenWidth - vidas_agotas.width) / 2, (screenHeight - vidas_agotas.height) / 2 - 300, WHITE);
                Vector2 centro_reiniciar = {
                    button_reiniciar.rectan.x + button_reiniciar.rectan.width / 2 - 150,
                    button_reiniciar.rectan.y + button_reiniciar.rectan.height / 2};
                float radio_reiniciar = button_reiniciar.rectan.width / 2;

                Vector2 centro_cerrar = {
                    button_cerrar.rectan.x + button_cerrar.rectan.width / 2 - 150,
                    button_cerrar.rectan.y + button_cerrar.rectan.height / 2};
                float radio_cerrar = button_cerrar.rectan.width / 2;

                DrawTexture(reiniciar, button_reiniciar.rectan.x - 150, button_reiniciar.rectan.y, WHITE);
                if (CheckCollisionPointCircle(GetMousePosition(), centro_reiniciar, radio_reiniciar))
                {
                    DrawCircleLines(centro_reiniciar.x, centro_reiniciar.y, radio_reiniciar, RED); // resaltar
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    {
                        vidas = 3;
                        aciertos = 0;
                        bandera_vidas = 0;

                        estructura.aciertos = 0; // reiniciar aciertos
                        validacionNivel3 = 0;    // reiniciar validacion nivel 3
                    }
                }

                DrawTexture(cerrar, button_cerrar.rectan.x - 150, button_cerrar.rectan.y, WHITE);
                if (CheckCollisionPointCircle(GetMousePosition(), centro_cerrar, radio_cerrar))
                {
                    DrawCircleLines(centro_cerrar.x, centro_cerrar.y, radio_cerrar, RED); //  resaltar
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    {
                        screen = MAPA;
                        aciertos = 0;
                        validacionNivel3 = 0;
                        estructura.aciertos = 0; // reiniciar aciertos
                    }
                }
            }

            break;
        case MAPA:

            button_lava.rectan = {screenWidth / 2.0f - 130, screenHeight / 2 - 240, 300, 250};
            DrawRectangleRec(button_lava.rectan, WHITE);
            button_nieve.rectan = {200, screenHeight / 2 - 20, 250, 280};
            DrawRectangleRec(button_lava.rectan, WHITE);
            button_final.rectan = {1440, screenHeight / 2 + 100, 250, 280};
            DrawRectangleRec(button_lava.rectan, WHITE);
            if (IsKeyPressed(KEY_ESCAPE))
            {
                screen = MENU;
            }

            DrawTexture(islas, 0, 0, WHITE);

            if (CheckCollisionPointRec(mouse, button_nieve.rectan))
            {
                DrawTexture(texto_nivel1, 100, 150, WHITE);
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    screen = NIVEL1;
                    bandera_vidas = 0;
                    estructura = GenerarNuevaPreguntaSuma(estructura); // o Resta si quieres
                    vidas = 3;
                    pausa = 0;
                    aciertos = 0;

                    estructura.modoResta = false;
                }
            }

            if (CheckCollisionPointRec(mouse, button_lava.rectan))
            {
                if (bandera_nivel2 == 0)
                {
                    DrawTexture(texto_bloqueado_nivel2, 750, screenHeight / 2 + 50, WHITE);
                }
                else
                {
                    DrawTexture(texto_nivel2, 600, screenHeight / 2 + 50, WHITE);
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    {
                        screen = NIVEL2;
                        vidas = 3;
                        bandera_vidas = 0;
                        aciertos = 0;
                        pausa = 0;
                        validacion = 0;
                        estructura.modoDivision = false; // reiniciar modo division
                    }
                }
            }

            if (CheckCollisionPointRec(mouse, button_final.rectan))
            {
                if (bandera_nivel3 == 0)
                {
                    DrawTexture(texto_bloqueado_nivel3, 1300, screenHeight / 2 - 80, WHITE);
                }
                else
                {
                    DrawTexture(texto_nivel3, 1300, screenHeight / 2 - 150, WHITE);
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    {
                        screen = NIVE3;
                        // estructura = GenerarNuevaPreguntaSuma(estructura); // o Resta si quieres
                        bandera_vidas = 0;
                        vidas = 3;
                        pausa = 0;
                        aciertos = 0;

                        validacionNivel3 = 0; // reiniciar validacion nivel 3
                    }
                }
            }
            break;
        }

        EndDrawing();
    }

    UnloadTexture(background);
    UnloadTexture(background2);
    UnloadTexture(fondo_nieve);
    UnloadTexture(bomba);
    UnloadTexture(bombaexplotando);
    UnloadMusicStream(cancion_medieval);
    UnloadMusicStream(cancion_navidad);
    UnloadMusicStream(cancion_principal);
    UnloadTexture(hielo_completo);
    UnloadTexture(hielo_derretido);
    UnloadTexture(hielo_parcial);

    CloseWindow(); // Close window and OpenGL context

    return 0;
}

Juego GenerarNuevaPreguntaSuma(Juego estructura)
{
    estructura.objetivo = rand() % 20 + 5;

    // generar los dos numero correctos
    int correctoA = rand() % estructura.objetivo;
    int correctoB = estructura.objetivo - correctoA;

    // generar los dos numeros incorrectos
    int incorrectoA, incorrectoB, incorrectoC, incorrectoD;
    do
    {
        incorrectoA = rand() % estructura.objetivo;
        incorrectoB = rand() % estructura.objetivo;
        incorrectoC = rand() % estructura.objetivo;
        incorrectoD = rand() % estructura.objetivo;
    } while (
        incorrectoA + incorrectoB == estructura.objetivo ||
        incorrectoA + incorrectoC == estructura.objetivo ||
        incorrectoA + incorrectoD == estructura.objetivo ||
        incorrectoB + incorrectoC == estructura.objetivo ||
        incorrectoB + incorrectoD == estructura.objetivo ||
        incorrectoC + incorrectoD == estructura.objetivo);

    int valores[6] = {correctoA, correctoB, incorrectoA, incorrectoB, incorrectoC, incorrectoD};

    // Mezclar los numeros
    for (int i = 0; i < 6; i++)
    {
        int j = rand() % 6;
        int temp = valores[i];
        valores[i] = valores[j];
        valores[j] = temp;
    }

    estructura.numA = valores[0];
    estructura.numB = valores[1];
    estructura.numC = valores[2];
    estructura.numD = valores[3];
    estructura.numE = valores[4];
    estructura.numF = valores[5];

    // resetear estado del juego
    estructura.resultadoJugador = 0;
    estructura.clicksHechos = 0;
    estructura.mostrarResultado = false;

    estructura.botonASeleccionado = false;
    estructura.botonBSeleccionado = false;
    estructura.botonCSeleccionado = false;
    estructura.botonDSeleccionado = false;
    estructura.botonESeleccionado = false;
    estructura.botonFSeleccionado = false;

    // timepo
    estructura.tiempoMaximo = 10.0f; // 10 segundos
    estructura.tiempoRestante = estructura.tiempoMaximo;
    estructura.tiempoAgotado = false;

    return estructura;
}

Juego GenerarNuevaPreguntaResta(Juego estructura)
{
    // Generar dos números donde A - B = objetivo
    int correctoA = rand() % 10 + 15; // número entre 10 y 19
    int correctoB = rand() % 10;      // número entre 0 y 9
    estructura.objetivo = correctoA - correctoB;

    // Generar pares incorrectos
    int incorrectoA, incorrectoB, incorrectoC, incorrectoD;
    do
    {
        incorrectoA = rand() % 25;
        incorrectoB = rand() % 25;
        incorrectoC = rand() % 25;
        incorrectoD = rand() % 25;
    } while (
        incorrectoA - incorrectoB == estructura.objetivo ||
        incorrectoA - incorrectoC == estructura.objetivo ||
        incorrectoA - incorrectoD == estructura.objetivo ||
        incorrectoB - incorrectoC == estructura.objetivo ||
        incorrectoB - incorrectoD == estructura.objetivo ||
        incorrectoC - incorrectoD == estructura.objetivo);

    int valores[6] = {correctoA, correctoB, incorrectoA, incorrectoB, incorrectoC, incorrectoD};

    // Mezclar los valores
    for (int i = 0; i < 6; i++)
    {
        int j = rand() % 6;
        int temp = valores[i];
        valores[i] = valores[j];
        valores[j] = temp;
    }

    estructura.numA = valores[0];
    estructura.numB = valores[1];
    estructura.numC = valores[2];
    estructura.numD = valores[3];
    estructura.numE = valores[4];
    estructura.numF = valores[5];

    estructura.resultadoJugador = 0;
    estructura.clicksHechos = 0;
    estructura.mostrarResultado = false;

    estructura.botonASeleccionado = false;
    estructura.botonBSeleccionado = false;
    estructura.botonCSeleccionado = false;
    estructura.botonDSeleccionado = false;
    estructura.botonESeleccionado = false;
    estructura.botonFSeleccionado = false;

    // timepo
    estructura.tiempoMaximo = 10.0f; // 10 segundos
    estructura.tiempoRestante = estructura.tiempoMaximo;
    estructura.tiempoAgotado = false;

    return estructura;
}

void funcion_pausa()
{
}

Juego GenerarNuevaPreguntaMultiplicacion(Juego estructura)
{
    // generar dos numeros pequeños
    int correctoA = rand() % 6 + 2; // entre 2 y 7
    int correctoB = rand() % 6 + 2; // entre 2 y 7
    estructura.objetivo = correctoA * correctoB;

    // generar pares incorrectos que no den el resultado correcto
    int incorrectoA, incorrectoB, incorrectoC, incorrectoD;
    do
    {
        incorrectoA = rand() % 10 + 1;
        incorrectoB = rand() % 10 + 1;
        incorrectoC = rand() % 10 + 1;
        incorrectoD = rand() % 10 + 1;
    } while (
        incorrectoA * incorrectoB == estructura.objetivo ||
        incorrectoA * incorrectoC == estructura.objetivo ||
        incorrectoA * incorrectoD == estructura.objetivo ||
        incorrectoB * incorrectoC == estructura.objetivo ||
        incorrectoB * incorrectoD == estructura.objetivo ||
        incorrectoC * incorrectoD == estructura.objetivo);

    int valores[6] = {correctoA, correctoB, incorrectoA, incorrectoB, incorrectoC, incorrectoD};

    // Mezclar los valores
    for (int i = 0; i < 6; i++)
    {
        int j = rand() % 6;
        int temp = valores[i];
        valores[i] = valores[j];
        valores[j] = temp;
    }

    estructura.numA = valores[0];
    estructura.numB = valores[1];
    estructura.numC = valores[2];
    estructura.numD = valores[3];
    estructura.numE = valores[4];
    estructura.numF = valores[5];

    // Resetear estado del juego
    estructura.resultadoJugador = 0;
    estructura.clicksHechos = 0;
    estructura.mostrarResultado = false;

    estructura.botonASeleccionado = false;
    estructura.botonBSeleccionado = false;
    estructura.botonCSeleccionado = false;
    estructura.botonDSeleccionado = false;
    estructura.botonESeleccionado = false;
    estructura.botonFSeleccionado = false;

    // timepo
    estructura.tiempoMaximo = 10.0f; // 10 segundos
    estructura.tiempoRestante = estructura.tiempoMaximo;
    estructura.tiempoAgotado = false;

    return estructura;
}

Juego GenerarNuevaPreguntaDivision(Juego estructura)
{
    int correctoB = rand() % 9 + 2;       // entre 2 y 10
    int objetivo = rand() % 10 + 1;       // resultado deseado entre 1 y 10
    int correctoA = correctoB * objetivo; // A = B * objetivo → división exacta

    estructura.objetivo = objetivo;

    // Generar números incorrectos
    int incorrectoA, incorrectoB, incorrectoC, incorrectoD;
    do
    {
        incorrectoA = rand() % 20 + 1;
        incorrectoB = rand() % 20 + 1;
        incorrectoC = rand() % 20 + 1;
        incorrectoD = rand() % 20 + 1;
    } while (
        (incorrectoA != 0 && correctoA / incorrectoA == objetivo) ||
        (incorrectoB != 0 && correctoA / incorrectoB == objetivo) ||
        (incorrectoC != 0 && correctoA / incorrectoC == objetivo) ||
        (incorrectoD != 0 && correctoA / incorrectoD == objetivo));

    // Colocar los números en un array
    int valores[6] = {correctoA, correctoB, incorrectoA, incorrectoB, incorrectoC, incorrectoD};

    // Mezclar los valores
    for (int i = 0; i < 6; i++)
    {
        int j = rand() % 6;
        int temp = valores[i];
        valores[i] = valores[j];
        valores[j] = temp;
    }

    estructura.numA = valores[0];
    estructura.numB = valores[1];
    estructura.numC = valores[2];
    estructura.numD = valores[3];
    estructura.numE = valores[4];
    estructura.numF = valores[5];

    // Resetear estado
    estructura.resultadoJugador = 0;
    estructura.clicksHechos = 0;
    estructura.mostrarResultado = false;

    estructura.botonASeleccionado = false;
    estructura.botonBSeleccionado = false;
    estructura.botonCSeleccionado = false;
    estructura.botonDSeleccionado = false;
    estructura.botonESeleccionado = false;
    estructura.botonFSeleccionado = false;

    // timepo
    estructura.tiempoMaximo = 10.0f; // 10 segundos
    estructura.tiempoRestante = estructura.tiempoMaximo;
    estructura.tiempoAgotado = false;

    return estructura;
}