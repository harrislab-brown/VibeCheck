import React, { useEffect } from 'react';
import { useAppSelector, useAppDispatch } from '../hooks/useReduxHooks';
import { selectIsSerialSupported, setError } from '../features/serial/serialSlice';
import { selectIsConnected } from '../features/serial/serialSelector';

const Layout: React.FC = () => {
  const dispatch = useAppDispatch();
  const isSerialSupported = useAppSelector(selectIsSerialSupported);
  const isConnected = useAppSelector(selectIsConnected);

  useEffect(() => {
    if (!isSerialSupported) {
      dispatch(setError('Web Serial API is not supported in this browser'));
    }
  }, [isSerialSupported, dispatch]);

  return (
    <div className="layout">
      {!isSerialSupported && (
        <div className="error-banner">
          Web Serial API is not supported in this browser. Some features may not work.
        </div>
      )}
      <div>
        Connection status: {isConnected ? 'Connected' : 'Disconnected'}
      </div>
      {/* Rest of your layout */}
    </div>
  );
};

export default Layout;