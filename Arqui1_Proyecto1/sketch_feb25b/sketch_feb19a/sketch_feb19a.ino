#include <Stepper.h>
#include <Key.h>
#include <Keypad.h>
#include <LiquidCrystal.h>

//pines de lcd
const int rs = 53;
const int enable = 51;
const int d4 = 29;
const int d3 = 27;
const int d2 = 25;
const int d1 = 23;
//pines de keypad
//filas
const int f1 = 4;
const int f2 = 3;
const int f3 = 2;
const int f4 = 1;
//columnas
const int c1 = 7;
const int c2 = 6;
const int c3 = 5;

//PASSWORD
const String user = "123456";

bool usuariocorrecto = false;

const byte rows = 4; //four rows
const byte cols = 3; //three columns
char keys[rows][cols] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte rowPins[rows] = {f1, f2, f3, f4}; //connect to the row pinouts of the keypad
byte colPins[cols] = {c1, c2, c3}; //connect to the column pinouts of the keypad
Keypad teclado = Keypad( makeKeymap(keys), rowPins, colPins, rows, cols );

LiquidCrystal lcd(rs, enable, d4, d3, d2, d1);

//------------------------------------------------------------------- para motor stepper
#define STEPS 100 // creo q entre mas diminuto mas lento es
Stepper stepper(STEPS, 11, 10, 9, 8);//defino su stpes, IN1,IN2,IN3,IN4
//-------------------------------------------------------------------
void setup() {

  Serial1.begin(9600);

  lcd.begin(16, 2);
  
  DDRK = B11111111;
  DDRL = B11111111;
  DDRC = B11111111;
  pinMode(A0, INPUT);//javier temperatura
  pinMode(A1, OUTPUT);//javier temperatura led green
  pinMode(A2, OUTPUT);//javier temperatura led yellow
  pinMode(A3, OUTPUT);//javier temperatura led red
  //stepperMove();//prueba del motor stepper
}




void loop() {
  // put your main code here, to run repeatedly:

  

  if(usuariocorrecto == false){
    ingreso();
  }

  else{

  //stepperMove();
  tiempo();//verifica los seg,dec,min de los 3 display hace la magia de timer

  temperatura();// verifica la temperatura y enciende los leds respectivos
  //stepperMove();//afecta llamar al steper aqui se lagea todo pero como dice q el reloj es del ensamblaje pues eso significa q en la cinta se esta moviendo de un lugar a otro lado por ende no se toca nada por ende el tiempo como q se detine xd

  delay(500);//obligatorio sin esto el reloj iria mal
    
  }

}

//**********************************************************   TODO LO RELACIONADO A TEMPERATURA   ****************************************************************
void temperatura() {
  int val = analogRead(A0);//leo en A1
  int porcentaje = map(val, 21, 359, -40, 124);//lo paso a porcentaje

  Serial1.print("temperatura: ");
  Serial1.print(porcentaje);
  Serial1.println();

  if (porcentaje < 37) {
    // Serial1.println("Led verde");
    digitalWrite(A1, HIGH);
    digitalWrite(A2, LOW);
    digitalWrite(A3, LOW);
    lcd.clear();
  } else if (porcentaje > 36 && porcentaje < 46) {
    // Serial1.println("Led amarilla");
    digitalWrite(A1, LOW);
    digitalWrite(A2, HIGH);
    digitalWrite(A3, LOW);

    lcd.setCursor(0, 0);
    lcd.print("WARNING");
    //delay(2000);
    //lcd.clear();
  
    // en lcd imprimir warning
  } else {
    // Serial1.println("Led rojo");
    digitalWrite(A1, LOW );
    digitalWrite(A2, LOW);
    digitalWrite(A3, HIGH);
    // en lcd imprimir ERROR
    lcd.setCursor(0, 0);
    lcd.print("ERROR");
    //delay(2000);
    //lcd.clear();
    
  }


}


//*************************************************************    TODO LO RELACIONADO AL RELOJ ****************************************************************8
//******************************************* no tocar variables iniciales osea resetear cuando sea el momento
int segundos = 0;
int decimas = 1;
int minuto = 1;
bool decOn = false;
bool minOn = false;
//*****************************************
void tiempo() {

  //calcula las decimas de segundo 0-5
  if (decOn == true) {
    switch (decimas) {
      case 0 :
        //Serial1.println("0 dec!" );
        decimas++;
        PORTL = B11111100;//print 0 in led
        decOn = false;
        break;
      case 1 :
        //Serial1.println("1 dec" );
        decimas++;
        PORTL = B01100000;//print 1 in led
        decOn = false;
        break;
      case 2 :
        //Serial1.println("2 dec" );
        decimas++;
        PORTL = B11011010;//print 2 in led
        decOn = false;
        break;
      case 3 :
        //Serial1.println("3 dec" );
        decimas++;
        PORTL = B11110010;//print 3 in led
        decOn = false;
        break;
      case 4 :
        //Serial1.println("4 dec" );
        decimas++;
        PORTL = B01100110;//print 4 in led
        decOn = false;
        break;
      case 5 :
        //Serial1.println("5 dec" );
        decimas = 0;
        PORTL = B10110110;//print 5 in led
        decOn = false;
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
        break;
      case 1 :
        //Serial1.println("1s" );
        minuto++;
        PORTC = B01100000;//print 1 in led
        minOn = false;
        break;
      case 2 :
        //Serial1.println("2s" );
        minuto++;
        PORTC = B11011010;//print 2 in led
        minOn = false;
        break;
      case 3 :
        //Serial1.println("3s" );
        minuto++;
        PORTC = B11110010;//print 3 in led
        minOn = false;
        break;
      case 4 :
        //Serial1.println("4s" );
        minuto++;
        PORTC = B01100110;//print 4 in led
        minOn = false;
        break;
      case 5 :
        //Serial1.println("5s" );
        minuto++;
        PORTC = B10110110;//print 5 in led
        minOn = false;
        break;
      case 6 :
        //Serial1.println("6s" );
        minuto++;
        PORTC = B10111110;//print 6 in led
        minOn = false;
        break;
      case 7 :
        //Serial1.println("7s" );
        minuto++;
        PORTC = B11100000;//print 7 in led
        minOn = false;
        break;
      case 8 :
        //Serial1.println("8s" );
        minuto++;
        PORTC = B11111110;//print 8 in led
        minOn = false;
        break;
      case 9 :
        //Serial1.println("9s" );
        //segundos = 0;//reinicio de 9 a 0
        minuto++;
        minOn = false;//xd
        PORTC = B11100110;//print 9 in led
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
      break;
    case 1 :
      //Serial1.println("1s" );
      segundos++;
      PORTK = B01100000;//print 1 in led
      break;
    case 2 :
      //Serial1.println("2s" );
      segundos++;
      PORTK = B11011010;//print 2 in led
      break;
    case 3 :
      //Serial1.println("3s" );
      segundos++;
      PORTK = B11110010;//print 3 in led
      break;
    case 4 :
      //Serial1.println("4s" );
      segundos++;
      PORTK = B01100110;//print 4 in led
      break;
    case 5 :
      //Serial1.println("5s" );
      segundos++;
      PORTK = B10110110;//print 5 in led
      break;
    case 6 :
      //Serial1.println("6s" );
      segundos++;
      PORTK = B10111110;//print 6 in led
      break;
    case 7 :
      //Serial1.println("7s" );
      segundos++;
      PORTK = B11100000;//print 7 in led
      break;
    case 8 :
      //Serial1.println("8s" );
      segundos++;
      PORTK = B11111110;//print 8 in led
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
      break;
    default :
      printf("F\n" );
  }

}


//**************************************    motor      **************************************************

void stepperMove() {

  stepper.setSpeed(10);// defino la velocidad del motor xd
  stepper.step(30);//bulgarmente digo q de 5 vueltas xd

}


//**************************************    teclado      **************************************************
void ingreso() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("INGRESE SU");
  lcd.setCursor(0, 1);
  lcd.print("USUARIO");
  delay(2000);
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
    delay(1000);
    lcd.clear();
    usuariocorrecto = !false;
  }
  else {

    lcd.print("ERROR");
    delay(1000);
    }
}
