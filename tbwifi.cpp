#include "tbwifi.hpp"
#include <string.h>

bool TBWiFi::connected;
int TBWiFi::retry_count;

TBWiFi &TBWiFi::getInstance()
{
	static TBWiFi instance{};
	ESP_LOGV(LOG_TAG, "returning instance %d", (int)&instance);
	return instance;
}

void TBWiFi::task(void *v)
{
	retry_count = 0;
	connected = false;

	//Initialize NVS for internal use of WiFi

	esp_err_t ret = nvs_flash_init();
	if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
	{
		ESP_ERROR_CHECK(nvs_flash_erase());
		ret = nvs_flash_init();
	}
	ESP_ERROR_CHECK(ret);

	ESP_ERROR_CHECK(esp_event_loop_create_default());

	ESP_ERROR_CHECK(esp_netif_init());

	esp_netif_create_default_wifi_sta();

	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
	ESP_ERROR_CHECK(esp_wifi_init(&cfg));

	esp_event_handler_instance_t instance_any_id;
	esp_event_handler_instance_t instance_got_ip;

	ESP_ERROR_CHECK(esp_event_handler_instance_register(
	    WIFI_EVENT,
	    ESP_EVENT_ANY_ID,
	    &TBWiFi::eventHandler,
	    NULL,
	    &instance_any_id));

	ESP_ERROR_CHECK(esp_event_handler_instance_register(
	    IP_EVENT,
	    ESP_EVENT_ANY_ID,
	    &TBWiFi::eventHandler,
	    NULL,
	    &instance_got_ip));

	wifi_config_t wifi_config;
	memset(&wifi_config, 0, sizeof(wifi_config));
	const char *ssid = "victanNG";
	const char *password = "yioX1koh ahN1aigo";
	strcpy((char *)wifi_config.sta.ssid, ssid);
	strcpy((char *)wifi_config.sta.password, password);

	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
	ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
	ESP_ERROR_CHECK(esp_wifi_start());

	ESP_LOGI(LOG_TAG, "wifi in station mode started");

	while (true)
	{
		ESP_LOGV(LOG_TAG, "heartbeat");
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}

TBWiFi::TBWiFi()
{
	ESP_LOGV(LOG_TAG, "tbwifi constructor called");

	xTaskCreate(task, "TBWiFi_task", 4096, NULL, 10, NULL);
}

void TBWiFi::eventHandler(void *arg, esp_event_base_t event_base,
			  int32_t event_id, void *event_data)
{
	if (event_base == WIFI_EVENT)
	{
		switch (event_id)
		{
		case WIFI_EVENT_STA_START:
			esp_wifi_connect();
			retry_count++;
			ESP_LOGI(LOG_TAG, "connecting to the AP");
			break;
		case WIFI_EVENT_STA_DISCONNECTED:
			TBWiFi::connected = false;

			if (retry_count < WIFI_CONNECT_MAXIMUM_RETRY)
			{
				ESP_LOGI(LOG_TAG, "disconnected from AP");
				esp_wifi_connect();
				retry_count++;
				ESP_LOGI(LOG_TAG, "reconnecting");
			}
			else
			{
				ESP_LOGE(LOG_TAG, "can't reconnect to the AP");
			}
			ESP_LOGI(LOG_TAG, "disconnected from the AP");
			break;
		default:
			ESP_LOGI(LOG_TAG, "unhandled WIFI_EVENT");
			break;
		}
	}
	else if (event_base == IP_EVENT)
	{
		ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
		switch (event_id)
		{
		case IP_EVENT_STA_GOT_IP:
			ESP_LOGI(LOG_TAG, "got ip:%s",
				 ip4addr_ntoa((ip4_addr_t *)&event->ip_info.ip));
			retry_count = 0;
			connected = true;
			break;
		case IP_EVENT_STA_LOST_IP:
			ESP_LOGI(LOG_TAG, "ip lost");
			break;
		default:
			ESP_LOGI(LOG_TAG, "unhandled IP_EVENT");
		}
	}
}