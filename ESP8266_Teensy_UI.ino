//www.themonoclecat.de
//26. Nov. 2014
//User interface for ESP8266
/*  screen /dev/tty.usbmodem453811
Commands (Just copy and paste into screen): 
              
Baud rate: AT+CIOBAUD? (check)       AT+CIOBAUD=9600 (set)
Wifi mode: AT+CWMODE?                AT+CWMODE=3 [1= Sta, 2= AP, 3=both, Sta is the default 
                                                  mode of router, AP is a normal mode for devices]
List the APs:         AT+CWLAP
Join AP:         AT+CWJAP=<ssid>,<p@$$word>
Current AP:       AT+CWJAP?
Address:       AT+CIFSR

Set parameters of AP:     AT+CWSAP=<ssid>,<p@$$wordMin8Char>,<chl>,<ecn>   ---- Current: AT+CWSAP="ESP8266 Wifi SoC","teensyyy",7,3
Current parameters:         AT+CWSAP?

Check joined devices' IP: AT+CWLIF

TCP:
Set mutiple connection: AT+CIPMUX=<mode>      AT+ CIPMUX? [0 for single connection 1 for multiple connection]
Set as server: AT+ CIPSERVER= <mode>[,<port]        [mode 0 to close server mode, mode 1 to open; port = port; 
   ---type IP:Port into browser to connect---           eg. turn on as a TCP server: AT+CIPSERVER=1,8888, 
                                                        check the self server IP address: AT+CIFSR=? ]


*/

int buf;

int timer = 0;

void setup()
{
  Serial.begin(9600); //Communication with computer
  Serial.setTimeout(1000);
  Serial1.begin(9600); //Communication with ESP8266
  Serial1.setTimeout(5000); //This is very important for the Serial.find() function
}

void nl() //== newline; you need these at the end of every command
{
  Serial1.write(13); //same as pressing ctrl+J in 'screen' 
  Serial1.write(10); //same as pressing the Enter key in 'screen'
}

void qu() //== a -> " <- symbol; you need this because sadly the Serial.print() command won't send a "
{
  Serial1.write(34);
}

void waitdebug()  //Print incoming and wait till over
{
  wait();
  while (Serial1.available() > 0)
  {
    delay(10);
    Serial.write(Serial1.read());
  } 
}

void wait()
{
  while (Serial1.available() == 0)
  {
    if (timer >= 10)
    {
      timer = 0;
      Serial.print(".");
    }
    timer += 1;
    delay(100); 
  }
}

void loop()
{
  while (Serial.available() > 0)
  {
    buf = Serial.read();
    switch (buf)
    {   
      case '@': //much information about the ESP8266
        Serial.print("Own IP:  ");
        Serial1.print("AT+CIFSR");
        nl();
        waitdebug();
        Serial.print("Parameters of own AP:  ");
        Serial1.print("AT+CWSAP?");
        nl();
        waitdebug();
        Serial.print("Connected devices' IP:  ");
        Serial1.print("AT+CWLIF");
        nl();
        waitdebug();
        Serial.print("The AP the ESP is connected to:  ");
        Serial1.print("AT+CWJAP?");
        nl();
        waitdebug();
        Serial.print("Connection status:  ");
        Serial1.print("AT+CIPSTATUS");
        nl();
        break;
        
      case '$': //Connect to Router
        Serial1.print("AT+CWMODE=3");
        nl();
        waitdebug();
        Serial1.print("AT+CWJAP=");
        qu();
        Serial1.print("---enter ssid here--");
        qu();
        Serial1.print(",");
        qu();
        Serial1.print("---enter password here---");
        qu();
        nl();
        if (Serial1.find("OK") == true)
        {
          //Connected to Wifi
          Serial.println("--Connected--");
        } 
        else
        {
          Serial.println("--Not Connected!--");
        }
        break;
        
      case 13:
        nl();
        break;
        
      case '#': //Manual
        Serial.println("Own commands: '$'=Connect to Wifi AP    '@'=Get lots of info");
        Serial.println("Commands (Just copy and paste into screen:");
        Serial.println("Baud rate: AT+CIOBAUD? (check)       AT+CIOBAUD=57600 (set)");
        Serial.println("Wifi mode: AT+CWMODE?                AT+CWMODE=3 [1= Sta, 2= AP, 3=both, Sta is the default");
        Serial.println("                                                  mode of router, AP is a normal mode for devices]");
        Serial.println(" ");
        Serial.println("List the APs:         AT+CWLAP");
        Serial.println("Join AP:         AT+CWJAP=<ssid>,<p@$$word>");
        Serial.println("Join AP:         AT+CWJAP=<ssid>,<p@$$word>");
        Serial.println("Current AP:       AT+CWJAP?");
        Serial.println("Address:       AT+CIFSR");
        Serial.println(" ");
        Serial.println("Set parameters of AP:     AT+ CWSAP= <ssid>,<p@$$wordMin8Char>,<chl>, <ecn>");
        Serial.println("Current parameters:         AT+CWSAP?");
        Serial.println(" ");
        Serial.println("Check joined devices' IP: AT+CWLIF");
        Serial.println(" ");
        Serial.println("Set TCP mutiple connection: AT+CIPMUX=<mode>      AT+ CIPMUX? [0 for single connection 1 for multiple connection]");
        Serial.println("Set as TCP server: AT+ CIPSERVER= <mode>[,<port]        [mode 0 to close server mode, mode 1 to open; port = port;");
        Serial.println("   ---type IP:Port into browser to connect---           eg. turn on as a TCP server: AT+CIPSERVER=1,8888,");
        Serial.println("                                                        check the self server IP address: AT+CIFSR=? ]");
        break;
        
      default:
        Serial1.write(buf);
        break;
    }
  } 
  
  while (Serial1.available() > 0)
  {
    Serial.write(Serial1.read());
  }
}

