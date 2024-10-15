// src/App.tsx

import React, { useEffect } from 'react'
import { Provider as ReduxProvider } from 'react-redux'
import { NextUIProvider } from '@nextui-org/react';
import { useAppDispatch } from './redux/hooks';
import store from './redux/store'
import { initializeApp } from './utils/appInitialization';
import Layout from './components/Layout'

const AppContent: React.FC = () => {
  const dispatch = useAppDispatch();

  useEffect(() => {
    console.log('initialize app');
    initializeApp(dispatch);
  }, [dispatch]);

  return <Layout />;
};

function App() {
  return (
    <ReduxProvider store={store}>
      <NextUIProvider>
        <AppContent/>
      </NextUIProvider>
    </ReduxProvider>
  )
}

export default App