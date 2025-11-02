#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>

// --- CONFIGURAÇÕES ---
#define WIFI_SSID "PATTYGIO - NOVA FIBRA"
#define WIFI_PASSWORD "sal8318@mat2414"

#define DATABASE_URL "https://espcontroller-a8a69-default-rtdb.europe-west1.firebasedatabase.app/"
#define DATABASE_SECRET "Wft5oxxjv7Y83bBftFF1Mr5zoZV6VG3KdF0bywT0"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

#define LED_PIN D2

bool firebaseConectado = false;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.println("\n=== INICIANDO ESP8266 + FIREBASE ===");

  // ========================================
  // TESTE 1: CONEXÃO WIFI
  // ========================================
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Conectando ao WiFi");
  
  int tentativas = 0;
  while (WiFi.status() != WL_CONNECTED && tentativas < 20) {
    delay(500);
    Serial.print(".");
    tentativas++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWIFI CONECTADO!");
    Serial.print("IP: "); Serial.println(WiFi.localIP());
    Serial.print("Sinal: "); Serial.print(WiFi.RSSI()); Serial.println(" dBm");
  } else {
    Serial.println("\nERRO: WiFi não conectado!");
    Serial.println("Reiniciando...");
    delay(5000);
    ESP.restart();
  }

  // ========================================
  // CONFIGURAR FIREBASE
  // ========================================
  config.database_url = DATABASE_URL;
  config.signer.tokens.legacy_token = DATABASE_SECRET;

  Serial.println("\nConfigurando Firebase...");
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // ========================================
  // TESTE 2: CONEXÃO COM FIREBASE
  // ========================================
  Serial.println("Testando conexão com Firebase...");

  // Tenta ler um nó de teste
  if (Firebase.getInt(fbdo, "/test_connection")) {  // ← API NOVA
    Serial.println("FIREBASE CONECTADO COM SUCESSO!");
    Serial.printf("Teste: %d\n", fbdo.intData());
    firebaseConectado = true;
  } else {
    Serial.printf("Falha na leitura: %s\n", fbdo.errorReason().c_str());
    
    if (Firebase.ready()) {
      Serial.println("Firebase READY (conexão OK, mas nó não existe)");
      firebaseConectado = true;
    } else {
      Serial.println("ERRO: Firebase NÃO conectado!");
      Serial.println("Verifique:");
      Serial.println("  1. Database Secret");
      Serial.println("  2. URL do banco");
      Serial.println("  3. Regras do Firebase");
      delay(5000);
      ESP.restart();
    }
  }

  Serial.println("\nSISTEMA PRONTO!");
  Serial.println("LED controlado por /led no Firebase");
  Serial.println("=====================================");
}

void loop() {
  if (!firebaseConectado) {
    Serial.println("Firebase desconectado...");
    delay(5000);
    return;
  }

  // ========================================
  // LER LED DO FIREBASE (API NOVA!)
  // ========================================
  if (Firebase.getBool(fbdo, "/led")) {  // ← CORRETO
    bool ledState = fbdo.to<bool>();
    digitalWrite(LED_PIN, ledState ? HIGH : LOW);
    Serial.printf("LED: %s\n", ledState ? "LIGADO" : "DESLIGADO");
  } else {
    Serial.printf("Erro ao ler /led: %s\n", fbdo.errorReason().c_str());
  }

  // Status WiFi a cada 30s
  static unsigned long ultimo = 0;
  if (millis() - ultimo > 30000) {
    Serial.printf("WiFi OK | Sinal: %d dBm\n", WiFi.RSSI());
    ultimo = millis();
  }

  delay(1000);
}