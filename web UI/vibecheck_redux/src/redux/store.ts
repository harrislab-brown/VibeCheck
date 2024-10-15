// src/redux/store.ts

import { configureStore } from '@reduxjs/toolkit';
import rootReducer from './rootReducer';
import serialDataMiddleware from '../middleware/serialDataMiddleware';
import { SerialService } from '../services/SerialService';
import serialOutputMiddleware from '../middleware/serialOutputMiddleware';

const serialService = new SerialService();

const store = configureStore({
  reducer: rootReducer,
  middleware: (getDefaultMiddleware) =>
    getDefaultMiddleware({
      thunk: {
        extraArgument: { serialService },
      },
    }).concat(serialDataMiddleware, serialOutputMiddleware),
});

// Set the dispatch function on the SerialService after store creation
serialService.setDispatch(store.dispatch);

export type RootState = ReturnType<typeof store.getState>;
export type AppDispatch = typeof store.dispatch;

export default store;