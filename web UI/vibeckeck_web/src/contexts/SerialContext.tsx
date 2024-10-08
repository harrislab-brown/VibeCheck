// src/contexts/SerialContext.tsx

import React, { createContext, useState, useContext, useCallback, ReactNode, useRef, useEffect } from 'react';
import { SerialService } from '../services/SerialService';
import { parseSerialData, DataPoint, Message } from '../utils/dataParser';

interface SerialContextType {
  isConnected: boolean;
  connect: (baudRate: number) => Promise<void>;
  disconnect: () => Promise<void>;
  sendData: (data: string) => Promise<void>;
  parsedData: DataPoint[];
  event: Record<string, any>;
  lastError: string | null;
}

const SerialContext = createContext<SerialContextType | undefined>(undefined);

interface SerialProviderProps {
  children: ReactNode;
}

const UPDATE_INTERVAL = 50; // Update every 100ms

export const SerialProvider: React.FC<SerialProviderProps> = ({ children }) => {
  const [isConnected, setIsConnected] = useState(false);
  const [parsedData, setParsedData] = useState<DataPoint[]>([]);
  const [event, setEvent] = useState<Record<string, any>>({});
  const [lastError, setLastError] = useState<string | null>(null);
  const serialService = useRef(new SerialService());
  const dataBuffer = useRef<Message[]>([]);

  useEffect(() => {
    const intervalId = setInterval(() => {
      if (dataBuffer.current.length > 0) {
        const newDataPoints: DataPoint[] = [];
        dataBuffer.current.forEach(message => {
          switch (message.type) {
            case 'data':
              newDataPoints.push(...message.data);
              break;
            case 'event':
              setEvent(prev => ({ ...prev, ...message.data }));
              break;
            case 'error':
              setLastError(message.data);
              break;
          }
        });
        if (newDataPoints.length > 0) {
          setParsedData(prevData => [...prevData, ...newDataPoints]);
        }
        dataBuffer.current = [];
      }
    }, UPDATE_INTERVAL);

    return () => clearInterval(intervalId);
  }, []);

  const connect = useCallback(async (baudRate: number) => {
    try {
      await serialService.current.connect(baudRate);
      setIsConnected(true);
      serialService.current.readData(
        (data) => {
          const messages = parseSerialData(data);
          dataBuffer.current.push(...messages);
        },
        (data) => {
          console.log('Received serial data:', data);
        }
      );
    } catch (error) {
      console.error('Failed to connect:', error);
      setIsConnected(false);
    }
  }, []);

  const disconnect = useCallback(async () => {
    await serialService.current.disconnect();
    setIsConnected(false);
    dataBuffer.current = [];
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
    event,
    lastError,
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