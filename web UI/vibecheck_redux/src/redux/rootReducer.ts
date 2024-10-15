// src/redux/rootReducer.ts
import { combineReducers } from '@reduxjs/toolkit';
import serialReducer from '../features/serialSlice';
import systemStatusReducer from '../features/systemStatusSlice'

const rootReducer = combineReducers({
  serial: serialReducer,
  systemStatus: systemStatusReducer,
  // ... other reducers
});

export type RootState = ReturnType<typeof rootReducer>;

export default rootReducer;