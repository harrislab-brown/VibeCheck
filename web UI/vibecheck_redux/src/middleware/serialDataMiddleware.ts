// src/middleware/serialDataMiddleware.ts

import { Middleware, MiddlewareAPI, Dispatch, UnknownAction } from 'redux';
import { parseSerialData, Message, DataPoint } from '../utils/dataParser';
import { appendStatusMessage } from '../features/systemStatusSlice';
import { receiveData } from '../features/serialSlice';

type SerialDataReceivedAction = {
  type: 'SERIAL_DATA_RECEIVED';
  payload: string;
};

function isSerialDataReceivedAction(action: UnknownAction): action is SerialDataReceivedAction {
  return action.type === 'SERIAL_DATA_RECEIVED' && typeof (action as any).payload === 'string';
}

const serialDataMiddleware: Middleware = 
  (store: MiddlewareAPI) => 
  (next: Dispatch) => 
  (action: UnknownAction) => {
    if (isSerialDataReceivedAction(action)) {
      const serialData: string = action.payload;
      const parsedMessages: Message[] = parseSerialData(serialData);

      parsedMessages.forEach(message => {
        switch (message.type) {
          case 'data':
            console.log('Received data points:', message.data);
            if (Array.isArray(message.data)) {
              // Assuming receiveData can handle an array of DataPoints
              store.dispatch(receiveData(message.data as DataPoint[]));
            }
            break;
          case 'event':
            console.log('Event message:', message.data);
            if (typeof message.data === 'string') {
              store.dispatch(appendStatusMessage(`Event: ${message.data}`));
            }
            break;
          case 'ack':
            console.log('Acknowledgement:', message.data);
            // Handle acknowledgements if needed
            break;
          case 'error':
            console.log('Error message:', message.data);
            if (typeof message.data === 'string') {
              store.dispatch(appendStatusMessage(`Error: ${message.data}`));
            }
            break;
        }
      });
    }

    return next(action);
  };

export default serialDataMiddleware;