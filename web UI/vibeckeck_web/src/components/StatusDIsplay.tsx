// ./components/StatusDisplay.tsx

import React, { useEffect } from 'react';

interface StatusDisplayProps {
  message: string;
}

const StatusDisplay: React.FC<StatusDisplayProps> = ({ message }) => {
  useEffect(() => {
    console.log('StatusDisplay received new message:', message);
  }, [message]);

  return (
    <div className="bg-gray-100 p-4 rounded-lg shadow-md">
      <h3 className="text-lg font-semibold mb-2 text-center">System Status</h3>
      <div className={`text-center ${message.startsWith('ERROR') ? 'text-red-500' : message.startsWith('EVENT') ? 'text-blue-500' : 'text-green-500'}`}>
        {message || 'No status message'}
      </div>
    </div>
  );
};

export default StatusDisplay;