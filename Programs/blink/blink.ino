//Test program
const uint8_t led = 2;//13 for Arduino UNO

void setup()
{
    Serial.begin(9600);
	pinMode(led, OUTPUT);
}

void loop()
{
	digitalWrite(led, HIGH);
    Serial.println("Led ences");
    delay(2000);
    digitalWrite(led, LOW);
    Serial.println("Led apagat");
    delay(2000);
}
