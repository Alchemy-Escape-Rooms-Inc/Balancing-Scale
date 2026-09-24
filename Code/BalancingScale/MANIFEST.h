// ============================================================
// MANIFEST.h — WatchTower Device Manifest
// This file is parsed by sync_manifests.py for the WatchTower dashboard.
// Keep all values as #define strings unless noted otherwise.
// ============================================================

#define DEVICE_NAME           "BalancingScale"
#define FIRMWARE_VERSION      "1.4.0"
#define BOARD_TYPE            "ESP32 (classic, CP210x USB bridge)"
#define ROOM                  "MermaidsTale"
#define DESCRIPTION           "RFID pendulum scale: two multi-tag RFID readers (spice pouches vs coins plates) match represented weights; servo indicates balance; publishes SOLVED on /status when all 5 pouch weights have been matched (M3 forwards as MermaidsTale/ScaleSolved)"

#define BUILD_STATUS          "stable"
#define CODE_HEALTH           "fair"
#define WATCHTOWER_COMPLIANCE "full"

// MQTT
#define BROKER_IP             "10.1.10.115"
#define BROKER_PORT           1883
#define HEARTBEAT_MS          300000

// Over-the-air updates (MANDATORY fleet protocol since 2026-09-22, added v1.4.0).
// Password = the Wi-Fi password (OTA_PASSWORD aliases WIFI_PASS in the sketch).
#define OTA_ENABLED           "yes"
#define OTA_HOSTNAME          "BalancingScale"      // = DEVICE_NAME
#define OTA_PORT              3232                  // classic ESP32

#define SUBSCRIBE_TOPICS      "MermaidsTale/BalancingScale/command"
#define PUBLISH_TOPICS        "MermaidsTale/BalancingScale/status, MermaidsTale/BalancingScale/log, MermaidsTale/BalancingScale/message, MermaidsTale/BalancingScale/message/servo, MermaidsTale/BalancingScale/message/Spice, MermaidsTale/BalancingScale/system/Spice/weight, MermaidsTale/BalancingScale/system/Coins/weight, MermaidsTale/BalancingScale/message/SuccessCount, MermaidsTale/BalancingScale/message/ServoState, MermaidsTale/BalancingScale/{Yeast|SugarCane|Vanilla|Molasses|Cloves}"
#define SUPPORTED_COMMANDS    "PING, STATUS (reply ... | IP:x.x.x.x | OTA:3232 | RSSI=x | UPxs), RESET, PUZZLE_RESET, SOLVE, HALTCOIN, HALTSPICE, REBOOTCOIN, REBOOTSPICE"

// Hardware
#define PIN_CONFIG            "SERVO=13 (was 35 = input-only on classic ESP32; moved by contractor 07-21), RFID1_RX=18, RFID1_TX=19 (spice plate), RFID2_RX=16, RFID2_TX=17 (coins plate)"
#define COMPONENTS            "2x multi-tag 125kHz RFID reader (0xF5-frame protocol, 5 tags max each), 1x indicator servo, 5 spice pouch tags, 8 coin tags"
#define KNOWN_QUIRKS          "Classic ESP32. Servo on GPIO13 (500-2400us, 100Hz; yay=155 nay=30 mid=70). Commands must not be retained. SOLVED on /status is deliberately not retained (would replay into fresh sessions). Per-pouch success topics + SuccessCount publish once per transition, not per scan. M3 AMT.xml event 162 sends PUZZLE_RESET on game reset. v1.4.0 OTA: wireless re-flash (hostname BalancingScale, port 3232, password = Wi-Fi) - arduino-cli upload --fqbn esp32:esp32:esp32 -p <board IP> --upload-field password=<Wi-Fi pw> Code/BalancingScale; IP is in every STATUS reply and the boot /log line. USB flashes go through the CP210x bridge (COM13 on 09-24, COM11 on 09-06 - port number moves)."

#define REPO_URL              "https://github.com/Alchemy-Escape-Rooms-Inc/Balancing-Scale"
