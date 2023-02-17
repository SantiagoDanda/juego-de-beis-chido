#include <ncurses.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>




void obtenerPreguntas(char pregunta[100], int fila)
{
    int  i;
    FILE *arch;
    // llenando el arreglo de \0
    for (i=0;i<100;i++) {
        pregunta[i] = '\0';
    }
    //hacer archivo de preguntas
    arch=fopen("preguntas/preguntas.txt", "r");
    if(!arch)
        printw("No se pudieron cargar las preguntas");

    //Guardar el renglón en pregunta
    for (i=0;i<fila;i++)
    {
        fgets(pregunta, 100,arch);
    }
    //Procurar que el punto y coma y demás caracteres sean nulos
    for(i=0;i<100;i++)
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

void obtenerOpciones(char opA[50], char opB[50], char opC[50], char opD[50],int fila) {
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
    for (i = 0; i < fila; i++) {
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


int triviaPantalla(int preguntado[40], int pasada, int dificultad)
{
    char opcionA[50], opcionB[50], opcionC[50], opcionD[50], pregunta[300];
    int coordenadasOp[4]={17, 18, 19, 20}, coordenada, coordenada1, coordenada2, coordenada3, coordenada4, turno=1;
    int coordenada1p, coordenada2p, coordenada3p, coordenada4p;
    int fila, correcto=0, i=0, numPregunta=0, limite;
    int repetido;
    //Elegir aleatoriamente la regunta
    do {
      srand(time(NULL));
      fila = rand()%40; 
      if(fila<15)
      {
        dificultad=1;
      }else if(fila<30)
      {
        dificultad=2;
      }else{
        dificultad =3;
      }
      repetido=0;
      limite = pasada;
      //Procurar que no haya pasado anteriormente
       for (i=0;i<=limite;i++) {
        if(preguntado[i] == fila)//NO se repitio la pregunta
        {
          repetido=1;
        }
        else{
          //Guarda que esa pregunta ya no se puede utilizar despues 
          preguntado[pasada++]=fila;
        }
      }  
    } while(repetido==1);
    i=0;
    obtenerPreguntas(pregunta, fila);
    //mostrar pregunta optenida
    mvprintw(10,20, pregunta);
    mvprintw(17,17, "A)");
    mvprintw(18,17, "B)");
    mvprintw(19,17, "C)");
    mvprintw(20,17, "D)");
    obtenerOpciones(opcionA, opcionB, opcionC, opcionD, fila);
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

int mainTrivia()
{  
  int preguntado[40];
  preguntado[0] = 90;
  int pasada=0;
  int fin = 0, 

      puntoEnElJuego = 1, 
      redibujar=0,minijuego=0, 
      tiempo=1, x1 = 0, 
      direccion = 1, 
      a = 20, 
      
      dificultad=0;

  int correcta=0, 
  
      respuesta=0, 
      cambioPregunta=1, 
      Preguntascorrectas=0, 
      jugando=0, 
      i=0, 

      opcion=0, 
      strike=0, base1=0,base2=0, puntaje1=0, puntaje2=0;
      
  char tecla='a', turno='1';
  srand(time(NULL));
  initscr(); 
  keypad(stdscr, TRUE);
  noecho();
  cbreak();
  while (fin==0 && base1<3 && base2<3)
  {

    mvprintw(29,29, "Turno %c", turno);
    /*
    if(opcion!=8)
    {
      correcta=triviaPantalla(preguntado, pasada, dificultad);
      refresh();       
    }*/
    mvprintw(0,0,"      :v    ");
    tecla=getch(); 
    
    //mvprintw(16,16, "%c", tecla);
    //refresh(); 
    switch(tecla)
    {
      case 'a':
      {
        opcion=0;
        break;
      }
      case 'b':
      {
        opcion=1;
        break;
      }
      case 'c':
      {
        opcion=2;
        break;
      }
      case 'd':
      {
        opcion=3;
        break;
      }
      default:
      {
        mvprintw(0,0,"inválido");
        opcion=8;
      }
    }
    /*
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
    
    }*/
    getch();
    clear(); 
  }
  endwin();  
           
  return 0;
}
