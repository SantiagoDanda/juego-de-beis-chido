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
    mvwin(vntntxt, 0, COLS/2-50);

    char cadena[500]; 
    while(!feof(texto)){
        fgets(cadena, 100, texto);
        wprintw(vntntxt, "%s", cadena);
    }
    wrefresh(vntntxt);
    fclose(texto);
}