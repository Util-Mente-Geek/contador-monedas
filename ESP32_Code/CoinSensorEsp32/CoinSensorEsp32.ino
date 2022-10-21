/*
 * Sensor de monedas v1
 * Codigo para ESP32
 * Escrito por: Útil Mente Geek
 * PaginaWeb: www.utilmentegeek.com
 * YouTube: https://www.youtube.com/channel/UCBqfcl_tue6Zsk4Hc3BErWg
 * Instagram: https://www.instagram.com/utilmentegeek/
 * 
 * Recuerda que tambien pudes contribuir a la mejora de este codigo en GitHub
 * Y apoyar el desarrollo de estos proyectos así como resolver dudas sobre el codigo
 * en nuestras redes socilaes @Util Mente Geek.
 */

//Puerto para boton de RESET
const int btnReset = 32;

//Puertos de entrada y salida del sensor
const int sensorEntrada = 13;
const int sensorSalida = 12;

//Puertos para diferenciar el tamaño de las monedas
const int sMoneda1 = 14; //Detecta moneda $1
const int sMoneda2 = 27; //Detecta moneda $2
const int sMoneda3 = 26; //Detecta moneda $5
const int sMoneda4 = 25; //Detecta moneda $10

//Variables para la cantidad de monedas por separado
int monCincuenta = 0;
int monUno = 0;
int monDos = 0;
int monCinco = 0;
int monDiez = 0;

//Variable para la suma total de monedas
double dineroTotal = 0;
int monedasTotales = 0;

//Variables que se utilizan dentro de la interrupcion
volatile boolean senEntrada = LOW;
volatile boolean senSalida = HIGH;
volatile boolean monedaNueva = LOW;

//Guarda la leectura de los tamaños de monedas
boolean boolMoneda1 = LOW;
boolean boolMoneda2 = LOW;
boolean boolMoneda3 = LOW;
boolean boolMoneda4 = LOW;

//Funcion que se ejecuta cuando se inserta una moneda en el sensor
void IRAM_ATTR Ext_INT_sensorEntrada(){  
  senEntrada = HIGH;
  senSalida = LOW;
}
//Funcion que se ejecuta cuando sale la moneda del sensor
void IRAM_ATTR Ext_INT_sensorSalida(){
  senEntrada = LOW;
  senSalida = HIGH;
}

//Configuracion inicial
void setup() {
  Serial.begin(9600);
     
  pinMode(sensorEntrada,INPUT);
  pinMode(sensorSalida,INPUT);
  pinMode(sMoneda1,INPUT);
  pinMode(sMoneda2,INPUT);
  pinMode(sMoneda3,INPUT);
  pinMode(sMoneda4,INPUT);
  pinMode(btnReset,INPUT); 
  
  attachInterrupt(sensorEntrada, Ext_INT_sensorEntrada, RISING);
  attachInterrupt(sensorSalida, Ext_INT_sensorSalida, FALLING);  
}

//Bucle repetitivo
void loop() {
    
  //Verifica la entrada de una moneda
  if (senEntrada == HIGH && senSalida == LOW){
    monedaNueva = HIGH;
    Serial.println("Sensando Moneda...");    
    leerValorMonedas();    
  }

  //Verifica que haya salido la moneda
  if (senEntrada == LOW && senSalida == HIGH){

    //Pregunta si tenemos una moneda nueva y determina su valor
    if(monedaNueva == HIGH){
      asignarValorMoneda();
      monedaNueva = LOW; 

      //Cálculo de total de monedas y dinero
      monedasTotales = monCincuenta + monUno + monDos + monCinco + monDiez;
      dineroTotal = (monCincuenta*0.5)+(monUno*1)+(monDos*2)+(monCinco*5)+(monDiez*10);
    
      Serial.print("$0.5:");
      Serial.print(monCincuenta);
      Serial.print(" $1:");
      Serial.print(monUno);
      Serial.print(" $2:");
      Serial.print(monDos);
      Serial.print(" $5:");
      Serial.print(monCinco);
      Serial.print(" $10:");
      Serial.print(monDiez);
      Serial.print(" Total: $");
      Serial.print(dineroTotal);
      Serial.print(" Monedas:");
      Serial.print(monedasTotales);
      Serial.println();        
    }
  }
  
  //Revisa que no se haya presionado el boton de RESET
  if(digitalRead(btnReset) == HIGH){
    
    monCincuenta = 0;
    monUno = 0;
    monDos = 0;
    monCinco = 0;
    monDiez = 0;

    //Cálculo de total de monedas y dinero
      monedasTotales = 0;
      dineroTotal = 0;
    
      Serial.print("$0.5:");
      Serial.print(monCincuenta);
      Serial.print(" $1:");
      Serial.print(monUno);
      Serial.print(" $2:");
      Serial.print(monDos);
      Serial.print(" $5:");
      Serial.print(monCinco);
      Serial.print(" $10:");
      Serial.print(monDiez);
      Serial.print(" Total: $");
      Serial.print(dineroTotal);
      Serial.print(" Monedas:");
      Serial.print(monedasTotales);
      Serial.println();    
  }
}

//Funcion encargada de almacenar los sensores que fueron activados
void leerValorMonedas(){
  if(digitalRead(sMoneda1) == HIGH){
    boolMoneda1 = HIGH;
  }
  if(digitalRead(sMoneda2) == HIGH){
    boolMoneda2 = HIGH;
  }
  if(digitalRead(sMoneda3) == HIGH){
    boolMoneda3 = HIGH;
  }
  if(digitalRead(sMoneda4) == HIGH){
    boolMoneda4 = HIGH;
  }
}

//Asigna el valor de la moneda y lo almacena
void asignarValorMoneda(){
  if(boolMoneda1==HIGH){
    if(boolMoneda2==HIGH){
      if(boolMoneda3==HIGH){
        if(boolMoneda4==HIGH){
          monDiez = monDiez + 1;
        }else{
          monCinco = monCinco + 1;
        }
      }else{
        monDos = monDos + 1;
      }
    }else{
      monUno = monUno + 1;
    }
  }else{
    monCincuenta = monCincuenta + 1;
  }
  //Resetea los valores de los sensores para leer otra moneda
  boolMoneda1 = LOW;
  boolMoneda2 = LOW;
  boolMoneda3 = LOW;
  boolMoneda4 = LOW;
}
