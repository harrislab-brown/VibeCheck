// src/components/LivePlot.tsx

import React, { useEffect, useState, useCallback } from 'react';
import { XYPlot, LineSeries, XAxis, YAxis, HorizontalGridLines, VerticalGridLines, DiscreteColorLegend } from 'react-vis';
import '../styles/LivePlot.css';
import { useSerial } from '../contexts/SerialContext';
import { DataPoint } from '../utils/dataParser';

// Make sure to import the react-vis CSS
import 'react-vis/dist/style.css';

const TIME_WINDOW = 2000; // 2 seconds in milliseconds
const MAX_DATA_POINTS = 100; // Maximum number of data points to display per channel

interface LivePlotProps {}

const colors = [
  '#19CDD7', '#DDB27C', '#88572C', '#FF991F', '#F15C17', '#223F9A', '#DA70BF', '#125C77', '#4DC19C', '#776E57'
];

const LivePlot: React.FC<LivePlotProps> = () => {
  const { parsedData } = useSerial();
  const [plotData, setPlotData] = useState<{ [key: string]: { x: number; y: number }[] }>({});
  const [yDomain, setYDomain] = useState<[number, number]>([-1, 1]);

  const processData = useCallback((data: DataPoint[]) => {
    const currentTime = data[data.length - 1]?.timestamp || 0;
    const startTime = Math.max(currentTime - TIME_WINDOW, data[0]?.timestamp || 0);
    
    const newPlotData: { [key: string]: { x: number; y: number }[] } = {};
    let minY = Infinity;
    let maxY = -Infinity;

    data.forEach((dp) => {
      if (dp.timestamp >= startTime) {
        const key = `Channel ${dp.channel}`;
        if (!newPlotData[key]) {
          newPlotData[key] = [];
        }
        const x = (dp.timestamp - startTime) / 1000;
        const y = dp.x; // Using x acceleration, change to dp.y or dp.z if needed
        newPlotData[key].push({ x, y });
        minY = Math.min(minY, y);
        maxY = Math.max(maxY, y);

        // Keep only the last MAX_DATA_POINTS for each channel
        if (newPlotData[key].length > MAX_DATA_POINTS) {
          newPlotData[key] = newPlotData[key].slice(-MAX_DATA_POINTS);
        }
      }
    });

    // Add a 10% buffer to y-axis
    const yBuffer = (maxY - minY) * 0.1;
    setYDomain([minY - yBuffer, maxY + yBuffer]);

    return newPlotData;
  }, []);

  useEffect(() => {
    setPlotData(processData(parsedData));
  }, [parsedData, processData]);

  const legendItems = Object.keys(plotData).map((key, index) => ({
    title: key,
    color: colors[index % colors.length]
  }));

  return (
    <div className="plot-container">
      <XYPlot
        width={600}
        height={300}
        xDomain={[0, 2]}
        yDomain={yDomain}
        margin={{left: 60, right: 10, top: 10, bottom: 40}}
      >
        <HorizontalGridLines />
        <VerticalGridLines />
        <XAxis title="Time (s)" />
        <YAxis title="Acceleration" />
        {Object.entries(plotData).map(([key, data], index) => (
          <LineSeries
            key={key}
            data={data}
            color={colors[index % colors.length]}
          />
        ))}
      </XYPlot>
      <DiscreteColorLegend items={legendItems} orientation="horizontal" />
    </div>
  );
};

export default LivePlot;