import React, { useState } from 'react';

const SensorSettings: React.FC = () => {
  const [value, setValue] = useState('');

  return (
    <div>
      <h3 className="text-lg font-semibold mb-2">Setting 1</h3>
      <input
        type="text"
        value={value}
        onChange={(e) => setValue(e.target.value)}
        className="w-full px-3 py-2 border rounded-md"
        placeholder="Enter a value"
      />
      <p className="mt-2">Current value: {value}</p>
    </div>
  );
};

export default SensorSettings;