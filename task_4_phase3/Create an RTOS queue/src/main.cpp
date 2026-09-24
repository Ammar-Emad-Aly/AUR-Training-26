#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <queue.h>

QueueHandle_t messageQueue;

void TaskConsumer(void *pvParameters);
void TaskProducerA(void *pvParameters);
void TaskProducerB(void *pvParameters);

void setup() {
  Serial.begin(115200);

  messageQueue = xQueueCreate(10, sizeof(char*));

  xTaskCreate(TaskConsumer, "Consumer", 128, NULL, 2, NULL);
  xTaskCreate(TaskProducerA, "ProducerA", 128, NULL, 1, NULL);
  xTaskCreate(TaskProducerB, "ProducerB", 128, NULL, 1, NULL);
}

void loop() {
}

void TaskConsumer(void *pvParameters) {
  const char *receivedMsg;

  for (;;) {
    if (uxQueueMessagesWaiting(messageQueue) > 0) {
      if (xQueueReceive(messageQueue, &receivedMsg, 10) == pdPASS) {
        Serial.println(receivedMsg);
      }
    }
    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}

void TaskProducerA(void *pvParameters) {
  const char *msgA = "Task one is working";

  for (;;) {
    xQueueSend(messageQueue, &msgA, portMAX_DELAY);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void TaskProducerB(void *pvParameters) {
  const char *msgB = "Task two is working";

  for (;;) {
    xQueueSend(messageQueue, &msgB, portMAX_DELAY);
    vTaskDelay(1500 / portTICK_PERIOD_MS);
  }
}