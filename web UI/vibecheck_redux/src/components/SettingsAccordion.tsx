import React from 'react';
import CustomAccordion from './CustomAccordion';
import SensorSettings from './SensorSettings';
import SerialInput from './SerialInput';

const SettingsAccordion: React.FC = () => {
  const items = [
    { title: "Setting 1", content: <SensorSettings /> },
    { title: "Setting 2", content: <SerialInput /> },
    { 
      title: "Setting 3", 
      content: (
        <div>
          <h3 className="text-lg font-semibold mb-2">Setting 3</h3>
          <p>This is a simple text content for Setting 3.</p>
        </div>
      ) 
    },
    { 
      title: "Setting 4", 
      content: (
        <div>
          <h3 className="text-lg font-semibold mb-2">Setting 4</h3>
          <button className="px-4 py-2 bg-blue-500 text-white rounded hover:bg-blue-600">
            Click me!
          </button>
        </div>
      ) 
    },
  ];

  return (
    <div className="max-h-full overflow-auto w-full p-4">
      <CustomAccordion items={items} allowMultiple={true} />
    </div>
  );
};

export default SettingsAccordion;