import { RootState } from '../../store';

export const selectIsConnected = (state: RootState) => state.serial.isConnected;

export class SerialService {
    private static isSupported: boolean;
  
    constructor() {
      SerialService.checkSupport();
    }
  
    private static checkSupport() {
      SerialService.isSupported = 'serial' in navigator;
      if (!SerialService.isSupported) {
        console.error('Web Serial API is not supported in this browser');
      }
    }
  
    static isSerialSupported(): boolean {
      return SerialService.isSupported;
    }
  
    async connect(baudRate: number): Promise<void> {
      if (!SerialService.isSupported) {
        throw new Error('Web Serial API is not supported in this browser');
      }
      // ... rest of connect method
    }
  
    // ... other methods
  }