/*
juego.c

En este código se crea prácticamente todo el juego. Está hecho de tal forma que cada apartado del menú (juego, instrucciones, créditos etc)
está hecho en una subrutina distinta. 

Podrá observar que en la subrutina de juegos hay algunas divisiones. Por una parte, se señala cuando el programa lee entradas del usuario 
y cuando "dibuja", dependiendo de si el juego está en pausa o ejecutándose. Asimismo se señala a través de la variable "parteDelJuego" en
qué preciso momento de la ejecución se encuentra el programa.

Autor: Santiago Danda
<email: quetzal.danda@gmail.com>
<github: SantiagoDanda>

Fecha: 21 de enero de 2023
*/

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ventanasYFicheros.c"
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

#define MITADCOLS COLS/2 //mitad para x
#define MITADLINES LINES/2 //mitad para y

typedef struct
{
    char nombre[6];
    int puntos;
    char fecha[11];
} Jugador;

void juego();
void pantallas_info(int opcion);
void instrucciones();
int escoger_respuestas(int bytesCursor, WINDOW* ventana);
int menu(); //devuelve la opción elegida

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
    while(opcion != 3){
        switch(opcion){
            case 0:
                juego();
                opcion = 4;
                break;
            case 1:
                pantallas_info(1); //instrucciones
                opcion = 4;
                break;
            case 2:
                pantallas_info(2); //creditos
                opcion = 4;
                break;
            case 3:
                break;
            case 4:
                opcion = menu();
                break;
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

    WINDOW* vntnopciones = crear_ventana(12, 15, MITADLINES, MITADCOLS-7);
    refresh();

    int opc = 0, tecla, redimension = 1;
    char opciones[5][14] = { "Jugar", //opc 0
                            "Instrucciones", //opc 1
                            "Créditos", //opc 2
                            "Salir" //op3
                            };

    do{
        switch (tecla){
            case 87: //W
                if(opc != 0)
                    opc--;
                else
                    opc = 3;
                redimension = 1;
                break;
            case 83: //S
                if(opc != 3)
                    opc++;
                else
                    opc = 0;
                redimension = 1;
                break;
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
                mvwin(vntntxt, 5, MITADCOLS-50);
                mvwin(vntnopciones, MITADLINES, MITADCOLS-7);
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
    init_pair(3, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(4, COLOR_WHITE, COLOR_RED);
    init_pair(5, COLOR_BLACK, COLOR_GREEN);
    int tecla;
    
    Jugador jugador1, jugador2;
    jugador1.puntos = 0;
    jugador2.puntos = 0;
    WINDOW* bienvenida = crear_ventana(3, 76, MITADLINES-2, MITADCOLS-38);
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

    WINDOW* bateador = crear_ventana(20, 60, MITADLINES-10, MITADCOLS-30);
    wattr_on(bateador, A_BOLD | COLOR_PAIR(3), "");
    char str[2] = {'0', '0'}; //sirve para manejar el inicio de la lectura del archivo sprites.txt
    for(int i = 1; i <= 3; i++){ //ir pasando los sprites del bateador
        str[1] += 1;
        lecturaFicheros("./archivos_texto/sprites.txt", bateador, ';', str);
        wrefresh(bateador);
        usleep(500000);
        wclear(bateador);
    }
    destruir_ventana(bateador);

    srand(time(NULL));
    int nPreguntas[54], elementos = 54, localidadRandom, preguntasRevueltas[54];
    for(int i = 0; i <= 53; i++){
        nPreguntas[i] = i+1;
    }

    int i = 0, reacomodar;
    while(elementos != 1){ //con esto "barajeamos" el arreglo para sacar preguntas aleatorias
        localidadRandom = rand() % elementos;
        preguntasRevueltas[i] = nPreguntas[localidadRandom];
        reacomodar = (elementos - 1) - localidadRandom;
        for(int j = 0; j <= reacomodar; j++){
            nPreguntas[localidadRandom + j] = nPreguntas[localidadRandom + j + 1];
        }
        elementos--;
        i++;
    }
    int parteDelJuego = 0; /*esta variable indicará qué momento del juego se está ejecutando (si se está
                            preguntando, si se está ejecutando una animación, etc)*/
    int innings = 1, //marcar las entradas 
    opcionPausa = 0, 
    ciclos = 0, //ciclos sirve para controlar las animaciones
    turno = 1, //referente al turno del jugador
    indicePreguntas = 0, //controla qué localidad del arreglo de preguntas barajeadas se toma en cuenta
    ciclosJuego = 44, //controla los segundos según la entrada
    opcPregunta = 1, //si la respuesta elegida es a b o c
    resCorrecta,
    vecesJugadas = 1;
    bool escape = false, activarMenu = false, dibujar = false, respondio = false, equivocado = false;
    char botonesPausa[2][9]= {
                                "regresar",
                                "Salir"
                                };
    str[1] = '0';
    str[0] = '1'; //sirve para controlar los sprites del número de innings
    char strPreguntas[3] = {'0', '0', '\0'}; //sirve para controlar la pregunta que se desplegará
    //VENTANAS PARA EL JUEGO
    //1
    WINDOW* textoInnings = crear_ventana(7, 63, MITADLINES-16, MITADCOLS-31);
    WINDOW* numInnings = crear_ventana(7, 10, MITADLINES+4, MITADCOLS-5);
    //2
    WINDOW* textoTurno = crear_ventana(8, 98, MITADLINES-16, MITADCOLS-49);
    //3
    WINDOW* pregunta = crear_ventana(4, 98, 4, MITADCOLS-49);
    wmove(pregunta, 1, 1);
    WINDOW* opA = crear_ventana(5, 10, MITADLINES + MITADLINES/2, MITADCOLS - MITADCOLS/2-5);
    WINDOW* opB = crear_ventana(5, 10, MITADLINES + MITADLINES/2, MITADCOLS-5);
    WINDOW* opC = crear_ventana(5, 10, MITADLINES + MITADLINES/2, MITADCOLS + MITADCOLS/2-5);
    WINDOW* respuestas = crear_ventana(5, 27, MITADLINES-2, MITADCOLS-13);

    //VENTANAS PARA LA PAUSA
    WINDOW* menuPausa = crear_ventana(4, 13, MITADLINES-4, MITADCOLS-6);
    box(menuPausa, '#', '*');
    WINDOW* textoPausa = crear_ventana(7, 53, 3, MITADCOLS-25);
    refresh();

    wattr_on(textoInnings, A_BOLD | COLOR_PAIR(3), "");
    wattr_on(textoPausa, A_BOLD | COLOR_PAIR(2), "");

    halfdelay(2); //inicio de la detección de eventos para el juego
    while(innings != 10 && escape != true){
        fflush(stdin);
        tecla = getch();
        switch(tecla){
            case 87: //W
                if(activarMenu == true){
                    if(opcionPausa != 0)
                        opcionPausa--;
                    else
                        opcionPausa = 1;
                    dibujar = true;
                    tecla = NULL;
                }
                break;
            case 65: //A
                if(parteDelJuego == 2){
                    if(opcPregunta == 1)
                        opcPregunta = 3;
                    else
                        opcPregunta--;
                    dibujar = true;
                    wclear(pregunta);
                    wclear(respuestas);
                }
                tecla = NULL;
                break;
            case 83: //S
                if(activarMenu == true){
                    if(opcionPausa != 1)
                        opcionPausa++;
                    else
                        opcionPausa = 0;
                    dibujar = true;
                    tecla = NULL;
                }
                break;
            case 68: //D
                if(parteDelJuego == 2){
                    if(opcPregunta == 3)
                        opcPregunta = 1;
                    else
                        opcPregunta ++;
                    dibujar = true;
                    wclear(pregunta);
                    wclear(respuestas);
                }
                tecla = NULL;
                break;
            case 27: //escape
                clear();
                if(activarMenu == false){
                    activarMenu = true;
                    dibujar = true;
                }
                else{
                    activarMenu = false;
                    wclear(textoPausa);
                    if(parteDelJuego == 0){
                        wclear(textoInnings);
                        wclear(numInnings);
                    }
                    if(parteDelJuego == 1){
                        wclear(textoTurno);
                        wclear(numInnings);
                    }
                    if(parteDelJuego == 2){
                        wclear(pregunta);
                        wclear(opA);
                        wclear(opB);
                        wclear(opC);
                        wclear(opC);
                        wclear(respuestas);
                    }
                    dibujar = true;
                    clear();
                }
                break;
            
            case 97: //a
                if(parteDelJuego == 2){
                    if(opcPregunta == 1)
                        opcPregunta = 3;
                    else
                        opcPregunta--;
                    dibujar = true;
                    wclear(pregunta);
                    wclear(respuestas);
                }
                tecla = NULL;
                break;
            case 100: //d
                if(parteDelJuego == 2){
                    if(opcPregunta == 3)
                        opcPregunta = 1;
                    else
                        opcPregunta ++;
                    dibujar = true;
                    wclear(pregunta);
                    wclear(respuestas);
                }
                tecla = NULL;
                break;
            
            case 410: //redimensión
                clear();
                refresh();
                if(activarMenu == true){
                    mvwin(menuPausa, MITADLINES-4, MITADCOLS-6);
                    mvwin(textoPausa, 3, MITADCOLS-25);
                    wrefresh(menuPausa);
                    wrefresh(textoPausa);
                }
                if(parteDelJuego == 0){
                    mvwin(textoInnings, MITADLINES-16, MITADCOLS-31);
                    mvwin(numInnings, MITADLINES+4, MITADCOLS-5);
                }
                if(parteDelJuego == 1){
                    mvwin(textoTurno, MITADLINES-16, MITADCOLS-49);
                    mvwin(numInnings, MITADLINES+4, MITADCOLS-5);
                }
                if(parteDelJuego == 2){
                    mvwin(pregunta, 4, MITADCOLS-49);
                    mvwin(opC, MITADLINES + MITADLINES/2, MITADCOLS + MITADCOLS/2-5);
                    mvwin(opB, MITADLINES + MITADLINES/2, MITADCOLS-5);
                    mvwin(opA,  MITADLINES + MITADLINES/2, MITADCOLS - MITADCOLS/2-5);
                    mvwin(respuestas, MITADLINES-2, MITADCOLS-13);
                }
                activarMenu = true;
                tecla = NULL;
                dibujar = true;
                break;

            case 119: //letra w
                if(activarMenu == true){
                    if(opcionPausa != 0)
                        opcionPausa--;
                    else
                        opcionPausa = 1;
                    dibujar = true;
                    tecla = NULL;
                }
                break;

            case 115: //letra s
                if(activarMenu == true){
                    if(opcionPausa != 1)
                        opcionPausa++;
                    else
                        opcionPausa = 0;
                    dibujar = true;
                    tecla = NULL;
                }
                break;
            
            case 10: //enter
                if(activarMenu == true){
                    if(opcionPausa == 1)
                        escape = true;
                    else{
                        activarMenu = false;
                        wclear(textoPausa);
                        if(parteDelJuego == 0){
                            wclear(textoInnings);
                            wclear(numInnings);
                        }
                        else if(parteDelJuego == 1){
                            wclear(textoTurno);
                            wclear(numInnings);
                        }
                        else if(parteDelJuego == 2){
                            wclear(pregunta);
                            wclear(opA);
                            wclear(opB);
                            wclear(opC);
                            wclear(respuestas);
                        }
                    }
                    clear();
                    dibujar = true;
                }
                else{
                    if(parteDelJuego == 2){
                        respondio = true;
                        if(opcPregunta == resCorrecta){
                            if(turno == 1)
                                jugador1.puntos++;
                            else
                                jugador2.puntos++;
                        }
                        else
                            equivocado = true;
                    }
                }
                tecla = NULL;
                refresh();
                break;
            
            default:
                tecla = NULL;
                break;
            }
        if(tecla == NULL){
            if(activarMenu == true){ //menu
                if(dibujar == true){ 
                    mvwin(menuPausa, MITADLINES-4, MITADCOLS-6); 
                    wclear(menuPausa);
                    box(menuPausa, '#', '*');
                    lecturaFicheros("./archivos_texto/sprites.txt", textoPausa, '{', "21");
                    wrefresh(textoPausa);
                    for(int i = 0; i <= 1; i++){ //remarcar las opciones
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
                switch(parteDelJuego){
                    case 0: //se reproduce animación que indica qué inning se está jugando
                        if(ciclos == 0 || dibujar == true){
                            lecturaFicheros("./archivos_texto/sprites.txt", textoInnings, ';', "07");
                            lecturaFicheros("./archivos_texto/sprites.txt", numInnings, ';', str);
                            wrefresh(textoInnings);
                            wrefresh(numInnings);
                            dibujar = false;
                        }
                        ciclos++;
                        if(ciclos != 14){
                            if(ciclos % 2 != 0){
                                wclear(numInnings);
                                wrefresh(numInnings);
                            }
                            else{
                                lecturaFicheros("./archivos_texto/sprites.txt", numInnings, ';', str);
                                wrefresh(numInnings);
                            }
                            usleep(125000);
                        }
                        else{
                            str[1] += 1;
                            parteDelJuego = 1;
                            ciclos = 0;
                            wclear(textoInnings);
                            wclear(numInnings);
                            clear();
                        }
                        break;

                    case 1: //se reproduce animación que indica turno de jugador 1 o 2
                        if(ciclos == 0 || dibujar == true){
                            if(turno == 1){
                                wattr_on(textoTurno, A_BOLD | COLOR_PAIR(1), "");
                                lecturaFicheros("./archivos_texto/sprites.txt", numInnings, ';', "10"); //en este caso numInnings se utiliza 
                                                                                                        //para marcar el turno correspondiente
                            }
                            else{
                                wattr_on(textoTurno, A_BOLD | COLOR_PAIR(2), "");
                                lecturaFicheros("./archivos_texto/sprites.txt", numInnings, ';', "11");
                            }
                            lecturaFicheros("./archivos_texto/sprites.txt", textoTurno, ';', "22");
    
                            wrefresh(numInnings);
                            wrefresh(textoTurno);
                            dibujar = false;
                        }
                        ciclos++;
                        if(ciclos != 4){
                            usleep(250000);
                        }
                        else{
                            parteDelJuego = 2;
                            ciclos = 0;
                            wclear(textoTurno);
                            wclear(numInnings);
                            clear();
                        }
                        break;
                    case 2: //se hacen las preguntas
                        if(ciclos == 0 || dibujar == true){
                            if(ciclos == 0){
                                if(preguntasRevueltas[indicePreguntas] / 10 != 0)
                                    strPreguntas[0] += preguntasRevueltas[indicePreguntas] / 10;
                                strPreguntas[1] += preguntasRevueltas[indicePreguntas] % 10;
                            }

                            wmove(pregunta, 1, 2);
                            resCorrecta = escoger_respuestas(lecturaFicheros("./preguntas/preguntas.txt", pregunta, ';', strPreguntas), respuestas);
                            box(opA, '|', '-');
                            box(opB, '|', '-');
                            box(opC, '|', '-');
                            box(pregunta, '|', '~');
                            wmove(opA, 2, 5);
                            wmove(opB, 2, 5);
                            wmove(opC, 2, 5);
                            wprintw(opA, "A");
                            wprintw(opB, "B");
                            wprintw(opC, "C");
                            if(opcPregunta == 1){
                                wbkgd(opA, COLOR_PAIR(5));
                                wbkgd(opB, COLOR_PAIR(0));
                                wbkgd(opC, COLOR_PAIR(0));
                            }
                            else if(opcPregunta == 2){
                                wbkgd(opB, COLOR_PAIR(5));
                                wbkgd(opA, COLOR_PAIR(0));
                                wbkgd(opC, COLOR_PAIR(0));
                            }
                            else if(opcPregunta == 3){
                                wbkgd(opC, COLOR_PAIR(5));
                                wbkgd(opA, COLOR_PAIR(0));
                                wbkgd(opB, COLOR_PAIR(0));
                            }
                            wrefresh(opA);
                            wrefresh(pregunta);
                            wrefresh(opB);
                            wrefresh(opC);
                            dibujar = false;
                        }
                        ciclos++;
                        if(ciclos != ciclosJuego){
                            usleep(55000);
                        }
                        if(ciclos == ciclosJuego || respondio == true){
                            if(ciclos == ciclosJuego && respondio == false)
                                equivocado = true;
                            respondio = false;
                            strPreguntas[0] = '0';
                            strPreguntas[1] = '0';
                            ciclos = 0;
                            wclear(pregunta);
                            wclear(opA);
                            wclear(opB);
                            wclear(opC);
                            wclear(respuestas);
                            clear();
                            if(turno == 1){
                                if(vecesJugadas != 4){
                                    parteDelJuego = 2;
                                    vecesJugadas++;
                                }
                                if(vecesJugadas == 4 || equivocado == true){
                                    turno = 2;
                                    vecesJugadas = 1;
                                    equivocado = false;
                                    parteDelJuego = 1;
                                }
                            }
                            else{
                                if(vecesJugadas != 4){
                                    parteDelJuego = 2;
                                    vecesJugadas++;
                                }
                                if(vecesJugadas == 4 || equivocado == true){
                                    turno = 1;
                                    vecesJugadas = 1;
                                    parteDelJuego = 0;
                                    innings++;
                                    ciclosJuego -= 4;
                                    equivocado = false;
                                }
                            }
                            indicePreguntas++;
                            ciclos = 0;
                            refresh();
                            if(innings == 10)
                                parteDelJuego = 3;
                            clear();
                        }
                        break;
                }
            }
        }
    }
    if(jugador1.puntos != jugador2.puntos){
        move(3, MITADCOLS - 10);
        if(jugador1.puntos > jugador2.puntos){
            printw("¡Felicidades, %s!", jugador1.nombre);
            attron(A_BOLD | COLOR_PAIR(1));
        }
        else{
            printw("¡Felicidades, %s!", jugador2.nombre);
            attron(A_BOLD | COLOR_PAIR(2));
        }
    }
    else{
        move(3, MITADCOLS - 10);
        attron(A_BOLD | COLOR_PAIR(3));
        printw("¡Ha sido un empate!");
    }
    move(8, MITADCOLS-7);
    printw("N. de carreras:\n");
    move(10, MITADCOLS-6);
    printw("Jugador 1: %i", jugador1.puntos);
    move(12, MITADCOLS-6);
    printw("Jugador 2: %i", jugador2.puntos);
    refresh();
    usleep(6000000);
    clear();
    refresh();
}

void pantallas_info(int opcion)
{
    WINDOW* texto = crear_ventana(35, 65, 3, MITADCOLS - 32);
    refresh();
    if(opcion == 1)
        lecturaFicheros("./archivos_texto/instrucciones.txt", texto, ';', "00");
    else
        lecturaFicheros("./archivos_texto/creditos.txt", texto, ';', "00");
    wrefresh(texto);
    int tecla;
    while((tecla = wgetch(texto)) != 27){
        continue;
    }
    destruir_ventana(texto);
    clear();
}

int escoger_respuestas(int bytesCursor, WINDOW* ventana)
{
    FILE* archivo = fopen("./preguntas/preguntas.txt", "r");
    if(archivo == NULL){
        endwin();
        printf("No se pudo a acceder al archivo de texto \"preguntas.txt al tratar de escoger respuestas.\"");
        exit(1);
    }
    char correcta;
    char crctr, crctrApoyo;

    fseek(archivo, bytesCursor+1, SEEK_SET);

    while((crctr = fgetc(archivo)) != ']'){
        if(crctr == '1' || crctr == '2' || crctr == '3'){
            crctrApoyo = crctr;
            crctr = fgetc(archivo);
            if(crctr == ':')
                correcta = crctrApoyo;
            wprintw(ventana, "\nA:");
        }
        else if(crctr != ':' || crctr != ';'){
            wprintw(ventana, "%c", crctr);
        }
    }
    refresh();
    wrefresh(ventana);
    int resCorrecta = correcta - 48;
    fclose(archivo);

    return resCorrecta;
}