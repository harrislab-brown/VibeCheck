// src/utils/appInitialization.ts
import { AppDispatch } from '../redux/store';
import { checkBrowserCompatibility } from '../features/serialSlice';

export const initializeApp = (dispatch: AppDispatch) => {
  // Check browser compatibility
  console.log('check broser compatibility');
  dispatch(checkBrowserCompatibility());
};