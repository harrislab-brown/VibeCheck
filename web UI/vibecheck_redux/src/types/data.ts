export interface DataState {
    points: DataPoint[];
  }
  
  export interface DataPoint {
    timestamp: number;
    channel: number;
    x: number;
    y: number;
    z: number;
  }