#include <LedControl.h> //Librería para el control de las matrices
#include <Stepper.h>
#include <Key.h>
#include <Keypad.h>
#include <LiquidCrystal.h>
#include <TimerOne.h>

//#####*****************************DEFINICIÓN DE VARIABLES GLOBALES*****************************#####
int vel_scroll = 0; //Variable para controlar la velocidad del mensaje scroll
const String user = "123456"; //Contraseña
const byte rows = 4; //4 filas
const byte cols = 3; //3 columnas
const int btnIn1 = 2; //boton de prueba pasar a pantalla 2 comentar cuando ya no sea necesario
//int btnIn1State = LOW; // boton prueba
int contadorMatriz = 0; //contador para insertar en las diferentes posiciones del array las filas de diseño
int contadorLugar = 1; //Contador para el lugar de impresión de cada matriz
String instruccion = ""; // recibe instruccion
char opcion; // concatena entrada
int contador = 0; //contador para concatenar instrucciones
int producto[8]; //Matriz que guarda el producto a imprimir
int contadorTemp = 0;  //Contador temporal del número de matrices
long tiempoMillis = 0; //Variable para llevar el control del tiempo y poder contar en el reloj
String mensajeEstacion = "ESTACION "; //Mensaje que mostrará la estación actual
// variables temporales para el reloj
int dectemp = 0; //Variable que lleva el conteo de las decimas transcurridas
int mintemp = 0; //Variable que lleva el conteo de los minutos transcurridos
int sectemp = 0; //Variable que lleva el conteo de los segundos transcurridos

int tempInicial = 0; //Inicialización del tiempo en 0
String valor; //Variable que guardará la conversión de la temperatura para transferirla por medio del puerto serial
String conjunto = "";

//Impresiones en matriz de mensajes predefinidos
byte UNO[8] {B00000000, B00011000, B00011000, B00111000, B00011000, B00011000, B00011000, B01111110}; //Número 1 para mostrar en matriz
byte DOS[8] {B00000000, B00111100, B01100110, B00000110, B00001100, B00110000, B01100000, B01111110}; //Número 2 para mostrar en matriz
byte TRES[8] {B00000000, B00111100, B01100110, B00000110, B00011100, B00000110, B01100110, B00111100}; //Número 3 para mostrar en matriz

//Variables booleanas
bool usuariocorrecto = false; //Bool para validación de usuario
static boolean mt1 = false; //Guarda el estado de la matriz 1
static boolean mt2 = false; //Guarda el estado de la matriz 2
static boolean mt3 = false; //Guarda el estado de la matriz 3
boolean datosRecibidos = false; //Determina si hay nuevos datos recibidos
boolean matrizActiva = false; //Determina si alguna la matriz está lista para imprimir
boolean stepperActivo = false; //Determina si el stepper fue activado en la aplicación

//pines de lcd
const int rs = 23;
const int enable = 22;
const int d4 = 38;//d4
const int d3 = 39;//d5
const int d2 = 40;//d6
const int d1 = 41;//d7
//pines de keypad
//filas
const int f1 = 4;
const int f2 = 3;
const int f3 = 2;
const int f4 = 16;
//columnas
const int c1 = 7;
const int c2 = 6;
const int c3 = 5;
byte rowPins[rows] = {f1, f2, f3, f4}; //connect to the row pinouts of the keypad
byte colPins[cols] = {c1, c2, c3}; //connect to the column pinouts of the keypad
char keys[rows][cols] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};



//Creación de estructuras e inicialización de librerias
#define STEPS 100 // creo q entre mas diminuto mas lento es
Keypad teclado = Keypad( makeKeymap(keys), rowPins, colPins, rows, cols );
LiquidCrystal lcd(rs, enable, d4, d3, d2, d1);
LedControl matrix = LedControl(12, 11, 10, 4); //Variable para controlar la matrices
Stepper stepper(STEPS, 53, 52, 51, 50);//defino su stpes, IN1,IN2,IN3,IN4


//Struct para manejar el estado actual de cada matriz
struct msj {
  int d = 0, col = 0, row = 0;
  msj(int d = 0, int col = 0, int row = 0): d(d), col(col), row(row) {}
};
msj msj(0, 0, 0); //Creación del struct msj matriz



//#####*****************************DEFINICIÓN DE VARIABLES GLOBALES*****************************#####





//#####*****************************INICIALIZACION DE ARDUINO*****************************#####

void setup() {
  Serial.begin(9600); //Serial de lectura de datos de entrada
  Serial3.begin(9600); //Serial de lectura de datos de salida desde arduino
  Timer1.initialize(10000000);
  Timer1.attachInterrupt(tiempo); //Inicialización del timer encargado del contador
  lcd.begin(16, 2);

  //Puertos DDR
  DDRK = B11111111;
  DDRL = B11111111;
  DDRC = B11111111;

  //Inicio de tiempo en millis actual
  tiempoMillis = millis();

  pinMode(A0, INPUT);//javier temperatura
  pinMode(A1, OUTPUT);//javier temperatura led green
  pinMode(A2, OUTPUT);//javier temperatura led yellow
  pinMode(A3, OUTPUT);//javier temperatura led red
  pinMode(btnIn1, INPUT);

  //Inicialización de las matrices
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


  if (usuariocorrecto == false) {
    ingreso();
    if (usuariocorrecto) {
      Serial.write("si");
      delay(50);
    }
  }

  else {


    instruccion = ""; //Lectura de instrucción entrante
    opcion = '\0'; //Opción a realizar
    contador = 0; //Contador de la instrucción entrante


    while (Serial.available()) //Lectura serial
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

    //Impresiones de prueba
    //Serial3.println(instruccion);
    //Serial3.println(opcion);


    //Tipo de instrucción a realizar
    switch (opcion) {
      case '<':
        //datos matriz
        delay(5);
        guardarObjeto(instruccion);
        break;
      case '&':
        //Se activa o desactiva el motor
        if (stepperActivo) {
          stepperActivo = false;
        } else {
          stepperActivo = true;
        }
        break;
      case'@':
        //boton aceptar reinicia todo
        //Reiniciar reloj
        reiniciar();
        break;
      case'$':
        //emergencia reiniciar todo
        reiniciar();
        break;
      case '#':
        matrizActiva = true;
        //Serial3.println("Se envio la estación");
        //Serial3.println(instruccion);
        //Serial3.println(instruccion.charAt(0));
        imprimirMatriz(instruccion.charAt(0), contadorLugar);
        contadorLugar++;
        //numero de estacion
        if (contadorLugar > 3) {
          imprimirMatriz('4', contadorLugar);
        }
        matrizActiva = false;
        break;
    }
    //Si no hay nada que hacer entonces imprime matriz
    letrero(); //Muestra msj en matrices que no están siendo utilizadas
  }
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


void imprimirMatriz(char numMatriz, int lugar) {
  mensajeEstacion += numMatriz;
  //lcd.clear();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(mensajeEstacion);
  int numeroMatriz = 0;
  if (numMatriz == '1') {
    numeroMatriz = 0;
  } else if (numMatriz == '2') {
    numeroMatriz = 1;
  } else if (numMatriz == '3') {
    numeroMatriz = 2;
  } else if (numMatriz == '4') {
    numeroMatriz = 3;
  }
  //Serial3.print("Estación: ");
  //Serial3.println(numMatriz);
  //Serial3.println(numeroMatriz);
  int parpadeo = 250;
  int contador = 1;

  if (numeroMatriz == 0) {
    mt1 = true;
  } else if (numeroMatriz == 1) {
    mt2 = true;
  } else if (numeroMatriz == 2) {
    mt3 = true;
  }
  switch (lugar) {
    case 1:
      parpadeo = 5;
      break;
    case 2:
      parpadeo = 5;
      break;
    case 3:
      parpadeo = 100;
      break;
    case 4:
      parpadeo = 100;
      break;
  }
  imprimirNumeroEstacion(numeroMatriz, lugar); //Imprimir como primer lugar el número de estación actual
  while (contador < 5) {
    //Imprime la matriz dependiendo del número de matriz seleccionado
    if (contador != 0) {
      if (numeroMatriz == 0 || numeroMatriz == 1 || numeroMatriz == 2 || numeroMatriz == 3 || numeroMatriz == 4) {
        delay(parpadeo);
        //contarTiempo(); //Verifica el contador del reloj
        mensajeScroll(); //Imprimir un movimiento del mensaje scroll
      }
      mensajeScroll();
      matrix.shutdown(numeroMatriz, false);
      matrix.clearDisplay(numeroMatriz);
      if (numeroMatriz == 0 || numeroMatriz == 1 || numeroMatriz == 2 || numeroMatriz == 3 || numeroMatriz == 4) {
        delay(parpadeo);
        //contarTiempo(); //Verifica el contador del reloj
        mensajeScroll(); //Imprimir un movimiento del mensaje scroll
      }

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
        parpadeo = 250;
        break;
      case 7:
        parpadeo = 250;
        break;
      case 9:
        parpadeo = 250;
        break;
      case 11:
        parpadeo = 250;
        break;
    }
    //contarTiempo();
    contador++;
  }
  if (stepperActivo) { //Si el stepper esta activo lo mueve luego de cada producción
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("MOVIENDOSE");
    //Serial3.print("Stepper moviendose");
    stepperMove(); //Mover el stepper
    //contarTiempo(); //Verifica el tiempo actual
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(mensajeEstacion);
  }
  mensajeEstacion = "ESTACION ";
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
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};



void mensajeScroll() { //Scroll del texto de izquierda a derecha
  [&] {
    for (int d = msj.d; d < sizeof(Mensaje[0]) - 7; d++) {
      //Serial3.println("Esta en el scroll");
      for (int col = msj.col; col < 8; col++) {

        for (int row = msj.row; row < 8; row++) {
          if (!mt1) {
            matrix.setLed(0, row, col, pgm_read_byte(&(Mensaje[row + 1][col + 2 - d])));
            //btnIn1State = digitalRead(btnIn1);
            if ( Serial.available()) {
              return;
            }
          }
          if (!mt2) {
            matrix.setLed(1, row, col, pgm_read_byte(&(Mensaje[row + 1][col + 2 - d])));
            //btnIn1State = digitalRead(btnIn1);
            if (Serial.available()) {
              return;
            }
          }
          if (!mt3) {
            matrix.setLed(2, row, col, pgm_read_byte(&(Mensaje[row + 1][col + 2 - d])));
            //btnIn1State = digitalRead(btnIn1);
            if (Serial.available()) {
              return;
            }
          }
          msj.row = msj.row + 1;
          //Serial3.println(msj.row);
          //return;
          //contarTiempo(); //Verifica el contador del reloj
        }
        msj.row = 0;
        msj.col = msj.col + 1;

      }
      msj.col = 0;
      msj.d = msj.d + 1;
      if ((matrizActiva)) {
        //Otra condición posible
        //(msj.d == 4 || msj.d == 8 || msj.d == 12|| msj.d == 16|| msj.d == 20|| msj.d == 24|| msj.d == 28|| msj.d == 32|| msj.d == 36|| msj.d == 40|| msj.d == 44|| msj.d == 48|| msj.d == 52) &&
        return;
      } else {
        //contarTiempo(); //Verifica el contador del reloj
      }
    }
    msj.d = 0;

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






//#####********************************STEPPER********************************#####
void stepperMove() {
  stepper.setSpeed(8);// defino la velocidad del motor xd
  stepper.step(15);//bulgarmente digo q de 5 vueltas xd
}

//#####********************************STEPPER********************************#####







//**********************************************************   TODO LO RELACIONADO A TEMPERATURA   ****************************************************************
void temperatura() {
  int val = analogRead(A0);//leo en A1
  int porcentaje = map(val, 21, 359, -40, 124);//lo paso a porcentaje

  String conversion = "";


  if (tempInicial != porcentaje) {
    //conversion = String(porcentaje);
    tempInicial = porcentaje;
    valor = "@ ";
    valor += porcentaje;
    Serial.println(valor);
    delay(50);
    conversion = "";
  }


  if (porcentaje < 37) {
    // Serial1.println("Led verde");
    digitalWrite(A1, HIGH);
    digitalWrite(A2, LOW);
    digitalWrite(A3, LOW);
    if (usuariocorrecto && matrizActiva == false && stepperActivo == false){
      lcd.clear();
    }  
  } else if (porcentaje > 36 && porcentaje < 46) {
    // Serial1.println("Led amarilla");
    
    digitalWrite(A1, LOW);
    digitalWrite(A2, HIGH);
    digitalWrite(A3, LOW);
    lcd.setCursor(0, 0);

    if(usuariocorrecto == true && matrizActiva == false && stepperActivo == false){
      lcd.clear();
      lcd.print("WARNING");
    }
    //delay(2000);
    //lcd.clear();

    // en lcd imprimir warning
  } else {
    // Serial1.println("Led rojo");
    //lcd.clear();
    digitalWrite(A1, LOW );
    digitalWrite(A2, LOW);
    digitalWrite(A3, HIGH);
    // en lcd imprimir ERROR
    lcd.setCursor(0, 0);
    //lcd.print("ERROR");
    delay(1000);
    //Enviar señal a app para reiniciar<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    if(usuariocorrecto == true && matrizActiva == false && stepperActivo == false){
      lcd.clear();
      lcd.print("ERROR");
    }
    reiniciar();
    //lcd.clear();
    
  }






}

//**********************************************************   TODO LO RELACIONADO A TEMPERATURA   ****************************************************************









//*************************************************************    TODO LO RELACIONADO AL RELOJ ****************************************************************8
//******************************************* no tocar variables iniciales osea resetear cuando sea el momento
int segundos = 0;
int decimas = 1;
int minuto = 1;
bool decOn = false;
bool minOn = false;
//*****************************************

void contarTiempo() { //Método que calcula si el contador del tiempo debe aumentar
  if ((millis() - tiempoMillis) >= 1000) {
    tiempo();
    tiempoMillis = millis(); //Calcula si ya ha pasado 1 segundo
    temperatura();// verifica la temperatura cada segundo
    //reloj();
  }
}

void tiempo() {
  if (usuariocorrecto) {
    
    //calcula las decimas de segundo 0-5
    if (decOn == true) {
      switch (decimas) {
        case 0 :
          //Serial1.println("0 dec!" );
          decimas++;
          PORTL = B11111100;//print 0 in led
          decOn = false;
          dectemp = 0;
          break;
        case 1 :
          //Serial1.println("1 dec" );
          decimas++;
          PORTL = B01100000;//print 1 in led
          decOn = false;
          dectemp = 1;
          break;
        case 2 :
          //Serial1.println("2 dec" );
          decimas++;
          PORTL = B11011010;//print 2 in led
          decOn = false;
          dectemp = 2;
          break;
        case 3 :
          //Serial1.println("3 dec" );
          decimas++;
          PORTL = B11110010;//print 3 in led
          decOn = false;
          dectemp = 3;
          break;
        case 4 :
          //Serial1.println("4 dec" );
          decimas++;
          PORTL = B01100110;//print 4 in led
          decOn = false;
          dectemp = 4;
          break;
        case 5 :
          //Serial1.println("5 dec" );
          decimas = 0;
          PORTL = B10110110;//print 5 in led
          decOn = false;
          dectemp = 5;
          break;
        default ://activamos minutos
          printf("F\n" );
      }

    }

    //calcula los min 0-9
    if (minOn == true) {
      switch (minuto) {

        case 0 :
          //Serial1.println("1s" );
          minuto++;
          PORTC = B11111100;//print 0 in led
          minOn = false;
          mintemp = 0;
          break;
        case 1 :
          //Serial1.println("1s" );
          minuto++;
          PORTC = B01100000;//print 1 in led
          minOn = false;
          mintemp = 1;
          break;
        case 2 :
          //Serial1.println("2s" );
          minuto++;
          PORTC = B11011010;//print 2 in led
          minOn = false;
          mintemp = 2;
          break;
        case 3 :
          //Serial1.println("3s" );
          minuto++;
          PORTC = B11110010;//print 3 in led
          minOn = false;
          mintemp = 3;
          break;
        case 4 :
          //Serial1.println("4s" );
          minuto++;
          PORTC = B01100110;//print 4 in led
          minOn = false;
          mintemp = 4;
          break;
        case 5 :
          //Serial1.println("5s" );
          minuto++;
          PORTC = B10110110;//print 5 in led
          minOn = false;
          mintemp = 5;
          break;
        case 6 :
          //Serial1.println("6s" );
          minuto++;
          PORTC = B10111110;//print 6 in led
          minOn = false;
          mintemp = 6;
          break;
        case 7 :
          //Serial1.println("7s" );
          minuto++;
          PORTC = B11100000;//print 7 in led
          minOn = false;
          mintemp = 7;
          break;
        case 8 :
          //Serial1.println("8s" );
          minuto++;
          PORTC = B11111110;//print 8 in led
          minOn = false;
          mintemp = 8;
          break;
        case 9 :
          //Serial1.println("9s" );
          //segundos = 0;//reinicio de 9 a 0
          minuto++;
          minOn = false;//xd
          PORTC = B11100110;//print 9 in led
          mintemp = 9;
          break;
        default :
          printf("F\n" );
      }
    }

    //segundos------------------------------------------------------------------------------ 0-9
    switch (segundos) {
      case 0 :
        //Serial1.println("0s!" );
        segundos++;
        decOn = false;
        PORTK = B11111100;//print 0 in led
        sectemp = 0;
        break;
      case 1 :
        //Serial1.println("1s" );
        segundos++;
        PORTK = B01100000;//print 1 in led
        sectemp = 1;
        break;
      case 2 :
        //Serial1.println("2s" );
        segundos++;
        PORTK = B11011010;//print 2 in led
        sectemp = 2;
        break;
      case 3 :
        //Serial1.println("3s" );
        segundos++;
        PORTK = B11110010;//print 3 in led
        sectemp = 3;
        break;
      case 4 :
        //Serial1.println("4s" );
        segundos++;
        PORTK = B01100110;//print 4 in led
        sectemp = 4;
        break;
      case 5 :
        //Serial1.println("5s" );
        segundos++;
        PORTK = B10110110;//print 5 in led
        sectemp = 5;
        break;
      case 6 :
        //Serial1.println("6s" );
        segundos++;
        PORTK = B10111110;//print 6 in led
        sectemp = 6;
        break;
      case 7 :
        //Serial1.println("7s" );
        segundos++;
        PORTK = B11100000;//print 7 in led
        sectemp = 7;
        break;
      case 8 :
        //Serial1.println("8s" );
        segundos++;
        PORTK = B11111110;//print 8 in led
        sectemp = 8;
        break;
      case 9 :
        //Serial1.println("9s" );
        segundos = 0;//reinicio de 9 a 0
        decOn = true;//cuando llega a nueve tonces reinicio a 0 el contador de seg y le sumo +1 en decimas
        PORTK = B11100110;//print 9 in led

        if (decimas == 0) {
          minOn = true;
        }

        if (decimas == 0 && minuto == 10) {
          minuto = 0;
          PORTC = B11111100;//print 0 in led

        }
        sectemp = 9;
        break;
      default :
        printf("F\n" );
    }
    //Envio de tiempo a la aplicación<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


  }
  temperatura();// verifica la temperatura cada segundo
}


//*************************************************************    TODO LO RELACIONADO AL RELOJ *****************************************************************

//void reloj() {
//conjunto="";
//  String d = String(dectemp);
//
//  String m = String(mintemp);
//
//  String s = String(sectemp);
//
//  // Serial.println("tiempo " + m + ":" + d + s);
//  if (valor.length() > 0   && d.length() > 0  && m.length() > 0 && s.length() > 0 ) {
//    conjunto = "@ " + valor + " " + m + ":" + d + s;
//    Serial.println(conjunto);
//    delay(300);
//    //Serial3.println(conjunto);
//    valor="";
//  }
//
//  //  Serial3.println(valor + " " + m + ":" + d + s);
//}











//**************************************    teclado      *********************************************************************************************************
void ingreso() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("INGRESE SU");
  lcd.setCursor(0, 1);
  lcd.print("USUARIO");
  delay(1000);
  lcd.clear();
  lcd.print("USUARIO:");
  char c = '\0';
  String pass_pad = "";
  int contador_ingresopad = 0;
  while (true) {
    c = teclado.getKey();
    if (c == '*') break;
    if (c != NO_KEY) {
      pass_pad += c;
      lcd.setCursor(contador_ingresopad, 1);
      lcd.print(c);
      contador_ingresopad++;
    }
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  if (pass_pad == user) {
    lcd.print("USUARIO");
    lcd.setCursor(0, 1);
    lcd.print("CORRECTO");
    delay(500);
    lcd.clear();
    usuariocorrecto = !false;
  }
  else {

    lcd.print("ERROR");
    delay(500);
    //Enviar señal a la app para reiniciar<---------------------------------------------
    reiniciar();
  }
}

//**************************************    teclado      *********************************************************************************************************



void reiniciar() {
  //Reiniciar reloj
  //lcd.clear();
  segundos = 0;
  decimas = 1;
  minuto = 1;
  PORTL = B11111100; //Impresión 0 en display
  PORTC = B11111100; //Impresión 0 en display
  decOn = false;
  minOn = false;
  //Reiniciar matrices y stepper
  contadorMatriz = 0;
  contadorLugar = 1;
  mt1 = false;
  mt2 = false;
  mt3 = false;
  matrix.clearDisplay(0);
  matrix.clearDisplay(1);
  matrix.clearDisplay(2);
  matrix.clearDisplay(3);
  matrizActiva = false;
  stepperActivo = false;
  msj.d = 0;
  msj.col = 0;
  msj.row = 0;
}
