// features/data/dataSlice.ts
import { createSlice, PayloadAction } from '@reduxjs/toolkit';
import { DataPoint } from '../../utils/dataParser';

interface DataState {
  points: DataPoint[];
}

const initialState: DataState = {
  points: [],
};

const dataSlice = createSlice({
  name: 'data',
  initialState,
  reducers: {
    addDataPoints(state, action: PayloadAction<DataPoint[]>) {
      state.points.push(...action.payload);
    },
    clearData(state) {
      state.points = [];
    },
  },
});

export const { addDataPoints, clearData } = dataSlice.actions;
export default dataSlice.reducer;