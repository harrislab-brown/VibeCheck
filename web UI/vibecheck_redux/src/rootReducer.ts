// rootReducer.ts
import { combineReducers } from '@reduxjs/toolkit';
import serialReducer from './features/serial/serialSlice';
import dataReducer from './features/data/dataSlice';

const rootReducer = combineReducers({
  serial: serialReducer,
  data: dataReducer,
});

export default rootReducer;
