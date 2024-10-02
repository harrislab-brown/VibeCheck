// src/App.tsx
import Layout from './components/Layout';
import { SerialProvider } from './contexts/SerialContext';

function App() {
  return (
    <SerialProvider>
      <Layout />
    </SerialProvider>
  );
}

export default App;