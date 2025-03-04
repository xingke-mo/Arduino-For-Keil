/******************************************************************************
 * The MIT License
 *
 * Copyright (c) 2011 LeafLabs LLC.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *****************************************************************************/

/**
 * @file libmaple/usb/stm32f1/usb_hid.c
 * @brief USB HID (human interface device) support
 *
 * FIXME: this works on the STM32F1 USB peripherals, and probably no
 * place else. Nonportable bits really need to be factored out, and
 * the result made cleaner.
 */

#include "usb_composite_serial.h"
#include "usb_generic.h"
#include <string.h>
#include <libmaple/usb.h>
#include <libmaple/delay.h>
//#include <libmaple/gpio.h>

/* Private headers */
#include "usb_lib_globals.h"
#include "usb_reg_map.h"

#define CDCACM_ENDPOINT_TX         0
#define CDCACM_ENDPOINT_MANAGEMENT 1
#define CDCACM_ENDPOINT_RX         2

#define USB_CDCACM_MANAGEMENT_ENDP    (serialEndpoints[CDCACM_ENDPOINT_MANAGEMENT].address)
#define USB_CDCACM_TX_ENDP            (serialEndpoints[CDCACM_ENDPOINT_TX].address)
#define USB_CDCACM_RX_ENDP            (serialEndpoints[CDCACM_ENDPOINT_RX].address)
#define USB_CDCACM_MANAGEMENT_ENDPOINT_INFO    (&serialEndpoints[CDCACM_ENDPOINT_MANAGEMENT])
#define USB_CDCACM_TX_ENDPOINT_INFO            (&serialEndpoints[CDCACM_ENDPOINT_TX])
#define USB_CDCACM_RX_ENDPOINT_INFO            (&serialEndpoints[CDCACM_ENDPOINT_RX])

/* usb_lib headers */
#include "usb_type.h"
#include "usb_core.h"
#include "usb_def.h"

static void serialUSBReset(void);
static RESULT serialUSBDataSetup(uint8 request, uint8 interface, uint8 requestType, uint8 wValue0, uint8 wValue1, uint16 wIndex, uint16 wLength);
static RESULT serialUSBNoDataSetup(uint8 request, uint8 interface, uint8 requestType, uint8 wValue0, uint8 wValue1, uint16 wIndex);
static void vcomDataTxCb(void);
static void vcomDataRxCb(void);

#define NUM_SERIAL_ENDPOINTS       3
#define CCI_INTERFACE_OFFSET    0x00
#define DCI_INTERFACE_OFFSET    0x01
//#define SERIAL_MANAGEMENT_INTERFACE_NUMBER (CCI_INTERFACE_OFFSET+usbSerialPart.startInterface)

/*
 * Descriptors
 */

static volatile int8 transmitting;
static uint32_t txEPSize = 64;
static uint32_t rxEPSize = 64;

typedef struct
{
    //CDCACM
    IADescriptor                    IAD;
    usb_descriptor_interface        CCI_Interface;
    CDC_FUNCTIONAL_DESCRIPTOR(2)    CDC_Functional_IntHeader;
    CDC_FUNCTIONAL_DESCRIPTOR(2)    CDC_Functional_CallManagement;
    CDC_FUNCTIONAL_DESCRIPTOR(1)    CDC_Functional_ACM;
    CDC_FUNCTIONAL_DESCRIPTOR(2)    CDC_Functional_Union;
    usb_descriptor_endpoint         ManagementEndpoint;
    usb_descriptor_interface        DCI_Interface;
    usb_descriptor_endpoint         DataOutEndpoint;
    usb_descriptor_endpoint         DataInEndpoint;
} __packed serial_part_config;


static const serial_part_config serialPartConfigData =
{
    .IAD = {
        .bLength            = 0x08,
        .bDescriptorType    = 0x0B,
        .bFirstInterface    = CCI_INTERFACE_OFFSET, // PATCH
        .bInterfaceCount    = 0x02,
        .bFunctionClass     = 0x02,
        .bFunctionSubClass  = 0x02,
        .bFunctionProtocol  = 0x01,
        .iFunction          = 0x02,
    },
    .CCI_Interface = {
        .bLength            = sizeof(usb_descriptor_interface),
        .bDescriptorType    = USB_DESCRIPTOR_TYPE_INTERFACE,
        .bInterfaceNumber   = CCI_INTERFACE_OFFSET, // PATCH
        .bAlternateSetting  = 0x00,
        .bNumEndpoints      = 0x01,
        .bInterfaceClass    = USB_INTERFACE_CLASS_CDC,
        .bInterfaceSubClass = USB_INTERFACE_SUBCLASS_CDC_ACM,
        .bInterfaceProtocol = 0x01, /* Common AT Commands */
        .iInterface         = 0x00,
    },

    .CDC_Functional_IntHeader = {
        .bLength         = CDC_FUNCTIONAL_DESCRIPTOR_SIZE(2),
        .bDescriptorType = 0x24,
        .SubType         = 0x00,
        .Data            = {0x01, 0x10},
    },

    .CDC_Functional_CallManagement = {
        .bLength         = CDC_FUNCTIONAL_DESCRIPTOR_SIZE(2),
        .bDescriptorType = 0x24,
        .SubType         = 0x01,
        .Data            = {0x03, DCI_INTERFACE_OFFSET}, // PATCH
    },

    .CDC_Functional_ACM = {
        .bLength         = CDC_FUNCTIONAL_DESCRIPTOR_SIZE(1),
        .bDescriptorType = 0x24,
        .SubType         = 0x02,
        .Data            = {0x06},
    },

    .CDC_Functional_Union = {
        .bLength         = CDC_FUNCTIONAL_DESCRIPTOR_SIZE(2),
        .bDescriptorType = 0x24,
        .SubType         = 0x06,
        .Data            = {CCI_INTERFACE_OFFSET, DCI_INTERFACE_OFFSET}, // PATCH, PATCH
    },

    .ManagementEndpoint = {
        .bLength          = sizeof(usb_descriptor_endpoint),
        .bDescriptorType  = USB_DESCRIPTOR_TYPE_ENDPOINT,
        .bEndpointAddress = (USB_DESCRIPTOR_ENDPOINT_IN |
        0), // PATCH: CDCACM_ENDPOINT_MANAGEMENT
        .bmAttributes     = USB_EP_TYPE_INTERRUPT,
        .wMaxPacketSize   = USBHID_CDCACM_MANAGEMENT_EPSIZE,
        .bInterval        = 0xFF,
    },

    .DCI_Interface = {
        .bLength            = sizeof(usb_descriptor_interface),
        .bDescriptorType    = USB_DESCRIPTOR_TYPE_INTERFACE,
        .bInterfaceNumber   = DCI_INTERFACE_OFFSET, // PATCH
        .bAlternateSetting  = 0x00,
        .bNumEndpoints      = 0x02,
        .bInterfaceClass    = USB_INTERFACE_CLASS_DIC,
        .bInterfaceSubClass = 0x00, /* None */
        .bInterfaceProtocol = 0x00, /* None */
        .iInterface         = 0x00,
    },

    .DataOutEndpoint = {
        .bLength          = sizeof(usb_descriptor_endpoint),
        .bDescriptorType  = USB_DESCRIPTOR_TYPE_ENDPOINT,
        .bEndpointAddress = (USB_DESCRIPTOR_ENDPOINT_OUT |
        0), // patch: CDCACM_ENDPOINT_RX
        .bmAttributes     = USB_EP_TYPE_BULK,
        .wMaxPacketSize   = 64, // patch
        .bInterval        = 0x00,
    },

    .DataInEndpoint = {
        .bLength          = sizeof(usb_descriptor_endpoint),
        .bDescriptorType  = USB_DESCRIPTOR_TYPE_ENDPOINT,
        .bEndpointAddress = (USB_DESCRIPTOR_ENDPOINT_IN | 0), // PATCH: CDCACM_ENDPOINT_TX
        .bmAttributes     = USB_EP_TYPE_BULK,
        .wMaxPacketSize   = 64, // patch
        .bInterval        = 0x00,
    }
};

#define OUT_BYTE(s,v) out[(uint8*)&(s.v)-(uint8*)&s]
#define OUT_16(s,v) *(uint16_t*)&OUT_BYTE(s,v) // OK on Cortex which can handle unaligned writes

static USBEndpointInfo serialEndpoints[3] =
{
    {
        .callback = vcomDataTxCb,
        .pmaSize = 64, // patch
        .type = USB_GENERIC_ENDPOINT_TYPE_BULK,
        .tx = 1,
    },
    {
        .callback = NULL,
        .pmaSize = USBHID_CDCACM_MANAGEMENT_EPSIZE,
        .type = USB_GENERIC_ENDPOINT_TYPE_INTERRUPT,
        .tx = 1,
    },
    {
        .callback = vcomDataRxCb,
        .pmaSize = 64, // patch
        .type = USB_GENERIC_ENDPOINT_TYPE_BULK,
        .tx = 0,
    },
};

static void getSerialPartDescriptor(uint8* out)
{
    memcpy(out, &serialPartConfigData, sizeof(serial_part_config));

    // patch to reflect where the part goes in the descriptor
    OUT_BYTE(serialPartConfigData, ManagementEndpoint.bEndpointAddress) += USB_CDCACM_MANAGEMENT_ENDP;
    OUT_BYTE(serialPartConfigData, DataOutEndpoint.bEndpointAddress) += USB_CDCACM_RX_ENDP;
    OUT_BYTE(serialPartConfigData, DataInEndpoint.bEndpointAddress) += USB_CDCACM_TX_ENDP;

    OUT_BYTE(serialPartConfigData, IAD.bFirstInterface) += usbSerialPart.startInterface;
    OUT_BYTE(serialPartConfigData, CCI_Interface.bInterfaceNumber) += usbSerialPart.startInterface;
    OUT_BYTE(serialPartConfigData, DCI_Interface.bInterfaceNumber) += usbSerialPart.startInterface;
    OUT_BYTE(serialPartConfigData, CDC_Functional_CallManagement.Data[1]) += usbSerialPart.startInterface;
    OUT_BYTE(serialPartConfigData, CDC_Functional_Union.Data[0]) += usbSerialPart.startInterface;
    OUT_BYTE(serialPartConfigData, CDC_Functional_Union.Data[1]) += usbSerialPart.startInterface;

    OUT_16(serialPartConfigData, DataOutEndpoint.wMaxPacketSize) = rxEPSize;
    OUT_16(serialPartConfigData, DataInEndpoint.wMaxPacketSize) = txEPSize;
}

void composite_cdcacm_setTXEPSize(uint32_t size)
{
    if(size == 0)
    {
        size = 64;
    }

    serialEndpoints[0].pmaSize = size;
    txEPSize = size;
}

void composite_cdcacm_setRXEPSize(uint32_t size)
{
    if(size == 0)
    {
        size = 64;
    }

    size = usb_generic_roundUpToPowerOf2(size);
    serialEndpoints[2].pmaSize = size;
    rxEPSize = size;
}

USBCompositePart usbSerialPart =
{
    .numInterfaces = 2,
    .numEndpoints = sizeof(serialEndpoints) / sizeof(*serialEndpoints),
    .descriptorSize = sizeof(serial_part_config),
    .getPartDescriptor = getSerialPartDescriptor,
    .usbInit = NULL,
    .usbReset = serialUSBReset,
    .usbDataSetup = serialUSBDataSetup,
    .usbNoDataSetup = serialUSBNoDataSetup,
    .endpoints = serialEndpoints
};

#define CDC_SERIAL_RX_BUFFER_SIZE   256 // must be power of 2
#define CDC_SERIAL_RX_BUFFER_SIZE_MASK (CDC_SERIAL_RX_BUFFER_SIZE-1)

/* Received data */
static volatile uint8 vcomBufferRx[CDC_SERIAL_RX_BUFFER_SIZE];
/* Write index to vcomBufferRx */
static volatile uint32 vcom_rx_head;
/* Read index from vcomBufferRx */
static volatile uint32 vcom_rx_tail;

#define CDC_SERIAL_TX_BUFFER_SIZE   256 // must be power of 2
#define CDC_SERIAL_TX_BUFFER_SIZE_MASK (CDC_SERIAL_TX_BUFFER_SIZE-1)
// Tx data
static volatile uint8 vcomBufferTx[CDC_SERIAL_TX_BUFFER_SIZE];
// Write index to vcomBufferTx
static volatile uint32 vcom_tx_head;
// Read index from vcomBufferTx
static volatile uint32 vcom_tx_tail;



/* Other state (line coding, DTR/RTS) */

static volatile composite_cdcacm_line_coding line_coding =
{
    /* This default is 115200 baud, 8N1. */
    .dwDTERate   = 115200,
    .bCharFormat = USBHID_CDCACM_STOP_BITS_1,
    .bParityType = USBHID_CDCACM_PARITY_NONE,
    .bDataBits   = 8,
};

/* DTR in bit 0, RTS in bit 1. */
static volatile uint8 line_dtr_rts = 0;


static void (*rx_hook)(unsigned, void*) = 0;
static void (*iface_setup_hook)(unsigned, void*) = 0;

void composite_cdcacm_set_hooks(unsigned hook_flags, void (*hook)(unsigned, void*))
{
    if(hook_flags & USBHID_CDCACM_HOOK_RX)
    {
        rx_hook = hook;
    }

    if(hook_flags & USBHID_CDCACM_HOOK_IFACE_SETUP)
    {
        iface_setup_hook = hook;
    }
}

/* This function is non-blocking.
 *
 * It copies data from a user buffer into the USB peripheral TX
 * buffer, and returns the number of bytes copied. */
uint32 composite_cdcacm_tx(const uint8* buf, uint32 len)
{
    if(len == 0)
    {
        return 0;    // no data to send
    }

    uint32 head = vcom_tx_head; // load volatile variable
    uint32 tx_unsent = (head - vcom_tx_tail) & CDC_SERIAL_TX_BUFFER_SIZE_MASK;

    // We can only put bytes in the buffer if there is place
    if(len > (CDC_SERIAL_TX_BUFFER_SIZE - tx_unsent - 1))
    {
        len = (CDC_SERIAL_TX_BUFFER_SIZE - tx_unsent - 1);
    }

    if(len == 0)
    {
        return 0;    // buffer full
    }

    uint16 i;

    // copy data from user buffer to USB Tx buffer
    for(i = 0; i < len; i++)
    {
        vcomBufferTx[head] = buf[i];
        head = (head + 1) & CDC_SERIAL_TX_BUFFER_SIZE_MASK;
    }

    vcom_tx_head = head; // store volatile variable

    while(transmitting >= 0);

    if(transmitting < 0)
    {
        vcomDataTxCb(); // initiate data transmission
    }

    return len;
}



uint32 composite_cdcacm_data_available(void)
{
    return (vcom_rx_head - vcom_rx_tail) & CDC_SERIAL_RX_BUFFER_SIZE_MASK;
}

uint16 composite_cdcacm_get_pending(void)
{
    return (vcom_tx_head - vcom_tx_tail) & CDC_SERIAL_TX_BUFFER_SIZE_MASK;
}

/* Non-blocking byte receive.
 *
 * Copies up to len bytes from our private data buffer (*NOT* the PMA)
 * into buf and deq's the FIFO. */
uint32 composite_cdcacm_rx(uint8* buf, uint32 len)
{
    /* Copy bytes to buffer. */
    uint32 n_copied = composite_cdcacm_peek(buf, len);

    /* Mark bytes as read. */
    uint16 tail = vcom_rx_tail; // load volatile variable
    tail = (tail + n_copied) & CDC_SERIAL_RX_BUFFER_SIZE_MASK;
    vcom_rx_tail = tail; // store volatile variable

    uint32 rx_unread = (vcom_rx_head - tail) & CDC_SERIAL_RX_BUFFER_SIZE_MASK;

    // If buffer was emptied to a pre-set value, re-enable the RX endpoint
    if(rx_unread <= 64)      // experimental value, gives the best performance
    {
        usb_generic_enable_rx(USB_CDCACM_RX_ENDPOINT_INFO);
    }

    return n_copied;
}

/* Non-blocking byte lookahead.
 *
 * Looks at unread bytes without marking them as read. */
uint32 composite_cdcacm_peek(uint8* buf, uint32 len)
{
    unsigned i;
    uint32 tail = vcom_rx_tail;
    uint32 rx_unread = (vcom_rx_head - tail) & CDC_SERIAL_RX_BUFFER_SIZE_MASK;

    if(len > rx_unread)
    {
        len = rx_unread;
    }

    for(i = 0; i < len; i++)
    {
        buf[i] = vcomBufferRx[tail];
        tail = (tail + 1) & CDC_SERIAL_RX_BUFFER_SIZE_MASK;
    }

    return len;
}

uint32 composite_cdcacm_peek_ex(uint8* buf, uint32 offset, uint32 len)
{
    unsigned i;
    uint32 tail = (vcom_rx_tail + offset) & CDC_SERIAL_RX_BUFFER_SIZE_MASK ;
    uint32 rx_unread = (vcom_rx_head - tail) & CDC_SERIAL_RX_BUFFER_SIZE_MASK;

    if(len + offset > rx_unread)
    {
        len = rx_unread - offset;
    }

    for(i = 0; i < len; i++)
    {
        buf[i] = vcomBufferRx[tail];
        tail = (tail + 1) & CDC_SERIAL_RX_BUFFER_SIZE_MASK;
    }

    return len;
}

/* Roger Clark. Added. for Arduino 1.0 API support of Serial.peek() */
int composite_cdcacm_peek_char()
{
    if(composite_cdcacm_data_available() == 0)
    {
        return -1;
    }

    return vcomBufferRx[vcom_rx_tail];
}

uint8 composite_cdcacm_get_dtr()
{
    return ((line_dtr_rts & USBHID_CDCACM_CONTROL_LINE_DTR) != 0);
}

uint8 composite_cdcacm_get_rts()
{
    return ((line_dtr_rts & USBHID_CDCACM_CONTROL_LINE_RTS) != 0);
}

void composite_cdcacm_get_line_coding(composite_cdcacm_line_coding *ret)
{
    ret->dwDTERate = line_coding.dwDTERate;
    ret->bCharFormat = line_coding.bCharFormat;
    ret->bParityType = line_coding.bParityType;
    ret->bDataBits = line_coding.bDataBits;
}

int composite_cdcacm_get_baud(void)
{
    return line_coding.dwDTERate;
}

int composite_cdcacm_get_stop_bits(void)
{
    return line_coding.bCharFormat;
}

int composite_cdcacm_get_parity(void)
{
    return line_coding.bParityType;
}

int composite_cdcacm_get_n_data_bits(void)
{
    return line_coding.bDataBits;
}

/*
 * Callbacks
 */
static void vcomDataTxCb(void)
{
    usb_generic_send_from_circular_buffer(USB_CDCACM_TX_ENDPOINT_INFO,
                                          vcomBufferTx, CDC_SERIAL_TX_BUFFER_SIZE, vcom_tx_head, &vcom_tx_tail, &transmitting);
}


static void vcomDataRxCb(void)
{
    uint32 head = vcom_rx_head;
    usb_generic_read_to_circular_buffer(USB_CDCACM_RX_ENDPOINT_INFO,
                                        vcomBufferRx, CDC_SERIAL_RX_BUFFER_SIZE, &head);
    vcom_rx_head = head; // store volatile variable

    uint32 rx_unread = (head - vcom_rx_tail) & CDC_SERIAL_RX_BUFFER_SIZE_MASK;

    // only enable further Rx if there is enough room to receive one more packet
    if(rx_unread < (CDC_SERIAL_RX_BUFFER_SIZE - rxEPSize))
    {
        usb_generic_enable_rx(USB_CDCACM_RX_ENDPOINT_INFO);
    }

    if(rx_hook)
    {
        rx_hook(USBHID_CDCACM_HOOK_RX, 0);
    }
}

static void serialUSBReset(void)
{
    //VCOM
    vcom_rx_head = 0;
    vcom_rx_tail = 0;
    vcom_tx_head = 0;
    vcom_tx_tail = 0;
    transmitting = -1;
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
static RESULT serialUSBDataSetup(uint8 request, uint8 interface, uint8 requestType, uint8 wValue0, uint8 wValue1, uint16 wIndex, uint16 wLength)
{
    RESULT ret = USB_UNSUPPORT;

    if((requestType & (REQUEST_TYPE | RECIPIENT)) == (CLASS_REQUEST | INTERFACE_RECIPIENT) && interface == CCI_INTERFACE_OFFSET)
    {
        switch(request)
        {
        case USBHID_CDCACM_GET_LINE_CODING:
            usb_generic_control_tx_setup(&line_coding, sizeof(line_coding), NULL);
            ret = USB_SUCCESS;
            break;

        case USBHID_CDCACM_SET_LINE_CODING:
            usb_generic_control_rx_setup(&line_coding, sizeof(line_coding), NULL);
            ret = USB_SUCCESS;
            break;

        default:
            break;
        }

        /* Call the user hook. */
        if(iface_setup_hook)
        {
            uint8 req_copy = request;
            iface_setup_hook(USBHID_CDCACM_HOOK_IFACE_SETUP, &req_copy);
        }
    }

    return ret;
}

static RESULT serialUSBNoDataSetup(uint8 request, uint8 interface, uint8 requestType, uint8 wValue0, uint8 wValue1, uint16 wIndex)
{
    RESULT ret = USB_UNSUPPORT;

    if((requestType & (REQUEST_TYPE | RECIPIENT)) == (CLASS_REQUEST | INTERFACE_RECIPIENT) && interface == CCI_INTERFACE_OFFSET)
    {
        switch(request)
        {
        case USBHID_CDCACM_SET_COMM_FEATURE:
            /* We support set comm. feature, but don't handle it. */
            ret = USB_SUCCESS;
            break;

        case USBHID_CDCACM_SET_CONTROL_LINE_STATE:
            /* Track changes to DTR and RTS. */
            line_dtr_rts = (wValue0 &
                            (USBHID_CDCACM_CONTROL_LINE_DTR |
                             USBHID_CDCACM_CONTROL_LINE_RTS));
            ret = USB_SUCCESS;
            break;
        }

        /* Call the user hook. */
        if(iface_setup_hook)
        {
            uint8 req_copy = request;
            iface_setup_hook(USBHID_CDCACM_HOOK_IFACE_SETUP, &req_copy);
        }
    }

    return ret;
}

