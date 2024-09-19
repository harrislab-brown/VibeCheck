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
            <div className="grid-item item1">
                VibeCheck Logo
            </div>
            <div className="grid-item item2">
                <SerialPortConnect />
            </div>
            <div className="grid-item item3">
                <DataSettings />
            </div>
            <div className="grid-item item4">
                <LivePlot controls={plotControls} />
            </div>
            <div className="grid-item item5">
                <TabbedInterface/>
            </div>
            <div className="grid-item item6">
                <PlotControls onUpdateControls={handleUpdateControls} />
            </div>
            <div className="grid-item item7">
                <HardwareControl/>
            </div>
            <div className="grid-item item8">
                <StatusDisplay/>
            </div>
        </div>
    );
};

export default Layout;