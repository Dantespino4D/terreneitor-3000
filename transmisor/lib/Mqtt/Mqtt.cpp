#include "Mqtt.h"
#include "Ota.h"
#include "esp_event_base.h"
#include "mqtt_client.h"
#include "esp_log.h"
#include "esp_system.h"
#include "cJSON.h"
#include <cstdint>

static const char *TAG = "MQTT";

void Mqtt::begin(){
	//se configura
	esp_mqtt_client_config_t mqttC = {};
	mqttC.broker.address.uri = BROKER;
	mqttC.credentials.username = USUARIO;
	mqttC.credentials.authentication.password = PASSWORD;

	//se inicializa
	this->cliente = esp_mqtt_client_init(&mqttC);

	//se registra la funcion que recibira los eventos
	esp_mqtt_client_register_event(this->cliente, (esp_mqtt_event_id_t)ESP_EVENT_ANY_ID, Mqtt::evento, this);

	//inicia
	esp_mqtt_client_start(cliente);
}

void Mqtt::evento(void* arg, esp_event_base_t base, int32_t id, void* data){
	//se guarda el obtejo
	Mqtt* self = (Mqtt*)arg;

	//guarda el evento en esta variable
	esp_mqtt_event_handle_t event = (esp_mqtt_event_handle_t)data;

	//decide que hacer con el evento
	switch((esp_mqtt_event_id_t)id){
		case MQTT_EVENT_CONNECTED:
			ESP_LOGI(TAG, "mqtt exitoso");
			esp_mqtt_client_subscribe(self->cliente, "terreneitor/ota", 1);
			self->pub("", "terreneitor/ota/log", 1, 0);
			break;
		case MQTT_EVENT_DISCONNECTED:
			ESP_LOGE(TAG, "mqtt se desconecto");
			break;
		case MQTT_EVENT_DATA:
			if(strncmp(event->topic, "terreneitor/ota", event->topic_len) == 0){
				ESP_LOGI(TAG, "url recibida");
				char *urlT = (char*)malloc(event->data_len + 1);
				if(urlT == NULL){
					ESP_LOGE(TAG, "error al procesar url");
					break;
				}
				memcpy(urlT, event->data, event->data_len);
                urlT[event->data_len] = '\0';
				self->pub("ejecutando ota", "terreneitor/ota/log", 1, 0);

				Ota ota;
				ota.ota(urlT);

				free(urlT);
			}
			break;
		case MQTT_EVENT_ERROR:
			ESP_LOGE(TAG, "mqtt fallo");
			break;
		default:
			break;
	}


}
void Mqtt::pub(const char* men, const char* top, int q, int r){
	esp_mqtt_client_publish(cliente, top, men, 0, q, r);
}


