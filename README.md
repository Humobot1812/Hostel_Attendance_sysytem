# 🏫 Hostel Attendance System using ESP8266 & RFID

An IoT-based hostel attendance system using ESP8266, MFRC522 RFID module, and Google Sheets for real-time entry/exit logging.

---

## 🚀 Features

- RFID-based student authentication
- Real-time Google Sheets logging
- Automatic Entry/Exit detection
- Gate-wise tracking
- WiFi-enabled cloud storage

---

## 🛠 Hardware Used

- ESP8266 (NodeMCU)
- MFRC522 RFID Module
- MIFARE 1K RFID Tags
- Breadboard + Jumper Wires

---

## 🔌 Wiring (NodeMCU)

| RFID | ESP8266 |
|------|----------|
| SDA  | D4       |
| SCK  | D5       |
| MOSI | D7       |
| MISO | D6       |
| RST  | D3       |
| GND  | GND      |
| 3.3V | 3.3V     |

---

## ☁ Google Sheets Integration

Uses Google Apps Script Web App to log attendance.

Flow:
RFID Scan → ESP8266 → HTTPS POST → Google Apps Script → Google Sheet

---

## 📂 Project Structure

## 📂 Project Structure

```
Hostel_Attendance_System/
│
├── ESP8266_Attendance/
│   └── attendance.ino
│
├── RFID_Tag_Writer/
│   └── rfid_tag.ino
│
├── Google_App_Script/
│   └── sheet.gs
│
├── images/
│   ├── hardware_setup.jpg
│   └── google_sheet_output.jpg
│
├── README.md
└── .gitignore
```
## 🔐 Security Notice

Before uploading:
- Replace WiFi credentials
- Replace Google Script ID
- Replace Google Sheet ID

---

## 📸 Output

Entry and Exit timestamps are recorded automatically.

---

## 🧠 How Entry/Exit Works

- First scan → Marks Time In
- Second scan → Marks Time Out
- Uses Student ID + Gate Number logic

---

## 👨‍💻 Author

Abhinav Goel  

