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

void obtenerPuntajes(int *puntajesRegistrados, char (*nombresRegistrados)[6], char (*fechasRegistrada)[11])
{
    int lengthNombre = 6, lengthFecha = 11, i, y=0, z=0;
    
    FILE *arch;/*
    arch = fopen("./puntajes.txt", "r");
    if(!arch)
    {
        endwin();
        printf("No se pudo a acceder al archivo de texto \"puntajes.txt\"");
        exit(1);        
    }*/
    bool comenzarALeer = false, parar = false;
    char crctr = '\0';

    char castToIntPuntaje[10], identificadorPuntaje='1', identPuntaje2='>';
    
    char asciiValor = 'y', asciiValor2 = '~';
    char strGetNombre[2]= {asciiValor, '>'};
    char strGetFecha[2]= {asciiValor2, '>'};

    for(i=0; i<5; i++)
    {
        arch = fopen("./puntajes.txt", "r");
        if(!arch)
        {
            endwin();
            printf("No se pudo a acceder al archivo de texto \"puntajes.txt\"");
            exit(1);        
        }
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
            }
            if(comenzarALeer == true){                
                while(crctr != ';')
                {
                    crctr = fgetc(arch);
                    castToIntPuntaje[y] = crctr;
                    y++;                    
                }
                puntajesRegistrados[i] = atoi(castToIntPuntaje);
                                            
                for(z=0; z<10; z++)                
                    castToIntPuntaje[z] = '\0';                
                y=0;            
                crctr = '\0';
                comenzarALeer = false;
                parar = true;
                //printw("%i\n", puntajesRegistrados[i]);
                //refresh();   
            }
        }
        parar = false;   

        fclose(arch); 

        arch = fopen("./puntajes.txt", "r");
        if(!arch)
        {
            endwin();
            printf("No se pudo a acceder al archivo de texto \"puntajes.txt\"");
            exit(1);        
        }
/**/
        while(parar == false){
            if(comenzarALeer == false){
                if((fgetc(arch)) == strGetNombre[0])
                {                        
                    if((fgetc(arch)) == strGetNombre[1])
                    {
                        comenzarALeer = true;
                        //printw("TROLLEO");
                        //refresh();                    
                        
                        strGetNombre[0]--;                                                
                    }                        
                }
            }
            
            if(comenzarALeer == true){                
                
                while(crctr != ';')
                {
                    crctr = fgetc(arch);
                    nombresRegistrados[0][0] = crctr;
                    y++;                    
                }      
                                                      
                y=0;            
                crctr = '\0';
                comenzarALeer = false;
                parar = true;
                //printw("%i\n", puntajesRegistrados[i]);
                //refresh();   
            }
        }
        parar = false;   
        fclose(arch); 

        arch = fopen("./puntajes.txt", "r");
        if(!arch)
        {
            endwin();
            printf("No se pudo a acceder al archivo de texto \"puntajes.txt\"");
            exit(1);        
        }
        while(parar == false){
            if(comenzarALeer == false){
                if((fgetc(arch)) == strGetFecha[0])
                {                        
                    if((fgetc(arch)) == strGetFecha[1])
                    {
                        comenzarALeer = true;
                        //printw("TROLLEO");
                        //refresh();                    
                        
                        strGetFecha[0]--;                                                
                    }                        
                }
            }
            if(comenzarALeer == true){                
                while(crctr != ';')
                {
                    crctr = fgetc(arch);
                    fechasRegistrada[i][y] = crctr;
                    y++;                    
                }        
                                                      
                y=0;            
                crctr = '\0';
                comenzarALeer = false;
                parar = true;
                //printw("%i\n", puntajesRegistrados[i]);
                //refresh();   
            }
        }
        parar = false;  
        fclose(arch);     
    } 
    printw("%c", nombresRegistrados[0][0]);


      

}

void imprimirPuntajes()
{
    WINDOW* PUNTAJES = crear_ventana(10, 98, 4, MITADCOLS-49);
    //WINDOW* PUNTAJES = crear_ventana(100, 100, 2, 0);

    char asciiValor = 'y', asciiValor2 = '~';
    char strGetNombre[2]= {asciiValor, '>'};
    char strGetFecha[2]= {asciiValor2, '>'};

    int puntajesRegistrados[5], i;
    char nombresRegistrados[5][6], //faltan por definir bien 
        fechaRegistrada[5][11] //faltan por definir bien 
    ;

    int y; //indice


    for(i=0; i<5; i++)
    {
        puntajesRegistrados[i] = 0;        
        for(y=0;y<6;y++)
            nombresRegistrados[i][y] = '\0';
        for(y=0;y<11;y++)
            fechaRegistrada[i][y] = '\0';
    }
    obtenerPuntajes(puntajesRegistrados, nombresRegistrados, fechaRegistrada);
    
    
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

void compararPuntajes(char *jugador, int *puntos, char *fecha, int lengthNombre, int lengthFecha)
{
    WINDOW* COMP_PUNTAJES = crear_ventana(10, 98, 4, MITADCOLS-49);
    //WINDOW* PUNTAJES = crear_ventana(100, 100, 2, 0);
    
    int puntajesRegistrados[5], i, y;
    char nombresRegistrados[5][lengthNombre], //faltan por definir bien 
        fechaRegistrada[5][lengthFecha] //faltan por definir bien 
    ;
    int reordenamientoPuntajes[6], espacioReordenamiento;
    char reordenamientoNombres[5][lengthNombre], reordenamientoFechas[5][lengthFecha];
    char espacioReordenamientoNombre[lengthNombre], espacioReordenamientoFecha[lengthFecha];

    char asciiValor = 'y', asciiValor2 = '~';
    char strGetNombre[2]= {asciiValor, '>'};
    char strGetFecha[2]= {asciiValor2, '>'};

    for(i=0; i<5; i++)
    {
        puntajesRegistrados[i] = 0;        
        for(y=0;y<lengthNombre;y++)
            nombresRegistrados[i][y] = '\0';
        for(y=0;y<lengthFecha;y++)
            fechaRegistrada[i][y] = '\0';
    }
    obtenerPuntajes(puntajesRegistrados, nombresRegistrados, fechaRegistrada);

    int terminar = 0, indiceX=0, indiceY=0;
    
    for(i=0;i<5;i++)
        reordenamientoPuntajes[i] = puntajesRegistrados[i];
    reordenamientoPuntajes[5] = *puntos;

    int strlenNombresReg = strlen(nombresRegistrados[i]); 
    int strlenJugador = strlen(jugador), strlenFecha = strlen(fecha);

    int strlenReordNombres, strlenReordFEchas;
    
    for(i=0;i<5;i++)
        for(y=0;y<strlenNombresReg;y++)
            reordenamientoNombres[i][y] = nombresRegistrados[i][y];
    
    for(y=0;y<strlenJugador;y++)
        reordenamientoNombres[5][y] = jugador[y];

    for(y=0;y<strlenFecha;y++)
        reordenamientoFechas[5][y] = fecha[y];
    /*
    while(terminar == 0)
    {
        if(reordenamientoPuntajes[indiceX]<reordenamientoPuntajes[indiceX+1])
        {
            espacioReordenamiento = reordenamientoPuntajes[indiceX];
            reordenamientoPuntajes[indiceX]=reordenamientoPuntajes[indiceX+1];
            reordenamientoPuntajes[indiceX+1]=espacioReordenamiento;

            strlenReordNombres = strlen(reordenamientoNombres[i]);
            strlenReordFEchas = strlen(reordenamientoFechas[i]);
            for(i=0;i<strlenReordNombres;i++)
            {
                espacioReordenamientoNombre[i]=reordenamientoNombres[indiceX][i];
                reordenamientoNombres[indiceX][i] = reordenamientoNombres[indiceX+1][i];
                reordenamientoNombres[indiceX+1][i] = espacioReordenamientoNombre[i];
            }
            for(i=0;i<strlenReordFEchas;i++)
            {
                espacioReordenamientoFecha[i]=reordenamientoFechas[indiceX][i];
                reordenamientoFechas[indiceX][i] = reordenamientoFechas[indiceX+1][i];
                reordenamientoFechas[indiceX+1][i] = espacioReordenamientoFecha[i];
            }


            
            for(i=0;i<strlenReordNombres;i++)
                espacioReordenamientoNombre[i] = '\0'; 
            for(i=0;i<strlenReordFEchas;i++)
                espacioReordenamientoFecha[i] = '\0'; 
            espacioReordenamiento = 0;

            indiceX=0;
            
        }
        else        
            indiceX++;
        if(indiceX == 5)
            terminar = 1;
    }*/
    

    for(i=0;i<5;i++)
        puntajesRegistrados[i] = reordenamientoPuntajes[i];
    y=0;

        
    for(i=0;i<5;i++)
    {
        strlenNombresReg = strlen(reordenamientoNombres[i]);
        for(y=0;y<strlenNombresReg;y++)
        {
            nombresRegistrados[i][y] = reordenamientoNombres[i][y];
        }
            
    }
        
    for(i=0;i<5;i++)
        for(y=0;y<lengthFecha;y++)
            fechaRegistrada[i][y] = reordenamientoFechas[i][y];

    
    for(i=0;i<lengthNombre;i++)
        reordenamientoNombres[5][i] = '\0';
    for(i=0;i<lengthFecha;i++)
        reordenamientoFechas[5][i]= '\0';

    reordenamientoPuntajes[5]=0;
    for(i=0; i<5; i++)
    {
        //for(j=0; j<3; j++)
        wprintw(COMP_PUNTAJES, "%i° Lugar: ->", i+1);
        //for(y=0; y<6; y++)
        wprintw(COMP_PUNTAJES, "%s", nombresRegistrados[i]);
        //lecturaFichero("./puntajes.txt", COMP_PUNTAJES, ';', strGetNombre);
        wprintw(COMP_PUNTAJES, "-- Puntaje: %i --", puntajesRegistrados[i]);
        wprintw(COMP_PUNTAJES, "-- Fecha: ");
        //lecturaFichero("./puntajes.txt", COMP_PUNTAJES, ';', strGetFecha);
        wprintw(COMP_PUNTAJES, "%s", fechaRegistrada[i]);

        wprintw(COMP_PUNTAJES, "\n");

        strGetNombre[0]--;
        strGetFecha[0]--;
    }
    wrefresh(COMP_PUNTAJES);
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