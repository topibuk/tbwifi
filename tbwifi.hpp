#ifndef TBWIFI_HPP
#define TBWIFI_HPP

#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
#include "esp_log.h"

#define LOG_TAG "component: TBWiFi"

#include "nvs_flash.h"
#include "esp_wifi.h"
#include "freertos/event_groups.h"

#define WIFI_CONNECT_MAXIMUM_RETRY 20

class TBWiFi
{
private:
	static int retry_count;
	static bool connected;
	static bool started;
	static char *ssid;
	static char *password;
	static bool use_password;
	static TBWiFi instance;
	static TaskHandle_t task_handle;

	TBWiFi();
	static void task(void *v);
	static void eventHandler(void *arg, esp_event_base_t event_base,
				 int32_t event_id, void *event_data);

public:
	static TBWiFi &getInstance();
	static bool isConnected()
	{
		return connected;
	};
	static void start();
	static void start(char *ssid, char *password)
	{
		TBWiFi::ssid = ssid;
		TBWiFi::password = password;
		TBWiFi::use_password = true;
		start();
	};
	static void stop();
	TBWiFi(const TBWiFi &) = delete;		  // remove copy constructor
	const TBWiFi &operator=(const TBWiFi &) = delete; // remove assignment constructor
};

#endif // TBWIFI_HPP