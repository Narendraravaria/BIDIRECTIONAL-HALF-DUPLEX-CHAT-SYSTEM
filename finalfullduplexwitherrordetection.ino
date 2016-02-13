#define LED1 RED_LED
int LDR_Pin = A4;  //analog pin for LDR
int LDRReading;
int data,i,k,j,sum,ack;
int binary[8]={0,0,0,0,0,0,0,0};
int convert[7]={64,32,16,8,4,2,1};
void setup()
{
  Serial.begin(9600);
  pinMode(LED1, OUTPUT);
}
void loop()
{
  //Serial.println('Ready to Recieve');
  start:
  digitalWrite(LED1, LOW);
  delay(250);
  LDRReading = analogRead(LDR_Pin);
  //Serial.println(LDRReading);
  if (Serial.available() > 0 && LDRReading < 100) {
    data=Serial.read();
    Serial.println("You are sending ");
    Serial.write(data);
    for (i=0; i<7;i++){
      binary[6-i]=data%2;
      data=data>>1;
    }
    sum=0;
    binary[7]=0;
    for (i=0; i<7; i++){
     sum=sum+binary[i];
     //Serial.print(binary[i]);
    }
    if (sum%2 != 0){
      binary[7]=1;
    }
    Serial.print('\n');
    n:
    digitalWrite(LED1, HIGH);
    delay(1000);
    for (i=0; i<8; i++){
      //Serial.print(binary[i]);
      if (binary[i]==0){
        digitalWrite(LED1, HIGH);
      }
      else{
      digitalWrite(LED1, LOW);
      }
      delay(1000);
    }
   digitalWrite(LED1, LOW);   
   delay(1000);
   LDRReading = analogRead(LDR_Pin);
   if (LDRReading > 100){
     delay(2000);
     Serial.println("Resending due to error in frame:");
     goto n;
   }
   delay(1000);
   
  }
  if (LDRReading >100){
   rec:
   Serial.println("Recieving...");
   delay(1500);
   for (i=0;i<8;i++){
      LDRReading = analogRead(LDR_Pin); 
      //Serial.println(LDRReading);
      if (LDRReading < 100){
        binary[i]=1;
      }
      else{
        binary[i]=0;
      }
      delay(1000);
    }
    sum=0;
    for (i=0;i<8;i++){
      sum=sum+binary[i];
    }
    if (sum%2 !=0){
      Serial.println("error in frame send again");
      digitalWrite(LED1, HIGH);
      delay(2000);
      goto start;
    }
    else{
      delay(1000);
      data=0;
      for (i=0; i<7; i++){
        data=data+(binary[i]*convert[i]);
        Serial.print(binary[i]);
      }
       Serial.print(data);
      if(data != 13){
        Serial.print("The recieved character is:");
        Serial.write(data);
        Serial.print('\n');
        delay(750);
        goto rec;
      }
    
    }
  } 
}
