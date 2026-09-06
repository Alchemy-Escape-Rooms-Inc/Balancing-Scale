// ============================================================
// MANIFEST.h — WatchTower Device Manifest
// This file is parsed by sync_manifests.py for the WatchTower dashboard.
// Keep all values as #define strings unless noted otherwise.
// ============================================================

#define DEVICE_NAME           "BalancingScale"
#define FIRMWARE_VERSION      "1.3.1"
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
#define PUBLISH_TOPICS        "MermaidsTale/BalancingScale/status, MermaidsTale/BalancingScale/log, MermaidsTale/BalancingScale/message, MermaidsTale/BalancingScale/message/servo, MermaidsTale/BalancingScale/message/Spice, MermaidsTale/BalancingScale/system/Spice/weight, MermaidsTale/BalancingScale/system/Coins/weight, MermaidsTale/BalancingScale/message/SuccessCount, MermaidsTale/BalancingScale/message/ServoState, MermaidsTale/BalancingScale/{Yeast|SugarCane|Vanilla|Molasses|Cloves}"
#define SUPPORTED_COMMANDS    "PING, STATUS, RESET, PUZZLE_RESET, SOLVE, HALTCOIN, HALTSPICE, REBOOTCOIN, REBOOTSPICE"

// Hardware
#define PIN_CONFIG            "SERVO=13 (was 35 = input-only on classic ESP32; moved by contractor 07-21), RFID1_RX=18, RFID1_TX=19 (spice plate), RFID2_RX=16, RFID2_TX=17 (coins plate)"
#define COMPONENTS            "2x multi-tag 125kHz RFID reader (0xF5-frame protocol, 5 tags max each), 1x indicator servo, 5 spice pouch tags, 8 coin tags"
#define KNOWN_QUIRKS          "Classic ESP32. Servo on GPIO13 (500-2400us, 100Hz; yay=155 nay=30 mid=70). Commands must not be retained. SOLVED on /status is deliberately not retained (would replay into fresh sessions). Per-pouch success topics + SuccessCount publish once per transition, not per scan. M3 AMT.xml event 162 sends PUZZLE_RESET on game reset."

#define REPO_URL              "https://github.com/Alchemy-Escape-Rooms-Inc/Balancing-Scale"
