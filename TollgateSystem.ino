/ ============================================== // COMPLETE TOLL 
GATE SYSTEM - ESP32 - SMOOTH SERVO // RFID + Ultrasonic + Servo + Buzzer + 
RGB LED // ============================================== #include 
<SPI.h> #include <MFRC522.h> // ==================== PIN DEFINITIONS 
==================== // RFID #define RST_PIN 2 // GPIO 2 #define SS_PIN 5 // 
GPIO 5 // Ultrasonic #define TRIG_PIN 13 // GPIO 13 #define ECHO_PIN 34 // GPIO 
34 (5V tolerant) // Servo (Connect to 5V power!) #define SERVO_PIN 4 // GPIO 4 // 
Buzzer #define BUZZER_PIN 15 // GPIO 15 // RGB LED #define RED_PIN 21 // GPIO 
21 #define GREEN_PIN 22 // GPIO 22 #define BLUE_PIN 23 // GPIO 23 // 
==================== SYSTEM SETTINGS ==================== // Servo positions #define GATE_CLOSE 0 // Gate closed position #define GATE_OPEN 90 // Gate open position // Distance settings (cm) #define MIN_DISTANCE 2 #define 
MAX_DISTANCE 400 #define CAR_DISTANCE 30 // Car within 30cm // Timing #define GATE_OPEN_TIME 5000 // 5 seconds gate open #define ALARM_TIME 3000 // 3 
seconds alarm // ==================== AUTHORIZED CARDS 
==================== // ADD YOUR RFID CARD UIDs HERE (uppercase, no spaces) String authorizedCards[] = { "416C9205", // Your authorized card 1 "3DCAF005", // Your authorized card 2 // Add more cards as needed }; int numAuthorizedCards = 2; // ==================== GLOBAL VARIABLES ==================== MFRC522 mfrc522(SS_PIN, RST_PIN); // RFID instance bool gateOpen = false; bool carDetected = false; bool alarmActive = false; bool servoMoving = false; unsigned long gateOpenTime = 0; unsigned long alarmStartTime 
= 0; unsigned long lastServoUpdate = 0; int currentServoAngle = 0; int targetServoAngle = 0; // ==================== SETUP FUNCTION 
==================== void setup() { Serial.begin(115200); delay(1000); 
Serial.println("\n\n"); 
Serial.println("╔══════════════════════════════════════════╗"); Serial.println("║ SMOOTH TOLL GATE SYSTEM - ESP32 ║"); Serial.println("╚══════════════════════════════════════════╝"); 
// Initialize all components initializeComponents(); // Play startup sequence startupSequence(); Serial.println("\n  SYSTEM READY!"); 
Serial.println("══════════════════════════════════════════"); } // ==================== INITIALIZE ALL COMPONENTS ==================== 
void initializeComponents() { Serial.println("\n  INITIALIZING COMPONENTS..."); // 1. Initialize Servo Serial.print("1. Servo Motor... "); pinMode(SERVO_PIN, OUTPUT); currentServoAngle = GATE_CLOSE; targetServoAngle = GATE_CLOSE; setServoSmooth(GATE_CLOSE); Serial.println("✓ READY (GPIO 4)"); Serial.println(" 
  Ensure RED wire → 5V (not 3.3V)"); // 2. Initialize RGB LED Serial.print("2. RGB LED... "); pinMode(RED_PIN, OUTPUT); pinMode(GREEN_PIN, OUTPUT); pinMode(BLUE_PIN, OUTPUT); setRGB("BLUE"); Serial.println("✓ READY (GPIO 
21,22,23)"); // 3. Initialize Buzzer Serial.print("3. Buzzer... "); pinMode(BUZZER_PIN, 
OUTPUT); digitalWrite(BUZZER_PIN, LOW); Serial.println("✓ READY (GPIO 15)"); // 4. 
Initialize Ultrasonic Serial.print("4. Ultrasonic Sensor... "); pinMode(TRIG_PIN, OUTPUT); pinMode(ECHO_PIN, INPUT); digitalWrite(TRIG_PIN, LOW); // Test ultrasonic float testDist = getDistance(); if (testDist > 0) { Serial.print("✓ WORKING ("); 
Serial.print(testDist); Serial.println(" cm)"); } else { Serial.println("  NO READING - Check wiring"); } // 5. Initialize RFID Serial.print("5. RFID Reader... "); SPI.begin(); mfrc522.PCD_Init(); delay(100); byte rfidVersion = 
mfrc522.PCD_ReadRegister(MFRC522::VersionReg); if (rfidVersion == 0x00 || rfidVersion == 0xFF) { Serial.println("  NOT DETECTED"); } else { Serial.print("✓ CONNECTED (v0x"); Serial.print(rfidVersion, HEX); Serial.println(")"); 
mfrc522.PCD_SetAntennaGain(MFRC522::RxGain_max); } delay(500); } // 
==================== STARTUP SEQUENCE ==================== void 
startupSequence() { Serial.println("\n  STARTUP SEQUENCE..."); // Beep pattern for 
(int i = 0; i < 3; i++) { digitalWrite(BUZZER_PIN, HIGH); delay(100); digitalWrite(BUZZER_PIN, LOW); delay(100); } // LED sequence setRGB("RED"); delay(300); setRGB("GREEN"); delay(300); setRGB("BLUE"); delay(300); setRGB("OFF"); // Smooth servo test Serial.println("\n  Testing servo smooth movement..."); openGate(); delay(2000); closeGate(); Serial.println("  STARTUP 
COMPLETE"); } // ==================== SMOOTH SERVO FUNCTIONS 
==================== void setServoPulse(int angle) { angle = constrain(angle, 0, 
180); int pulseWidth = map(angle, 0, 180, 500, 2400); // 0.5ms to 2.4ms digitalWrite(SERVO_PIN, HIGH); delayMicroseconds(pulseWidth); 
digitalWrite(SERVO_PIN, LOW); // IMPORTANT: Servos need consistent 20ms refresh rate delayMicroseconds(20000 - pulseWidth); } void setServoSmooth(int angle) { angle = constrain(angle, 0, 180); currentServoAngle = angle; setServoPulse(angle); } void updateServoSmooth() { if (!servoMoving || millis() - lastServoUpdate < 20) return; lastServoUpdate = millis(); // Move 1 degree per step for smooth movement if 
(currentServoAngle < targetServoAngle) { currentServoAngle++; } else if 
(currentServoAngle > targetServoAngle) { currentServoAngle--; } else { servoMoving = false; Serial.print("  Servo reached target: "); Serial.println(currentServoAngle); return; } setServoPulse(currentServoAngle); } void openGate() { if (gateOpen || servoMoving) return; Serial.println("\n  OPENING GATE..."); setRGB("GREEN"); beep(2, 100); targetServoAngle = GATE_OPEN; servoMoving = true; lastServoUpdate = millis(); gateOpen = true; gateOpenTime = millis(); } void closeGate() { if (!gateOpen || servoMoving) return; Serial.println("\n  CLOSING GATE..."); setRGB("BLUE"); beep(1, 100); targetServoAngle = GATE_CLOSE; servoMoving = true; lastServoUpdate = millis(); gateOpen = false; } // ==================== ULTRASONIC FUNCTIONS ==================== float getDistance() { digitalWrite(TRIG_PIN, LOW); delayMicroseconds(2); digitalWrite(TRIG_PIN, HIGH); delayMicroseconds(10); digitalWrite(TRIG_PIN, LOW); long duration = pulseIn(ECHO_PIN, HIGH, 30000); if (duration > 0) { return duration * 0.0343 / 2; } return -1; } void checkCarPresence() { static unsigned long lastCheck = 0; if (millis() - lastCheck < 500) return; float distance = getDistance(); lastCheck = millis(); bool previousState = carDetected; carDetected = (distance >= MIN_DISTANCE && distance <= CAR_DISTANCE); if (carDetected != previousState) { if (carDetected) { Serial.print("  CAR DETECTED: "); Serial.print(distance); Serial.println(" cm"); setRGB("YELLOW"); } else { 
Serial.println("  CAR LEFT"); if (!gateOpen && !alarmActive) { setRGB("BLUE"); } } } } 
// ==================== BUZZER FUNCTIONS ==================== void 
beep(int count, int duration) { for (int i = 0; i < count; i++) { digitalWrite(BUZZER_PIN, HIGH); delay(duration); digitalWrite(BUZZER_PIN, LOW); if (i < count - 1) delay(duration); } } void startAlarm() { alarmActive = true; alarmStartTime = millis(); 
Serial.println("\n  ALARM ACTIVATED - UNAUTHORIZED ACCESS!"); setRGB("RED"); } void stopAlarm() { alarmActive = false; digitalWrite(BUZZER_PIN, 
LOW); setRGB("BLUE"); Serial.println("\n  ALARM STOPPED"); } void updateAlarm() { if (!alarmActive) return; if (millis() - alarmStartTime >= ALARM_TIME) { stopAlarm(); return; } // Rapid beeping static unsigned long lastBeep = 0; if (millis() - lastBeep >= 100) { digitalWrite(BUZZER_PIN, !digitalRead(BUZZER_PIN)); lastBeep = millis(); } } // 
==================== RGB LED FUNCTIONS ==================== void 
setRGB(String color) { // Turn all OFF first digitalWrite(RED_PIN, HIGH); digitalWrite(GREEN_PIN, HIGH); digitalWrite(BLUE_PIN, HIGH); if (color == "RED") { digitalWrite(RED_PIN, LOW); } else if (color == "GREEN") { digitalWrite(GREEN_PIN, LOW); } else if (color == "BLUE") { digitalWrite(BLUE_PIN, LOW); } else if (color == 
"YELLOW") { digitalWrite(RED_PIN, LOW); digitalWrite(GREEN_PIN, LOW); } else if (color == "CYAN") { digitalWrite(GREEN_PIN, LOW); digitalWrite(BLUE_PIN, LOW); } else if (color == "MAGENTA") { digitalWrite(RED_PIN, LOW); digitalWrite(BLUE_PIN, LOW); } else if (color == "WHITE") { digitalWrite(RED_PIN, LOW); 
digitalWrite(GREEN_PIN, LOW); digitalWrite(BLUE_PIN, LOW); } } // 
==================== RFID FUNCTIONS ==================== bool 
isCardAuthorized(String cardUID) { for (int i = 0; i < numAuthorizedCards; i++) { if 
(cardUID.equals(authorizedCards[i])) { return true; } } return false; } void checkRFID() { // Reset RFID if needed mfrc522.PCD_Init(); if (!mfrc522.PICC_IsNewCardPresent()) return; if (!mfrc522.PICC_ReadCardSerial()) return; // Get card UID String cardUID = ""; for (byte i = 0; i < mfrc522.uid.size; i++) { if (mfrc522.uid.uidByte[i] < 0x10) cardUID += "0"; cardUID += String(mfrc522.uid.uidByte[i], HEX); } cardUID.toUpperCase(); 
Serial.println("\n══════════════════════════════════════════"); 
Serial.print("  CARD DETECTED: "); Serial.println(cardUID); if (isCardAuthorized(cardUID)) { Serial.println("  ACCESS GRANTED"); setRGB("GREEN"); beep(2, 100); openGate(); } else { Serial.println("  ACCESS DENIED - UNAUTHORIZED"); startAlarm(); } 
Serial.println("══════════════════════════════════════════\n"); mfrc522.PICC_HaltA(); mfrc522.PCD_StopCrypto1(); } // ==================== MAIN LOOP ==================== void loop() { // 1. Check for RFID cards checkRFID(); // 2. Check car presence checkCarPresence(); // 3. Update alarm if active updateAlarm(); // 4. Update smooth servo movement updateServoSmooth(); // 5. Autoclose gate if no car and gate open for too long if (gateOpen && !carDetected && 
!servoMoving && (millis() - gateOpenTime >= GATE_OPEN_TIME)) { 
Serial.println("\n  AUTO-CLOSING GATE - No car detected"); closeGate(); } // 6. Debug output every 3 seconds static unsigned long lastDebug = 0; if (millis() - lastDebug >= 3000) { Serial.print("  Status: Gate="); Serial.print(gateOpen ? "OPEN" : 
"CLOSED"); Serial.print(" | Car="); Serial.print(carDetected ? "YES" : "NO"); 
Serial.print(" | Servo="); Serial.print(currentServoAngle); Serial.print("° | Target="); Serial.print(targetServoAngle); Serial.println("°"); lastDebug = millis(); } // No delay needed - smooth servo uses timing } 
