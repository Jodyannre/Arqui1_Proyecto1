#include <LedControl.h> //Librería para el control de las matrices



//#####*****************************DEFINICIÓN DE VARIABLES GLOBALES*****************************#####




LedControl matrix = LedControl(12, 11, 10, 4); //Variable para controlar la matrices
int vel_scroll = 0;



//#####*****************************DEFINICIÓN DE VARIABLES GLOBALES*****************************#####





//####****************************VARIABLES PARA RECIBIR DATOS***************************#####
const byte numChars = 81; //Tamaño de buffer para recibir datos
char stringRecibido[numChars]; //Array que guarda los datos recibidos en char
char tempChars[numChars]; //Array backup de stringRecibido porque el separador va eliminado partes del array origina
char delimitador[] = ","; //Deliminator de los elementos que trae el string desde la APP para separalos luego
boolean newData = false; //Booleano que indica si hay nuevos datos para leer desde el serial
char myStrings[8]; //Char que guardará cada fila del array de bytes para pintar cada estación
char copia[9]; //Otra copia, de momento esta en desuso
//#####*****************************VARIABLES PARA RECIBIR DATOS*****************************#####







//#####*****************************INICIALIZACION DE ARDUINO*****************************#####

void setup() {
  Serial.begin(9600);
  inicializarMatriz();
}



//#####*****************************INICIALIZACION DE ARDUINO*****************************#####




//#####*****************************LOOP PRINCIPAL*****************************#####

void loop() {

  //letrero(); //Letrero mientras no se usan las matrices

  
    lecturaSerial();//Leyendo la entrada serial
    if (newData == true) {
      Serial.println("Nuevos datos encontrados");
        strcpy(tempChars, stringRecibido);
            // Hacer una copia de seguridad de los datos
        parseData(); //Convirtiendo datos de entrada
        newData = false;
    }
  

}

//#####*****************************LOOP PRINCIPAL*****************************#####



//#####********************************LECTURA SERIAL E IMPRESIÓN DE ESTACIONES********************************#####


void lecturaSerial() { //Método que se encarga de leer las entradas en string
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMark = '<';
  char endMark = '>';
  char rc;
  //Serial.println("Leyendo serial");
  while (Serial.available() > 0 && newData == false) {
    Serial.println("Leyendo serial");
    rc = Serial.read();

    if (recvInProgress == true) {
      if (rc != endMark) {
        stringRecibido[ndx] = rc;
        ndx++;
        if (ndx >= numChars) {
          ndx = numChars - 1;
        }
      }
      else {
        stringRecibido[ndx] = '\0'; // Termino el string
        recvInProgress = false;
        ndx = 0;
        newData = true;
      }
    }

    else if (rc == startMark) {
      recvInProgress = true;
    }
  }
}



void parseData() {      //Separar char, convertirlo a string y pintar la matriz
  int i = 0;
  char * index; 

  index = strtok(tempChars, delimitador);
  String s = "";
  while (index != NULL) {
    myStrings[i] = index;
    s=index;
    s = s.substring(0, s.length()-1);
    for (int j=0;j<s.length();j++){
      Serial.println("Esta en el for");
      Serial.println("Evaluando");
      Serial.println(s);
      Serial.println("caracter");
      Serial.println(s.charAt(j));
      if ((int)s.charAt(j)=='1'){
        Serial.println("Es igual");
        matrix.setLed(1,i,j,true);
      }
    }
    index = strtok(NULL, delimitador);
    i++;   
    delay(100);
  }
  delay(10000);  
  for (int i = 0; i < sizeof(myStrings); i++) {
    Serial.println("En el for");
    //Serial.println(myStrings[i]);
    delay(500);
    if(myStrings[i] == '\0') {
      Serial.println("Ya no hay mas informacion");
      }
  }

}


//#####********************************LECTURA SERIAL E IMPRESIÓN DE ESTACIONES********************************#####







//#####********************************INICIALIZAR MATRIZ********************************#####

//Inicialización de la matriz
//Obtener cantidad de matrices
//Cancelar modo ahorro
void inicializarMatriz() {
  for (int i = 0; i < 4; i++) {
    matrix.shutdown(i, false);
    //Configurar brillo
    matrix.setIntensity(i, 8);
    //Limpiar la matriz
    matrix.clearDisplay(i);
  }
}





//#####********************************INICIALIZAR MATRIZ********************************#####




//#####********************************LETRERO MIENTRAS NO SE USAN LAS ESTACIONES********************************#####
void letrero() { //Método para mostrar el letrero
  mensajeScroll(1);


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

void mensajeScroll(int estado) { //Scroll del texto de izquierda a derecha
  [&] {
    for (int d = 0; d < sizeof(Message[0]) - 7; d++) { 
      for (int col = 0; col < 8; col++) {

        for (int row = 0; row < 8; row++) {

          matrix.setLed(0, row, col, pgm_read_byte(&(Message[row + 1][col + 2 - d])));
          matrix.setLed(1, row, col, pgm_read_byte(&(Message[row + 1][col + 2 - d])));
          matrix.setLed(2, row, col, pgm_read_byte(&(Message[row + 1][col + 2 - d])));//****
        }
      }
    }
  }();
  matrix.clearDisplay(0);
  matrix.clearDisplay(1);
}

//#####********************************LETRERO MIENTRAS NO SE USAN LAS ESTACIONES********************************#####
