// src/components/SerialPortConnect.tsx

import React, { useState } from 'react';
import { useSerial } from '../contexts/SerialContext';
import '../styles/SerialPortConnect.css';

const SerialPortConnect: React.FC = () => {
    const { isConnected, connect, disconnect } = useSerial();
    const [baudRate, setBaudRate] = useState(115200);

    const toggleSerialConnection = async () => {
        if (isConnected) {
            await disconnect();
        } else {
            await connect(baudRate);
        }
    };

    const handleBaudRateChange = (event: React.ChangeEvent<HTMLSelectElement>) => {
        setBaudRate(Number(event.target.value));
    };

    return (
        <div className="serial-port-connect">
            <button onClick={toggleSerialConnection}>
                {isConnected ? 'Disconnect' : 'Connect to Serial Port'}
            </button>
            <div className="baud-rate-container">
                <label htmlFor="baudRate">Baud Rate:</label>
                <select 
                    id="baudRate" 
                    value={baudRate} 
                    onChange={handleBaudRateChange}
                    disabled={isConnected}
                >
                    <option value="9600">9600</option>
                    <option value="19200">19200</option>
                    <option value="38400">38400</option>
                    <option value="57600">57600</option>
                    <option value="115200">115200</option>
                </select>
            </div>
        </div>
    );
};

export default SerialPortConnect;