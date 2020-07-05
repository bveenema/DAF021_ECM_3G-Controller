#include "ble-uart.h"
#include "globals.h"

void onDataReceived(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context);

BleCharacteristic txCharacteristic("tx", BleCharacteristicProperty::NOTIFY, txUuid, serviceUuid);
BleCharacteristic rxCharacteristic("rx", BleCharacteristicProperty::WRITE_WO_RSP, rxUuid, serviceUuid, onDataReceived, NULL);

void onDataReceived(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context) {
    // Log.trace("Received data from: %02X:%02X:%02X:%02X:%02X:%02X:", peer.address()[0], peer.address()[1], peer.address()[2], peer.address()[3], peer.address()[4], peer.address()[5]);

    // Data is sent as a comma seperated variable string formatted as: Ratio,Pressure,Viscosity
    
    // Print Data to Terminal
    char RatioBuffer[16];
    char PressureBuffer[16];
    char ViscosityBuffer[16];
    uint CurrentVariable = 0;
    uint i = 0;
    for (size_t j = 0; j < len; j++) {
        // Print the character to the terminal
        Serial.write(data[j]);

        // If Character is a comma, start next variable
        if(data[j] == ',' || data[j] == '\n')
        {
            // Place null terminator at end of buffer
            if(CurrentVariable == 0) // Ratio
                RatioBuffer[i+1] = '\0';
            else if(CurrentVariable == 1) // Pressure
                PressureBuffer[i+1] = '\0';
            else if(CurrentVariable == 2) // Viscosity
                ViscosityBuffer[i+1] = '\0';

            // reset i
            i = 0;

            // Increment to next variable
            CurrentVariable += 1; 

        }
            
        // else store the character in the appropriate buffer
        else
        {
            if(CurrentVariable == 0) // Ratio
                RatioBuffer[i] = data[j];
            else if(CurrentVariable == 1) // Pressure
                PressureBuffer[i] = data[j];
            else if(CurrentVariable == 2) // Viscosity
                ViscosityBuffer[i] = data[j];
        }
    }

    // ViscosityBuffer[i+1] = '\0';

    Serial.printlnf("Ratio: %s, Pressure: %s, Viscosity: %s", RatioBuffer, PressureBuffer, ViscosityBuffer);
    CHIME_BLE_Confirm.setStatus(Active);
}

void BLE_UART_init() {
    BLE.on();

    BLE.addCharacteristic(txCharacteristic);
    BLE.addCharacteristic(rxCharacteristic);

    BleAdvertisingData data;
    data.appendServiceUUID(serviceUuid);
    BLE.advertise(&data);
}

void BLE_UART_update() {
    if (BLE.connected()) {
        uint8_t txBuf[UART_TX_BUF_SIZE];
        size_t txLen = 0;

        while(Serial.available() && txLen < UART_TX_BUF_SIZE) {
            txBuf[txLen++] = Serial.read();
            Serial.write(txBuf[txLen - 1]);
        }
        if (txLen > 0) {
            txCharacteristic.setValue(txBuf, txLen);
        }
    }
}