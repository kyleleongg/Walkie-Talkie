/*
Date: August 17, 2025
Author: Kyle Leong

This is a driver for the nrf905 RFIC with SPI communication to be used in conjunction with STM32F100 in a Walkie Talkie project.
This .h file will declare constant hex addresses/values and functions that the .c file will implement. This will abstract away the 
low level code and allow for clean, readable code in the main.c file.
*/

#ifndef NRF905_H
#define NRF905_H

//Define the stm32fxxx files needed for spi...? Should be generated in cubeide

//RF - Configuration Register Descriptions pg. 23
//All hex values are pre-shifted to according spots so that they can be OR'd together (starting with an empty
//byte of 8'b0000_0000) when composing bytes of data

//Config1[1]
typedef enum
{
    NRF905_PLL_433 = 0x00, //433MHz
    NRF905_PLL_868 = 0x02, //868MHz
    NRF905_PLL_915 = 0x02  //915MHz
} NRF905_band;


//Config1[3:2] 00xx
typedef enum
{
    NRF905_PWR_N10DB = 0x00, //-10db
    NRF905_PWR_N2DB  = 0x04, //-2db
    NRF905_PWR_6DB   = 0x08, //6db
    NRF905_PWR_10DB  = 0x0C  //10db
} NRF905_pwr_db;


//Config1[4]
typedef enum
{
    NRF905_RX_RED_PWR_EN = 0x10, 
    NRF905_RX_RED_PWR_DISABLE = 0x00
} NRF905_rx_reduced_pwr;


//Config1[5]
typedef enum
{
    NRF905_AUTO_RETRAN_EN = 0x20,
    NRF905_AUTO_RETRAN_DISABLE = 0x00
} NRF905_auto_retran;


//Config2[2:0]
typedef enum
{
    NRF905_RX_ADD_WIDTH_1 = 0x01, //1 byte address width
    NRF905_RX_ADD_WIDTH_4 = 0x04  //4 byte address width
} NRF905_rx_add_width;


//Config2[6:4]
typedef enum
{
    NRF905_TX_ADD_WIDTH_1 = 0x10, //1 byte address width
    NRF905_TX_ADD_WIDTH_4 = 0x40  //4 byte address width
} NRF905_tx_add_width;


//Config3[5:0]
typedef enum
{
    NRF905_RX_PAYLOAD_WIDTH_1  = 0x01, //1 byte
    NRF905_RX_PAYLOAD_WIDTH_2  = 0x02, //2 bytes
    NRF905_RX_PAYLOAD_WIDTH_32 = 0x20  //32 bytes
} NRF905_rx_payload_width;


//Config4[5:0]
typedef enum
{
    NRF905_TX_PAYLOAD_WIDTH_1  = 0x01, //1 byte
    NRF905_TX_PAYLOAD_WIDTH_2  = 0x02, //2 bytes
    NRF905_TX_PAYLOAD_WIDTH_32 = 0x20  //32 bytes
} NRF905_tx_payload_width;


//Omitted RX address here, will be done manually in .c 


//Config9[1:0]
typedef enum
{
    NRF905_UP_CLK_4M   = 0x00, //4MHz
    NRF905_UP_CLK_2M   = 0x01, //2MHz
    NRF905_UP_CLK_1M   = 0x02, //1MHz
    NRF905_UP_CLK_500K = 0x03 //500kHz
} NRF905_up_clk_freq;


//Config9[2]
typedef enum
{
    NRF905_UP_CLK_DISABLE = 0x00, //Disable output clock
    NRF905_UP_CLK_EN      = 0x04  //Enable output clock
} NRF905_up_clk;


//Config9[5:3] 
typedef enum
{
    NRF905_XOF_4  = 0x00, //4MHz external crystal oscillator
    NRF905_XOF_8  = 0x08, //8MHz
    NRF905_XOF_12 = 0x10, //12MHz
    NRF905_XOF_16 = 0x18, //16MHz
    NRF905_XOF_20 = 0x20  //20MHz
} NRF905_xof;


//Config9[7] 
typedef enum
{
    NRF905_CRC_MODE_8  = 0x00, //8 CRC check bit
    NRF905_CRC_MODE_16 = 0x80  //16 CRC check bit
} NRF905_crc_mode;

//Config9[6]
typedef enum
{
    NRF905_CRC_EN = 0X40,      //Enable CRC mode
    NRF905_CRC_DISABLE = 0x00  //Disable CRC mode
} NRF905_crc;


//Define functions to be written in .c driver






#endif

