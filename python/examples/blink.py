import mraa
import time

print(mraa.getVersion())
# Initialize the GPIO 13.
led = mraa.Gpio(13)
# Set the initialized GPIO as an output.
led.dir(mraa.DIR_OUT)

while True:
    # Set the GPIO 13 high.
    led.write(1)
    # Wait for 0.5 second.
    time.sleep(0.5)
    # Set the GPIO 13 low.
    led.write(0)
    # Wait for 0.5 second.
    time.sleep(0.5)
