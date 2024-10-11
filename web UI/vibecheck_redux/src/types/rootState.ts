import { SerialState } from './serial';
import { DataState } from './data';

export interface RootState {
  serial: SerialState;
  data: DataState;
  // Add other slices as needed
}