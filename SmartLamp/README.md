# Custom Home Assistant Smart Lamp

## Creds.h
#ifndef CREDS
#define CREDS
const char *HA_ENDPOINT="[IP ADDRESS]";
const char *HA_USERNAME="[USERNAME]";
const char *HA_PASSWORD="[PASSWORD]";
const char *DeviceName="SmartLamp";

const char *SSID="[WIFI SSID]";
const char *PWD="[WIFI PAssword]";

const char *StateTopic = "home/rgb1";
const char *CommandTopic=  "home/rgb1/set";
#endif