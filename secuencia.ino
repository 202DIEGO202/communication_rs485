int leds[] = {2, 3, 4, 5, 6, 7};
int numLeds = 6;
int patternDuration = 500; // Duración de cada patrón en milisegundos

void setup() {
  for (int i = 0; i < numLeds; i++) {
    pinMode(leds[i], OUTPUT);
  }
}

void loop() {
  pattern1(); // Secuencia 1: Todos los LEDs parpadean juntos
  delay(1000);
  
  pattern2(); // Secuencia 2: Encender y apagar los LEDs uno por uno
  delay(1000);
  
  pattern3(); // Secuencia 3: Dos LEDs parpadean en secuencia
  delay(1000);
  
  pattern4(); // Secuencia 4: Encender LEDs en patrón de zigzag
  delay(1000);
  
  pattern5(); // Secuencia 5: LEDs aleatorios encendiendo y apagando
  delay(1000);
  
  pattern6(); // Secuencia 6: LEDs encendidos en forma de ola
  delay(1000);
}

void allOff() {
  for (int i = 0; i < numLeds; i++) {
    digitalWrite(leds[i], LOW);
  }
}

void pattern1() {
  for (int i = 0; i < numLeds; i++) {
    digitalWrite(leds[i], HIGH);
  }
  delay(patternDuration);
  allOff();
  delay(patternDuration);
}

void pattern2() {
  for (int i = 0; i < numLeds; i++) {
    digitalWrite(leds[i], HIGH);
    delay(patternDuration);
    digitalWrite(leds[i], LOW);
  }
  delay(patternDuration);
}

void pattern3() {
  for (int i = 0; i < numLeds; i += 2) {
    digitalWrite(leds[i], HIGH);
    digitalWrite(leds[i + 1], HIGH);
    delay(patternDuration);
    digitalWrite(leds[i], LOW);
    digitalWrite(leds[i + 1], LOW);
    delay(patternDuration);
  }
}

void pattern4() {
  for (int i = 0; i < numLeds; i++) {
    digitalWrite(leds[i], HIGH);
    delay(patternDuration);
    allOff();
    delay(patternDuration);
  }
  for (int i = numLeds - 1; i >= 0; i--) {
    digitalWrite(leds[i], HIGH);
    delay(patternDuration);
    allOff();
    delay(patternDuration);
  }
}

void pattern5() {
  for (int i = 0; i < 10; i++) {
    int randomLed = random(numLeds);
    digitalWrite(leds[randomLed], HIGH);
    delay(patternDuration);
    digitalWrite(leds[randomLed], LOW);
    delay(patternDuration);
  }
}

void pattern6() {
  for (int i = 0; i < numLeds; i++) {
    digitalWrite(leds[i], HIGH);
    delay(patternDuration);
  }
  for (int i = numLeds - 1; i >= 0; i--) {
    digitalWrite(leds[i], LOW);
    delay(patternDuration);
  }
}
