#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include "ventanasYFicheros.c"
#include "trivia.c"

typedef struct
{
    char nombre[6];
    int puntos;
    char fecha[11];
} Jugador;

void juego();
void puntuaciones();
void creditos();
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
                //instrucciones();
                opcion = 5;
                break;
            case 2:
                puntuaciones();
                opcion = 5;
                break;
            case 3:
                creditos();
                opcion = 5;
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
    wrefresh(vntntxt);
    wstandend(vntntxt);

    WINDOW* vntnopciones = crear_ventana(12, 15, LINES/2, COLS/2-7);
    refresh();

    char tecla;
    int opc = 0,  redimension = 1;
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
//------------------------------------------------------------
    int preguntado[40];
    preguntado[0] = 90;
    int pasada=0;
    int fin = 0, 
      correcta=0, 

      puntoEnElJuego = 1, 
      redibujar=0,minijuego=0, 
      tiempo=1, 
      
      x1 = 0, 
      direccion = 1, 
      a = 20, 
      
      dificultad=0,

  
  
      respuesta=0, 
      cambioPregunta=1, 
      Preguntascorrectas=0, 
      jugando=0, 
      i=0, 

      opcion=0, 
      strike=0, base1=0,base2=0, puntaje1=0, puntaje2=0;
      
    char teclaChar='a', turno='1';


//------------------------------------------------------------


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

    //TRANSICION (ANIMACION): BATEADOR
    WINDOW* bateador = crear_ventana(20, 60, LINES/2-7, COLS/2-30);
    char str[2] = {'0', '0'}; //sirve para manejar el inicio de la lectura del archivo sprites.txt
    for(int i = 1; i <= 3; i++){ //ir pasando los sprites del bateador
        str[1] += 1;
        lecturaFicheros("./archivos_texto/sprites.txt", bateador, ';', str);
        wrefresh(bateador);
        usleep(500000);
        wclear(bateador);
    }
    destruir_ventana(bateador);

    WINDOW* menuPausa = crear_ventana(4, 13, LINES/2-4, COLS/2-6);
    box(menuPausa, '#', '*');
    WINDOW* textoPausa = crear_ventana(7, 53, 3, COLS/2-25);
    refresh();
    lecturaFicheros("./archivos_texto/sprites.txt", textoPausa, '{', "21");

    int innings = 1, opcionPausa = 0;
    bool escape = false, activarMenu = false, dibujar = false;
    char botonesPausa[2][9]= {
                                "Regresar",
                                "Salir"
                                };      


    halfdelay(2); //inicio de la detección de eventos para el juego
    
    srand(time(NULL));
  
    //getch();
    //clear(); 
    //FUNCIONALIDAD JUEGO 
    while(innings != 9 && escape != true && fin==0 && base1<3 && base2<3){//"innings" = "entradas"
  

        mvprintw(29,29, "Turno %c", turno);
        if(opcion!=8)
        {
            correcta=triviaPantalla(preguntado, pasada, dificultad);
            refresh();       
        }

        mvprintw(0,0,"          ");   

        if(opcion==correcta)
        {
            mvprintw(0,0,"correcto");
            if(turno=='1'){
                base1++;
                turno='2';
                puntaje1+=dificultad;
            }else{
                turno='1';
                base2++;
                puntaje2+=dificultad;
            }
        }
        else{
            mvprintw(0,0,"Incorrecto");
            strike++;
            if(strike==3)
            {
                if(turno=='1')
                {
                turno='2';
                }else{
                turno='2';
                }
            
            }            
        }

        //EVENTOS
        tecla = getch();        
    
        mvprintw(16,16, "%c", tecla);
        refresh(); 
        switch(tecla)
        {
            case 'a':
            {
                opcion=0;
                break;
            }
            case 'A':
            {
                opcion=0;
                break;
            }

            case 'b':
            {
                opcion=1;
                break;
            }
            case 'B':
            {
                {
                opcion=1;
                break;
            }
            }
            case 'c':
            {
                opcion=2;
                break;
            }
            case 'C':
            {
                opcion=2;
                break;
            }
            case 'd':
            {
                opcion=3;
                break;
            }
            case 'D':
            {
                opcion=3;
                break;
            }
      
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
                    if(opcionPausa != 0)//EN el menu de Pausa ->Reconfigurable
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
            
            case 10: //enter
                if(activarMenu == true){
                    if(opcionPausa == 1)
                        escape = true;
                    else
                        activarMenu = false;
                    clear();
                }
                break;
            
            default:
            {
                mvprintw(0,0,"inválido");
                opcion=8; //**     
                tecla = NULL;            
            }
       
                
        }

        //IMPRESIONES (VENTANAS-GRAFICOS)
        if(tecla == NULL){
            //-MENU PAUSA
            if(activarMenu == true){ //menuPausa?
                if(dibujar == true){
                    mvwin(menuPausa, LINES/2-4, COLS/2-6);
                    wclear(menuPausa);
                    box(menuPausa, '#', '*');
                    for(int i = 0; i <= 1; i++){
                        if(i == opcionPausa){
                            wattr_on(menuPausa, A_BOLD | A_UNDERLINE, "");
                            mvwprintw(menuPausa, 1+i, 6-(strlen(botonesPausa[i])/2), "%s", botonesPausa[i]);
                            wstandend(menuPausa);
                        }
                        else
                            mvwprintw(menuPausa, 1+i, 6-(strlen(botonesPausa[i])/2), "%s", botonesPausa[i]);
                    }
                    wrefresh(menuPausa);
                    dibujar = false;
                }
            }
            else{ //juego
                mainTrivia();
                //PRUEBA (ignorar siguientes 10 lineas)
                /*
                WINDOW* sas = crear_ventana(20, 60, LINES/2-7, COLS/2-30);
                char stal[2] = {'0', '0'}; //sirve para manejar el inicio de la lectura del archivo sprites.txt
                for(int i = 1; i <= 3; i++){ //ir pasando los sprites del bateador
                    stal[1] += 1;
                    lecturaFicheros("./archivos_texto/sprites.txt", sas, ';', stal);
                    wrefresh(sas);
                    usleep(500000);
                    wclear(sas);
                }
                destruir_ventana(sas);
                */
                //mainTrivia();

/*

  */
/************************************************************************************/

            }
        }
    }
    clear();
    destruir_ventana(menuPausa);
    destruir_ventana(salir);
    destruir_ventana(textoPausa);

    refresh();
    clear();
}

void puntuaciones()
{

}

void creditos()
{
    WINDOW* texto = crear_ventana(26, 65, 0, 0);
    refresh();
    lecturaFicheros("./archivos_texto/creditos.txt", texto, ';', "00");
    wrefresh(texto);
    int tecla;
    while((tecla = wgetch(texto)) != 10){

    }
}
