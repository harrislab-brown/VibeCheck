// ./components/StatusDisplay.tsx

import React, { useState, useEffect } from 'react';
import '../styles/StatusDisplay.css';

const StatusDisplay: React.FC = () => {
  const [messages, setMessages] = useState<string[]>([]);

  useEffect(() => {
    const exampleMessages = [
      "System initialized successfully.",
      "ERROR: Failed to connect to serial port.",
      "Data collection started.",
      "WARNING: Low battery detected.",
      "Calibration complete.",
      "ERROR: Sensor 2 not responding.",
      "Data saved to file: data_20230919_001.csv",
      "System entering standby mode.",
    ];

    let index = 0;
    const interval = setInterval(() => {
      if (index < exampleMessages.length) {
        setMessages(prev => [...prev, exampleMessages[index]]);
        index++;
      } else {
        clearInterval(interval);
      }
    }, 2000);

    return () => clearInterval(interval);
  }, []);

  const getMessageClass = (message: string): string => {
    if (typeof message !== 'string') return '';
    if (message.startsWith('ERROR')) return 'error';
    if (message.startsWith('WARNING')) return 'warning';
    return '';
  };

  const renderMessage = (message: string, index: number) => {
    if (typeof message !== 'string') {
      console.error(`Invalid message at index ${index}:`, message);
      return null;
    }
    return (
      <div key={index} className={`message ${getMessageClass(message)}`}>
        {message}
      </div>
    );
  };

  return (
    <div className="status-display">
      <h3>System Status</h3>
      <div className="message-box">
        {messages.map((message, index) => renderMessage(message, index))}
      </div>
    </div>
  );
};

export default StatusDisplay;