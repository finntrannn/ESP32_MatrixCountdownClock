/**
 * @file DHTSensor.cpp
 * @brief Implementation of FreeRTOS-based DHT11 sensor polling.
 * 
 * @author finntrannn (finntrannn.id.vn)
 * @github https://github.com/finntrannn
 */

#include "DHTSensor.h"

void DHTSensor::begin(uint8_t pin) {
    dht_ = new DHT(pin, DHT11);
    dht_->begin();

    // Launch background read task on core 0 (main loop runs on core 1)
    xTaskCreatePinnedToCore(
        readTask,       // Task function
        "dht_read",     // Name
        2048,           // Stack size (bytes)
        this,           // Parameter (pointer to this instance)
        1,              // Priority (low — sensor reads are not urgent)
        nullptr,        // Task handle (not needed)
        0               // Core 0 (keep main loop free on core 1)
    );
}

void DHTSensor::readTask(void* param) {
    DHTSensor* self = static_cast<DHTSensor*>(param);

    // Wait 2s after boot for sensor to stabilize
    vTaskDelay(pdMS_TO_TICKS(2000));

    for (;;) {
        float t = self->dht_->readTemperature();
        float h = self->dht_->readHumidity();

        // Only update stable values if read was successful
        if (!isnan(t) && !isnan(h)) {
            // Apply deadband filter: ignore small fluctuations
            bool tempChanged = !self->ready_ || (fabs(t - self->stableTemp_) >= kTempDeadband);
            bool humiChanged = !self->ready_ || (fabs(h - self->stableHumi_) >= kHumiDeadband);

            if (tempChanged) self->stableTemp_ = t;
            if (humiChanged) self->stableHumi_ = h;
            self->ready_ = true;
        }

        vTaskDelay(pdMS_TO_TICKS(kReadIntervalMs));
    }
}
