// features/serial/serialSlice.ts
import { createSlice, PayloadAction } from '@reduxjs/toolkit';
import { SerialState } from '../../types/serial';
import { RootState } from '../../types/rootState';

  const initialState: SerialState = {
    isSupported: 'serial' in navigator,
    isConnected: false,
    error: null,
  };

const serialSlice = createSlice({
  name: 'serial',
  initialState,
  reducers: {
    setConnected(state, action: PayloadAction<boolean>) {
      state.isConnected = action.payload;
    },
    setError(state, action: PayloadAction<string | null>) {
      state.error = action.payload;
    },
  },
});

export const selectIsSerialSupported = (state: RootState) => state.serial.isSupported;
export const { setConnected, setError } = serialSlice.actions;
export default serialSlice.reducer;

