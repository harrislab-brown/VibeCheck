// src/components/SerialPortConnect.tsx

import React, { useState } from 'react';
import { useSerial } from '../contexts/SerialContext';
import '../styles/SerialPortConnect.css';

const SerialPortConnect: React.FC = () => {
    const { isConnected, connect, disconnect } = useSerial();
    const [baudRate] = useState(115200);

    const toggleSerialConnection = async () => {
        if (isConnected) {
            await disconnect();
        } else {
            await connect(baudRate);
        }
    };

    return (
        <div className="serial-port-connect">
            <button onClick={toggleSerialConnection}>
                {isConnected ? 'Disconnect' : 'Connect to Serial Port'}
            </button>
        </div>
    );
};

export default SerialPortConnect;