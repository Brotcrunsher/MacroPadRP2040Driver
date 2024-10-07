import usb_hid

CUSTOM_HID_REPORT_DESCRIPTOR = bytes((
    0x06, 0x00, 0xFF,       # Usage Page (Vendor Defined 0xFF00)
    0x09, 0x01,             # Usage (Vendor Usage 0x01)
    0xA1, 0x01,             # Collection (Application)
    0x85, 0x01,             # Report ID (1)
    0x15, 0x00,             # Logical Minimum (0)
    0x26, 0xFF, 0x00,       # Logical Maximum (255)
    0x75, 0x08,             # Report Size (8 bits)
    0x95, 0x02,             # Report Count (2 fields)
    0x09, 0x01,             # Usage (Vendor Usage 0x01)
    0x81, 0x02,             # Input (Data, Variable, Absolute)
    0xC0                    # End Collection
))

custom_hid = usb_hid.Device(
    report_descriptor=CUSTOM_HID_REPORT_DESCRIPTOR,
    usage_page=0xFF00,
    usage=0x01,
    report_ids=(1,),
    in_report_lengths=(2,),
    out_report_lengths=(0,),
)

usb_hid.enable((custom_hid,))
