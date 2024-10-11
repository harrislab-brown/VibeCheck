// utils/dataParser.ts
export interface DataPoint {
    timestamp: number;
    channel: number;
    x: number;
    y: number;
    z: number;
  }
  
  export function parseSerialData(data: string): DataPoint[] {
    // Implement parsing logic here
    return [];
  }