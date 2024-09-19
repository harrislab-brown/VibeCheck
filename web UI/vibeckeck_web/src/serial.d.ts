// src/serial.d.ts

interface SerialPort {
    open(options: SerialOptions): Promise<void>;
    close(): Promise<void>;
    readable: ReadableStream;
    writable: WritableStream;
  }
  
  interface SerialOptions {
    baudRate: number;
    dataBits?: number;
    stopBits?: number;
    parity?: 'none' | 'even' | 'odd';
    bufferSize?: number;
    flowControl?: 'none' | 'hardware';
  }
  
  interface NavigatorSerial {
    requestPort(options?: SerialPortRequestOptions): Promise<SerialPort>;
    getPorts(): Promise<SerialPort[]>;
  }
  
  interface SerialPortRequestOptions {
    filters?: SerialPortFilter[];
  }
  
  interface SerialPortFilter {
    usbVendorId?: number;
    usbProductId?: number;
  }
  
  interface Navigator {
    serial: NavigatorSerial;
  }