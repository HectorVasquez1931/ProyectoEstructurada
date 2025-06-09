#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct // struct para las partidas guardadas
{
    int partida; // 1, 2 o 3 para los 3 slots de partidas
    int nivel;
    int status;
} Ts_partida;

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
    ESCOGER_PARTIDA,
    NIVEL1,
    NIVEL2,
    NIVE3,
    MAPA
} SCREEN_MODE;

Juego GenerarNuevaPreguntaSuma(Juego estructura);
Juego GenerarNuevaPreguntaResta(Juego estructura);
void funcion_pausa();
Juego GenerarNuevaPreguntaMultiplicacion(Juego estructura);
Juego GenerarNuevaPreguntaDivision(Juego estructura);
Ts_partida carga_partida(int partida);
int verifica_si_archivo_existe(const char nombre_arch[]);
void actualiza_partida(int partida, Ts_partida nueva_partida);
void mostrar_slots_en_pantalla(Ts_partida slot1, Ts_partida slot2, Ts_partida slot3, Vector2 posicion, Font fuente);

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

Ts_partida carga_partida(int partida)
{
    FILE *archivo;
    archivo = fopen("saves.dat", "rb");
    Ts_partida slot = {0};

    if (fseek(archivo, sizeof(Ts_partida) * partida, SEEK_SET) != 0)
    {
        fclose(archivo);
        return slot;
    }
    else
    {
        if (fread(&slot, sizeof(Ts_partida), 1, archivo))
        {
            fclose(archivo);
            return slot;
        }
    }

    fclose(archivo);
    return slot;
}

int verifica_si_archivo_existe(const char nombre_arch[])
{
    FILE *archivo;
    archivo = fopen(nombre_arch, "rb");

    if (archivo)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void actualiza_partida(int partida, Ts_partida nueva_partida)
{
    FILE *archivo = fopen("saves.dat", "r+b");

    if (fseek(archivo, sizeof(Ts_partida) * partida, SEEK_SET) != 0)
    {
        fclose(archivo);
    }
    else
    {
        fwrite(&nueva_partida, sizeof(Ts_partida), 1, archivo);
    }

    fclose(archivo);
}

void mostrar_slots_en_pantalla(Ts_partida slot1, Ts_partida slot2, Ts_partida slot3, Vector2 posicion, Font fuente)
{
    char texto[128];

    // Slot 1
    sprintf(texto, "Slot 1 - Partida: %d  Nivel: %d  Status: %d", slot1.partida, slot1.nivel, slot1.status);
    DrawTextEx(fuente, texto, posicion, 24, 1, BLACK);

    // Slot 2
    sprintf(texto, "Slot 2 - Partida: %d  Nivel: %d  Status: %d", slot2.partida, slot2.nivel, slot2.status);
    DrawTextEx(fuente, texto, (Vector2){posicion.x, posicion.y + 30}, 24, 1, BLACK);

    // Slot 3
    sprintf(texto, "Slot 3 - Partida: %d  Nivel: %d  Status: %d", slot3.partida, slot3.nivel, slot3.status);
    DrawTextEx(fuente, texto, (Vector2){posicion.x, posicion.y + 60}, 24, 1, BLACK);
}