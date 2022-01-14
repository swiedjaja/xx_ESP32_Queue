#include <Arduino.h>
#include "device.h"

void taskTrigger(void* arg);
void taskProcess(void* arg);
QueueHandle_t queue;

void setup() {
  Serial.begin(115200);
  delay(100);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);

  queue = xQueueCreate(3, sizeof(int));
  xTaskCreatePinnedToCore(taskTrigger, "taskProducer", configMINIMAL_STACK_SIZE+1024, NULL, 2, NULL, 0);
  xTaskCreatePinnedToCore(taskProcess, "taskConsumer", configMINIMAL_STACK_SIZE+1024, NULL, 1, NULL, 1);
}

void loop() {
  vTaskDelay(0);
}

void taskTrigger(void* arg)
{
    int nValue = 0;
    while (1)
    {    
        digitalWrite(LED_RED, LED_ON);
        xQueueSend(queue, &nValue, portMAX_DELAY);
        printf("Produce: %d\n", nValue);
        fflush(stdout);
        nValue++;
        vTaskDelay(10/portTICK_RATE_MS);
        digitalWrite(LED_RED, LED_OFF);
        vTaskDelay(1000/portTICK_RATE_MS);
    }
}

void taskProcess(void* arg)
{
    while (1)
    {
        int nValue;
        xQueueReceive(queue, &nValue, portMAX_DELAY);
        digitalWrite(LED_GREEN, LED_ON);        
        vTaskDelay(100/portTICK_RATE_MS);
        printf("Consumer receive: %d\n", nValue);
        fflush(stdout);
        digitalWrite(LED_GREEN, LED_OFF);
        vTaskDelay(3000/portTICK_RATE_MS);
    }
}
