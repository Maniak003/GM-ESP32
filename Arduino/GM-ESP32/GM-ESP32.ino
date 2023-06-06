
#define SERIALON
#define LED 16
#define PULSEPIN 27
#define ANALOGOS 25
#define COUNTER 33
#define SENDTIME 2000
#define LEDTIME 20
#define PULSETIME 5
/* (400v ÷ (5G + 30M) × 30M) × (4096 ÷ 3.3v) */
#define VOLTAGE 2961

unsigned long tm, pulse_tm, led_tm, tm_tmp, cnt = 0, old_cnt, summ_interval = 0;
uint16_t OS, pulse2sec = 0;

void countPulse() {
  summ_interval = summ_interval + (millis() - led_tm);
  led_tm = millis();
  digitalWrite(LED, LOW);
  cnt++;
}

void setup() {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  pinMode(PULSEPIN, OUTPUT);
  digitalWrite(PULSEPIN, LOW);
  pinMode(ANALOGOS, INPUT);
  pinMode(COUNTER, INPUT_PULLUP);
  #ifdef SERIALON
  Serial.begin(115200);
  #endif
  attachInterrupt(COUNTER, countPulse, RISING);
  tm = millis();
  led_tm = tm;
}

void loop() {
  tm_tmp = millis();
  if (tm + SENDTIME < tm_tmp) {
    pulse2sec = cnt - old_cnt;
    old_cnt = cnt;
    tm = tm_tmp;
    #ifdef SERIALON
    Serial.print("CNT: ");
    Serial.print(cnt);
    Serial.print(" SUMM_INTERVAL: ");
    Serial.print(summ_interval);
    Serial.print(" now: ");
    Serial.println((float) ((1000.0 * cnt)/summ_interval) * 5);
    #endif
  }
  if (led_tm + LEDTIME < tm_tmp) {
    digitalWrite(LED, HIGH);
  }
  if (pulse_tm + PULSETIME < tm_tmp) {
    pulse_tm = tm_tmp;
    OS = analogRead(ANALOGOS);
    if (OS < (uint16_t) VOLTAGE) {
      noInterrupts();
      digitalWrite(PULSEPIN, HIGH);
      digitalWrite(PULSEPIN, LOW);
      interrupts();
    }
  }
}
