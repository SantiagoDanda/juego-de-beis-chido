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

void lecturaFicheros(char* const nombreArchivo, WINDOW* ventana, char limite, char* inicioLectura)
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
  
  wrefresh(ventana);
  fclose(archivo);
}