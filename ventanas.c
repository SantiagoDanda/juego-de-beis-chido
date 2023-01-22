#include <ncurses.h>

WINDOW* crear_ventana(int alto, int ancho, int y, int x)
{
  WINDOW* ventana;
  ventana = newwin(alto, ancho, y, x);
  wrefresh(ventana);
  return ventana;
}