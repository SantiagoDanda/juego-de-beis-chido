#include <ncurses.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>




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


int triviaPantalla(int preguntado[40], int numPreguntasPasadas, int dificultad)
//correcta=triviaPantalla(preguntado, numPreguntasPasadas, dificultad);//asignacion de la respuesta correcta 
  //y captura de pregutnas y respuestas, asi como su impresion
/*
  preguntado: un arreglo de enteros que contiene las preguntas que ya se preguntaron 
  anteriormente para evitar preguntar dos veces la misma pregunta.
  numPreguntasPasadas: la cantidad de preguntas que ya se preguntaron anteriormente.
  dificultad: un entero que representa la dificultad de la pregunta actual.
*/
{
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

int mainTrivia()//EN un princpio ya no utilizable
{  
  int preguntado[40];
  int indice;
  for (indice = 0; indice < 40; indice++)
  {
    preguntado[indice] = NULL;
  }
  //preguntado[0] = 90;//?
  int numPreguntasPasadas=0;
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
    
    if(opcion!=8)
    {
      correcta=triviaPantalla(preguntado, numPreguntasPasadas, dificultad);
      refresh();       
    }
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
    getch();
    clear(); 
  }
  endwin();  
           
  return 0;
}
