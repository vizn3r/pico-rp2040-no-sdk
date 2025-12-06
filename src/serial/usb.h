//
// USB definitions
//

#pragma once

#include <stdint.h>

void usb_init_b(void);
void usb_irq_handle(void);

void usb_cdc_send(const uint8_t *data, uint16_t len);
uint16_t usb_cdc_recv(uint8_t *data, uint16_t max_len);

typedef struct {
  uint8_t bmRequestType;
  uint8_t bRequest;
  uint16_t wValue;
  uint16_t wIndex;
  uint16_t wLength;
} __attribute__((packed)) usb_setup_packet_t;

extern uint16_t usb_cdc_recv_buff_len;
extern uint8_t usb_cdc_recv_buff[64];

static const uint8_t device_descriptor[] = {
    18,         // bLength
    0x01,       // bDescriptorType (Device)
    0x00, 0x02, // bcdUSB (USB 2.0)
    0x00,       // bDeviceClass
    0x00,       // bDeviceSubClass
    0x00,       // bDeviceProtocol
    64,         // bMaxPacketSize0
    0x09, 0x12, // idVendor (example: 0x1209)
    0x01, 0x00, // idProduct (example: 0x0001)
    0x00, 0x01, // bcdDevice
    0x01,       // iManufacturer (string index)
    0x02,       // iProduct (string index)
    0x00,       // iSerialNumber
    0x01        // bNumConfigurations;
};

static const uint8_t config_descriptor[] = {
    // Configuration Descriptor
    9,     // bLength
    0x02,  // bDescriptorType (Configuration)
    67, 0, // wTotalLength (9+9+5+5+4+5+7+9+7+7 = 67 bytes total)
    0x02,  // bNumInterfaces (2: control + data)
    0x01,  // bConfigurationValue
    0x00,  // iConfiguration
    0x80,  // bmAttributes (bus powered)
    250,   // bMaxPower (500mA)

    // Interface 0: CDC Control
    9,    // bLength
    0x04, // bDescriptorType (Interface)
    0x00, // bInterfaceNumber
    0x00, // bAlternateSetting
    0x01, // bNumEndpoints (1 interrupt endpoint)
    0x02, // bInterfaceClass (CDC)
    0x02, // bInterfaceSubClass (ACM)
    0x01, // bInterfaceProtocol (AT commands)
    0x00, // iInterface

    // CDC Header Functional Descriptor
    5,          // bLength
    0x24,       // bDescriptorType (CS_INTERFACE)
    0x00,       // bDescriptorSubtype (Header)
    0x10, 0x01, // bcdCDC (1.10)

    // CDC Call Management Functional Descriptor
    5,    // bLength
    0x24, // bDescriptorType (CS_INTERFACE)
    0x01, // bDescriptorSubtype (Call Management)
    0x00, // bmCapabilities
    0x01, // bDataInterface

    // CDC ACM Functional Descriptor
    4,    // bLength
    0x24, // bDescriptorType (CS_INTERFACE)
    0x02, // bDescriptorSubtype (ACM)
    0x02, // bmCapabilities

    // CDC Union Functional Descriptor
    5,    // bLength
    0x24, // bDescriptorType (CS_INTERFACE)
    0x06, // bDescriptorSubtype (Union)
    0x00, // bControlInterface
    0x01, // bSubordinateInterface0

    // Endpoint 1 IN (Interrupt)
    7,    // bLength
    0x05, // bDescriptorType (Endpoint)
    0x81, // bEndpointAddress (IN, endpoint 1)
    0x03, // bmAttributes (Interrupt)
    8, 0, // wMaxPacketSize
    16,   // bInterval

    // Interface 1: CDC Data
    9,    // bLength
    0x04, // bDescriptorType (Interface)
    0x01, // bInterfaceNumber
    0x00, // bAlternateSetting
    0x02, // bNumEndpoints (2: bulk in + bulk out)
    0x0A, // bInterfaceClass (CDC Data)
    0x00, // bInterfaceSubClass
    0x00, // bInterfaceProtocol
    0x00, // iInterface

    // Endpoint 2 OUT (Bulk)
    7,     // bLength
    0x05,  // bDescriptorType (Endpoint)
    0x02,  // bEndpointAddress (OUT, endpoint 2)
    0x02,  // bmAttributes (Bulk)
    64, 0, // wMaxPacketSize
    0,     // bInterval

    // Endpoint 2 IN (Bulk)
    7,     // bLength
    0x05,  // bDescriptorType (Endpoint)
    0x82,  // bEndpointAddress (IN, endpoint 2)
    0x02,  // bmAttributes (Bulk)
    64, 0, // wMaxPacketSize
    0      // bInterval
};

// String 0: Language IDs
static const uint8_t string_descriptor_0[] = {
    4,         // bLength
    0x03,      // bDescriptorType (String)
    0x09, 0x04 // wLANGID (0x0409 = US English)
};

// String 1: Manufacturer
static const uint8_t string_descriptor_1[] = {
    14,   // bLength
    0x03, // bDescriptorType (String)
    'p',  0,   'O', 0,   'S', 0,   ' ',
    0,    'd', 0,   'e', 0,   'v', 0 // "pOS dev" in UTF-16LE
};

// String 2: Product
static const uint8_t string_descriptor_2[] = {
    18,   // bLength
    0x03, // bDescriptorType (String)
    'p',  0, 'O', 0, 'S', 0, ' ', 0, 'S', 0,
    'e',  0, 'r', 0, 'i', 0, 'a', 0, 'l', 0 // "pOS Serial" in UTF-16LE
};
