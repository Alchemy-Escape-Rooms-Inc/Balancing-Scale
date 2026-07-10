// ============================================================
// MANIFEST.h — WatchTower Device Manifest
// This file is parsed by sync_manifests.py for the WatchTower dashboard.
// Keep all values as #define strings unless noted otherwise.
// ============================================================

#define DEVICE_NAME           "BalancingScale"
#define FIRMWARE_VERSION      "1.1.0"
#define BOARD_TYPE            "ESP32 (variant unverified)"
#define ROOM                  "MermaidsTale"
#define DESCRIPTION           "RFID pendulum scale: two multi-tag RFID readers (spice pouches vs coins plates) match represented weights; servo indicates balance; publishes SOLVED on /status when all 5 pouch weights have been matched (M3 forwards as MermaidsTale/ScaleSolved)"

#define BUILD_STATUS          "stable"
#define CODE_HEALTH           "fair"
#define WATCHTOWER_COMPLIANCE "full"

// MQTT
#define BROKER_IP             "10.1.10.115"
#define BROKER_PORT           1883
#define HEARTBEAT_MS          300000

#define SUBSCRIBE_TOPICS      "MermaidsTale/BalancingScale/command"
#define PUBLISH_TOPICS        "MermaidsTale/BalancingScale/status, MermaidsTale/BalancingScale/log, MermaidsTale/BalancingScale/message, MermaidsTale/BalancingScale/message/servo, MermaidsTale/BalancingScale/message/Spice, MermaidsTale/BalancingScale/system/Spice/weight, MermaidsTale/BalancingScale/system/Coins/weight"
#define SUPPORTED_COMMANDS    "PING, STATUS, RESET, PUZZLE_RESET, SOLVE, HALTCOIN, HALTSPICE, REBOOTCOIN, REBOOTSPICE"

// Hardware
#define PIN_CONFIG            "SERVO=35, RFID1_RX=18, RFID1_TX=19 (spice plate), RFID2_RX=16, RFID2_TX=17 (coins plate)"
#define COMPONENTS            "2x multi-tag 125kHz RFID reader (0xF5-frame protocol, 5 tags max each), 1x indicator servo, 5 spice pouch tags, 8 coin tags"
#define KNOWN_QUIRKS          "Board variant undocumented: GPIO35 as servo output requires an S3-class part (input-only on classic ESP32); README describes an obsolete Arduino Nano build. Commands must not be retained. SOLVED on /status is deliberately not retained (would replay into fresh sessions). M3 AMT.xml event 162 sends RESET on game reset, which reboots the board per protocol - PUZZLE_RESET is the non-reboot alternative."

#define REPO_URL              "https://github.com/Alchemy-Escape-Rooms-Inc/Balancing-Scale"
