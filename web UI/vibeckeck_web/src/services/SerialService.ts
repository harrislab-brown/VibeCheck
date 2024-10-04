// src/services/serialService.ts

export class SerialService {
  private port: SerialPort | null = null;
  private reader: ReadableStreamDefaultReader | null = null;

  async connect(baudRate: number): Promise<void> {
    try {
      const selectedPort = await navigator.serial.requestPort();
      await selectedPort.open({ baudRate });
      this.port = selectedPort;
      console.log('Connected to serial port');
    } catch (error) {
      console.error('Error connecting to serial port:', error);
      throw error;
    }
  }

  async disconnect(): Promise<void> {
    if (this.reader) {
      await this.reader.cancel();
      this.reader = null;
    }
    if (this.port) {
      await this.port.close();
      this.port = null;
      console.log('Disconnected from serial port');
    }
  }

  async readData(onDataReceived: (data: string) => void, onLog: (data: string) => void): Promise<void> {
    if (!this.port) {
      throw new Error('Not connected to a serial port');
    }

    while (this.port.readable) {
      this.reader = this.port.readable.getReader();
      try {
        while (true) {
          const { value, done } = await this.reader.read();
          if (done) {
            break;
          }
          const decodedValue = new TextDecoder().decode(value);
          onDataReceived(decodedValue);
          onLog(decodedValue); // Log the received data
        }
      } catch (error) {
        console.error('Error reading data:', error);
      } finally {
        this.reader.releaseLock();
      }
    }
  }
}