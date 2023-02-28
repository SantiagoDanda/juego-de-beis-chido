/*
ventanasYFicheros.c

Este código tiene el propósito de ejecutar funciones útiles para manipular ventanas y ficheros. Con las funciones crear_ventana y 
destruir_ventana inicializamos y eliminamos respectivamente la información de las ventanas que se utilizan en el programa.

A través de la función lecturaFicheros se obtienen caracteres de un archivo de texto y se exportan a una ventana.

Autor: Santiago Danda
<email: quetzal.danda@gmail.com>
<github: SantiagoDanda>

Fecha: 22 de enero de 2023
*/
#include <ncurses.h>
#include <stdbool.h>

WINDOW* crear_ventana(int alto, int ancho, int y, int x)
{
  WINDOW* ventana;
  ventana = newwin(alto, ancho, y, x);
  wrefresh(ventana);
  return ventana;
}

void destruir_ventana(WINDOW* ventana)
{
  wborder(ventana, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
  wrefresh(ventana);
  delwin(ventana);
}

int lecturaFicheros(char* const nombreArchivo, WINDOW* ventana, char limite, char* inicioLectura)
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

  int bytesCursor = ftell(archivo);
  fclose(archivo);

  return bytesCursor;
}