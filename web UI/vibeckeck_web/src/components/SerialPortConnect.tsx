import React, { useState } from 'react';
import '../styles/SerialPortConnect.css';

const SerialPortConnect: React.FC = () => {
    const [port, setPort] = useState<SerialPort | null>(null);
    const [baudRate, setBaudRate] = useState(115200);

    const toggleSerialConnection = async () => {
        if (port) {
            // Disconnect
            try {
                await port.close();
                setPort(null);
                console.log('Disconnected from serial port');
            } catch (error) {
                console.error('Error disconnecting from serial port:', error);
            }
        } else {
            // Connect
            try {
                const selectedPort = await navigator.serial.requestPort();
                await selectedPort.open({ baudRate: baudRate });
                setPort(selectedPort);
                console.log('Connected to serial port');
            } catch (error) {
                console.error('Error connecting to serial port:', error);
            }
        }
    };

    const handleBaudRateChange = (event: React.ChangeEvent<HTMLSelectElement>) => {
        setBaudRate(Number(event.target.value));
    };

    return (
        <div className="serial-port-connect">
            <button onClick={toggleSerialConnection}>
                {port ? 'Disconnect' : 'Connect to Serial Port'}
            </button>
            <div className="baud-rate-container">
                <label htmlFor="baudRate">Baud Rate:</label>
                <select 
                    id="baudRate" 
                    value={baudRate} 
                    onChange={handleBaudRateChange}
                    disabled={port !== null}
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