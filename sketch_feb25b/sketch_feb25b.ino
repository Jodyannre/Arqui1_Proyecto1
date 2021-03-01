#include <LedControl.h> //Librería para el control de las matrices



//#####*****************************DEFINICIÓN DE VARIABLES GLOBALES*****************************#####




LedControl matrix = LedControl(12, 11, 10, 4); //Variable para controlar la matrices
int vel_scroll = 0;



//#####*****************************DEFINICIÓN DE VARIABLES GLOBALES*****************************#####


//#####*****************************BOTÓN DE LOGIN*****************************#####
const int btnIn1 = 2; //boton de prueba pasar a pantalla 2 comentar cuando ya no sea necesario
int btnIn1State = LOW; // boton prueba
int contadorMatriz = 0; //contador para insertar en las diferentes posiciones del array las filas de diseño
String instruccion = ""; // recibe instruccion
char opcion; // concatena entrada
int contador = 0; //contador para concatenar instrucciones
//#####*****************************BOTÓN DE LOGIN*****************************#####



byte UNO[8] {B00000000, B00011000, B00011000, B00111000, B00011000, B00011000, B00011000, B01111110};
byte DOS[8] {B00000000, B00111100, B01100110, B00000110, B00001100, B00110000, B01100000, B01111110};
byte TRES[8] {B00000000, B00111100, B01100110, B00000110, B00011100, B00000110, B01100110, B00111100};


//####****************************VARIABLES PARA RECIBIR DATOS***************************#####

int producto[8];

//#####*****************************VARIABLES PARA RECIBIR DATOS*************************#####



//####****************************VARIABLES DE ESTADOS DE LAS MATRICES***************************#####

boolean mt1 = false;
boolean mt2 = false;
boolean mt3 = false;


boolean datosRecibidos = false;
int contadorTemp = 0;
int contadorLugar = 1;

//####****************************VARIABLES DE ESTADOS DE LAS MATRICES***************************#####



//#####*****************************INICIALIZACION DE ARDUINO*****************************#####

void setup() {
  Serial.begin(9600);
  Serial3.begin(9600);
  pinMode(btnIn1, INPUT);
  for (int i = 0; i < 4; i++) {
    matrix.shutdown(i, false);
    //Configurar brillo
    matrix.setIntensity(i, 8);
    //Limpiar la matriz
    matrix.clearDisplay(i);
  }
}


//#####*****************************INICIALIZACION DE ARDUINO*****************************#####



//#####*****************************LOOP PRINCIPAL*****************************#####

void loop() {

  //--------------------------VARIABLES LECTURA SERIAL Y OPCION
  instruccion = "";
  opcion = '\0';
  contador = 0;
  //--------------------------VARIABLES LECTURA SERIAL Y OPCION

  //--------------------------LECTURA SERIAL
  while (Serial.available())
  {
    delay(5);
    char c = Serial.read();
    if (contador == 0) {
      opcion = c;//solo guarda el primer caracter indicando el enrutador para mostrar en la matriz es con la letra d
    }
    else {
      instruccion += c;  //aqui almacena cada instruccion
    }
    contador++;
  }
  //--------------------------LECTURA SERIAL
  Serial3.println(instruccion);
  Serial3.println(opcion);


  //--------------------------TIPO DE INSTRUCCION
  switch (opcion) {
    case '<':
      //datos matriz
      delay(1000);
      guardarObjeto(instruccion);
      break;
    case '&':
      //motor
      break;
    case'@':
      //aceptar
      break;
    case'$':
      //emergencia
      break;
    case '#':
      //numero de estacion
      break;
  }
  //--------------------------TIPO DE INSTRUCCION

  if (datosRecibidos) {
    imprimirMatriz(contadorTemp, contadorLugar);
    contadorTemp++;
    contadorLugar++;
    if (contadorLugar == 5) {
      datosRecibidos = false;
    }
  }


  //--------------------------BOTON DE LOGIN

  btnIn1State = digitalRead(btnIn1);


  if (btnIn1State == HIGH) {
    matrix.clearDisplay(0);
    Serial.write("si");
    delay(100);
  }
  //--------------------------BOTON DE LOGIN
}

//#####*****************************LOOP PRINCIPAL*****************************#####







//#####********************************GUARDAR LA MATRIZ DEL OBJETO EN INT********************************####
void guardarObjeto(String cadena) {
  String conca = "";
  int actual = 0;

  for (int i = 0; i < cadena.length(); i++) {
    conca += cadena[i];
    if (cadena[i] == ',') {
      actual = conca.toInt();
      producto[contadorMatriz] = actual;
      conca = "";
      actual = 0;
      contadorMatriz++;
    }
  }
  actual = conca.toInt();
  producto[7] = actual;
  //lc.setRow(0,7,a);
  contadorMatriz = 0;
  actual = 0;
  datosRecibidos = true;
  delay(50);
}

//#####********************************GUARDAR LA MATRIZ DEL OBJETO EN INT********************************####





//#####********************************IMPRESION DE MATRIZ********************************####


void imprimirMatriz(int numeroMatriz, int lugar) {
  int parpadeo = 300;
  int contador = 1;
  //&on  motor
  //&off  motor
  //$on emergencia
  //@on aceptar
  // #2 estación
    if (numeroMatriz == 1) {
      mt1 = true;
    }else if (numeroMatriz==2){
      mt2 = true;
    }else if (numeroMatriz==3){
      mt3 = true;
    }
  letrero();    
  imprimirNumeroEstacion(numeroMatriz, lugar);
  while (contador < 14) {
    //Imprime la matriz dependiendo del número de matriz seleccionado
    if (contador != 1) {
      delay(parpadeo);
      matrix.shutdown(numeroMatriz, false);
      matrix.clearDisplay(numeroMatriz);
      delay(parpadeo);
    }
    matrix.setColumn(numeroMatriz, 0, producto[0]);
    matrix.setColumn(numeroMatriz, 1, producto[1]);
    matrix.setColumn(numeroMatriz, 2, producto[2]);
    if (lugar == 2) {
      
      matrix.setColumn(numeroMatriz, 3, producto[3]);
      matrix.setColumn(numeroMatriz, 4, producto[4]);
      matrix.setColumn(numeroMatriz, 5, producto[5]);

    } else if (lugar == 3 || lugar == 4) {
      
      matrix.setColumn(numeroMatriz, 3, producto[3]);
      matrix.setColumn(numeroMatriz, 4, producto[4]);
      matrix.setColumn(numeroMatriz, 5, producto[5]);
      matrix.setColumn(numeroMatriz, 6, producto[6]);
      matrix.setColumn(numeroMatriz, 7, producto[7]);
    }
    switch (contador) {
      case 3:
        parpadeo = 250;
        break;
      case 5:
        parpadeo = 150;
        break;
      case 7:
        parpadeo = 100;
        break;
      case 9:
        parpadeo = 50;
        break;
      case 11:
        parpadeo = 30;
        break;
    }
    contador++;
  }

}


void imprimirNumeroEstacion(int numeroMatriz, int lugar) {
  if (lugar != 4) {
    switch (numeroMatriz) {
      case 0:
        matrix.setRow(3, 0, UNO[0]);
        matrix.setRow(3, 1, UNO[1]);
        matrix.setRow(3, 2, UNO[2]);
        matrix.setRow(3, 3, UNO[3]);
        matrix.setRow(3, 4, UNO[4]);
        matrix.setRow(3, 5, UNO[5]);
        matrix.setRow(3, 6, UNO[6]);
        matrix.setRow(3, 7, UNO[7]);
        break;
      case 1:
        matrix.setRow(3, 0, UNO[0]);
        matrix.setRow(3, 1, DOS[1]);
        matrix.setRow(3, 2, DOS[2]);
        matrix.setRow(3, 3, DOS[3]);
        matrix.setRow(3, 4, DOS[4]);
        matrix.setRow(3, 5, DOS[5]);
        matrix.setRow(3, 6, DOS[6]);
        matrix.setRow(3, 7, DOS[7]);
        break;
      case 2:
        matrix.setRow(3, 0, UNO[0]);
        matrix.setRow(3, 1, TRES[1]);
        matrix.setRow(3, 2, TRES[2]);
        matrix.setRow(3, 3, TRES[3]);
        matrix.setRow(3, 4, TRES[4]);
        matrix.setRow(3, 5, TRES[5]);
        matrix.setRow(3, 6, TRES[6]);
        matrix.setRow(3, 7, TRES[7]);
        break;
    }
  }
}



//#####********************************IMPRESION DE MATRIZ********************************#####











//#####********************************LETRERO MIENTRAS NO SE USAN LAS ESTACIONES********************************#####
void letrero() { //Método para mostrar el letrero
  mensajeScroll();
}

const PROGMEM bool Message[8][104] = { //Mensaje a mostrar
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};



const PROGMEM bool Mensaje[8][60] =
{
  {0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0},
  {0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0,0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,0},
  {0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0,1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,0},
  {0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0,1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,0},
  {0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0,1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0},
  {0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0,1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,0},
  {0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0},
  {0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0}
};



void mensajeScroll() { //Scroll del texto de izquierda a derecha
  [&] {
    for (int d = 0; d < sizeof(Mensaje[0]) - 7; d++) {
      for (int col = 0; col < 8; col++) {

        for (int row = 0; row < 8; row++) {
          if (!mt1) {
            matrix.setLed(0, row, col, pgm_read_byte(&(Mensaje[row + 1][col + 2 - d])));
          }
          if (!mt2) {
            matrix.setLed(1, row, col, pgm_read_byte(&(Mensaje[row + 1][col + 2 - d])));
          }
          if (!mt3) {
            matrix.setLed(2, row, col, pgm_read_byte(&(Mensaje[row + 1][col + 2 - d])));
          }

        }

      }
    }
    
  }();
          if (!mt1) {
            matrix.clearDisplay(0);
          }
          if (!mt2) {
            matrix.clearDisplay(1);
          }
          if (!mt3) {
            matrix.clearDisplay(2);
          }
}

//#####********************************LETRERO MIENTRAS NO SE USAN LAS ESTACIONES********************************#####
