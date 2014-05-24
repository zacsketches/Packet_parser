/*  
 *  The Packet_parser simplifies serial comms onboard a device
 *  that is listening for Serial commands that affect more
 *  than one function.  
 *  
 *  For example, I use it on a small robot that I control from
 *  an Arduino Esplora.  I send joystick commands to the right
 *  and left motor.  I send commands from the buttons to turn
 *  and spin.  I also send commands to make the LEDs light up.
 * 
 *  The Packet_parser will monitor all the serial comms and save
 *  the latest data for each function.
 *
 *  Then each function uses a query() to get the latest
 *  data from the parser.  If there is new information the query will
 *  deliver a pointer to a char array with the new information.
 *  The length of the array will be equal to the payload size
 *  set up for that target when it was added to the parser.
 *
 *  In my example, I use add_packet('L', 1) to tell the parser
 *  to listen for packets with the target 'L' and a payload size
 *  of one byte.  The payload size determines how many bytes of
 *  information the parser stores for the 'L' message.  So 
 *  my parser is looking for the message header '||' followed
 *  by the target identifier, 'L'.  This is followed by one byte
 *  of data.  Whenever a properly formed packet is received via
 *  the serial connection (i.e. '||LH' would be a properly formed
 *  packet) then it stores the data awaiting a query.
 * 
 *  Further in my example, the led_control function queries the 
 *  parser with query(char tgt, char* data).  That char* is a pointer
 *  to the location where you want the parser to dump it's latest
 *  data.  In the example I use a global variable so it is easily 
 *  accessible from anywhere in the sketch.
 *
 *  Then led_command() acts on the two choices and turns the Arduino's
 *  onboard LED on or off depending on the payload value received.
 *
 *  When you run the example, try typing lots of junk into the serial
 *  monitor.  You will notice the recieve light on the Arduino is 
 *  getting all the data, but since this data is not properly formed
 *  packets the data is discarded.  
 * 
 *  Finally, the example declares parser like this,
 *        Packet_parser parser(true);  
 *  The 'true' tells the parser to echo packets that it receives back
 *  to the serial port so you can troubleshoot your implementation.  
 *  In deployment, implement a parser like this,
 *        Packet_parser parser();
 *  This will prevent the echo and keep less traffic on the serial 
 *  line.
 * 
 *  The config() function only prints packets monitored within the
 *  parser.  It is NOT necessary to call config() in the script that
 *  uses the parser.
 *
 */

#include <Packet_parser.h>

//#include <Esplora.h>

Packet_parser parser(true);
char led_cmd[1] = {'H'};    //default led to L, other state is H 

void led_control() {
  parser.query('L', led_cmd);
  char cmd = led_cmd[0];
  if(cmd == 'H') digitalWrite(13, HIGH);
  if(cmd == 'L') digitalWrite(13, LOW);
}

void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);

  delay(200);
  
  parser.config();
  
  parser.add_packet(1, 'L');
  parser.add_packet(3, 'M');
  
  parser.config();
  
  char test = 'L';
  if(parser.target_is_monitored(test) ) {
    Serial.print("Monitoring target: ");
    Serial.println(test);
    int ps = parser.payload_size(test);
    Serial.print("\tpayload size is: ");
    Serial.println(ps);
  } else {
    Serial.print("Not monitoring target: ");
    Serial.println(test);
  }
  
  test = 'R';
  if(parser.target_is_monitored(test) ) {
    Serial.print("Monitoring target: ");
    Serial.println(test);
    int ps = parser.payload_size(test);
    Serial.print("\tpayload size is: ");
    Serial.println(ps);
  } else {
    Serial.print("Not monitoring target: ");
    Serial.println(test);
  }
  Serial.print("led_cmd is: ");
  Serial.println(led_cmd);
}

void loop() {
  while(parser.listen()) { delay(1); }
  
  led_control();

}
