// src/services/SerialService.ts
import store, { AppDispatch } from '../redux/store';
import { setConnected, setDisconnected, setError } from '../features/serialSlice';
import { UnknownAction } from 'redux';

// Define the action type
type SerialDataReceivedAction = {
    type: 'SERIAL_DATA_RECEIVED';
    payload: string;
  };

export class SerialService {
    private port: SerialPort | null = null;
    private reader: ReadableStreamDefaultReader<Uint8Array> | null = null;
    private isReading: boolean = false;
    private dispatch: AppDispatch | null = null;
  
    setDispatch(dispatch: AppDispatch) {
      this.dispatch = dispatch;
    }

    private dispatchAction(action: UnknownAction | SerialDataReceivedAction | ((dispatch: AppDispatch) => void)) {
        if (this.dispatch) {
            if (typeof action === 'function') {
                this.dispatch(action);
            } else {
                this.dispatch(action);
            }
        } else {
            console.error('Dispatch function not set in SerialService');
        }
    }

    async connect(baudRate: number): Promise<void> {
        try {
            const selectedPort = await navigator.serial.requestPort();
            await selectedPort.open({ baudRate });
            this.port = selectedPort;
            this.dispatchAction(setConnected());
            console.log('Connected to serial port');
            this.readData(); // Start reading data after successful connection
        } catch (error) {
            console.error('Error connecting to serial port:', error);
            this.dispatchAction(setError((error as Error).message));
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
                this.dispatchAction(setError((error as Error).message));
            } finally {
                this.port = null;
                this.dispatchAction(setDisconnected());
                console.log('Disconnected from serial port');
            }
        }
    }

    private async readData(): Promise<void> {
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
                    const action: SerialDataReceivedAction = {
                        type: 'SERIAL_DATA_RECEIVED',
                        payload: decodedValue
                    };
                    this.dispatchAction(action);
                }
            } catch (error) {
                console.error('Error reading data:', error);
                this.dispatchAction(setError((error as Error).message));
            } finally {
                this.reader.releaseLock();
            }
        }
    }
    
    async sendData(message: string): Promise<void> {
        if (!this.port || !this.port.writable) {
            throw new Error('Not connected to a serial port');
        }

        const writer = this.port.writable.getWriter();
        try {
            const data = new TextEncoder().encode(message + '\n');
            await writer.write(data);
        } finally {
            writer.releaseLock();
        }
    }

    isConnected(): boolean {
        return this.port !== null;
    }
}
