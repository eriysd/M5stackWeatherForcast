#include <M5Stack.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
 
const char* ssid = "{yourSSID}";
const char* password =  "{yourPassword}";
 
const String endpoint = "http://api.openweathermap.org/data/2.5/weather?q={yourCity},{yourCountryInitial}&APPID=";
const String key = "{yourApiKey}";
 
void setup() {
  M5.begin();  
  M5.Power.begin();
  //dacWrite(25, 0); //hardware noice reduction
 
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);//every 1 sec
    M5.Lcd.println("Connecting to WiFi..");
  }
 
  M5.Lcd.println("Connected to the WiFi network");
  M5.Lcd.setTextSize(2);
}

 
void loop() {
 
  if ((WiFi.status() == WL_CONNECTED)) {
 
    HTTPClient http;
 
    http.begin(endpoint + key); //url initialization
    int httpCode = http.GET();  //sending get request
 
    if (httpCode > 0) { //if we get response 
 

        String payload = http.getString();  //getting the data in json format
        //M5.Lcd.println(httpCode); //uncomment if necessary
        //M5.Lcd.println(payload);  //uncomment if necessary

        //creating json object
        DynamicJsonBuffer jsonBuffer;
        String json = payload;
        JsonObject& weatherdata = jsonBuffer.parseObject(json);

        //checking wether parsing was successful 
        if(!weatherdata.success()){
          M5.Lcd.println("parseObject() failed");
        }

        //printing each data

        const char* city = weatherdata["name"].as<char*>();
        const char* weather = weatherdata["weather"][0]["description"].as<char*>();
        const double temp = weatherdata["main"]["temp"].as<double>();
        const double feels_like_temp = weatherdata["main"]["feels_like"].as<double>();
        const double wind = weatherdata["wind"]["speed"].as<double>();
        const double pressure = weatherdata["main"]["pressure"].as<double>();
        const double humidity = weatherdata["main"]["humidity"].as<double>();

        M5.Lcd.print("\nweather forcast of ");
        M5.Lcd.println(city);
        M5.Lcd.print("weather:");
        M5.Lcd.println(weather);
        M5.Lcd.print("temperature:");
        M5.Lcd.println(temp-273.15);
        M5.Lcd.print("feels like:");
        M5.Lcd.println(feels_like_temp-273.15);
        M5.Lcd.print("wind speed:");
        M5.Lcd.print(wind);
        M5.Lcd.println("m/s");
        M5.Lcd.print("pressure:");
        M5.Lcd.print(pressure);
        M5.Lcd.println("hPa");
        M5.Lcd.print("humidity:");
        M5.Lcd.print(humidity);
        M5.Lcd.println("%");
        
      }
 
    else {
      M5.Lcd.println("Error on HTTP request");
    }
 
    http.end(); //finishing the code
  }
 
  delay(120000);   //repeat every 60 sec
 
}
