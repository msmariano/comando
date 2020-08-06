#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>



char ssid[] = "Mariano_2G";
char password[] = "20061977";
WiFiServer server(81);
String mens = "";
int total = 0;
boolean atendeu = true;
int lamp1 = D2;
int lampOn = LOW;
int lampOff = HIGH;
int lampState[4];
int numLamp = 4;
int lampadas[] = {D0, D2, D1, D3};
String mensagens[4];
String variaveis[] = {"lamp0=on", "lamp1=on", "lamp2=on", "lamp3=on"};

void setup() {
  Serial.begin(115200);
  delay(10);

  for (int i = 0 ; i < numLamp; i++)
    pinMode(lampadas[i], OUTPUT);

  for (int i = 0 ; i < numLamp; i++) {

    for (int j = 0 ; j < 4 ; j++) {
      mudarEstadoLampada(i, lampOn);
      delay(500);
      mudarEstadoLampada(i, lampOff);
      delay(500);
    }
  }


  for (int i = 0 ; i < numLamp; i++) {
    lampState[i] = 0;
  }



  //digitalWrite(lamp1, HIGH);
  //mudarEstadoLampada(2,lampOn);

  // Connect to WiFi network
  Serial.print("Conectando...");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi conectado...");

  // Start the server
  server.begin();
  Serial.println("Servidor Iniciado...");

  // Print the IP address
  Serial.print("Use this URL : ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");


}

void loop() {

  if (atendeu)
    Serial.println("Aguardando cliente...");
  WiFiClient client = server.available();
  if (!client) {
    atendeu = false;
    return;
  }

  /*atendeu = true;
    // Wait until the client sends some data
    Serial.println("Aguargando estabelecer cliente...");
    total = 0;
    while(!client.available()){
    delay(1000);
    Serial.println("tentando estabelecer cliente...");
    total++;
    if(total==10){
      total = 0;
      return;
    }
    }*/



  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
  if (request.indexOf("acionarLampadas=OK") != -1) {
    for (int i = 0; i < numLamp ; i++) {
      if (request.indexOf(variaveis[i]) != -1)
        mudarEstadoLampada(i, lampOn);
      else
        mudarEstadoLampada(i, lampOff);
    }
  }

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html><title>Controle Quarto Ramon</title><body><form name=\"ativar\" method=\"GET\" action=\"quarto.html\">");
  client.println("<div align=\"center\">");
  client.println("<p align = \"center\" ><b><a><font size=\"5\" style=\"background-color:powderblue;\">Controle Quarto Ramon</font></a></b></p>");
  client.println("<table width=\"25%\" border=\"1px\" >");
  client.println("<tr bgcolor=\"#666666\"><td>Lampada</td><td>Ligar</td><td>Estado</td></tr>");
  client.println("<tr><td>Lampada 1</td><td><input type = \"checkBox\" name=\"lamp0\"  " + mensagens[0] + "></input></td><td>" + retEstado(mensagens[0]) + "</td></tr>");
  client.println("<tr><td>Lampada 2</td><td><input type = \"checkBox\" name=\"lamp1\"  " + mensagens[1] + " ></input></td><td>" + retEstado(mensagens[1]) + "</td></tr>");
  client.println("<tr><td>Lampada 3</td><td><input type = \"checkBox\" name=\"lamp2\"  " + mensagens[2] + "></input></td><td>" + retEstado(mensagens[2]) + "</td></tr>");
  client.println("<tr><td>Lampada 4</td><td><input type = \"checkBox\" name=\"lamp3\"  " + mensagens[3] + " ></input></td><td>" + retEstado(mensagens[3]) + "</td></tr>");
  client.println("</table></div><div align=\"center\"><input  type=\"submit\"  name =\"acionarLampadas\" value=\"OK\"></input></div></form></body></html>");


}

String retEstado(String ent) {

  if (ent == "checked")
    return "Ligada";

  return "Desligada";
}

void mudarEstadoLampada(int lampada, int estado) {

  lampState[lampada] = estado;
  digitalWrite(lampadas[lampada], estado);
  if (estado == lampOn)
    mensagens[lampada] = "checked";
  else
    mensagens[lampada] = "";

}
