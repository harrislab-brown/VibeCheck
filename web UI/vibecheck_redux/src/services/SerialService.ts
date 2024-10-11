// services/SerialService.ts
export class SerialService {
    private port: SerialPort | null = null;
  
    async connect(baudRate: number): Promise<void> {
      try {
        const port = await navigator.serial.requestPort();
        await port.open({ baudRate });
        this.port = port;
      } catch (error) {
        throw new Error('Failed to connect to serial port');
      }
    }
  
    async disconnect(): Promise<void> {
      if (this.port) {
        await this.port.close();
        this.port = null;
      }
    }
  
    // Add methods for reading/writing data as needed
  }