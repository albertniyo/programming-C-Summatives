/**
 * Smart Parking Lot Monitoring System
 * Starts with all spaces available
 * After boot, nothing changes until a real button click (set LOW to HIGH)
 * sketch used in Tinkercad
 */

#define num_spaces 4
#define debounce_delay 50
#define boot_settle_ms 200

// one parking space as button, LED, occupy state, debounce helpers
struct ParkingSpace {
    uint8_t id;
    uint8_t buttonPin;
    uint8_t ledPin;
    bool isOccupied;
    bool lastButtonState;
    bool stableState;
    unsigned long lastDebounceTime;
};

// dynamic array of spaces (filled in setup)
ParkingSpace *parkingLot = nullptr;
int totalOccupied = 0;
bool systemArmed = false;
unsigned long bootMillis = 0;

void print_system_status();
void sync_button_baseline();

void setup() {
    Serial.begin(9600);

    parkingLot = new ParkingSpace[num_spaces];

    for (int i = 0; i < num_spaces; i++) {
        ParkingSpace *slot = parkingLot + i;
        slot->id = i + 1;
        slot->buttonPin = 2 + (i * 2); // button pins 2, 4, 6, 8
        slot->ledPin = 3 + (i * 2);    // LED pins 3, 5, 7, 9

        // force empty lot at boot to start fresh
        slot->isOccupied = false;
        slot->lastDebounceTime = 0;

        pinMode(slot->buttonPin, INPUT);
        pinMode(slot->ledPin, OUTPUT);
        digitalWrite(slot->ledPin, LOW); // LEDs off means available
    }

    // read real pin levels so we do not invent a fake press at startup
    sync_button_baseline();

    bootMillis = millis();
    systemArmed = false;

    Serial.println(F("system booting..."));
    Serial.println(F("waiting for button clicks..."));
    print_system_status();
}

void loop() {
    unsigned long currentMillis = millis();

    // ignore all edges until pins have settled after power-on
    if (!systemArmed) {
        sync_button_baseline();
        if (currentMillis - bootMillis < boot_settle_ms) {
            return;
        }
        systemArmed = true;
        sync_button_baseline();
        return;
    }

    bool stateChanged = false;

    for (int i = 0; i < num_spaces; i++) {
        ParkingSpace *slot = parkingLot + i;
        int reading = digitalRead(slot->buttonPin);

        // reset debounce timer if the raw reading bounced
        if (reading != slot->lastButtonState) {
            slot->lastDebounceTime = currentMillis;
        }

        // only act once the signal has been stable long enough
        if ((currentMillis - slot->lastDebounceTime) > debounce_delay) {
            if (reading != slot->stableState) {
                slot->stableState = reading;

                // one action per click: rising edge only (press), ignore release
                if (slot->stableState == HIGH) {
                    if (!slot->isOccupied) {
                        // car drives in
                        slot->isOccupied = true;
                        totalOccupied++;
                        digitalWrite(slot->ledPin, HIGH);
                        stateChanged = true;
                    } else {
                        // car leaves
                        slot->isOccupied = false;
                        totalOccupied--;
                        digitalWrite(slot->ledPin, LOW);
                        stateChanged = true;
                    }
                }
            }
        }
        slot->lastButtonState = reading;
    }

    if (stateChanged) {
        print_system_status();
    }
}

// copy real pin levels into last or stable state
void sync_button_baseline() {
    unsigned long now = millis();
    for (int i = 0; i < num_spaces; i++) {
        ParkingSpace *slot = parkingLot + i;
        bool level = digitalRead(slot->buttonPin);
        slot->lastButtonState = level;
        slot->stableState = level;
        slot->lastDebounceTime = now;
    }
}

// print occupied or available for every space
void print_system_status() {
    Serial.println(F("\n--- Parking Lot Monitor System ---"));
    for (int i = 0; i < num_spaces; i++) {
        ParkingSpace *slot = parkingLot + i;
        Serial.print(F("space "));
        Serial.print(slot->id);
        Serial.print(F(": "));
        Serial.println(slot->isOccupied ? F("occupied [X]") : F("available [ ]"));
    }

    Serial.print(F("total occupied: "));
    Serial.print(totalOccupied);
    Serial.print(F(" - total available: "));
    Serial.println(num_spaces - totalOccupied);
}
