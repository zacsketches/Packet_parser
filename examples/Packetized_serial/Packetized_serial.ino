/*  v0.3.  Implements the Packet_parser class with a different
 * 	library structure.
 *  
 *  The Packet_parser class attemps to simplify serial comms
 *  from a controller by listening for packetized information
 *  that is then stored and available for a user to query
 *  at any time.  If there is new information the query will
 *  deliver a pointer to a char array with the new information.
 *  The length of the array will be equal to the payload size
 *  set up for that target when it was added to the parser.
 * 
 *  v0.3 upgrades the Packet_parser to
 *  monitor multiple targets, with a single instance of 'parser' and 
 *  then respond to processes that query for a particular target.
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
  while(parser.listen()) { delay(5); }
  
  led_control();

  delay(10);

}
