
const int ledPins[3] = { 13, 10, 7 };
const int buttonPins[3] = { 12, 9, 6 };
const int buzzer = 3;

int sequence[50];
int currentRound = 3;
const int notes[3] = { 523, 587, 659 };
const int wrongTone = 1000;

const unsigned long TIMEOUT_LIMIT = 5000;

void setup() {
  for (int i = 0; i < 3; i++) {
    pinMode(ledPins[i], OUTPUT);
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
  pinMode(buzzer, OUTPUT);
  randomSeed(analogRead(A0));
}

void loop() {
  delay(1000);

  for (int i = 0; i < currentRound; i++) {
    sequence[i] = random(0, 3);
  }

  playCurrentSequence();

  for (int i = 0; i < currentRound; i++) {
    int expectedIdx = sequence[i];
    int pressedIdx = -1;
    unsigned long lastActionTime = millis();

    while (pressedIdx == -1) {
      if (millis() - lastActionTime > TIMEOUT_LIMIT) {
        playCurrentSequence();
        lastActionTime = millis();
      }

      for (int j = 0; j < 3; j++) {
        if (digitalRead(buttonPins[j]) == LOW) {
          delay(50);
          pressedIdx = j;

          digitalWrite(ledPins[j], HIGH);
          tone(buzzer, notes[j]);

          while (digitalRead(buttonPins[j]) == LOW);
          delay(50);

          digitalWrite(ledPins[j], LOW);
          noTone(buzzer);
        }
      }
    }
    if (pressedIdx != expectedIdx) {
      handleGameOver();
      return;
    }
  }

  delay(500);
  currentRound++;
  if (currentRound >= 50) currentRound = 3;
}

void playCurrentSequence() {
  for (int i = 0; i < currentRound; i++) {
    int idx = sequence[i];
    digitalWrite(ledPins[idx], HIGH);
    tone(buzzer, notes[idx]);
    delay(500);
    digitalWrite(ledPins[idx], LOW);
    noTone(buzzer);
    delay(200);
  }
}

void handleGameOver() {
  for (int i = 0; i < 3; i++) {
    tone(buzzer, wrongTone);
    for (int j = 0; j < 3; j++) digitalWrite(ledPins[j], HIGH);
    delay(300);
    noTone(buzzer);
    for (int j = 0; j < 3; j++) digitalWrite(ledPins[j], LOW);
    delay(300);
  }
  currentRound = 3;
}
