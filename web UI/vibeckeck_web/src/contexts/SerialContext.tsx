// src/contexts/SerialContext.tsx

import React, { createContext, useState, useContext, useCallback, ReactNode, useRef, useEffect } from 'react';
import { SerialService } from '../services/SerialService';
import { parseSerialData, DataPoint } from '../utils/dataParser';

interface SerialContextType {
  isConnected: boolean;
  connect: (baudRate: number) => Promise<void>;
  disconnect: () => Promise<void>;
  sendData: (data: string) => Promise<void>;
  parsedData: DataPoint[];
}

const SerialContext = createContext<SerialContextType | undefined>(undefined);

interface SerialProviderProps {
  children: ReactNode;
}

const UPDATE_INTERVAL = 100; // Update every 100ms

export const SerialProvider: React.FC<SerialProviderProps> = ({ children }) => {
  const [isConnected, setIsConnected] = useState(false);
  const [parsedData, setParsedData] = useState<DataPoint[]>([]);
  const serialService = useRef(new SerialService());
  const dataBuffer = useRef<DataPoint[]>([]);

  useEffect(() => {
    const intervalId = setInterval(() => {
      if (dataBuffer.current.length > 0) {
        setParsedData(prevData => [...prevData, ...dataBuffer.current]);
        dataBuffer.current = [];
      }
    }, UPDATE_INTERVAL);

    return () => clearInterval(intervalId);
  }, []);

  const connect = useCallback(async (baudRate: number) => {
    await serialService.current.connect(baudRate);
    setIsConnected(true);
    serialService.current.readData(
      (data) => {
        const newDataPoints = parseSerialData(data);
        dataBuffer.current.push(...newDataPoints);
      },
      (data) => {
        console.log('Received serial data:', data);
      }
    );
  }, []);

  const disconnect = useCallback(async () => {
    await serialService.current.disconnect();
    setIsConnected(false);
  }, []);

  const sendData = useCallback(async (data: string) => {
    // Implement send data functionality
  }, []);

  const value = {
    isConnected,
    connect,
    disconnect,
    sendData,
    parsedData,
  };

  return <SerialContext.Provider value={value}>{children}</SerialContext.Provider>;
};

export const useSerial = () => {
  const context = useContext(SerialContext);
  if (context === undefined) {
    throw new Error('useSerial must be used within a SerialProvider');
  }
  return context;
};