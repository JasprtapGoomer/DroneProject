#include <Wire.h>
#include <MPU6050.h>
#include <Ethernet.h>

// Network settings
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 177);
EthernetServer server(80);

MPU6050 mpu;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  mpu.initialize();

  if (mpu.testConnection()) {
    Serial.println("MPU6050 connection successful");
  } else {
    Serial.println("MPU6050 connection failed");
  }

  // Initialize Ethernet
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("Server is at ");
  Serial.println(Ethernet.localIP());
}

void loop() {
  EthernetClient client = server.available();
  if (client) {
    Serial.println("New Client.");
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);

        if (c == '\n' && currentLineIsBlank) {
          // Read MPU6050 data
          int16_t ax, ay, az, gx, gy, gz;
          mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

          // Convert raw values to human-readable format
          float ax_scaled = ax / 16384.0;
          float ay_scaled = ay / 16384.0;
          float az_scaled = az / 16384.0;
          float gx_scaled = gx / 131.0;
          float gy_scaled = gy / 131.0;
          float gz_scaled = gz / 131.0;

          // Send a standard HTTP response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println();

          // Send the HTML page
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<head><meta http-equiv=\"refresh\" content=\"1\"></head>");  // Refresh every second
          client.println("<body><h1>MPU6050 Sensor Data</h1>");
          client.println("<div id=\"gyro\"></div>");
          client.println("<canvas id=\"canvas\" width=\"400\" height=\"200\" style=\"border:1px solid #000000;\"></canvas>");
          client.println("<script>");
          client.println("var c = document.getElementById('canvas');");
          client.println("var ctx = c.getContext('2d');");
          client.println("ctx.clearRect(0, 0, c.width, c.height);");

          // Send data as JavaScript
          client.print("var ax = "); client.print(ax_scaled); client.println(";");
          client.print("var ay = "); client.print(ay_scaled); client.println(";");
          client.print("var az = "); client.print(az_scaled); client.println(";");
          client.print("var gx = "); client.print(gx_scaled); client.println(";");
          client.print("var gy = "); client.print(gy_scaled); client.println(";");
          client.print("var gz = "); client.print(gz_scaled); client.println(";");

          client.println("ctx.beginPath();");
          client.println("ctx.moveTo(200, 100);");
          client.println("ctx.lineTo(200 + gx, 100 + gy);");
          client.println("ctx.stroke();");

          client.println("</script>");
          client.println("</body></html>");
          break;
        }
        if (c == '\n') {
          currentLineIsBlank = true;
        } else if (c != '\r') {
          currentLineIsBlank = false;
        }
      }
    }
    delay(1);
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
