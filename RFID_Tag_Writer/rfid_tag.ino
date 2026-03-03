#include <SPI.h>
#include <MFRC522.h>

//--------------------------------------------------
// Connections (NodeMCU ESP8266)
// RST -> D3  |  SDA(SS) -> D4  |  MOSI -> D7  |  MISO -> D6  |  SCK -> D5
//--------------------------------------------------
const uint8_t RST_PIN = D3;
const uint8_t SS_PIN  = D4;
MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;

//--------------------------------------------------
int blockNum;
byte bufferLen = 18;
byte readBlockData[18];
MFRC522::StatusCode status;
//--------------------------------------------------

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("Scan a MIFARE 1K Tag to write preset data...");
}

/****************************************************************************************************
 * loop()
 ****************************************************************************************************/
void loop() {
  // Default authentication key
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  // Wait for a new RFID tag
  if (!mfrc522.PICC_IsNewCardPresent()) return;
  if (!mfrc522.PICC_ReadCardSerial()) return;

  Serial.println("\n**Card Detected**");
  Serial.print("Card UID:");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println();
  Serial.print("PICC Type: ");
  Serial.println(mfrc522.PICC_GetTypeName(mfrc522.PICC_GetType(mfrc522.uid.sak)));

  // Automatically write predefined data to RFID tag
  writeFixedData();

  // Halt card before next scan
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}

/****************************************************************************************************
 * writeFixedData()
 * Writes preset values to specific memory blocks on the card
 ****************************************************************************************************/
void writeFixedData() {
  Serial.println("\nWriting fixed data to RFID card...");

  // Each block can store 16 bytes
  writeStringToBlock(4,  "24BEC095");         // Student ID
  writeStringToBlock(5,  "Pranshu");          // First Name
  writeStringToBlock(6,  "Sharma");             // Last Name
  writeStringToBlock(8,  "9201644957");       // Phone Number
  writeStringToBlock(9,  "IIITDM Jabalpur");  // Address

  Serial.println("✅ All data written successfully!");
}

/****************************************************************************************************
 * writeStringToBlock()
 ****************************************************************************************************/
void writeStringToBlock(int blockNum, const char *text) {
  byte buffer[16];
  byte len = strlen(text);
  for (byte i = 0; i < 16; i++) {
    if (i < len) buffer[i] = text[i];
    else buffer[i] = ' ';
  }

  // Authenticate
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A,
                                    blockNum, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print("Auth failed (Block ");
    Serial.print(blockNum);
    Serial.print("): ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  // Write to block
  status = mfrc522.MIFARE_Write(blockNum, buffer, 16);
  if (status != MFRC522::STATUS_OK) {
    Serial.print("Write failed (Block ");
    Serial.print(blockNum);
    Serial.print("): ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  // Read back to confirm
  ReadDataFromBlock(blockNum, readBlockData);
  dumpSerial(blockNum, readBlockData);
}

/****************************************************************************************************
 * ReadDataFromBlock()
 ****************************************************************************************************/
void ReadDataFromBlock(int blockNum, byte readBlockData[]) {
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A,
                                    blockNum, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print("Auth failed for Read (Block ");
    Serial.print(blockNum);
    Serial.print("): ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  status = mfrc522.MIFARE_Read(blockNum, readBlockData, &bufferLen);
  if (status != MFRC522::STATUS_OK) {
    Serial.print("Read failed (Block ");
    Serial.print(blockNum);
    Serial.print("): ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
}

/****************************************************************************************************
 * dumpSerial()
 ****************************************************************************************************/
void dumpSerial(int blockNum, byte blockData[]) {
  Serial.print("Block ");
  Serial.print(blockNum);
  Serial.print(" -> ");
  for (int j = 0; j < 16; j++) {
    Serial.write(readBlockData[j]);
  }
  Serial.println();
  // Clear buffer
  for (int i = 0; i < sizeof(readBlockData); ++i)
    readBlockData[i] = 0;
}
