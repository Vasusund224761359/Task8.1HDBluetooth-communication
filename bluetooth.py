import RPi.GPIO as GPIO
from bluepy.btle import Peripheral, UUID
import time

# Setup
LED_PIN = 18
GPIO.setmode(GPIO.BCM)
GPIO.setup(LED_PIN, GPIO.OUT)
pwm = GPIO.PWM(LED_PIN, 1000)  # 1 kHz frequency
pwm.start(0)  # Start with LED off

# Connect to Arduino
arduino_mac = "EC:62:60:8F:5A:BA"  # replace with your Arduino's MAC
p = Peripheral(arduino_mac)

# Get characteristic
service_uuid = UUID("19B10000-E8F2-537E-4F6C-D104768A1214")
char_uuid = UUID("19B10001-E8F2-537E-4F6C-D104768A1214")
service = p.getServiceByUUID(service_uuid)
char = service.getCharacteristics(char_uuid)[0]

try:
    while True:
        value = char.read()[0]  # Read 1 byte light level
        print(f"Light intensity: {value} lux")

        # Map light value (0-255) to PWM duty cycle (0-100%)
        brightness = min(int(value / 255 * 100), 100)
        pwm.ChangeDutyCycle(brightness)
        print(f"LED brightness: {brightness}%")

        time.sleep(0.5)

except KeyboardInterrupt:
    pass
finally:
    pwm.stop()
    GPIO.cleanup()
    p.disconnect()