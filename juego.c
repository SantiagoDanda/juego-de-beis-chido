#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include "ventanas.c"

void menu();

int main (int argc, char* const argv[]) 
{
    initscr();
    clear();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);

    menu();

    getch();
    endwin();
    return 0;
}

void menu()
{
    FILE *texto;

    texto = fopen("sprites.txt", "r");
    if(texto == NULL){
        printw("No se pudo acceder al archivo de texto");
        exit(1);
    }   

    WINDOW* vntntxt = crear_ventana(5, 100, 0, 0);
    refresh();
    mvwin(vntntxt, 5, COLS/2-50); //mueve la ventana del texto al centro

    char crctr;
    int i = 0; 
    while(!feof(texto)){
        crctr = fgetc(texto);
        if(((char)crctr) == '1') //Si el caracter encontrado en el fichero es "1", deja de leerse
            break;
        wprintw(vntntxt, "%c", crctr);
    }
    wrefresh(vntntxt);
    fclose(texto);

    WINDOW* vntnopciones = crear_ventana(12, 15, 15, COLS/2-7);
    refresh();

    //wattron(vntnopciones, A_BOLD || A_ITALIC); HACER QUE SIRVA
    mvwprintw(vntnopciones, 2, 5, "Jugar");
    mvwprintw(vntnopciones, 4, 1, "Instrucciones");
    mvwprintw(vntnopciones, 6, 2, "Puntuaciones");
    mvwprintw(vntnopciones, 8, 4, "Créditos");
    wrefresh(vntnopciones);
    /*
    Jugar
    Instrucciones
    Puntuaciones
    Créditos
    */
}