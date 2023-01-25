#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ventanas.c"

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

    menu();

    getch();
    endwin();
    return 0;
}

int menu()
{
    FILE *texto;

    texto = fopen("./sprites.txt", "r");
    if(texto == NULL){
        endwin();
        printf("No se pudo acceder al archivo de texto");
        exit(1);
    }   

    init_pair(1, COLOR_RED, COLOR_BLACK);

    WINDOW* vntntxt = crear_ventana(5, 100, 0, 0);
    refresh();
    wattr_on(vntntxt, A_BOLD | COLOR_PAIR(1), "");
    mvwin(vntntxt, 5, COLS/2-50); //mueve la ventana del texto al centro

    char crctr;
    while(!feof(texto)){
        crctr = fgetc(texto);
        if(((char)crctr) == '1') //Si el caracter encontrado en el fichero es "1", deja de leerse
            break;
        wprintw(vntntxt, "%c", crctr);
    }
    wrefresh(vntntxt);
    fclose(texto);

    WINDOW* vntnopciones = crear_ventana(12, 15, LINES/2, COLS/2-7);
    refresh();

    int opc = 0, tecla, redimension = 1;
    char opciones[4][14] = { "Jugar", //opc 0
                            "Instrucciones", //opc 1
                            "Puntuaciones", //opc 2
                            "Créditos" //opc 3
                            };

    do{
        switch (tecla){
        case 119: //letra w
            if(opc != 0)
                opc--;
            else
                opc = 3;
            redimension = 1;
            break;
        case 115: //letra s
            if(opc != 3)
                opc++;
            else
                opc = 0;
            redimension = 1;
            break;
        case 410: //caso de redimensionamiento
            clear();
            refresh();
            mvwin(vntntxt, 5, COLS/2-50);
            wrefresh(vntntxt);
            mvwin(vntnopciones, LINES/2, COLS/2-7);
            wrefresh(vntnopciones);
        }
        if(redimension == 1){ //se imprimen las opciones
            for(int i = 0; i <= 3; i++){
                if(i == opc){
                    wattr_on(vntnopciones, A_BOLD | A_UNDERLINE, "");
                    mvwprintw(vntnopciones, i*2, 7-((strlen(opciones[i]))/2), "%s", opciones[i]);
                    wstandend(vntnopciones);
                }
                else{
                    mvwprintw(vntnopciones, i*2, 7-((strlen(opciones[i]))/2), "%s", opciones[i]);
                }
            }
            wrefresh(vntnopciones);
        }
        redimension = 0;
    }while((tecla = wgetch(vntnopciones)) != 10); //hasta presionar enter
    
    return opc; //devuelve la opcion
}