#include "tbwifi.hpp"

TBWiFi &TBWiFi::getInstance()
{
	static TBWiFi instance{};
	return instance;
}