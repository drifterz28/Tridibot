
//--------------------------------------------------------------
//       Autor del Programa: Konrad Peschka && Andreas Haller
//       Fecha: Septiembre 2017
//--------------------------------------------------------------

#include <Adafruit_NeoPixel.h>

#define PIN            10
#define NUMPIXELS      8


Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


#include <Servo.h>

//-------------------------------------------------------------------
//      Alias definition of each servo and the Arduino pins
//-------------------------------------------------------------------

#define servo1_pin 8      // Pin Front Right Body
#define servo2_pin 7      // Pin Back Right Body
#define servo3_pin 3      // Pin Back Left Body
#define servo4_pin 4      // Pin Front Left Body

#define servo5_pin 9     // Pin Front Right Leg
#define servo6_pin 6     // Pin Back Right Leg
#define servo7_pin 2     // Pin Back Left Leg
#define servo8_pin 5     // Pin Left Front Leg

//-------------------------------------------------------------------
//  Definition of the constant initial position of each servo
//-------------------------------------------------------------------

//subir = antihor twist on body and legs

#define servo1_center     140      //Front Right Body Center
#define servo2_center     140      //Back right body center
#define servo3_center     140      //Back Left body center
#define servo4_center     140      //Front Left body center

#define servo5_center      2200      //Front right paw center
#define servo6_center      2200      //Back right center leg
#define servo7_center      2200      //Back Left center leg
#define servo8_center      2200      //Front left center leg

//-------------------------------------------------------------------

int lift_paw = 300;
int lift_paw2 = 800;

int paw_toward_front = 800;

int left_movement;
int right_movement;

int contador_sentarse;
int distancia_actual = 0;

int aux = 0;  //auxiliary I use for special functions
int variable_seat = 1500;

int time_between_servo = 70;

const int tiempo_sienta = 50;
int brillo = 50;

int auxiliar_saluda = 0;
int auxiliar_sienta = 0;

//-------------------------------------------------------------------
//       I define what each servo will call it
//-------------------------------------------------------------------

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

Servo servo5;
Servo servo6;
Servo servo7;
Servo servo8;


//-------------------------------------------------------------------
//                 Inputs, outputs and functions setting
//-------------------------------------------------------------------

void setup()
{
  Serial.begin(115200);
  Serial.println("start");
  pixels.begin();

  //activa luces rojas
  for (int i = 0; i < NUMPIXELS; i++)
  {
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(brillo, 0, 0)); // Moderately bright green color.
    pixels.show(); // This sends the updated pixel color to the hardware.
    delay(200); // Delay for a period of time (in milliseconds).
  }

  //muestro blanco
  for (int i = 0; i < NUMPIXELS; i++)
  {
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(0, brillo, brillo)); // Moderately bright green color.
    pixels.show(); // This sends the updated pixel color to the hardware.
    //delay(1); // Delay for a period of time (in milliseconds).
  }

}

//-------------------------------------------------------------------
//         Sector where the main function begins,
//        Where will the rest of the execution stay
//-------------------------------------------------------------------

char auxColor;

void loop() {
  char variable = 'X';
  if (Serial.available()) {
    variable = Serial.read();

  }

  switch (variable) {
    case 'Y':
      disableServos();
      delay(50);
      Serial.write(variable);
      break;
    case 'Z':
      activateServos();
      delay(50);
      restingPosition();
      delay(50);
      Serial.write(variable);
      break;

    case 'F':
      walkFront();
      Serial.write(variable);
      break;
    case 'B':
      walkBack();
      Serial.write(variable);
      break;
    case 'L':
      trunLeft();
      Serial.write(variable);
      break;
    case 'R':
      trunRight() ;
      Serial.write(variable);
      break;
    case 'S':
      auxiliar_saluda = 0;
      auxiliar_sienta = 0;
      restingPosition();
      Serial.write(variable);
      break;

    case 'I':
      if (auxiliar_sienta == 0)
      {
        sienta();
        auxiliar_sienta = 1;
      }
      Serial.write(variable);
      break;

    case 'O':
      if (auxiliar_saluda == 0)
      {
        sit_hello();
        auxiliar_saluda = 1;
      }
      moveFrontPaw();
      Serial.write(variable);
      break;

    case 'E':
      kickLeft();
      Serial.write(variable);
      break;

    case 'A':
      kickRight();
      Serial.write(variable);
      break;
    case 'C':
      showColor(brillo, brillo, 0);
      delay(50);
      auxColor = Serial.read();
      if (auxColor == '0')
        showColor(0, 0, 0);
      else if (auxColor == '1')
        showColor(brillo, 0, 0);
      else if (auxColor == '2')
        showColor(brillo, 0, brillo);
      else if (auxColor == '3')
        showColor(0, 0, brillo);
      else if (auxColor == '4')
        showColor(0, brillo, brillo);
      else if (auxColor == '5')
        showColor(0, brillo, 0);
      else if (auxColor == '6')
        showColor(brillo, brillo, 0);
      else if (auxColor == '7')
        showColor(brillo, brillo, brillo);


      Serial.write(variable);
      //activateServos();
      break;
    case 'V':
      time_between_servo = map(Serial.parseInt(), 0, 4, 100, 30);
      Serial.write(variable);
      break;
    case 'b':
      brillo = map(Serial.parseInt(), 0, 9, 0, 50);
      showColor(brillo, brillo, brillo);
      Serial.write(variable);
      break;
  }

}

void activateServos(void)
{
  servo1.attach(servo1_pin);
  servo1.writeMicroseconds(servo1_center);

  servo2.attach(servo2_pin);
  servo2.writeMicroseconds(servo2_center);

  servo3.attach(servo3_pin);
  servo3.writeMicroseconds(servo3_center);

  servo4.attach(servo4_pin);
  servo4.writeMicroseconds(servo4_center);

  servo5.attach(servo5_pin);
  servo5.writeMicroseconds(servo5_center );

  servo6.attach(servo6_pin);
  servo6.writeMicroseconds(servo6_center );

  servo7.attach(servo7_pin);
  servo7.writeMicroseconds(servo7_center );

  servo8.attach(servo8_pin);
  servo8.writeMicroseconds(servo8_center );

  delay(time_between_servo);
}

/*  --------  Desactivate Servos ---------   */

void disableServos(void)
{
  servo1.detach();
  servo2.detach();
  servo3.detach();
  servo4.detach();
  servo5.detach();
  servo6.detach();
  servo7.detach();
  servo8.detach();
  delay(time_between_servo);
}
//------------------------Funcion donde todos los servos van a reposo---------------

void restingPosition(void)
{
  servo1.writeMicroseconds(servo1_center);
  delay(time_between_servo);
  servo2.writeMicroseconds(servo2_center);
  delay(time_between_servo);
  servo3.writeMicroseconds(servo3_center);
  delay(time_between_servo);
  servo4.writeMicroseconds(servo4_center);
  delay(time_between_servo);
  servo5.writeMicroseconds(servo5_center);
  delay(time_between_servo);
  servo6.writeMicroseconds(servo6_center);
  delay(time_between_servo);
  servo7.writeMicroseconds(servo7_center);
  delay(time_between_servo);
  servo8.writeMicroseconds(servo8_center);
  delay(time_between_servo);

}

void baseMovement(void)
{
  // ---------------- lift 8 and 6 ------------------
  servo6.writeMicroseconds(servo6_center - lift_paw);
  delay(time_between_servo);
  servo8.writeMicroseconds(servo8_center - lift_paw);
  delay(time_between_servo);

  // ------------- advance all paws-------------------
  servo4.writeMicroseconds(servo4_center + left_movement);
  servo2.writeMicroseconds(servo2_center - right_movement);
  servo1.writeMicroseconds(servo1_center + right_movement);
  servo3.writeMicroseconds(servo3_center - left_movement);
  delay(time_between_servo);

  // ---------------- down paws 8 and 6 ------------------
  servo6.writeMicroseconds(servo6_center );
  delay(time_between_servo);
  servo8.writeMicroseconds(servo8_center );
  delay(time_between_servo);

  //----------lift paws 5 and 7-----------------
  servo7.writeMicroseconds(servo7_center - lift_paw);
  delay(time_between_servo);
  servo5.writeMicroseconds(servo5_center - lift_paw);
  delay(time_between_servo);

  // ------------- advance all paws-------------------
  servo1.writeMicroseconds(servo1_center - right_movement);
  servo3.writeMicroseconds(servo3_center + left_movement);
  servo4.writeMicroseconds(servo4_center - left_movement);
  servo2.writeMicroseconds(servo2_center + right_movement);
  delay(time_between_servo);

  // ----------------down paws 5 and 7 ------------------
  servo7.writeMicroseconds(servo7_center );
  delay(time_between_servo);
  servo5.writeMicroseconds(servo5_center );
  delay(time_between_servo);

}


void walkFront() {
  left_movement = 300;
  right_movement = 300;
  baseMovement();
}


void trunRight() {
  left_movement = -300;
  right_movement = 300;
  baseMovement();
}

void trunLeft() {
  left_movement = 300;
  right_movement = -300;
  baseMovement();
}

void walkBack() {
  left_movement = -300;
  right_movement = -300;
  baseMovement();
}

void sit(void) {
  servo1.writeMicroseconds(servo1_center);
  delay(time_between_servo);
  servo4.writeMicroseconds(servo4_center);
  delay(time_between_servo);

  servo2.writeMicroseconds(servo2_center - variable_seat / 3);
  delay(time_between_servo);
  servo3.writeMicroseconds(servo3_center + variable_seat / 3);
  delay(time_between_servo);
  servo6.writeMicroseconds(servo6_center - variable_seat / 3);
  delay(time_between_servo);
  servo7.writeMicroseconds(servo7_center - variable_seat / 3);
  delay(time_between_servo);

  servo2.writeMicroseconds(servo2_center - variable_seat / 2);
  delay(time_between_servo);
  servo3.writeMicroseconds(servo3_center + variable_seat / 2);
  delay(time_between_servo);
  servo6.writeMicroseconds(servo6_center - variable_seat / 2);
  delay(time_between_servo);
  servo7.writeMicroseconds(servo7_center - variable_seat / 2);
  delay(time_between_servo);

  servo2.writeMicroseconds(servo2_center - variable_seat / 2);
  delay(time_between_servo);
  servo3.writeMicroseconds(servo3_center + variable_seat / 2);
  delay(time_between_servo);
}

void sit_hello(void) {
  servo1.writeMicroseconds(servo1_center);
  delay(time_between_servo / 2);
  servo4.writeMicroseconds(servo4_center);
  delay(time_between_servo / 2);

  servo2.writeMicroseconds(servo2_center - variable_seat / 2);
  delay(time_between_servo / 2);
  // servo3.writeMicroseconds(servo3_center - variable_seat / 2);
  // delay(time_between_servo / 2);

  servo5.writeMicroseconds(servo5_center - variable_seat);
  delay(time_between_servo / 2);
  servo7.writeMicroseconds(servo7_center - variable_seat / 3);
  delay(time_between_servo / 2);
  servo7.writeMicroseconds(servo7_center - variable_seat / 2);
  delay(time_between_servo / 2);
  servo7.writeMicroseconds(servo7_center - variable_seat);
  delay(time_between_servo / 2);
  servo7.writeMicroseconds(servo6_center - variable_seat);
  delay(time_between_servo / 2);
}

void moveFrontPaw(void) {
  int maximum = 500;
  for (int j = 0; j < maximum; j = j + 100) {
    servo1.writeMicroseconds(servo1_center + j);
    delay(time_between_servo);
  }

  for (int j = maximum; j > 0; j = j - 100) {
    servo1.writeMicroseconds(servo1_center + j);
    delay(time_between_servo);
  }

  for (int j = 0; j < maximum; j = j + 100) {
    servo1.writeMicroseconds(servo1_center - j);
    delay(time_between_servo);
  }

  for (int j = maximum; j > 0; j = j - 100) {
    servo1.writeMicroseconds(servo1_center - j);
    delay(time_between_servo);
  }
}

void kickRight(void) {
  servo1.writeMicroseconds(servo1_center - paw_toward_front);
  delay(time_between_servo * 4);
  servo7.writeMicroseconds(servo7_center - lift_paw);
  delay(time_between_servo * 6);
  servo5.writeMicroseconds(servo5_center - lift_paw2);
  delay(time_between_servo * 6);
  servo5.writeMicroseconds(servo5_center);
  delay(time_between_servo);
  servo1.writeMicroseconds(servo1_center);
  delay(time_between_servo);
}

void kickLeft(void) {
  servo4.writeMicroseconds(servo4_center + paw_toward_front);
  delay(time_between_servo * 4);
  servo6.writeMicroseconds(servo6_center - lift_paw);
  delay(time_between_servo * 6);
  servo8.writeMicroseconds(servo8_center - lift_paw2);
  delay(time_between_servo * 6);
  servo8.writeMicroseconds(servo8_center );
  delay(time_between_servo);
  servo4.writeMicroseconds(servo4_center);
  delay(time_between_servo);
}

void showColor(int r, int g, int b) {
  for (int i = 0; i < NUMPIXELS; i++) {
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(r, g, b)); // Moderately bright green color.
    pixels.show(); // This sends the updated pixel color to the hardware.
    //delay(200); // Delay for a period of time (in milliseconds).
  }
}
