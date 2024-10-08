// src/services/serialService.ts

export class SerialService {
  private port: SerialPort | null = null;
  private reader: ReadableStreamDefaultReader | null = null;
  private isReading: boolean = false;
  private buffer: string = '';

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
    this.isReading = false;
    
    if (this.reader) {
      try {
        await this.reader.cancel();
      } catch (error) {
        console.error('Error cancelling reader:', error);
      } finally {
        this.reader.releaseLock();
        this.reader = null;
      }
    }
    
    if (this.port) {
      try {
        await this.port.close();
      } catch (error) {
        console.error('Error closing port:', error);
      } finally {
        this.port = null;
        console.log('Disconnected from serial port');
      }
    }
  }

  async readData(onDataReceived: (data: string) => void, onLog: (data: string) => void): Promise<void> {
    if (!this.port) {
      throw new Error('Not connected to a serial port');
    }

    this.isReading = true;

    while (this.port.readable && this.isReading) {
      this.reader = this.port.readable.getReader();
      try {
        while (this.isReading) {
          const { value, done } = await this.reader.read();
          if (done) {
            break;
          }
          const decodedValue = new TextDecoder().decode(value);
          this.buffer += decodedValue;
          onLog(decodedValue); // Log the received data

          // Process complete messages
          let newlineIndex;
          while ((newlineIndex = this.buffer.indexOf('\n')) !== -1) {
            const completeMessage = this.buffer.slice(0, newlineIndex + 1);
            onDataReceived(completeMessage);
            this.buffer = this.buffer.slice(newlineIndex + 1);
          }
        }
      } catch (error) {
        console.error('Error reading data:', error);
      } finally {
        this.reader.releaseLock();
      }
    }
  }

  isConnected(): boolean {
    return this.port !== null;
  }
}