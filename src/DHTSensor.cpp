/**
 * @file DHTSensor.cpp
 * @brief Implementation of DHT11 sensor reads via FreeRTOS background task.
 *
 * @author finntrannn (finntrannn.id.vn)
 * @github https://github.com/finntrannn
 */
#include "DHTSensor.h"

void DHTSensor::begin(uint8_t pin) {
	dht_ = new DHT(pin, DHT11);
	dht_->begin();

	xTaskCreatePinnedToCore(readTask, "dht_read", 2048, this, 1, nullptr, 1);
}

void DHTSensor::readTask(void* param) {
	DHTSensor* self = static_cast<DHTSensor*>(param);

	vTaskDelay(pdMS_TO_TICKS(2000));

	for (;;) {
		float t = self->dht_->readTemperature();
		float h = self->dht_->readHumidity();

		if (!isnan(t) && !isnan(h)) {
			bool tempChanged =
				!self->ready_ || (fabs(t - self->stableTemp_) >= kTempDeadband);
			bool humiChanged =
				!self->ready_ || (fabs(h - self->stableHumi_) >= kHumiDeadband);

			if (tempChanged) self->stableTemp_ = t;
			if (humiChanged) self->stableHumi_ = h;
			self->ready_ = true;
		}

		vTaskDelay(pdMS_TO_TICKS(kReadIntervalMs));
	}
}
