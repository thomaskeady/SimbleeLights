
#define ONBOARD_LED 13

void setup() {
  // put your setup code here, to run once:

  // Setup serial to receive from Simblee
  Serial.begin(9600);

  // Anything else?
  pinMode(ONBOARD_LED, OUTPUT);
}

bool LED_ON = false;

void loop() {
  // put your main code here, to run repeatedly:
  // Check if anything in UART buffer
  //  if something, execute
  while (Serial.available() > 0)
  {
    while (Serial.available() > 0)
    {
      Serial.read();
    }
    if (LED_ON)
    {
      digitalWrite(ONBOARD_LED, LOW);
      LED_ON = false;
    }
    else
    {
      digitalWrite(ONBOARD_LED, HIGH);
      LED_ON = true;
    }

  }

  // Run strip


}
