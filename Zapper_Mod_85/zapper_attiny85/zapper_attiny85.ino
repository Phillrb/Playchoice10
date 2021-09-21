
const int ACTION_PIN = 0;
const int ZAPPER_FIRE_MIN_MS = 100;

void setup() {

  // Prepare for read
  pinMode(ACTION_PIN, INPUT);

  // WAIT 1 sec (let inputs settle)
  delay(1000);
}

void loop() {

  // Firing?
  if(digitalRead(ACTION_PIN) == LOW) {

    // Hold the firing signal for longer!
    pinMode(ACTION_PIN, OUTPUT);
    digitalWrite(ACTION_PIN, LOW);
    delay(ZAPPER_FIRE_MIN_MS);

    // Stop extending the fire signal
    pinMode(ACTION_PIN, INPUT);
  }
  
}
