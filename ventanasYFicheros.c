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

void lecturaFicheros(char* const nombreArchivo, WINDOW* ventana, char limite, char inicioLectura)
{
  FILE* archivo;

  archivo = fopen(nombreArchivo, "r");
  if(archivo == NULL){
    endwin();
    printf("No se pudo a acceder al archivo de texto \"%s\"", nombreArchivo);
    exit(1);
  }
  bool comenzarALeer = false;
  char crctr;
  while(!feof(archivo)){
    if((fgetc(archivo)) == inicioLectura && comenzarALeer != true)
      comenzarALeer = true;
    if(comenzarALeer == true){
      while((crctr = fgetc(archivo)) != limite)
        wprintw(ventana, "%c", crctr);
      break;
    }
  }
  wrefresh(ventana);
  fclose(archivo);
}