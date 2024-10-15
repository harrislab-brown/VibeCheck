// src/components/Layout.tsx
import React from 'react';
import '../styles/layout.css';
import Logo from './Logo'
import SerialConnect from './SerialConnect';
import SystemStatus from './SystemStatus';
import SettingsAccordion from './SettingsAccordion';

const Layout: React.FC = () => {
  return (
    <div className = "layout">
        <div className = "logo-serial-plot-controls container">
            <div className = "logo-serial container">
                <Logo/>
                <SerialConnect/>

            </div>
            <div className = "plot scrollable container">
                <h1>scrollable plot container</h1>

            </div>
            <div className = 'plot-controls container'>
                <h1>Plot Controls</h1>

            </div>
        </div>
        <div className = "status-settings-files container">
            <div className = "status container">
                <SystemStatus/>
            </div>
            <div className = "settings scrollable container">
            <SettingsAccordion />
            </div>
            <div className = "file container">

            </div>
        </div>
    </div>
  );
};

export default Layout;
