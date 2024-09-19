// ./components/TabbedInterface.tsx

import React, { useState } from 'react';
import '../styles/TabbedInterface.css';

const TabbedInterface: React.FC = () => {
  const [activeTab, setActiveTab] = useState(1);
  const [inputValues, setInputValues] = useState<{ [key: number]: string }>({
    1: '', 2: '', 3: '', 4: '', 5: ''
  });

  const handleTabClick = (tabNumber: number) => {
    setActiveTab(tabNumber);
  };

  const handleInputChange = (tabNumber: number, value: string) => {
    setInputValues(prev => ({ ...prev, [tabNumber]: value }));
  };

  const handleButtonClick = (tabNumber: number) => {
    console.log(`Button clicked in Tab ${tabNumber}. Input value: ${inputValues[tabNumber]}`);
    // Add your button click logic here
  };

  return (
    <div className="tabbed-interface">
      <div className="tab-headers">
        {[1, 2, 3, 4, 5].map(tabNumber => (
          <button
            key={tabNumber}
            className={`tab-header ${activeTab === tabNumber ? 'active' : ''}`}
            onClick={() => handleTabClick(tabNumber)}
          >
            Tab {tabNumber}
          </button>
        ))}
      </div>
      <div className="tab-content">
        {[1, 2, 3, 4, 5].map(tabNumber => (
          <div
            key={tabNumber}
            className={`tab-pane ${activeTab === tabNumber ? 'active' : ''}`}
          >
            <input
              type="text"
              value={inputValues[tabNumber]}
              onChange={(e) => handleInputChange(tabNumber, e.target.value)}
              placeholder={`Enter text for Tab ${tabNumber}`}
            />
            <button onClick={() => handleButtonClick(tabNumber)}>
              Button {tabNumber}
            </button>
          </div>
        ))}
      </div>
    </div>
  );
};

export default TabbedInterface;