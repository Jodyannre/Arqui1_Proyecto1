void setup() {

  Serial1.begin(9600);
  DDRK = B11111111;
  DDRL = B11111111;
  DDRC = B11111111;
  pinMode(A0, INPUT);//javier temperatura
  pinMode(A1, OUTPUT);//javier temperatura led green
  pinMode(A2, OUTPUT);//javier temperatura led yellow
  pinMode(A3, OUTPUT);//javier temperatura led red

}


void loop() {
  // put your main code here, to run repeatedly:



  tiempo();//verifica los seg,dec,min de los 3 display hace la magia de timer

  temperatura();// verifica la temperatura y enciende los leds respectivos
  delay(1000);//obligatorio sin esto el reloj iria mal





}

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
  } else if (porcentaje > 36 && porcentaje < 46) {
    // Serial1.println("Led amarilla");
    digitalWrite(A1, LOW);
    digitalWrite(A2, HIGH);
    digitalWrite(A3, LOW);
    // en lcd imprimir warning
  } else {
    // Serial1.println("Led rojo");
    digitalWrite(A1, LOW );
    digitalWrite(A2, LOW);
    digitalWrite(A3, HIGH);
    // en lcd imprimir ERROR
  }


}
//******************************************* no tocar
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
