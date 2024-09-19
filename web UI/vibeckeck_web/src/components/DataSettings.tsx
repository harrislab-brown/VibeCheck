// ./components/DataSettings.tsx

import React, { useState } from 'react';
import '../styles/DataSettings.css';

const DataSettings: React.FC = () => {
    const [saveLocation, setSaveLocation] = useState<string>('');
    const [fileFormat, setFileFormat] = useState<string>('yyyy-MM-dd');
    const [sessionName, setSessionName] = useState<string>('');

    const handleFileSelect = async () => {
        try {
            // @ts-ignore (fileHandle is not in the types yet)
            const fileHandle = await window.showDirectoryPicker();
            setSaveLocation(fileHandle.name);
        } catch (error) {
            console.error('Error selecting directory:', error);
        }
    };

    const handleFormatChange = (event: React.ChangeEvent<HTMLSelectElement>) => {
        setFileFormat(event.target.value);
    };

    const handleSessionNameChange = (event: React.ChangeEvent<HTMLInputElement>) => {
        setSessionName(event.target.value);
    };

    return (
        <div className="data-settings">
            <div className="setting">
                <label htmlFor="saveLocation">Save Location:</label>
                <button onClick={handleFileSelect}>{saveLocation || 'Select Folder'}</button>
            </div>
      
            <div className="setting">
                <label htmlFor="sessionName">Session Name:</label>
                <input
                    type="text"
                    id="sessionName"
                    value={sessionName}
                    onChange={handleSessionNameChange}
                    placeholder="Enter session name"
                />
            </div>
            <div className="setting">
                <label htmlFor="fileFormat">Naming Format:</label>
                <select id="fileFormat" value={fileFormat} onChange={handleFormatChange}>
                    <option value="hh-mm-ss">hh-mm-ss</option>
                    <option value="MM-dd-hh-mm-ss">MM-dd-hh-mm-ss</option>
                    <option value="yyyy-MM-dd-hh-mm-ss">yyyy-MM-dd-hh-mm-ss</option>
                </select>
            </div>
        </div>
    );
};

export default DataSettings;