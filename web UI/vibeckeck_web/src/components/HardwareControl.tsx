// ./components/HardwareControl.tsx

import React, { useState } from 'react';
import '../styles/HardwareControl.css';

const HardwareControl: React.FC = () => {
  const [toggleStates, setToggleStates] = useState([false, false, false]);
  const [isPlaying, setIsPlaying] = useState(false);
  const [isRecording, setIsRecording] = useState(false);

  const handleToggle = (index: number) => {
    setToggleStates(prev => {
      const newState = [...prev];
      newState[index] = !newState[index];
      return newState;
    });
  };

  const handlePlayPause = () => {
    setIsPlaying(prev => !prev);
  };

  const handleRecord = () => {
    setIsRecording(prev => !prev);
  };

  return (
    <div className="hardware-control">
      <div className="toggle-switches">
        {toggleStates.map((state, index) => (
          <label key={index} className="switch">
            <input
              type="checkbox"
              checked={state}
              onChange={() => handleToggle(index)}
            />
            <span className="slider"></span>
          </label>
        ))}
      </div>
      <div className="buttons">
        <button className="play-pause" onClick={handlePlayPause}>
          {isPlaying ? '❚❚' : '▶'}
        </button>
        <button 
          className={`record ${isRecording ? 'recording' : ''}`} 
          onClick={handleRecord}
        >
          ●
        </button>
      </div>
    </div>
  );
};

export default HardwareControl;