#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include "ventanasYFicheros.c"
//#include "trivia.c"

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

void obtenerPreguntas(char pregunta[100], int filaRand)
{
    int  i;
    FILE *arch;
    // llenando el arreglo de \0
    for (i=0;i<100;i++) {
        pregunta[i] = '\0';
    }
    //abrir y archivo de preguntas
    arch=fopen("preguntas/preguntas.txt", "r");
    if(!arch)
      printw("No se pudieron cargar las preguntas");

    //Guardar el renglón en pregunta
    for (i=1;i<=filaRand;i++)//Se desplazaria n cantidad de renglones que no nos interesan, porque
    //lo que nos interesa en la fila "filaRand"
    {
      fgets(pregunta, 100,arch);//tal vez aqui sea sizeof(pregunta) en vez de "100"
    }

    //Procurar que el punto y coma y demás caracteres sean nulos
    for(i=0;i<100;i++)//UNa vez encontrada la pregunta deseada, borra (\0) la basura
    //de los demas caracteres despues del ;
    {
        if(pregunta[i] == ';')
        {
            while(i<100)
            {
              pregunta[i]='\0';
              i++;
            }
        }
    }
    fclose(arch);
}

void obtenerOpciones(char opA[50], char opB[50], char opC[50], char opD[50],int filaRand) {
    //A siempre es la correcta
    //Las cordenadas donde se muestran de manera aleatoria
    int o = 0, u = 0, i;
    FILE *arch;
    char opciones[300];

    //llenando el arreglo de \0
    for (i = 0; i < 300; i++) {
      opciones[i] = '\0';
    }
    //cargar archivos de respuestas
    arch = fopen("preguntas/respuestas.txt", "r");
    if (!arch)
      printw("No se pudieron cargar las opciones");

    //Obtener el renglon de opciones
    for (i = 1; i <= filaRand; i++) {
        fgets(opciones, 300, arch);
    }

    //opcion A
    while (opciones[o] != ';') {
        opA[u] = opciones[o];
        u++;
        o++;
    }
    o++;
    opA[u] = '\0';
    u = 0;
     //opcion B
    while (opciones[o] != ';') {
        opB[u] = opciones[o];
        u++;
        o++;
    }
    o++;
    opB[u] = '\0';
    u = 0;
     //opcion C
    while (opciones[o] != ';') {
        opC[u] = opciones[o];
        u++;
        o++;
    }
    o++;
    opC[u] = '\0';
    u = 0;
  //opcion D
    while (opciones[o] != ';') {
        opD[u] = opciones[o];
        u++;
        o++;
    }
    opD[u] = '\0';
    fclose(arch);
}


int triviaPantalla(int preguntado[40], int *numPreguntasPasadas, int *ultimaPreguntaRegistrada)
//correcta=triviaPantalla(preguntado, numPreguntasPasadas, dificultad);//asignacion de la respuesta correcta 
  //y captura de pregutnas y respuestas, asi como su impresion
/*
  preguntado: un arreglo de enteros que contiene las preguntas que ya se preguntaron 
  anteriormente para evitar preguntar dos veces la misma pregunta.
  numPreguntasPasadas: la cantidad de preguntas que ya se preguntaron anteriormente.
  dificultad: un entero que representa la dificultad de la pregunta actual.
*/
{
    int dificultad;

    char opcionA[50], opcionB[50], opcionC[50], opcionD[50], pregunta[300]; //Varuables para guardar el texto (caracteres) del archivo
    int coordenadasOp[4]={17, 18, 19, 20}, 
        coordenada, //son variables que sirven de
    //indices para usarlos en arreglos que le daran una posicion aleatoria 
    //el posicionamiento de las respuestas
        coordenada1, 
        coordenada2, coordenada3, coordenada4, turno=1;
    int coordenada1p, 
        coordenada2p, coordenada3p, coordenada4p;
    int filaRand, correcto=0, i=0, numPregunta=0, limite;
    
    
    int repetido;// *Sirve para saber, por medio de valores de 1 y 0, si la pregunta se repitio o no
    //se usa para mantenerse o salirse de un while

    //Elegir aleatoriamente la pregunta
    do {
      srand(time(NULL));
      filaRand = rand()%17; //rand()%40+1
      if(filaRand<15)//if de asignacion de dificultad segun la fila en la que se encuentren
      {
        dificultad=1;
      }else if(filaRand<30)
      {
        dificultad=2;
      }else{
        dificultad =3;
      }
      repetido=0; //para saber si se repitio la pregunta o no

      limite = numPreguntasPasadas;

      //Procurar que no haya pasado anteriormente
       for (i=0;i<=limite;i++) {
        if(preguntado[i] == filaRand)//SI se repitio la pregunta, por lo que se tiene que volver a repetir el ciclo
        {
          repetido=1; //Se le asgina 1 para que mantenerse en el while volviendo a generar una pregunta aleatoria
        }
        else{
          //NO se repitio la pregunta, por lo que puede proceder a trollear (utlizarla)
          //Guarda que esa pregunta ya no se puede utilizar despues 
          preguntado[numPreguntasPasadas++]=filaRand; //AGUAS, porque puede ser que sea ++numPreguntasPasadas
        }
      }  
    } while(repetido==1);


    i=0;
    obtenerPreguntas(pregunta, filaRand);
    //mostrar pregunta optenida
    mvprintw(10,20, pregunta);
    mvprintw(17,17, "A)");
    mvprintw(18,17, "B)");
    mvprintw(19,17, "C)");
    mvprintw(20,17, "D)");
    obtenerOpciones(opcionA, opcionB, opcionC, opcionD, filaRand);


    //Las opciones se mostrarán en un orden aleatorio
    //coordenada de la opcion a
    srand(time(NULL));
    coordenada1=rand() % 4;
    coordenada1p=coordenadasOp[coordenada1];
    mvprintw(coordenada1p,20, opcionA);


    //coordenada de la opcion b
    do{
      srand(time(NULL));
      coordenada2 = rand() % 4;
    }while (coordenada2 == coordenada1) ;
    coordenada2p=coordenadasOp[coordenada2];
    mvprintw(coordenada2p,20, opcionB);
    //coordenada de la opcion c
    do{
       srand(time(NULL));
      coordenada3 = rand() % 4;
    }while (coordenada3 == coordenada1 || coordenada2 == coordenada3) ;
    coordenada3p=coordenadasOp[coordenada3];
     mvprintw(coordenada3p,20, opcionC);
    //coordenada de la opcion d
      do{
      srand(time(NULL));
      coordenada4 = rand() % 4;
    }while (coordenada4 == coordenada1 || coordenada2 == coordenada4 || coordenada3 == coordenada4 ) ;
    coordenada4p=coordenadasOp[coordenada4];
   mvprintw(coordenada4p,20, opcionD);
   


     mvprintw(100,1, coordenada1);
    return coordenada1;
}

// -------------------------------------------->    TRANSICIONES // ANIMACIONES     <----------------------------------------------
/*
void ANIMACION_bateador()
    {
        WINDOW* bateador = crear_ventana(20, 60, LINES/2-7, COLS/2-30);
        wattr_on(bateador, A_BOLD | COLOR_PAIR(3), "");
        
        for(int i = 1; i <= 3; i++){ //ir pasando los sprites del bateador
            str[1] += 1;
            lecturaFicheros("./archivos_texto/sprites.txt", bateador, ';', str);
            wrefresh(bateador);
            usleep(500000);
            wclear(bateador);
        }
        destruir_ventana(bateador);
    }
*/
// ------------------------------------------------------------------------------------------

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

int menu()//COMPORTAMIENTO DEL MENU PRINCIPAL (EVENTOS MENU PRINCIPAL)
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

    int opc = 0, tecla, redimension = 1;
    char opciones[5][14] = { "Jugar", //opc 0
                            "Instrucciones", //opc 1
                            "Puntuaciones", //opc 2
                            "Créditos", //opc 3
                            "Salir" //op4
                            };

    do{
        if (tecla == 87)//W
        {
            tecla = 119;//w
        }
        else if (tecla == 83)//S
        {
            tecla = 115;//s
        }
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
    init_pair(3, COLOR_MAGENTA, COLOR_BLACK);

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

    //Peticion de nombres
    for(int i = 1; i <= 2; i++){
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

    


    char str[2] = {'0', '0'}; //sirve para manejar el inicio de la lectura del archivo sprites.txt


//----------------------> ANIMACION // TRANSION LOCAL <----------------------------------------
    WINDOW* bateador = crear_ventana(20, 60, LINES/2-7, COLS/2-30);
    wattr_on(bateador, A_BOLD | COLOR_PAIR(3), "");
    for(int i = 1; i <= 3; i++){ //ir pasando los sprites del bateador
        str[1] += 1;
        lecturaFicheros("./archivos_texto/sprites.txt", bateador, ';', str);
        wrefresh(bateador);
        usleep(500000);
        wclear(bateador);
    }
    destruir_ventana(bateador);
//------------------------------------------------------------------------------------------------------


//Inicializa variables importantes

    
    int parteDelJuego = 2; /*esta variable indicará qué momento del juego se está ejecutando (si se está
                            preguntando, si se está ejecutando una animación, etc)*/
                            /*
                            0=
                            1=
                            2? juego y EVENTOS  de preguntas y respuestas
                            */

    int innings = 1, opcionPausa = 0, 
        ciclos = 0; //sirve para controlas las ANIMACIONES y su momento en pausa
    bool escape = false, activarMenuPausa = false, dibujar = false;
    char botonesPausa[2][9]= {
                                "Regresar",
                                "Salir"
                                };
    str[1] = '0';
    str[0] = '1'; //sirve para controlar los sprites del número de innings
    int tecla;
    
    //VENTANAS PARA EL JUEGO
    WINDOW* textoInnings = crear_ventana(7, 63, LINES/2-16, COLS/2-31);
    WINDOW* numInnings = crear_ventana(7, 10, LINES/2+4, COLS/2-5);
    //VENTANAS PARA LA PAUSA
    WINDOW* menuPausa = crear_ventana(4, 13, LINES/2-4, COLS/2-6);
    box(menuPausa, '#', '*');
    WINDOW* textoPausa = crear_ventana(7, 53, 3, COLS/2-25);
    refresh();

    wattr_on(textoInnings, A_BOLD | COLOR_PAIR(3), "");
    wattr_on(textoPausa, A_BOLD | COLOR_PAIR(2), "");

    halfdelay(2); //inicio de la detección de eventos para el juego
    //Asigna un valor de ERR, osea de -1 si no se ha mandado nada por el usuario despues de esperar 2 (décimas de segundo?)


    int preguntado[40], indice,
        //dificultad=0;
    int numPreguntasPasadas=0;

    int ultimaPreguntaRegistrada=0;


    for (indice = 0; indice < 40; indice++)
    {
        preguntado[indice] = NULL;
    }
  //preguntado[0] = 90;
 
  int fin = 0, 

      puntoEnElJuego = 1, 
      redibujar=0,minijuego=0, 
      tiempo=1, x1 = 0, 
      direccion = 1, 
      a = 20; 
      
      

  int correcta=0, 
  
      respuesta=0, 
      cambioPregunta=1, 
      Preguntascorrectas=0, 
      jugando=0, 
      i=0, 

      opcion=1, //Para indicar que tipo de tecla presiono en PREGUNTAS
      strike=0, base1=0,base2=0, puntaje1=0, puntaje2=0, 
      turno='1';





      int verifTecla = 0;


srand(time(NULL));



    //AÑadir cambio por mayusculas
    while(innings != 9 && escape != true && fin==0 /*&& base1<3 && base2<3*/){
        tecla = getch(); // Deteccion de eventos
        
        if (tecla == 87)
        {
            tecla = 119;
        }
        else if (tecla == 83)
        {
            tecla = 115;
        }
        else if (tecla == 65)
        {
            tecla = 97;
        }
        else if (tecla == 66)
        {
            tecla = 98;
        }
        else if (tecla == 67)
        {
            tecla = 99;
        }
        else if (tecla == 68)
        {
            tecla = 100;
        }
        
        if (tecla == 'W')
        {
            tecla = 'w';
        }
        else if (tecla == 'S')
        {
            tecla = 's';
        }
        else if (tecla == 'A')
        {
            tecla = 'a';
        }
        else if (tecla == 'B')
        {
            tecla = 'b';
        }
        else if (tecla == 'C')
        {
            tecla = 'c';
        }
        else if (tecla == 'D')
        {
            tecla = 'd';
        }

        switch(tecla){
        case 27: //escape
            clear();
            if(activarMenuPausa == false){
                activarMenuPausa = true;
                dibujar = true;
            }
            else{ 
                activarMenuPausa = false;
                wclear(textoPausa);
                wclear(textoInnings);
                wclear(numInnings);
                dibujar = true;
                clear();
            }
            break;
        
        case 410: //redimensión --> ??
            clear();
            refresh();
            if(activarMenuPausa == true){
                mvwin(menuPausa, LINES/2-4, COLS/2-6);
                mvwin(textoPausa, 3, COLS/2-25);
                wrefresh(menuPausa);
                wrefresh(textoPausa);
            }
            else{
                if(parteDelJuego == 0){
                    mvwin(textoInnings, LINES/2-16, COLS/2-31);
                    mvwin(numInnings, LINES/2+4, COLS/2-5);
                    wrefresh(textoInnings);
                    wrefresh(numInnings);
                }
            }
            break;
        case 97:// a
            opcion = 1;
            verifTecla = 1;
         
    
            break;
        case 98:// b
            opcion = 2;
            verifTecla = 1;
         
    
            break;
        case 99:// c
            opcion = 3;
            verifTecla = 1;
          
    
            break;
        case 100:// d
            opcion = 4;
            verifTecla = 1;
          
    
            break;

    


        case 119: //letra w
            if(activarMenuPausa == true){
                if(opcionPausa != 0)
                    opcionPausa--;
                else
                    opcionPausa = 1;
                dibujar = true;
                
            }
            tecla = NULL;
            
            break;

        case 115: //letra s
            if(activarMenuPausa == true){
                if(opcionPausa != 1)
                    opcionPausa++;
                else
                    opcionPausa = 0;
                dibujar = true;
                
            }
            tecla = NULL;
            
            break;
        
        case 10: //enter
            if(activarMenuPausa == true){
                if(opcionPausa == 1)
                    escape = true;
                else{
                    activarMenuPausa = false;
                    wclear(textoPausa);
                    wclear(textoInnings);
                    wclear(numInnings);
                    dibujar = true;
                }
                clear();
            }
            break;
        
        default:
            if((tecla != 97 && tecla != 98 && tecla != 99 && tecla != 100)  && tecla != ERR) // ERR = -1
            {
                verifTecla = -1;
                opcion=0;
            }
            
            if((tecla == 119 || tecla == 115) && activarMenuPausa == false)//ACTUALMENTE NO SIRVE 
            {
                verifTecla = -1;
            }
            
            /*
            else if (tecla == 'a' && tecla == 'b' && tecla == 'c' && tecla == 'd')
            {
                verifTecla = 1;
            }*/
            tecla = NULL;
            

            //opcion=0;
            break;
        }
        if(tecla == NULL || verifTecla ==1){ /* tecla = NULL -> 1° Filtro para autorizar el dibujo de SPRITES (CUando no hay eventos)
                                                verifTecla sirve igual de fltro para que permita entrar al dibujo sin que entre el conflicto
                                                con el NULL de 'tecla'
                                                */
            if(activarMenuPausa == true){ //MENU DE PAUSA
                if(dibujar == true){ 
                    mvwin(menuPausa, LINES/2-4, COLS/2-6); 
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
            else{ //JUEGO 
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
                        if(ciclos != 7){ //CUando ya acabaron los ciclos, (acabo la ANIMACION)
                            usleep(250000); // 2 segundos y medio
                            //usleep(100000); //1 segundo

                        }
                        else{
                            str[1] += 1;
                            //parteDelJuego = 1;
                            ciclos = 0;
                            wclear(textoInnings);
                            wclear(numInnings);
                            clear();
                        }
                        break;

                    case 1: //se reproduce animación que indica turno de jugador 1 o 2
                        break;
                    case 2: //se hacen las preguntas
                        if(verifTecla == -1)
                        {
                            //opcion = 0;
                            verifTecla = 0;
                        }

                        if(opcion > 0 && opcion <= 4 && tecla != ERR)//Generar una nueva preguntaaaaa
                        {
                            opcion = -1;
                            mvprintw(0,0,"                                  ");
                            mvprintw(29,29, "Turno %c", turno);
                            correcta=triviaPantalla(preguntado, &apu_numPregPasadas, &apu_ultimaPregReg);//asignacion de la respuesta correcta                     
                            printw("%i",correcta);
                            //y captura de pregutnas y respuestas, asi como su impresion                        
/*
                            preguntado: un arreglo de enteros que contiene las preguntas que ya se preguntaron 
                            anteriormente para evitar preguntar dos veces la misma pregunta.
                            numPreguntasPasadas: la cantidad de preguntas que ya se preguntaron anteriormente.
                            dificultad: un entero que representa la dificultad de la pregunta actual.
*/
                            
/*
                            mvprintw(10,0,"      :v    ");// PRUEBA
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
                                    }
                                    else{
                                        turno='2';
                                    }        
                                }
                            
                            }
                            getch();
                            clear();*/ 
                           
                        }    
                        else if(opcion == 0)
                        {
                            //clear();
                            mvprintw(0,0,"inválido :v");
                            //opcion = 0;
                            verifTecla = -1;
                            
                        }  
                        /*
                        else if(opcion == -1)
                        {

                        }*/
                        
                        

                        refresh(); 
                        break;
                    case 3:
                        break;
                }
            }
        }
        
        
        //TAl vez las PUNTUACIONES
        //Guardar las PUNTUACIONES en el archivo de texto


    }
    clear();
    destruir_ventana(menuPausa);
    destruir_ventana(salir);
    destruir_ventana(textoPausa);
    destruir_ventana(textoInnings);
    refresh();
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
    destruir_ventana(texto);
}
