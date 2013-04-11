/////////////////////////////////////////////////////////////////////////////
//                        My Project from Arduino                          //
//                      Project WebServer Control LED                      //
//                        By Mr.Rattaphong Sutamma                         //
//            Control LED pin 2 on/off on Web ip 192.168.137.20            //
/////////////////////////////////////////////////////////////////////////////

//include SPI and Ethernet from library
#include <SPI.h>
#include <Ethernet.h>

// MAC address from Ethernet shield sticker under board
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// IP address, may need to change depending on network
IPAddress ip(192, 168, 137, 20);

// create a server at port 80
EthernetServer server(80);

// stores the HTTP request
String HTTP_req;

// state of LED, off by default
boolean LED_status = 0;

////////////////////////////////setup program////////////////////////////////
void setup()
{
  
    // initialize Ethernet device
    Ethernet.begin(mac, ip);
    
    // start to listen for clients
    server.begin();
    
    // for diagnostics
    Serial.begin(9600);
    
    // LED on pin 2
    pinMode(2, OUTPUT);
}

///////////////////////////////loop program//////////////////////////////////
void loop()
{
  
    // try to get client
    EthernetClient client = server.available();

    // got client?
    if (client) {
        boolean currentLineIsBlank = true;
        while (client.connected()) {
          
            // client data available to read
            if (client.available()) {
              
                // read 1 byte (character) from client
                char c = client.read();
                
                // save the HTTP request 1 char at a time
                HTTP_req += c;
                
                // last line of client request is blank and ends with \n
                // respond to client only after last line received
                if (c == '\n' && currentLineIsBlank) {
                
                    // send a standard http response header
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-Type: text/html");
                    client.println("Connection: close");
                    client.println();
                    
                    // send web page
                    client.println("<!DOCTYPE html>");
                    client.println("<html>");
                    client.println("<head>");
                    client.println("<title>Arduino LED Control</title>");
                    client.println("</head>");
                    client.println("<body>");
                    client.println("<h1>LED</h1>");
                    client.println("<p>Click to switch LED on and off.</p>");
                    client.println("<form method=\"get\">");
                    ProcessCheckbox(client);
                    client.println("</form>");
                    client.println("</body>");
                    client.println("</html>");
                    Serial.print(HTTP_req);
                    
                    // finished with request, empty string
                    HTTP_req = "";
                    break;
                }
                
                // every line of text received from the client ends with \r\n
                if (c == '\n') {
                  
                    // last character on line of received text
                    // starting new line with next character read
                    currentLineIsBlank = true;
                } 
                else if (c != '\r') {
                  
                    // a text character was received from client
                    currentLineIsBlank = false;
                }
                
            // end if (client.available())
            }
            
        // end while (client.connected())
        }
        
        // give the web browser time to receive the data
        delay(1);
        
        // close the connection
        client.stop();
        
    // end if (client)
    }
}

// switch LED and send back HTML for LED checkbox
void ProcessCheckbox(EthernetClient cl)
{
  
    // see if checkbox was clicked
    if (HTTP_req.indexOf("LED2=2") > -1) {
      
        // the checkbox was clicked, toggle the LED
        if (LED_status) {
            LED_status = 0;
        }
        else {
            LED_status = 1;
        }
    }
    
    // switch LED on
    if (LED_status) {
        digitalWrite(2, HIGH);
        
        // checkbox is checked
        cl.println("<input type=\"checkbox\" name=\"LED2\" value=\"2\" \
        onclick=\"submit();\" checked>LED2");
    }
    
    // switch LED off
    else {
        digitalWrite(2, LOW);
        
        // checkbox is unchecked
        cl.println("<input type=\"checkbox\" name=\"LED2\" value=\"2\" \
        onclick=\"submit();\">LED2");
    }
}

///////////////////////////////Good Luck/////////////////////////////////////
