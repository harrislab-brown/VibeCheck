// src/components/LivePlot.tsx

import React, { useEffect, useState } from 'react';
import { Chart as ChartJS, ChartOptions, ChartData, LinearScale, PointElement, LineElement, Title, Tooltip, Legend, ScatterDataPoint, BubbleDataPoint } from 'chart.js';
import { Line } from 'react-chartjs-2';
import '../styles/LivePlot.css';
import { PlotControlsState } from './PlotControls';
import { useSerial } from '../contexts/SerialContext';

ChartJS.register(LinearScale, PointElement, LineElement, Title, Tooltip, Legend);

const NUM_OF_DATASTREAMS = 9;
const MAX_DATAPOINTS = 100;

interface DataPoint {
  x: number;
  y: number;
}

type ChartDataPoint = ScatterDataPoint | BubbleDataPoint | DataPoint;

interface Dataset {
  label: string;
  data: ChartDataPoint[];
  borderColor: string;
  tension: number;
}

interface LivePlotProps {
  controls: PlotControlsState;
}

const colors = [
  'rgb(75, 192, 192)', 'rgb(255, 99, 132)', 'rgb(54, 162, 235)',
  'rgb(255, 206, 86)', 'rgb(153, 102, 255)', 'rgb(255, 159, 64)',
  'rgb(0, 128, 0)', 'rgb(128, 0, 128)', 'rgb(128, 128, 0)'
];

const createDataset = (index: number): Dataset => ({
  label: `Data Stream ${index + 1}`,
  data: [],
  borderColor: colors[index],
  tension: 0.1
});

const LivePlot: React.FC<LivePlotProps> = ({ controls }) => {
  const { receivedData } = useSerial();
  const [data, setData] = useState<ChartData<'line', ChartDataPoint[], unknown>>({
    datasets: Array.from({ length: NUM_OF_DATASTREAMS }, (_, i) => createDataset(i)),
  });

  useEffect(() => {
    // Parse receivedData and update the chart data
    const parsedData = parseReceivedData(receivedData);
    if (parsedData.length > 0) {
      setData(prevData => {
        const newDatasets = prevData.datasets.map((dataset, index) => {
          const typedDataset = dataset as Dataset;
          const newDataPoint: DataPoint = { 
            x: parsedData[index].timestamp,
            y: parsedData[index].value
          };
          const updatedData = [...typedDataset.data, newDataPoint]
            .slice(-MAX_DATAPOINTS) as ChartDataPoint[];

          return {
            ...typedDataset,
            data: updatedData,
          };
        });

        return { datasets: newDatasets };
      });
    }
  }, [receivedData]);

  const options: ChartOptions<'line'> = {
    responsive: true,
    maintainAspectRatio: false,
    animation: {
      duration: 0
    },
    scales: {
      x: {
        type: 'linear',
        display: true,
        title: {
          display: true,
          text: 'Time (s)'
        },
        min: getXValue(data.datasets[0]?.data[0]),
        max: getXValue(data.datasets[0]?.data[data.datasets[0]?.data.length - 1]) + 5,
        ticks: {
          stepSize: 5
        }
      },
      y: {
        beginAtZero: true,
        min: controls.minBound,
        max: controls.maxBound
      }
    },
    plugins: {
      legend: {
        position: 'top' as const,
      },
      title: {
        display: true,
        text: 'Live Data Plot',
      },
    },
  };

  return (
    <div className="plot-container">
      <Line options={options} data={data} />
    </div>
  );
};

// Helper function to safely get the x value from a data point
function getXValue(point: ChartDataPoint | undefined): number {
  if (!point) return 0;
  if (typeof point === 'object' && 'x' in point) return point.x as number;
  return 0;
}

// You'll need to implement this function based on your data format
function parseReceivedData(data: string): { timestamp: number, value: number }[] {
  // Parse the received data string and return an array of data points
  // This implementation will depend on your specific data format
  return [];
}

export default LivePlot;