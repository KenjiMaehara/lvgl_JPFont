#ifndef AWSIOT_H
#define AWSIOT_H

#include <WiFiClientSecure.h>
#include <PubSubClient.h>

void setupAWSIoT();
void sendDataToAWS(void * parameter);

extern String gDeviceName;

extern WiFiClientSecure net;
extern PubSubClient client;

#endif
