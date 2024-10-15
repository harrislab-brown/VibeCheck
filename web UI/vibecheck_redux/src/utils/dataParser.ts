// src/utils/dataParser.ts

export interface DataPoint {
    channel: number;
    timestamp: number;
    x: number;
    y: number;
    z: number;
  }
  
  export interface Message {
    type: 'data' | 'event' | 'ack' | 'error';
    data: DataPoint[] | string;
  }
  
  export function parseSerialData(data: string): Message[] {
    const messages: Message[] = [];
    const lines = data.trim().split('\n');
  
    for (const line of lines) {
      if (line.startsWith('data')) {
        messages.push({
          type: 'data',
          data: parseDataMessage(line)
        });
      } else if (line.startsWith('msg')) {
        messages.push({
          type: 'event',
          data: line.substring(4).trim() // Remove 'msg ' prefix
        });
      } else if (line.startsWith('ack')) {
        messages.push({
          type: 'ack',
          data: line.substring(4).trim() // Remove 'ack ' prefix
        });
      } else if (line.startsWith('error')) {
        messages.push({
          type: 'error',
          data: line.substring(6).trim() // Remove 'error ' prefix
        });
      }
      // Ignore any other message types
    }
  
    return messages;
  }
  
  function parseDataMessage(data: string): DataPoint[] {
    const tokens = data.split(' ');
    if (tokens[0] !== 'data') return [];
  
    const numDataPoints = parseInt(tokens[1], 10);
    const dataPoints: DataPoint[] = [];
  
    for (let i = 0; i < numDataPoints; i++) {
      const startIndex = 2 + i * 5;
      dataPoints.push({
        channel: parseInt(tokens[startIndex], 10),
        timestamp: parseInt(tokens[startIndex + 1], 10),
        x: parseFloat(tokens[startIndex + 2]),
        y: parseFloat(tokens[startIndex + 3]),
        z: parseFloat(tokens[startIndex + 4])
      });
    }
  
    return dataPoints;
  }