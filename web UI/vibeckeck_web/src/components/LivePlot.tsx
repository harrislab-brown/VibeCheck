import React, { useEffect, useState, useMemo, useCallback } from 'react';
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

interface LivePlotProps {
  setYAutoScale?: (autoScale: boolean) => void;
  setYRange?: (min: number | null, max: number | null) => void;
  getYRange?: () => { min: number | null; max: number | null };
}

const colors = [
  'rgb(75, 192, 192)', 'rgb(255, 99, 132)', 'rgb(54, 162, 235)',
  'rgb(255, 206, 86)', 'rgb(153, 102, 255)', 'rgb(255, 159, 64)',
  'rgb(0, 128, 0)', 'rgb(128, 0, 128)', 'rgb(128, 128, 0)'
];

const WINDOW_DURATION = .005; // time in seconds
const TIME_RES = 1000000; // timestamp is reported in microseconds: 1/1000000 s 

const LivePlot = React.forwardRef<any, LivePlotProps>(({ setYAutoScale, setYRange, getYRange }, ref) => {
  const { parsedData } = useSerial();
  const [chartData, setChartData] = useState<ChartData<'line'>>({ datasets: [] });
  const [xRange, setXRange] = useState({ min: 0, max: WINDOW_DURATION });
  const [autoRangeY, setAutoRangeY] = useState(true);
  const [localYRange, setLocalYRange] = useState<{ min: number | null; max: number | null }>({ min: null, max: null });

  const processData = useMemo(() => {
    return (data: DataPoint[]) => {
      if (data.length === 0) return { datasets: [], xMin: 0, xMax: WINDOW_DURATION, yMin: 0, yMax: 1 };

      const channels = new Set(data.map(dp => dp.channel));
      const xValues = data.map(dp => dp.timestamp/TIME_RES);
      const xMax = Math.max(...xValues);
      const xMin = Math.max(xMax - WINDOW_DURATION, 0);

      const datasets = Array.from(channels).flatMap(channel => [
        {
          label: `Channel ${channel} - X`,
          data: data
            .filter(dp => dp.channel === channel && dp.timestamp/TIME_RES >= xMin)
            .map(dp => ({ x: dp.timestamp/TIME_RES, y: dp.x })),
          borderColor: colors[(channel * 3) % colors.length],
          backgroundColor: colors[(channel * 3) % colors.length],
          tension: 0,
          pointRadius: 1,
          pointHoverRadius: 3,
        },
        {
          label: `Channel ${channel} - Y`,
          data: data
            .filter(dp => dp.channel === channel && dp.timestamp/TIME_RES >= xMin)
            .map(dp => ({ x: dp.timestamp/TIME_RES, y: dp.y })),
          borderColor: colors[(channel * 3 + 1) % colors.length],
          backgroundColor: colors[(channel * 3 + 1) % colors.length],
          tension: 0,
          pointRadius: 1,
          pointHoverRadius: 3,
        },
        {
          label: `Channel ${channel} - Z`,
          data: data
            .filter(dp => dp.channel === channel && dp.timestamp/TIME_RES >= xMin)
            .map(dp => ({ x: dp.timestamp/TIME_RES, y: dp.z })),
          borderColor: colors[(channel * 3 + 2) % colors.length],
          backgroundColor: colors[(channel * 3 + 2) % colors.length],
          tension: 0,
          pointRadius: 1,
          pointHoverRadius: 3,
        },
      ]);

      const allYValues = data.flatMap(dp => [dp.x, dp.y, dp.z]);
      const yMin = Math.min(...allYValues);
      const yMax = Math.max(...allYValues);

      return { datasets, xMin, xMax, yMin, yMax };
    };
  }, []);

  useEffect(() => {
    const { datasets, xMin, xMax, yMin, yMax } = processData(parsedData);
    setChartData({ datasets });
    setXRange({ min: xMin, max: xMax });
    if (autoRangeY) {
      if (setYRange) {
        setYRange(yMin, yMax);
      } else {
        setLocalYRange({ min: yMin, max: yMax });
      }
    }
  }, [parsedData, processData, autoRangeY, setYRange]);

  const getYAxisRange = useCallback(() => {
    let yRange;
    if (typeof getYRange === 'function') {
      yRange = getYRange();
    } else {
      yRange = localYRange;
    }
    return {
      min: yRange.min !== null ? yRange.min : undefined,
      max: yRange.max !== null ? yRange.max : undefined
    };
  }, [getYRange, localYRange]);

  const options: ChartOptions<'line'> = {
    responsive: true,
    maintainAspectRatio: false,
    animation: { duration: 0 },
    scales: {
      x: {
        type: 'linear',
        position: 'bottom',
        title: { display: true, text: 'Time (s)' },
        min: xRange.min,
        max: xRange.max,
        ticks: {
          stepSize: Math.max(1, Math.floor(WINDOW_DURATION / 10))
        }
      },
      y: {
        beginAtZero: false,
        title: { display: true, text: 'Acceleration' },
        ...getYAxisRange()
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

  const handleSetYAutoScale = useCallback((autoScale: boolean) => {
    setAutoRangeY(autoScale);
    if (setYAutoScale) {
      setYAutoScale(autoScale);
    }
  }, [setYAutoScale]);

  const handleSetYRange = useCallback((min: number | null, max: number | null) => {
    if (setYRange) {
      setYRange(min, max);
    } else {
      setLocalYRange({ min, max });
    }
  }, [setYRange]);

  React.useImperativeHandle(ref, () => ({
    setYAutoScale: handleSetYAutoScale,
    setYRange: handleSetYRange,
    getYRange: getYRange || (() => localYRange),
  }));

  return (
    <div className="plot-container">
      <Line options={options} data={chartData} />
    </div>
  );
});

export default LivePlot;