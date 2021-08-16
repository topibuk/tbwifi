#include "tbwifi.hpp"

TBWiFi &TBWiFi::getInstance()
{
	static TBWiFi instance{};
	ESP_LOGV(LOG_TAG, "returning instance %d", (int)&instance);
	return instance;
}

void TBWiFi::task(void *v)
{
	while (true)
	{
		ESP_LOGV(LOG_TAG, "heartbeat");
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}

TBWiFi::TBWiFi()
{
	ESP_LOGV(LOG_TAG, "tbwifi constructor called");
	xTaskCreate(TBWiFi::task, "TBWiFi_task", 4096, NULL, 10, NULL);
}