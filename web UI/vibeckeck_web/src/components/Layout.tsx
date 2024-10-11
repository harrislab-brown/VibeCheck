import React, { useState, useRef, useCallback } from 'react';
import '../styles/Layout.css';
import SerialPortConnect from '../components/SerialPortConnect';
import DataSettings from '../components/DataSettings';
import LivePlot from '../components/LivePlot';
import PlotControls, { PlotControlsState } from '../components/PlotControls';
import TabbedInterface from './TabbedInterface';
import StatusDisplay from './StatusDIsplay';
import HardwareControl from './HardwareControl';
import VibeCheckLogo from '../assets/VibeCheck_Logo.png';

const Layout: React.FC = () => {
    const [plotControls, setPlotControls] = useState<PlotControlsState>({
        minBound: 0,
        maxBound: 100,
        scrollSpeed: 50,
    });

    const plotRef = useRef<any>(null);

    const handleUpdateControls = (newControls: PlotControlsState) => {
        setPlotControls(newControls);
    };

    const handleYAutoScaleChange = useCallback((autoScale: boolean) => {
        console.log('Auto scale changed:', autoScale);
        // Implement your logic here, e.g., update state or perform some action
    }, []);

    const handleYRangeChange = useCallback((min: number | null, max: number | null) => {
        console.log('Y range changed:', { min, max });
        // Implement your logic here, e.g., update state or perform some action
    }, []);

    const getYRange = useCallback(() => {
        // Implement your logic here to return the current Y range
        // For now, we'll return null values
        return { min: null, max: null };
    }, []);

    // Example of how you might use the plotRef
    const resetPlot = () => {
        if (plotRef.current) {
            plotRef.current.setYAutoScale(true);
            plotRef.current.setYRange(null, null);
        }
    };

    return (
        <div className="grid-container">
            <div className="grid-item logo">
                <img src={VibeCheckLogo} alt="VibeCheck Logo" className="vibecheck-logo" />
            </div>
            <div className="grid-item session_settings">
                <SerialPortConnect/>
                <DataSettings/>
            </div>
            <div className="grid-item tabbed_interface">
                <TabbedInterface/>
            </div>
            <div className="grid-item live_plot">
                <LivePlot
                    ref={plotRef}
                    setYAutoScale={handleYAutoScaleChange}
                    setYRange={handleYRangeChange}
                    getYRange={getYRange}
                />
            </div>
            <div className="grid-item plot_controls">
                <PlotControls onUpdateControls={handleUpdateControls} />
                <button onClick={resetPlot}>Reset Plot</button>
            </div>
            <div className="grid-item hardware_controls">
                <HardwareControl/>
            </div>
            <div className="grid-item status_display">
                <StatusDisplay/>
            </div>
        </div>
    );
};

export default Layout;