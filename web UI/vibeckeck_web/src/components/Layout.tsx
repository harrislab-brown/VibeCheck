// ./Layout.tsx

import React, { useState } from 'react';
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

    const handleUpdateControls = (newControls: PlotControlsState) => {
        setPlotControls(newControls);
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
                <LivePlot controls={plotControls} />
            </div>
            <div className="grid-item plot_controls">
                <PlotControls onUpdateControls={handleUpdateControls} />
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