int ledPin = D3;
int EP =D2;

void setup(){
  pinMode(ledPin, OUTPUT);
  pinMode(EP, INPUT); //set EP input for measurment
  Serial.begin(115200); //init serial 9600
  Serial.println("\nWelcome, Vibration Detectore V2.5, 2017/3/5");
  Serial.println("Begin at " + String(millis()/1000));
}
void loop(){
  long measurement =TP_init();
  delay(50);
 // Serial.print("measurment = ");
  if (true || measurement > 0) {
    Serial.print(String(millis()/1000) + ": ");
    for (int i=0; i< measurement/40; i++)
      Serial.print('O');
    Serial.print(" ");
    Serial.println(measurement);
  }
  if (measurement > 1000){
    digitalWrite(ledPin, HIGH);
  }
  else{
    digitalWrite(ledPin, LOW); 
  }
}

long TP_init(){
  delay(10);
  long measurement=pulseIn (EP, HIGH);  //wait for the pin to get HIGH and returns measurement
  return measurement;
}
