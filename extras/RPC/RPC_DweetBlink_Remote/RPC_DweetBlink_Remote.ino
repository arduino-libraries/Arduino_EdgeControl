/*
  Remote RPC client for Dweet
  To be used with the "RPC_DweetBlink" example.

  Please, refer to the "RPC_DweetBlink" sketch for istructions.

  created 12 Feb 2021
  by Giampaolo Mancini
  
*/

#include <ArduinoHttpClient.h>
#include <Arduino_JSON.h>
#include <WiFiNINA.h>

// Configure the RPC client
#include <openmvrpc.h>
openmv::rpc_scratch_buffer<256> scratch_buffer; // static memory buffer
openmv::rpc_callback_buffer<2> callback_buffer; // max number of callbacks
openmv::rpc_hardware_serial_uart_slave remote(115200);

#include "arduino_secrets.h"
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

String serverName = "dweet.io";
String getDweetPath = "/get/latest/dweet/for/";
String postDweetPath = "/dweet/for/";
int port = 80;

WiFiClient wifi;
HttpClient client(wifi, serverName, port);
int status = WL_IDLE_STATUS;

void setup()
{
    Serial.begin(115200);
    const uint32_t startNow { millis() + 2500 };

    while (!Serial && millis() < startNow)
        ;

    delay(1000);

    // Register RPC callbacks and start the client
    remote.register_callback("postDweetFor", postDweetFor);
    remote.register_callback("getLatestDweetFor", getLatestDweetFor);
    remote.begin();

    Serial.println("Waiting for commands");
}

void loop()
{
    if (WiFi.status() != WL_CONNECTED) {
        connectWiFi();
    }

    // RPC client loop
    // Remember to set high value of timeouts when dealing with slow callbacks
    // (the Network is SLOW!).
    // Alternatively, decouple the network and RPC code.
    remote.loop(1000, 3000);    
}

void postDweetFor(void* in_data, size_t in_data_len)
{
    auto contentType { "application/json" };

    // Parse data from RPC controller
    JSONVar data = JSON.parse((char *)in_data);
    String device = (const char *)data["deviceID"];

    // Create new data for Dweet
    JSONVar content;
    content["ledStatus"] = (bool)data["ledStatus"];

    auto path = postDweetPath + device + "-status";
    String body = JSON.stringify(content);

    Serial.print("Posting Dweet Content for ");
    Serial.print(device);

    auto ret = client.post(path, contentType, body);
    if (ret < 0) {
        Serial.print("Error in POST Request: ");
        Serial.println(ret);
        return;
    }

    // Read the status code...
    auto statusCode = client.responseStatusCode();
    Serial.print(" [");
    Serial.print(statusCode);
    Serial.println("]");
    // and go on.
    client.skipResponseHeaders();
    
    return;
}

void getLatestDweetFor(void* in_data, size_t in_data_len, void** out_data, size_t* out_data_len)
{
    auto *device = (char *)in_data;
    device[in_data_len] = 0;

    Serial.print("Getting Dweet Content from ");
    Serial.print(device);
    auto ret = client.get(getDweetPath + device);
    if (ret < 0) {
        Serial.print("Error in GET Request: ");
        Serial.println(ret);
        *out_data = nullptr;
        *out_data_len = 0;
        return;
    }
    // read the status code and body of the response
    auto statusCode = client.responseStatusCode();
    auto contentLen = client.contentLength();
    auto response = client.responseBody();

    Serial.print(" [");
    Serial.print(statusCode);
    Serial.println("]");

    if (statusCode != 200) {
        *out_data = nullptr;
        *out_data_len = 0;
        return;
    }

    // Set the return data
    const auto len = response.length();
    memcpy(*out_data, response.c_str(), len);
    *out_data_len = len;

    return;
}

void connectWiFi()
{
    Serial.print("Attempting to connect to SSID: ");
    Serial.print(ssid);
    Serial.print(" ");

    while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
        // failed, retry
        Serial.print(".");
        delay(5000);
    }
    Serial.println();
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);

    Serial.println("You're connected to the network");
    Serial.println();
}
