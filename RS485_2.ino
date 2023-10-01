#include <SPIFFS.h>

#define DE_RE_PIN 4
#define MODE_SEND HIGH
#define MODE_RECV LOW

#define LED_PIN 2

byte bufferPH[7];
byte i = 0;

const char* filePath = "/data.txt";

bool shouldPrint = false; // Bandera para controlar si se debe imprimir
bool shouldClear = false; // Bandera para controlar si se debe borrar el datalogger

unsigned long previousMillis = 0;
const unsigned long interval = 100; // 5 minutos en milisegundos

void readSoilRS485() {
  uint8_t buff[] = {
    0x01, // Devices Address
    0x03, // Function code
    0x00, // Start Address HIGH
    0x00, // Start Address LOW
    0x00, // Quantity HIGH
    0x01, // Quantity LOW
    0x84, // CRC LOW
    0x0A  // CRC HIGH
  };

  digitalWrite(DE_RE_PIN, MODE_SEND);
  Serial2.write(buff, sizeof(buff));
  Serial2.flush(); // wait MODE_SEND completed
  digitalWrite(DE_RE_PIN, MODE_RECV);
  delay(500);

  while (Serial2.available()) {
    uint8_t data = Serial2.read();
    bufferPH[i] = data;
    i = i + 1;
  }
  i = 0;
}


void clearDataLogger() {
  SPIFFS.remove(filePath);
  Serial.println("Datalogger limpiado.");
}

void setup() {
  pinMode(DE_RE_PIN, OUTPUT);
  digitalWrite(DE_RE_PIN, MODE_RECV);
  Serial.begin(115200);
  Serial2.begin(4800, SERIAL_8N1, 17, 16); // Rx, Tx 17, 16
  Serial2.setTimeout(1000);

  if (!SPIFFS.begin(true)) {
    Serial.println("Error al montar SPIFFS");
    return;
  }
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  unsigned long currentMillis = millis();

  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n'); // Leer la entrada desde el monitor serial
    input.trim(); // Eliminar espacios en blanco al principio y al final

    if (input == "print") {
      shouldPrint = true; // Establecer la bandera para imprimir
    } else if (input == "clear") {
      shouldClear = true; // Establecer la bandera para borrar el datalogger
    } else {
      Serial.println("Escribe 'print' para mostrar los datos almacenados o 'clear' para borrar el datalogger.");
    }
  }

  if (shouldClear) {
    clearDataLogger();
    shouldClear = false; // Restablecer la bandera después de borrar
  }

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis; // Guardar el tiempo actual

    readSoilRS485();
    float conversion = (bufferPH[4]) / 10.0;
    Serial.print("Soil: ");
    Serial.print(conversion);
    Serial.println(" %");

    // Almacenar la variable conversion en un archivo en SPIFFS
    File file = SPIFFS.open(filePath, "a");
    if (file) {
      file.print(conversion);
      file.print("\n");
      file.close();
    } else {
      Serial.println("Error al abrir el archivo en SPIFFS");
      digitalWrite(LED_PIN, HIGH); // Encender el LED
    }
  }

  if (shouldPrint) {
    // Leer y mostrar el contenido del archivo
    File readFile = SPIFFS.open(filePath, "r");
    if (readFile) {
      Serial.println("Contenido del archivo:");
      while (readFile.available()) {
        Serial.println(readFile.readStringUntil('\n'));
      }
      readFile.close();
    } else {
      Serial.println("Error al abrir el archivo para lectura en SPIFFS");
      digitalWrite(LED_PIN, HIGH); // Encender el LED
    }

    shouldPrint = false; // Restablecer la bandera después de imprimir
  }
}
