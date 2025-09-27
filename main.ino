#include <DHT.h>

#define DHTPIN D2         // DHT11 connected to NodeMCU D2 (GPIO4)
#define DHTTYPE DHT11     // DHT sensor type
#define MQ135_PIN A0      // MQ135 connected to Analog A0

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Serial.println("Initializing sensors...");
  
  dht.begin();
  delay(2000);  // Allow sensors to stabilize
}

void loop() {
  float temperature, humidity;
  int attempts = 0;
  
  // Retry reading DHT11 sensor up to 3 times
  while (attempts < 3) {
    temperature = dht.readTemperature();
    humidity = dht.readHumidity();
    
    if (!isnan(temperature) && !isnan(humidity)) {
      break;  // Exit loop if valid readings are received
    }
    
    attempts++;
    Serial.println("Retrying DHT sensor reading...");
    delay(1000);  // Wait 1 second before retrying
  }

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Error: Failed to read from DHT sensor after 3 attempts!");
    return;
  }

  // Read Air Quality Data from MQ135 Sensor
  int airQualityRaw = analogRead(MQ135_PIN); 
  
  // Convert Raw Value to PPM (Simple Approximation)
  float airQualityPPM = (airQualityRaw / 1023.0) * 1000;  // Basic conversion formula

  // Display Readings
  Serial.println("\n=== Air Quality Data ===");
  Serial.printf("Temperature: %.2f °C\n", temperature);
  Serial.printf("Humidity: %.2f %%\n", humidity);
  Serial.printf("Air Quality (Raw Value): %d\n", airQualityRaw);
  Serial.printf("Air Quality (Estimated PPM): %.2f ppm\n", airQualityPPM);
  Serial.println("========================\n");

  delay(3000);  // Wait 3 seconds before next reading
}
