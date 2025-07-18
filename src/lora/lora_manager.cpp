/*
 * LORA_MANAGER.CPP - Coordinación y Utilidades
 * 
 * ACTUALIZADO: Muestra información de región y frecuencia
 */

#include "../lora.h"

/*
 * MÉTODOS DE INFORMACIÓN Y DIAGNÓSTICO - ACTUALIZADOS
 */
void LoRaManager::printConfiguration() {
    // Obtener frecuencia actual desde ConfigManager
    float frequency = configManager.getFrequencyMHz();
    String regionStr = "";
    
    switch (configManager.getRegion()) {
        case REGION_US: regionStr = "US (Estados Unidos/México)"; break;
        case REGION_EU: regionStr = "EU (Europa)"; break;
        case REGION_CH: regionStr = "CH (China)"; break;
        case REGION_AS: regionStr = "AS (Asia)"; break;
        case REGION_JP: regionStr = "JP (Japón)"; break;
        default: regionStr = "DESCONOCIDO"; break;
    }
    
    Serial.println("\n[LoRa] === CONFIGURACIÓN ACTUAL ===");
    Serial.println("Device ID: " + String(deviceID));
    Serial.println("Role: " + String(currentRole));
    Serial.println("Región: " + regionStr);
    Serial.println("Frecuencia: " + String(frequency) + " MHz");
    Serial.println("TX Power: " + String(LORA_TX_POWER) + " dBm");
    Serial.println("Bandwidth: " + String(LORA_BANDWIDTH) + " kHz");
    Serial.println("Spreading Factor: " + String(LORA_SPREADING_FACTOR));
    Serial.println("Coding Rate: 4/" + String(LORA_CODING_RATE));
    Serial.println("Sync Word: 0x" + String(LORA_SYNC_WORD, HEX));
    Serial.println("Estado: " + getStatusString());
    Serial.println("Algoritmo: Meshtastic Managed Flood Routing");
    Serial.println("================================");
}

void LoRaManager::printStats() {
    Serial.println("\n[LoRa] === ESTADÍSTICAS BÁSICAS ===");
    Serial.println("Packets enviados: " + String(stats.packetsSent));
    Serial.println("Packets recibidos: " + String(stats.packetsReceived));
    Serial.println("Packets perdidos: " + String(stats.packetsLost));
    Serial.println("Último RSSI: " + String(stats.lastRSSI) + " dBm");
    Serial.println("Último SNR: " + String(stats.lastSNR) + " dB");
    Serial.println("Tiempo total aire: " + String(stats.totalAirTime) + " ms");
    Serial.println("Frecuencia actual: " + String(configManager.getFrequencyMHz()) + " MHz");
    Serial.println("=======================");
}

void LoRaManager::printMeshStats() {
    Serial.println("\n[LoRa] === ESTADÍSTICAS MESH ===");
    Serial.println("Duplicados ignorados: " + String(stats.duplicatesIgnored));
    Serial.println("Retransmisiones: " + String(stats.rebroadcasts));
    Serial.println("Hop limit alcanzado: " + String(stats.hopLimitReached));
    Serial.println("Packets en memoria: " + String(recentBroadcasts.size()) + "/" + String(MAX_RECENT_PACKETS));
    Serial.println("Role actual: " + String(currentRole));
    Serial.println("Región LoRa: " + String(configManager.getRegion()));
    Serial.println("Frecuencia: " + String(configManager.getFrequencyMHz()) + " MHz");
    Serial.println("CW Min/Max: " + String(ContentionWindow::CWmin) + "/" + String(ContentionWindow::CWmax));
    Serial.println("Slot time: " + String(ContentionWindow::slotTimeMsec) + " ms");
    Serial.println("========================");
}

/*
 * GETTERS DE ESTADO Y ESTADÍSTICAS
 */
LoRaStatus LoRaManager::getStatus() {
    return status;
}

String LoRaManager::getStatusString() {
    switch (status) {
        case LORA_STATUS_INIT: return "INICIALIZANDO";
        case LORA_STATUS_READY: return "LISTO";
        case LORA_STATUS_TRANSMITTING: return "TRANSMITIENDO";
        case LORA_STATUS_RECEIVING: return "RECIBIENDO";
        case LORA_STATUS_ERROR: return "ERROR";
        default: return "DESCONOCIDO";
    }
}

LoRaStats LoRaManager::getStats() {
    return stats;
}

float LoRaManager::getLastRSSI() {
    return stats.lastRSSI;
}

float LoRaManager::getLastSNR() {
    return stats.lastSNR;
}

/*
 * RESET DE ESTADÍSTICAS
 */
void LoRaManager::resetStats() {
    stats.packetsSent = 0;
    stats.packetsReceived = 0;
    stats.packetsLost = 0;
    stats.totalAirTime = 0;
    stats.duplicatesIgnored = 0;
    stats.rebroadcasts = 0;
    stats.hopLimitReached = 0;
    Serial.println("[LoRa] Estadísticas reseteadas");
}

/*
 * MÉTODOS DE ROLE MANAGEMENT
 */
void LoRaManager::setRole(DeviceRole role) {
    currentRole = role;
    Serial.println("[LoRa] Role configurado: " + String(role));
}

DeviceRole LoRaManager::getRole() {
    return currentRole;
}

/*
 * MÉTODOS DE ESTADÍSTICAS MESH
 */
uint32_t LoRaManager::getDuplicatesIgnored() {
    return stats.duplicatesIgnored;
}

uint32_t LoRaManager::getRebroadcasts() {
    return stats.rebroadcasts;
}

uint32_t LoRaManager::getHopLimitReached() {
    return stats.hopLimitReached;
}