#include "mi_antena.h"

#include <stdio.h>
#include <string.h> // Para memcpy en el paso #3 de añadir usuario/amigo para recibir nuestros datos
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_mac.h" // Libreria para obtener la mac address
#include "nvs_flash.h" // Para inicializar WI-FI (Prepara el disco duro, no entiende señales)
#include "esp_wifi.h" // Controla el hardware de radio, contiene funciones para manipular la antena fisica
#include "esp_netif.h" // Agregamos esta para esp_netif_init
#include "esp_event.h" // Agregamos esta para el event_loop
#include "esp_err.h"


MiAntena::MiAntena(){
    expedienteReceptor = {};
}
    
    
void MiAntena::begin(){
    // #1
    ESP_ERROR_CHECK(nvs_flash_init()); //Almacenamiento no Volatil, pedacito de la memoria flash, abrimos la lectura de NVS
    ESP_ERROR_CHECK(esp_netif_init()); //Network Interface, sabe manejar direcciones IP, esp no necesita ip ni routers pero esp32 obliga que este despierto antes de encer cualquier antena
    ESP_ERROR_CHECK(esp_event_loop_create_default()); //Tablero de notificaciones, hace que el circuito fisico avise al programa principal si algo se conecto o le llego algun paquete
    // #2
    wifi_init_config_t configuracionWIFI = WIFI_INIT_CONFIG_DEFAULT(); //Cargamos la configuracion de fabrica  
    ESP_ERROR_CHECK(esp_wifi_init(&configuracionWIFI)); //Le pasamos la configuracion al driver del WI-FI
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA)); // Le decimos que funcione como "Estacion" (Celular buscando Red)

}

void MiAntena::encenderWiFi(bool apagarEspnow){
    // #2
    ESP_ERROR_CHECK(esp_wifi_start()); // Encendemos la Antena
    //esp_wifi_set_max_tx_power(20); Esto sirve para bajar la potencia de la antena, lo que consume menos WiFi pero podria reducir la conexion

    // #3
    if (apagarEspnow)
    {
        ESP_ERROR_CHECK(esp_now_init()); //Iniciamos ESP-NOW 
        printf("ESP_NOW ENCENDIDO!\n");
    }
    printf("Antena WiFi Encendia!\n");
}

void MiAntena::agregarMacAddress(const uint8_t* nuevaMac){
    memcpy(macReceptor, nuevaMac, 6);
    esp_read_mac(macReceptor, ESP_MAC_WIFI_STA);
    printf("Direccion Mac Address: %02X, %02x, %02X, %02X, %02X, %02X", macReceptor[0], macReceptor[1], macReceptor[2], macReceptor[3], macReceptor[4], macReceptor[5]);
}

void MiAntena::expediente(){ 
    // Estamos configurando el expediente para identificar quien es amigo
    
    //esp_now_peer_info_t expediente_ferrari = {}; // Creamos un expediente en blanco para nuestro amigo (peer to peer)
    memcpy(expedienteReceptor.peer_addr, macReceptor, 6); // Le copiamos la MAC Address que guardamos arriba en las globales, en C y C++ no podemos usar "=", no se puede copiar/igualar arreglos como si fueran variables
    // memcpy(Desitno donde va a pegar los datos, Origen de donde vas a copiar los datos, Cantidad,los bytes que va a copiar, 6 por ser MAC ADDRESS)

    expedienteReceptor.channel = 0; // Canal de radio por defecto 0 (Van del 1 al 13, o equivale al 1 que es por defecto)
    expedienteReceptor.encrypt = false; // Sin contraseña/encriptacion

    esp_now_add_peer(&expedienteReceptor); //Le agregamos la libreta de contactos oficial del chip
}
    

void MiAntena::empaquetar(Datos* paqueteListo){
     esp_err_t resultado = esp_now_send(macReceptor, (uint8_t *) &paqueteListo, sizeof(paqueteListo));

        // Comprobamos que se envio el paquete
        //ESP_OK == 0, si lo que guardo paquete es 0, entonces salio bien
        if(resultado == ESP_OK)
            printf("El paquete fue enviado con exito!\n");
        else
            printf("Error al enviar el paquete... D:\n");
}   

void MiAntena::apagarWiFi(){
    // 1. Primero destruimos la sesión de ESP-NOW de forma segura
    ESP_ERROR_CHECK(esp_now_deinit()); 
    printf("ESP_NOW APAGADO. (Ahorro de Energia)\n");

    // 2. Apagamos físicamente el radio Wi-Fi del ESP32
    ESP_ERROR_CHECK(esp_wifi_stop()); 
    printf("Antena Wi-Fi APAGADA. (Ahorro de Energia)\n");
}
