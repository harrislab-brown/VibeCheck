import React, { createContext, useContext, useCallback, ReactNode, useRef, useEffect, useReducer, useMemo } from 'react';
import { SerialService } from '../services/SerialService';
import { parseSerialData, DataPoint, Message } from '../utils/dataParser';
import StatusDisplay from '../components/StatusDIsplay';

interface SerialState {
  isConnected: boolean;
  parsedData: DataPoint[];
  event: Record<string, any>;
  lastError: string | null;
  statusMessage: string;
}

type SerialAction =
  | { type: 'SET_CONNECTED'; payload: boolean }
  | { type: 'SET_PARSED_DATA'; payload: DataPoint[] }
  | { type: 'SET_EVENT'; payload: Record<string, any> }
  | { type: 'SET_ERROR'; payload: string | null }
  | { type: 'SET_STATUS_MESSAGE'; payload: string };

const initialState: SerialState = {
  isConnected: false,
  parsedData: [],
  event: {},
  lastError: null,
  statusMessage: 'No status message',
};

function serialReducer(state: SerialState, action: SerialAction): SerialState {
  switch (action.type) {
    case 'SET_CONNECTED':
      return { ...state, isConnected: action.payload };
    case 'SET_PARSED_DATA':
      return { ...state, parsedData: [...state.parsedData, ...action.payload] };
    case 'SET_EVENT':
      return { ...state, event: { ...state.event, ...action.payload } };
    case 'SET_ERROR':
      return { ...state, lastError: action.payload };
    case 'SET_STATUS_MESSAGE':
      return { ...state, statusMessage: action.payload };
    default:
      return state;
  }
}

interface SerialContextType extends SerialState {
  connect: (baudRate: number) => Promise<void>;
  disconnect: () => Promise<void>;
  sendData: (data: string) => Promise<void>;
}

const SerialContext = createContext<SerialContextType | undefined>(undefined);

interface SerialProviderProps {
  children: ReactNode;
}

const UPDATE_INTERVAL = 16; // Update every 16ms

export const SerialProvider: React.FC<SerialProviderProps> = ({ children }) => {
  const [state, dispatch] = useReducer(serialReducer, initialState);
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
              dispatch({ type: 'SET_EVENT', payload: message.data });
              dispatch({ type: 'SET_STATUS_MESSAGE', payload: `EVENT: ${JSON.stringify(message.data)}` });
              console.log('New event status:', `EVENT: ${JSON.stringify(message.data)}`);
              break;
            case 'error':
              dispatch({ type: 'SET_ERROR', payload: message.data });
              dispatch({ type: 'SET_STATUS_MESSAGE', payload: `ERROR: ${message.data}` });
              console.log('New error status:', `ERROR: ${message.data}`);
              break;
          }
        });
        if (newDataPoints.length > 0) {
          dispatch({ type: 'SET_PARSED_DATA', payload: newDataPoints });
        }
        dataBuffer.current = [];
      }
    }, UPDATE_INTERVAL);

    return () => clearInterval(intervalId);
  }, []);

  const connect = useCallback(async (baudRate: number) => {
    try {
      await serialService.current.connect(baudRate);
      dispatch({ type: 'SET_CONNECTED', payload: true });
      dispatch({ type: 'SET_STATUS_MESSAGE', payload: 'Connected to serial port' });
      console.log('New connect status: Connected to serial port');
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
      dispatch({ type: 'SET_CONNECTED', payload: false });
      dispatch({ type: 'SET_STATUS_MESSAGE', payload: `Failed to connect: ${error}` });
      console.log('New error status:', `Failed to connect: ${error}`);
    }
  }, []);

  const disconnect = useCallback(async () => {
    await serialService.current.disconnect();
    dispatch({ type: 'SET_CONNECTED', payload: false });
    dispatch({ type: 'SET_STATUS_MESSAGE', payload: 'Disconnected from serial port' });
    console.log('New disconnect status: Disconnected from serial port');
    dataBuffer.current = [];
  }, []);

  const sendData = useCallback(async (data: string) => {
    // Implement send data functionality
  }, []);

  const value = useMemo(() => ({
    ...state,
    connect,
    disconnect,
    sendData,
  }), [state, connect, disconnect, sendData]);

  return (
    <SerialContext.Provider value={value}>
      <div>
        <StatusDisplay message={state.statusMessage} />
        {children}
      </div>
    </SerialContext.Provider>
  );
};

export const useSerial = () => {
  const context = useContext(SerialContext);
  if (context === undefined) {
    throw new Error('useSerial must be used within a SerialProvider');
  }
  return context;
};