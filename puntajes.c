#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
//#include "ventanasYFicheros.c"

#define MITADCOLS COLS/2 //mitad para x

void lecturaFichero(char* const nombreArchivo, WINDOW* ventana, char limite, char* inicioLectura)
{
  FILE* archivo;

  archivo = fopen(nombreArchivo, "r");
  if(archivo == NULL){
    endwin();
    printf("No se pudo a acceder al archivo de texto \"%s\"", nombreArchivo);
    exit(1);
  }

  bool comenzarALeer = false, parar = false;
  char crctr = NULL;

  while(parar == false){
    if(comenzarALeer == false){
      if((fgetc(archivo)) == inicioLectura[0]){
        if((fgetc(archivo)) == inicioLectura[1])
          comenzarALeer = true;
      }
    }
    if(comenzarALeer == true){
      while(crctr != limite){
        wprintw(ventana, "%c", crctr);
        crctr = fgetc(archivo);
      }
      parar = true;
    }
  }
  fclose(archivo);
}

void obtenerPuntajes(int *puntajesRegistrados, char *nombresRegistrados, char *fechaRegistrada)
{
    int tamanoArray = 5, i, y=0, z=0;
    FILE *arch;
    arch = fopen("./puntajes.txt", "r");
    if(!arch)
    {
        endwin();
        printf("No se pudo a acceder al archivo de texto \"puntajes.txt\"");
        exit(1);        
    }

    /*
        printw("TROLLEO");
        refresh();
      */  


    bool comenzarALeer = false, parar = false;
    char crctr = '\0';

    char castToIntPuntaje[10], identificadorPuntaje='1', identPuntaje2='>';

    for(i=0; i<5; i++)
    {
        while(parar == false){
            if(comenzarALeer == false){
                if((fgetc(arch)) == identificadorPuntaje)
                {
                    
                    
                    if((fgetc(arch)) == identPuntaje2)
                    {
                        comenzarALeer = true;
                        //printw("TROLLEO");
                        //refresh();
                    
                        switch (i)
                        {
                            case 0:
                                identificadorPuntaje = '2';
                                break;
                            case 1:
                                identificadorPuntaje = '3';
                                break;
                            case 2:
                                identificadorPuntaje = '4';
                                break;                                                                                           
                            default:
                                identificadorPuntaje = '5';
                                break;
                        }
                        //identificadorPuntaje;
                        
                        
                    }
                        
                }
            }/*
            printw("TROLLEO");
            refresh();*/
            if(comenzarALeer == true){
                
                while(crctr != ';')
                {
                    crctr = fgetc(arch);
                    castToIntPuntaje[y] = crctr;
                    y++;                    
                }
                puntajesRegistrados[i] = atoi(castToIntPuntaje);
                
                crctr = '\0';
                
                for(z=0; z<10; z++)
                {
                    castToIntPuntaje[z] = '\0';
                }
                y=0;
                
               
            
                comenzarALeer = false;
                parar = true;

                //printw("%i\n", puntajesRegistrados[i]);
                //refresh();
                
            }
    
            
        }
        parar = false;
        /*
        printw("TROLLEO");
        refresh();*/
    } 
    


}

void imprimirPuntajes(/*char *jugador, int longNombre, int *puntos, char *fecha, int longFecha*/)
{
    
    WINDOW* PUNTAJES = crear_ventana(10, 98, 4, MITADCOLS-49);
    //WINDOW* PUNTAJES = crear_ventana(100, 100, 2, 0);


    char asciiValor = 'y', asciiValor2 = '~';
    char strGetNombre[2]= {asciiValor, '>'};
    char strGetFecha[2]= {asciiValor2, '>'};

    int puntajesRegistrados[5], i;
    char nombresRegistrados[5],
        fechaRegistrada[5]
    ;


    for(i=0; i<5; i++)
    {
        puntajesRegistrados[i] = 0;
        nombresRegistrados[i] = "\0";
        fechaRegistrada[i] = "\0";
    }
//wprintw(PUNTAJES, "-- Fecha: ");
    obtenerPuntajes(puntajesRegistrados, nombresRegistrados, fechaRegistrada);
    

    int j;
    
    for(i=0; i<5; i++)
    {
        //for(j=0; j<3; j++)
        wprintw(PUNTAJES, "%i° Lugar: ->", i+1);
        lecturaFichero("./puntajes.txt", PUNTAJES, ';', strGetNombre);

        wprintw(PUNTAJES, "-- Puntaje: %i --", puntajesRegistrados[i]);
        wprintw(PUNTAJES, "-- Fecha: ");
        lecturaFichero("./puntajes.txt", PUNTAJES, ';', strGetFecha);
        wprintw(PUNTAJES, "\n");

        strGetNombre[0]--;
        strGetFecha[0]--;

    }

    
    wrefresh(PUNTAJES);
    refresh();
    getchar();



}
void registrarPuntajeNuevo()
{

}
/*
int main()
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

    imprimirPuntajes();

    getchar();
    endwin();

    return 0;
}
*/