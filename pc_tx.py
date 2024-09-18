import serial
import time
import threading

message = "Finance Minister Arun Jaitley Tuesday hit out at former RBI governor Raghuram Rajan for\npredicting that the next banking crisis would be triggered by MSME lending, saying postmortem is\neasier than taking action when it was required. Rajan, who had as the chief economist at IMF\nwarned of impending financial crisis of 2008, in a note to a parliamentary committee warned\nagainst ambitious credit targets and loan waivers, saying that they could be the sources of next\nbanking crisis. Government should focus on sources of the next crisis, not just the last one.\n\tIn particular, government should refrain from setting ambitious credit targets or waiving loans.\nCredit targets are sometimes achieved by abandoning appropriate due diligence, creating the\nenvironment for future NPAs, Rajan said in the note. Both MUDRA loans as well as the Kisan\nCredit Card, while popular, have to be examined more closely for potential credit risk. Rajan, who\nwas RBI governor for three years till September 2016, is currently.#"
            

def handle_serial_data(ser):
    while True:
        if ser.in_waiting > 0:  # Check if there's data to read
            data=0x0;
            data = ser.readline().decode('utf-8').strip()
            print(f"{data}")
            
            # Process the received data here
            if data[-1] == '#':  # Break the loop if "#" is received
             break
             
    ser.close()  # Close the serial port when done
    print("done")

def main():
 
 arduino = serial.Serial('COM3', 9600, timeout=1)  # Replace COM3 with the correct port
 time.sleep(2)  # Give time for the Arduino to reset
 # Create and start a thread to listen for incoming data
 serial_thread = threading.Thread(target=handle_serial_data, args=(arduino,))
 serial_thread.daemon = True  # Thread will exit when main program exits
 serial_thread.start()
 arduino.write(message.encode())  # Send data to Arduino (send the character 'H')
 print("done")
 #arduino.close()  # Close the serial connection
 while True:
   pass  
 
if __name__ == "__main__":
    main()
