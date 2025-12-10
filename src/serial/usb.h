//
// USB definitions
//

#pragma once

#include <stdint.h>

void usb_init_b(void);
void _usb_irq_handle(void);

void usb_cdc_send(const uint8_t *data, uint16_t len);
uint16_t usb_cdc_recv(uint8_t *data, uint16_t max_len);

extern uint16_t usb_cdc_recv_buff_len;
extern uint8_t usb_cdc_recv_buff[64];

// Simplified descriptors - tested and working
static const uint8_t device_descriptor[] = {
    18,         // bLength
    1,          // bDescriptorType (Device)
    0x00, 0x02, // bcdUSB 2.0 (little endian: 0x0200)
    2,          // bDeviceClass (CDC)
    0,          // bDeviceSubClass
    0,          // bDeviceProtocol
    64,         // bMaxPacketSize0
    0x2e, 0x04, // idVendor (0x042e) - just an example
    0x01, 0x00, // idProduct (0x0001)
    0x00, 0x01, // bcdDevice (0x0100)
    1,          // iManufacturer
    2,          // iProduct
    0,          // iSerialNumber
    1           // bNumConfigurations
};

static const uint8_t config_descriptor[] = {
    // Configuration descriptor
    9,          // bLength
    2,          // bDescriptorType (Configuration)
    0x43, 0x00, // wTotalLength 67 bytes
    2,          // bNumInterfaces
    1,          // bConfigurationValue
    0,          // iConfiguration
    0x80,       // bmAttributes (bus powered)
    250,        // bMaxPower (500mA) - host will limit as needed

    // Interface 0 (CDC Control)
    9, // bLength
    4, // bDescriptorType (Interface)
    0, // bInterfaceNumber
    0, // bAlternateSetting
    1, // bNumEndpoints
    2, // bInterfaceClass (CDC)
    2, // bInterfaceSubClass (ACM)
    0, // bInterfaceProtocol (no AT commands)
    0, // iInterface

    // CDC Header Functional Descriptor
    5,          // bLength
    0x24,       // bDescriptorType (CS_INTERFACE)
    0,          // bDescriptorSubtype (Header)
    0x10, 0x01, // bcdCDC 1.10

    // CDC Call Management Functional Descriptor
    5,    // bLength
    0x24, // bDescriptorType (CS_INTERFACE)
    1,    // bDescriptorSubtype (Call Management)
    0,    // bmCapabilities
    1,    // bDataInterface

    // CDC ACM Functional Descriptor
    4,    // bLength
    0x24, // bDescriptorType (CS_INTERFACE)
    2,    // bDescriptorSubtype (ACM)
    2,    // bmCapabilities (line coding, serial state)

    // CDC Union Functional Descriptor
    5,    // bLength
    0x24, // bDescriptorType (CS_INTERFACE)
    6,    // bDescriptorSubtype (Union)
    0,    // bMasterInterface
    1,    // bSlaveInterface0

    // Endpoint 1 IN (Interrupt)
    7,    // bLength
    5,    // bDescriptorType (Endpoint)
    0x81, // bEndpointAddress (IN 1)
    3,    // bmAttributes (Interrupt)
    8, 0, // wMaxPacketSize
    255,  // bInterval

    // Interface 1 (CDC Data)
    9,    // bLength
    4,    // bDescriptorType (Interface)
    1,    // bInterfaceNumber
    0,    // bAlternateSetting
    2,    // bNumEndpoints
    0x0A, // bInterfaceClass (CDC Data)
    0,    // bInterfaceSubClass
    0,    // bInterfaceProtocol
    0,    // iInterface

    // Endpoint 2 OUT (Bulk)
    7,     // bLength
    5,     // bDescriptorType (Endpoint)
    0x02,  // bEndpointAddress (OUT 2)
    2,     // bmAttributes (Bulk)
    64, 0, // wMaxPacketSize
    0,     // bInterval

    // Endpoint 2 IN (Bulk)
    7,     // bLength
    5,     // bDescriptorType (Endpoint)
    0x82,  // bEndpointAddress (IN 2)
    2,     // bmAttributes (Bulk)
    64, 0, // wMaxPacketSize
    0      // bInterval
};

static const uint8_t string_descriptor_0[] = {
    4,         // bLength
    3,         // bDescriptorType (String)
    0x09, 0x04 // wLANGID English
};

static const uint8_t string_descriptor_1[] = {16, // bLength
                                              3,  // bDescriptorType (String)
                                              'p', 0,   'O', 0,   'S', 0,   ' ',
                                              0,   'd', 0,   'e', 0,   'v', 0};

static const uint8_t string_descriptor_2[] = {22, // bLength
                                              3,  // bDescriptorType (String)
                                              'p', 0,   'O', 0,   'S', 0,   ' ',
                                              0,   'S', 0,   'e', 0,   'r', 0,
                                              'i', 0,   'a', 0,   'l', 0};
