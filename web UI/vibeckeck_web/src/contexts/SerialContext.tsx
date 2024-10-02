// src/contexts/SerialContext.tsx

import React, { createContext, useState, useContext, useCallback, ReactNode } from 'react';
import { SerialService } from '../services/SerialService';

interface SerialContextType {
  isConnected: boolean;
  connect: (baudRate: number) => Promise<void>;
  disconnect: () => Promise<void>;
  sendData: (data: string) => Promise<void>;
  receivedData: string;
}

const SerialContext = createContext<SerialContextType | undefined>(undefined);

interface SerialProviderProps {
  children: ReactNode;
}

export const SerialProvider: React.FC<SerialProviderProps> = ({ children }) => {
  const [isConnected, setIsConnected] = useState(false);
  const [receivedData, setReceivedData] = useState('');
  const serialService = new SerialService();

  const connect = useCallback(async (baudRate: number) => {
    await serialService.connect(baudRate);
    setIsConnected(true);
    serialService.readData((data) => {
      setReceivedData(prev => prev + data);
    });
  }, []);

  const disconnect = useCallback(async () => {
    await serialService.disconnect();
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
    receivedData,
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