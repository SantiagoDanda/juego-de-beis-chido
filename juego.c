#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ventanasYFicheros.c"
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

typedef struct
{
    char nombre[6];
    int puntos;
    char fecha[11];
} Jugador;

void juego();
void puntuaciones();
int menu();

int main (int argc, char* const argv[]) 
{
    initscr();
    if(has_colors() == FALSE){
        endwin();
        printf("No puedes utilizar colores");
        exit(1);
    }
    start_color();
    clear();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    refresh();

    int opcion = menu();
    while(opcion != 4){
        switch(opcion){
            case 0:
                juego();
                opcion = 5;
                break;
            case 1:
                break;
            case 2:
                puntuaciones();
                break;
            case 3:
                break;
            case 4:
                break;
            case 5:
                opcion = menu();
        }
    }
    endwin();
    return 0;
}

int menu()
{
    init_pair(1, COLOR_RED, COLOR_BLACK);

    WINDOW* vntntxt = crear_ventana(5, 100, 0, 0);
    refresh();
    wattr_on(vntntxt, A_BOLD | COLOR_PAIR(1), "");
    mvwin(vntntxt, 5, COLS/2-50); //mueve la ventana del texto al centro

    lecturaFicheros("./archivos_texto/sprites.txt", vntntxt, ';', "00");
    wstandend(vntntxt);

    WINDOW* vntnopciones = crear_ventana(12, 15, LINES/2, COLS/2-7);
    refresh();

    int opc = 0, tecla, redimension = 1;
    char opciones[5][14] = { "Jugar", //opc 0
                            "Instrucciones", //opc 1
                            "Puntuaciones", //opc 2
                            "Créditos", //opc 3
                            "Salir" //op4
                            };

    do{
        switch (tecla){
        case 119: //letra w
            if(opc != 0)
                opc--;
            else
                opc = 4;
            redimension = 1;
            break;
        case 115: //letra s
            if(opc != 4)
                opc++;
            else
                opc = 0;
            redimension = 1;
            break;
        case 410: //caso de redimensionamiento
            clear();
            refresh();
            mvwin(vntntxt, 5, COLS/2-50);
            mvwin(vntnopciones, LINES/2, COLS/2-7);
            wrefresh(vntntxt);
            wrefresh(vntnopciones);
            break;
        }
        if(redimension == 1){ //se imprimen las opciones
            for(int i = 0; i <= 4; i++){
                if(i == opc){
                    wattr_on(vntnopciones, A_BOLD | A_UNDERLINE, "");
                    mvwprintw(vntnopciones, i*2, 7-((strlen(opciones[i]))/2), "%s", opciones[i]);
                    wstandend(vntnopciones);
                }
                else
                    mvwprintw(vntnopciones, i*2, 7-((strlen(opciones[i]))/2), "%s", opciones[i]);
            }
            wrefresh(vntnopciones);
        }
        redimension = 0;
    }while((tecla = wgetch(vntnopciones)) != 10); //hasta presionar enter

    destruir_ventana(vntntxt);
    destruir_ventana(vntnopciones);
    clear();
    refresh();
    
    return opc;
}

void juego()
{
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_BLUE, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    int tecla;

    FILE* puntuaciones = fopen("./archivos_texto/puntuaciones.txt", "w");
    if(puntuaciones == NULL){
        endwin();
        printf("No se pudo acceder a  la escritura de puntuaciones");
        exit(1);
    }
    
    Jugador jugador1, jugador2;
    WINDOW* bienvenida = crear_ventana(3, 76, LINES/2-2, COLS/2-38);
    WINDOW* salir = crear_ventana(1, 24, 0, 0);
    refresh();

    for(int i = 1; i <= 2; i++){ //pedir nombres
        if(i == 1)
            wattr_on(bienvenida, A_BOLD | COLOR_PAIR(1), "");
        else
            wattr_on(bienvenida, A_BOLD | COLOR_PAIR(2), "");
        wprintw(bienvenida, "¡Bienvenido, jugador %i! Por favor escribe tu nombre (máximo 5 caracteres).", i);
        wrefresh(bienvenida);
        echo();
        wmove(bienvenida, 2, (getmaxx(bienvenida)/2)-5);
        if(i == 1)
            wgetnstr(bienvenida, jugador1.nombre, 5);
        else
            wgetnstr(bienvenida, jugador2.nombre, 5);
        wclear(bienvenida);
        wrefresh(bienvenida);
    }
    noecho();

    destruir_ventana(bienvenida);
    refresh();
    clear();

    WINDOW* bateador = crear_ventana(20, 60, LINES/2-7, COLS/2-30);
    char str[2] = {'0', '0'}; //sirve para manejar el inicio de la lectura del archivo sprites.txt
    for(int i = 1; i <= 3; i++){ //ir pasando los sprites del bateador
        str[1] += 1;
        lecturaFicheros("./archivos_texto/sprites.txt", bateador, ';', str);
        usleep(500000);
        wclear(bateador);
        wrefresh(bateador);
    }
    destruir_ventana(bateador);

    WINDOW* menuPausa = crear_ventana(4, 13, LINES/2-4, COLS/2-6);
    box(menuPausa, '#', '*');
    wmove(menuPausa, 1, 2);
    refresh();

    int innings = 1, opcionPausa = 0;
    bool escape = false, activarMenu = false, dibujar = false;
    char botonesPausa[2][9]= {
                                "Regresar",
                                "Salir"
                                };
    halfdelay(2); //inicio de la detección de eventos para el juego
    while(innings != 9 && escape != true){
        tecla = getch();
        switch(tecla){
        case 27: //escape
            if(activarMenu == false){
                activarMenu = true;
                dibujar = true;
            }
            else{
                activarMenu = false;
                clear();
            }
            break;
        
        case 410: //redimensión
            clear();
            refresh();
            if(activarMenu == true){
                mvwin(menuPausa, LINES/2-4, COLS/2-6);
                wrefresh(menuPausa);
            }
            break;

        case 119: //letra w
            if(activarMenu == true){
                if(opcionPausa != 0)
                    opcionPausa--;
                else
                    opcionPausa = 1;
                dibujar = true;
            }
            break;

        case 115: //letra s
            if(activarMenu == true){
                if(opcionPausa != 1)
                    opcionPausa++;
                else
                    opcionPausa = 0;
                dibujar = true;
            }
            break;
        
        default:
            tecla = NULL;
            break;
        }
        if(tecla == NULL){
            if(activarMenu == true){
                mvwin(menuPausa, LINES/2-4, COLS/2-6);
                if(dibujar == true){
                    wclear(menuPausa);
                    box(menuPausa, '#', '*');
                    for(int i = 0; i <= 1; i++){
                        if(i == opcionPausa){
                            wattr_on(menuPausa, A_BOLD | A_UNDERLINE, "");
                            mvwprintw(menuPausa, 1+i, (strlen(botonesPausa[i])/2)-1, "%s", botonesPausa[i]);
                            wstandend(menuPausa);
                        }
                        else
                            mvwprintw(menuPausa, 1+i, (strlen(botonesPausa[i])/2)-1, "%s", botonesPausa[i]);
                    }
                    wrefresh(menuPausa);
                    dibujar = false;
                }
            }
        }
    }
    clear();

    destruir_ventana(menuPausa);
    destruir_ventana(salir);
    refresh();
    clear();
}

void puntuaciones()
{
}
