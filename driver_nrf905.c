/*
Date: August 19, 2025
Author: Kyle Leong

This is a driver for the nrf905 RFIC with SPI communication to be used in conjunction with STM32F100 in a Walkie Talkie project.
This .c file will implement the nrf905_t struct and all functions that were declared in the driver_nrf905.h
*/

#include "driver_nrf905.h"

//Define the hex values for instruction formats (pg.21)
#define NRF905_W_CONFIG            0x00
#define NRF905_R_CONFIG            0x10
#define NRF905_W_TX_PAYLOAD        0x20
#define NRF905_R_TX_PAYLOAD        0x21
#define NRF905_W_TX_ADDRESS        0x22
#define NRF905_R_TX_ADDRESS        0x23
#define NRF905_R_RX_PAYLOAD        0x24
#define NRF905_STATUS_REGISTER_NOP 0xFF

void nrf905_write_config(nrf905_t* nrf, uint8_t start_reg, const uint8_t* data, uint8_t len) {
    //Set instruction format 
    uint8_t opcode = NRF905_W_CONFIG | start_reg;

    //Pull ncs low
    nrf->ncs_low();
    //Send the opcode to let RFIC we are writing to config
    nrf->spi_xfer(opcode);
    //Send the config data byte(s) we want to configure
    nrf->spi_write(data, len);
    //Pull ncs high
    nrf->ncs_high();
}

uint8_t nrf905_read_config(nrf905_t* nrf, uint8_t config_register) {
    //Set instruction format
    uint8_t opcode = NRF905_R_CONFIG | config_register;
    //Pull ncs low
    nrf->ncs_low();
    //Send opcode
    nrf->spi_xfer(opcode);
    //Send over dummy value while tracking the config register value
    uint8_t val = nrf->spi_xfer(0xFF);
    //Pull ncs high
    nrf->ncs_high();

    return val;
}

void nrf905_write_tx_payload(nrf905_t* nrf, const void* buf, uint8_t len) {
    //Set instruction format
    uint8_t opcode = NRF905_W_TX_PAYLOAD;
    //Pull ncs low
    nrf->ncs_low();
    //Send opcode
    nrf->spi_xfer(opcode);
    //Send tx payload
    nrf->spi_write(buf, len);
    //Pull ncs high
    nrf->ncs_high();
}

//Utilize HAL_SPI_TransmitReceive_DMA later (faster + frees cpu)
void nrf905_read_tx_payload(nrf905_t* nrf, void* buf, uint8_t len) {
    uint8_t* out = (uint8_t*)buf;
    //Set instruction format
    uint8_t opcode = NRF905_R_TX_PAYLOAD;
    //Pull ncs low
    nrf->ncs_low();
    //Send opcode
    nrf->spi_xfer(opcode);
    //Loop len times sending dummy tx but taking in Rx data 
    for(int i = 0; i < len; i++) {
        out[i] = nrf->spi_xfer(0x00);
    }
    //Pull ncs high
    nrf->ncs_high();
}


void nrf905_write_tx_address(nrf905_t* nrf, const uint8_t* addr, uint8_t len) {
    //Set instruction format
    uint8_t opcode = NRF905_W_TX_ADDRESS;
    //Pull ncs low
    nrf->ncs_low();
    //Send opcode
    nrf->spi_xfer(opcode);
    //Write len bytes 
    nrf->spi_write(addr, len);
    //Pull ncs high
    nrf->ncs_high();
}


void nrf905_read_tx_address(nrf905_t* nrf, void* buf, uint8_t len) {
    uint8_t* out = (uint8_t*)buf;
    //Set instruction format
    uint8_t opcode = NRF905_R_TX_ADDRESS;
    //Pull ncs low
    nrf-> ncs_low();
    //Send opcode
    nrf->spi_xfer(opcode);
    //Send garbage data and store the address len times
    for(int i = 0; i < len; i++) {
        out[i] = nrf->spi_xfer(0x00);
    }
    //Pull ncs high
    nrf->ncs_high();
}

//Utilize HAL_SPI_TransmitReceive_DMA later (faster + frees cpu)
void nrf905_read_rx_payload(nrf905_t* nrf, void* buf, uint8_t len) {
    uint8_t* out = (uint8_t*) buf;
    //Set insturction format
    uint8_t opcode = NRF905_R_RX_PAYLOAD;
    //Pull ncs low
    nrf-> ncs_low();
    //Send opcode 
    nrf->spi_xfer(opcode);
    //Loop len times storing the data
    for(uint8_t i = 0; i < len; i++) {
        out[i] = nrf->spi_xfer(0x00);
    }
    //Pull ncs high
    nrf->ncs_high();
}

//Send a NOP to receive status register contents (AM, DR, pg.25)
uint8_t nrf905_read_status(nrf905_t* nrf) {
    uint8_t out;
    //Pull ncs low
    nrf->ncs_low();
    //Read the content coming from MISO
    out = nrf->spi_xfer(0x00);
    //Pull ncs high
    nrf->ncs_high();
    //Return the whole byte
    return out;
}


void nrf905_spi_programming(nrf905_t* nrf) {
    //PWR_UP high, TRX_CE high
    nrf->pwr_up_high();
    nrf->chip_en_high();
}

//Put chip into TX mode and send out the tx payload (pg. 16)
void nrf905_tx_mode(nrf905_t* nrf) {
    nrf->pwr_up_high();
    nrf->chip_en_high();
    nrf->tx_en_high();
    //Do we want to put chip back into standby mode after sending packet?
    nrf->chip_en_low();
}

//Put chip into RX mode, 
void nrf905_rx_mode(nrf905_t* nrf) {
    nrf->pwr_up_high();
    nrf->chip_en_high();
    nrf->tx_en_low();
    //IF (DATA READY PIN HIGH)
    nrf->chip_en_low();
}

void nrf905_print_config(nrf905_t* nrf); //Printf all config settings (to SWO on STM32)
