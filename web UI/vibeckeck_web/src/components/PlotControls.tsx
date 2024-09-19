// ./components/PlotControls.tsx

import React, { useState } from 'react';
import '../styles/PlotControls.css';

interface PlotControlsProps {
  onUpdateControls: (controls: PlotControlsState) => void;
}

export interface PlotControlsState {
  minBound: number;
  maxBound: number;
  scrollSpeed: number;
}

const PlotControls: React.FC<PlotControlsProps> = ({ onUpdateControls }) => {
  const [controls, setControls] = useState<PlotControlsState>({
    minBound: 0,
    maxBound: 100,
    scrollSpeed: 50,
  });

  const handleInputChange = (e: React.ChangeEvent<HTMLInputElement>) => {
    const { name, value } = e.target;
    setControls(prev => {
      const newControls = { ...prev, [name]: parseFloat(value) };
      onUpdateControls(newControls);
      return newControls;
    });
  };

  return (
    <div className="plot-controls">
      <div className="control-group">
        <label htmlFor="minBound">Y Min:</label>
        <input
          type="number"
          id="minBound"
          name="minBound"
          value={controls.minBound}
          onChange={handleInputChange}
        />
      </div>
      <div className="control-group">
        <label htmlFor="maxBound">Y Max:</label>
        <input
          type="number"
          id="maxBound"
          name="maxBound"
          value={controls.maxBound}
          onChange={handleInputChange}
        />
      </div>
      <div className="control-group">
        <label htmlFor="scrollSpeed">Scroll Speed px/s:</label>
        <input
          type="number"
          id="scrollSpeed"
          name="scrollSpeed"
          value={controls.scrollSpeed}
          onChange={handleInputChange}
        />
      </div>
    </div>
  );
};

export default PlotControls;