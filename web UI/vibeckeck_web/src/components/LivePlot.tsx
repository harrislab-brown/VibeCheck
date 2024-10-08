// src/components/LivePlot.tsx

import React, { useEffect, useState, useMemo } from 'react';
import {
  Chart as ChartJS,
  LinearScale,
  PointElement,
  LineElement,
  Title,
  Tooltip,
  Legend,
  ChartOptions,
  ChartData
} from 'chart.js';
import { Line } from 'react-chartjs-2';
import '../styles/LivePlot.css';
import { useSerial } from '../contexts/SerialContext';
import { DataPoint } from '../utils/dataParser';

ChartJS.register(
  LinearScale,
  PointElement,
  LineElement,
  Title,
  Tooltip,
  Legend
);

interface LivePlotProps {}

const colors = [
  'rgb(75, 192, 192)', 'rgb(255, 99, 132)', 'rgb(54, 162, 235)',
  'rgb(255, 206, 86)', 'rgb(153, 102, 255)', 'rgb(255, 159, 64)',
  'rgb(0, 128, 0)', 'rgb(128, 0, 128)', 'rgb(128, 128, 0)'
];

const LivePlot: React.FC<LivePlotProps> = () => {
  const { parsedData } = useSerial();
  const [chartData, setChartData] = useState<ChartData<'line'>>({ datasets: [] });
  const [xRange, setXRange] = useState({ min: 0, max: 1 });

  const processData = useMemo(() => {
    return (data: DataPoint[]) => {
      if (data.length === 0) return { datasets: [], xMin: 0, xMax: 1 };

      const channels = new Set(data.map(dp => dp.channel));
      const xValues = data.map(dp => dp.timestamp);
      const xMin = Math.min(...xValues);
      const xMax = Math.max(...xValues);

      const datasets = Array.from(channels).flatMap(channel => [
        {
          label: `Channel ${channel} - X`,
          data: data
            .filter(dp => dp.channel === channel)
            .map(dp => ({ x: dp.timestamp, y: dp.x })),
          borderColor: colors[(channel * 3) % colors.length],
          backgroundColor: colors[(channel * 3) % colors.length],
          tension: 0,
          pointRadius: 1,
          pointHoverRadius: 3,
        },
        {
          label: `Channel ${channel} - Y`,
          data: data
            .filter(dp => dp.channel === channel)
            .map(dp => ({ x: dp.timestamp, y: dp.y })),
          borderColor: colors[(channel * 3 + 1) % colors.length],
          backgroundColor: colors[(channel * 3 + 1) % colors.length],
          tension: 0,
          pointRadius: 1,
          pointHoverRadius: 3,
        },
        {
          label: `Channel ${channel} - Z`,
          data: data
            .filter(dp => dp.channel === channel)
            .map(dp => ({ x: dp.timestamp, y: dp.z })),
          borderColor: colors[(channel * 3 + 2) % colors.length],
          backgroundColor: colors[(channel * 3 + 2) % colors.length],
          tension: 0,
          pointRadius: 1,
          pointHoverRadius: 3,
        }
      ]);

      return { datasets, xMin, xMax };
    };
  }, []);

  useEffect(() => {
    const { datasets, xMin, xMax } = processData(parsedData);
    setChartData({ datasets });
    setXRange({ min: xMin, max: xMax });
  }, [parsedData, processData]);

  const options: ChartOptions<'line'> = {
    responsive: true,
    maintainAspectRatio: false,
    animation: { duration: 0 },
    scales: {
      x: {
        type: 'linear',
        position: 'bottom',
        title: { display: true, text: 'Time (us)' },
        beginAtZero: false,
        min: xRange.min,
        max: xRange.max,
        ticks: {
          stepSize: Math.max(1, Math.floor((xRange.max - xRange.min) / 10))
          //stepSize: 100
        }
      },
      y: {
        beginAtZero: false,
        title: { display: true, text: 'Acceleration' },
      }
    },
    plugins: {
      legend: { position: 'top' as const },
      title: { display: true, text: 'Accelerometer Data' },
    },
    elements: {
      line: { tension: 0 },
      point: {
        radius: 1,
        hoverRadius: 3,
      },
    },
  };

  return (
    <div className="plot-container">
      <Line options={options} data={chartData} />
    </div>
  );
};

export default LivePlot;