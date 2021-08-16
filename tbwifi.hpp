#ifndef TBWIFI_HPP
#define TBWIFI_HPP

/*
#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
#include "esp_log.h"
#define LOG_TAG "component: TBWiFi"
*/

#include "nvs_flash.h"
#include "esp_wifi.h"
#include "freertos/event_groups.h"

class TBWiFi
{
private:
	TBWiFi();

public:
	static TBWiFi &getInstance();
	TBWiFi(const TBWiFi &) = delete;		  // remove copy constructor
	const TBWiFi &operator=(const TBWiFi &) = delete; // remove assignment constructor
};

#endif // TBWIFI_HPP