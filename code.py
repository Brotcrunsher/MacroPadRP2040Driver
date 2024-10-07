import usb_hid
from adafruit_macropad import MacroPad

custom_hid = None
for device in usb_hid.devices:
    if device.usage_page == 0xFF00 and device.usage == 0x01:
        custom_hid = device
        break

if custom_hid is None:
    raise RuntimeError("Couldn't find hid!")

macropad = MacroPad()
macropad.pixels.fill((0, 0, 0))

last_position = macropad.encoder

while True:
    key_event = macropad.keys.events.get()
    if key_event:
        key_number = key_event.key_number
        if key_event.pressed:
            report = bytes([key_number, 0])
            custom_hid.send_report(report)
            macropad.pixels[key_number] = (5, 5, 15)
        else:
            report = bytes([key_number, 1])
            custom_hid.send_report(report)
            macropad.pixels[key_number] = (0, 0, 0)
            
    position = macropad.encoder
    if position != last_position:
        position_change = position - last_position
        last_position = position
        if position_change > 0:
            report = bytes([100, 0])
        else:
            report = bytes([100, 1])
        custom_hid.send_report(report)

    macropad.encoder_switch_debounced.update()
    if macropad.encoder_switch_debounced.pressed:
        report = bytes([101, 0])
        custom_hid.send_report(report)
    elif macropad.encoder_switch_debounced.released:
        report = bytes([101, 1])
        custom_hid.send_report(report)
