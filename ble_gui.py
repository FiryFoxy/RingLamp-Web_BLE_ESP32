import ui
import cb
import console
import os

SERVICE_UUID = "19b10000-e8f2-537e-4f6c-d104768a1214"
LED_CHAR_UUID = "19b10002-e8f2-537e-4f6c-d104768a1214"
OTA_SERVICE_UUID = "19b10000-e8f2-537e-4f6c-d104768a1215"
OTA_CHAR_UUID = "19b10002-e8f2-537e-4f6c-d104768a1215"

class BLEDelegate:
    def __init__(self, ui_elements):
        self.ui_elements = ui_elements
        self.discovered_devices = []
        self.device = None
        self.led_characteristic = None
        self.ota_characteristic = None

    def did_discover_peripheral(self, p):
        self.discovered_devices.append(p)
        self.ui_elements['device_list'].reload()

    def did_connect_peripheral(self, p):
        console.hud_alert('Connected')
        self.device = p
        p.discover_services()

    def did_fail_to_connect_peripheral(self, p, error):
        console.hud_alert('Failed to connect')

    def did_disconnect_peripheral(self, p, error):
        console.hud_alert('Disconnected')
        self.ui_elements['controls'].hidden = True

    def did_discover_services(self, p, error):
        for s in p.services:
            if s.uuid == SERVICE_UUID:
                p.discover_characteristics(s)
            elif s.uuid == OTA_SERVICE_UUID:
                p.discover_characteristics(s)

    def did_discover_characteristics(self, s, error):
        for c in s.characteristics:
            if c.uuid == LED_CHAR_UUID:
                self.led_characteristic = c
            elif c.uuid == OTA_CHAR_UUID:
                self.ota_characteristic = c
        self.ui_elements['controls'].hidden = False

    def device_selected(self, tableview, section, row):
        selected_device = self.discovered_devices[row]
        console.hud_alert('Connecting to selected device...')
        cb.connect_peripheral(selected_device)

    def send_command(self, command):
        if self.led_characteristic:
            self.device.write_characteristic_value(self.led_characteristic, command, True)

    def upload_firmware(self, file_path):
        if not self.ota_characteristic:
            console.hud_alert('OTA characteristic not found')
            return
        with open(file_path, 'rb') as f:
            firmware = f.read()
            chunk_size = 512
            for i in range(0, len(firmware), chunk_size):
                chunk = firmware[i:i+chunk_size]
                self.device.write_characteristic_value(self.ota_characteristic, chunk, True)
        console.hud_alert('Firmware upload complete')

def connect_action(sender):
    cb.set_central_delegate(BLEDelegate(ui_elements))
    cb.scan_for_peripherals()

def led_on_action(sender):
    ui_elements['ble_delegate'].send_command(b'\x01')

def led_off_action(sender):
    ui_elements['ble_delegate'].send_command(b'\x00')

def color_picker_action(sender):
    color = sender.value
    r, g, b = int(color[1:3], 16), int(color[3:5], 16), int(color[5:7], 16)
    ui_elements['ble_delegate'].send_command(bytes([67, r, g, b]))

def upload_firmware_action(sender):
    file_path = console.open_file()
    if file_path:
        ui_elements['ble_delegate'].upload_firmware(file_path)

ui_elements = {
    'controls': None,
    'ble_delegate': None
}

def main():
    v = ui.View()
    v.name = 'ESP32 LED Control & OTA'
    v.background_color = 'white'
    
    status_label = ui.Label(frame=(10, 10, 300, 40), text='Status: Disconnected')
    v.add_subview(status_label)
    
    connect_button = ui.Button(frame=(10, 60, 100, 40), title='Connect')
    connect_button.action = connect_action
    v.add_subview(connect_button)
    
    device_list = ui.TableView(frame=(0, 110, 320, 200))
    device_list.data_source = ui.ListDataSource([])
    device_list.delegate = ui.ListDataSource([])
    device_list.data_source.items = self.discovered_devices
    device_list.delegate.tableview_did_select = self.device_selected
    v.add_subview(device_list)

    controls = ui.View(frame=(0, 320, 320, 400))
    controls.hidden = True
    
    led_on_button = ui.Button(frame=(10, 10, 100, 40), title='Turn LED On')
    led_on_button.action = led_on_action
    controls.add_subview(led_on_button)
    
    led_off_button = ui.Button(frame=(120, 10, 100, 40), title='Turn LED Off')
    led_off_button.action = led_off_action
    controls.add_subview(led_off_button)
    
    color_picker = ui.TextField(frame=(10, 60, 100, 40), placeholder='Color (e.g., #ff0000)')
    color_picker.action = color_picker_action
    controls.add_subview(color_picker)
    
    upload_firmware_button = ui.Button(frame=(10, 110, 150, 40), title='Upload Firmware')
    upload_firmware_button.action = upload_firmware_action
    controls.add_subview(upload_firmware_button)
    
    v.add_subview(controls)
    
    ui_elements['controls'] = controls
    ui_elements['device_list'] = device_list
    ui_elements['ble_delegate'] = BLEDelegate(ui_elements)
    
    v.present('sheet')

if __name__ == '__main__':
    main()