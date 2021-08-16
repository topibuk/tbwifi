#include "tbwifi.hpp"

TBWiFi &TBWiFi::getInstance()
{
	static TBWiFi instance{};
	ESP_LOGV(LOG_TAG, "returning instance %d", (int)&instance);
	return instance;
}

TBWiFi::TBWiFi()
{
	ESP_LOGV(LOG_TAG, "tbwifi constructor called");
}