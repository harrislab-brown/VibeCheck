// ./components/LivePlot.tsx

import React, { useEffect, useRef, useState } from 'react';
import { Chart as ChartJS, ChartOptions, ChartData, LinearScale, PointElement, LineElement, Title, Tooltip, Legend } from 'chart.js';
import { Line } from 'react-chartjs-2';
import '../styles/LivePlot.css';
import { PlotControlsState } from './PlotControls';

ChartJS.register(LinearScale, PointElement, LineElement, Title, Tooltip, Legend);

const NUM_OF_DATASTREAMS = 9;
const MAX_DATAPOINTS = 100;

interface DataPoint {
  x: number;
  y: number;
}

interface Dataset {
  label: string;
  data: DataPoint[];
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
  const [data, setData] = useState<ChartData<'line', DataPoint[]>>({
    datasets: Array.from({ length: NUM_OF_DATASTREAMS }, (_, i) => createDataset(i)),
  });

  const intervalRef = useRef<number | null>(null);
  const startTimeRef = useRef<number>(Date.now());

  useEffect(() => {
    intervalRef.current = window.setInterval(() => {
      const currentTime = (Date.now() - startTimeRef.current) / 1000; // time in seconds

      setData(prevData => {
        const newDatasets = prevData.datasets.map(dataset => {
          const typedDataset = dataset as Dataset;
          const newDataPoint: DataPoint = { 
            x: currentTime, 
            y: Math.random() * (controls.maxBound - controls.minBound) + controls.minBound 
          };
          const updatedData = [...typedDataset.data, newDataPoint]
            .slice(-MAX_DATAPOINTS); // Keep only the last MAX_DATAPOINTS

          return {
            ...typedDataset,
            data: updatedData,
          };
        });

        return { datasets: newDatasets };
      });
    }, 100); // Update every 100ms for smoother animation

    return () => {
      if (intervalRef.current) {
        clearInterval(intervalRef.current);
      }
    };
  }, [controls]);

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
        min: (data.datasets[0] as Dataset)?.data[0]?.x ?? 0,
        max: ((data.datasets[0] as Dataset)?.data.slice(-1)[0]?.x ?? 0) + 5,
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

export default LivePlot;