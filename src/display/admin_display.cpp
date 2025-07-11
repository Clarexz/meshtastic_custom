/*
 * ADMIN_DISPLAY.CPP - Modo de visualización ADMIN
 */

#include "admin_display.h"
#include "../config/config_manager.h"
#include "../gps/gps_manager.h"
#include "../lora.h"
#include "../battery/battery_manager.h"

// Instancia global
AdminDisplay adminDisplay;

/*
 * CONSTRUCTOR
 */
AdminDisplay::AdminDisplay() {
    // Inicialización si es necesaria
}

/*
 * DESTRUCTOR
 */
AdminDisplay::~AdminDisplay() {
    // Cleanup si es necesario
}

/*
 * MOSTRAR OUTPUT ADMIN DEL TRACKER
 */
void AdminDisplay::showTrackerOutput(uint16_t deviceID, bool sent) {
    // Mostrar información completa como antes
    DeviceConfig config = configManager.getConfig();
    GPSData gpsData = gpsManager.getCurrentData();
    LoRaStats stats = loraManager.getStats();
    
    Serial.println("\n[TRACKER] === TRANSMISIÓN GPS + LoRa MESH ===");
    Serial.println("Device ID: " + String(deviceID));
    Serial.println("Role: TRACKER (CLIENT priority)");
    Serial.println("Coordenadas: " + gpsManager.formatCoordinates());
    Serial.println("Battery: " + String(batteryManager.getVoltage()) + " mV");
    Serial.println("Timestamp: " + String(gpsData.timestamp));
    Serial.println("Packet: " + gpsManager.formatPacketWithDeviceID(deviceID));
    Serial.println("LoRa Status: " + String(sent ? "ENVIADO" : "FALLIDO"));
    Serial.println("Estado LoRa: " + loraManager.getStatusString());
    
    if (sent) {
        Serial.println("RSSI último: " + String(loraManager.getLastRSSI()) + " dBm");
        Serial.println("SNR último: " + String(loraManager.getLastSNR()) + " dB");
        Serial.println("Packets enviados: " + String(stats.packetsSent));
        Serial.println("Duplicados ignorados: " + String(stats.duplicatesIgnored));
        Serial.println("Retransmisiones: " + String(stats.rebroadcasts));
    } else {
        Serial.println("ERROR: Fallo en transmisión LoRa");
    }
    
    Serial.println("Próxima transmisión en " + String(config.gpsInterval) + " segundos");
    Serial.println("==========================================\n");
}

/*
 * MOSTRAR OUTPUT ADMIN DEL REPEATER
 */
void AdminDisplay::showRepeaterOutput() {
    // Mostrar información completa del repeater
    LoRaStats stats = loraManager.getStats();
    
    Serial.println("\n[REPEATER] === ESTADO DEL REPETIDOR ===");
    Serial.println("Escuchando red mesh - Listo para retransmitir");
    Serial.println("Role: REPEATER (ROUTER priority)");
    Serial.println("Estado LoRa: " + loraManager.getStatusString());
    Serial.println("RX: " + String(stats.packetsReceived) + 
                   " | TX: " + String(stats.packetsSent) + 
                   " | Retransmisiones: " + String(stats.rebroadcasts));
    Serial.println("Duplicados ignorados: " + String(stats.duplicatesIgnored));
    Serial.println("Hop limit alcanzado: " + String(stats.hopLimitReached));
    Serial.println("===================================\n");
}

/*
 * MOSTRAR OUTPUT ADMIN DEL RECEIVER
 */
void AdminDisplay::showReceiverOutput() {
    // Mostrar información completa del receiver
    LoRaStats stats = loraManager.getStats();
    GPSData gpsData = gpsManager.getCurrentData();
    
    Serial.println("\n[RECEIVER] === ESTADO DEL RECEPTOR MESH ===");
    Serial.println("Escuchando posiciones GPS de la red...");
    Serial.println("Role: RECEIVER (CLIENT priority)");
    Serial.println("Estado LoRa: " + loraManager.getStatusString());
    
    // Mostrar nuestra propia posición como referencia
    if (gpsData.hasValidFix) {
        Serial.println("Posición propia: " + gpsManager.formatCoordinates());
        Serial.println("Estado GPS: " + gpsManager.getStatusString());
    }
    
    // Mostrar estadísticas de red
    Serial.println("Packets recibidos: " + String(stats.packetsReceived));
    Serial.println("Duplicados ignorados: " + String(stats.duplicatesIgnored));
    Serial.println("Retransmisiones hechas: " + String(stats.rebroadcasts));
    
    // Detectar nuevos packets recibidos
    static uint32_t lastPacketCount = 0;
    if (stats.packetsReceived > lastPacketCount) {
        Serial.println("¡NUEVOS DATOS RECIBIDOS!");
        Serial.println("Último RSSI: " + String(stats.lastRSSI) + " dBm");
        Serial.println("Último SNR: " + String(stats.lastSNR) + " dB");
        lastPacketCount = stats.packetsReceived;
    }
    
    Serial.println("========================================\n");
}