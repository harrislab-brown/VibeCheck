// src/redux/rootReducer.ts
import { combineReducers } from '@reduxjs/toolkit';
import serialReducer from '../features/serialSlice';
import systemStatusReducer from '../features/systemStatusSlice'
import { serialOutputReducer } from '../features/serialOutputSlice';

const rootReducer = combineReducers({
  serial: serialReducer,
  systemStatus: systemStatusReducer,
  serialOutput: serialOutputReducer,
  // ... other reducers
});

export type RootState = ReturnType<typeof rootReducer>;

export default rootReducer;