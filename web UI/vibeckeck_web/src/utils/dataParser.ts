// src/utils/dataParser.ts

export interface DataPoint {
    channel: number;
    timestamp: number;
    x: number;
    y: number;
    z: number;
  }
  
  export function parseSerialData(data: string): DataPoint[] {
    const lines = data.trim().split('\n');
    const allDataPoints: DataPoint[] = [];
  
    for (const line of lines) {
      const tokens = line.split(' ');
      if (tokens[0] !== 'data') continue;
  
      const numDataPoints = parseInt(tokens[1], 10);
      for (let i = 0; i < numDataPoints; i++) {
        const startIndex = 2 + i * 5;
        allDataPoints.push({
          channel: parseInt(tokens[startIndex], 10),
          timestamp: parseInt(tokens[startIndex + 1], 10),
          x: parseFloat(tokens[startIndex + 2]),
          y: parseFloat(tokens[startIndex + 3]),
          z: parseFloat(tokens[startIndex + 4])
        });
      }
    }
  
    return allDataPoints;
  }