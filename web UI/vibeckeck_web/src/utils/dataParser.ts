  // src/utils/dataParser.ts

export interface DataPoint {
  channel: number;
  timestamp: number;
  x: number;
  y: number;
  z: number;
}

export interface Message {
  type: string;
  data: any;
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
        data: parseEventMessage(line)
      });

    } else if (line.startsWith('ack')){
      messages.push({
        type: 'ack',
        data: parseAckMessage(line)
      });
    } else if (line.startsWith('error')) {
      messages.push({
        type: 'error',
        data: parseErrorMessage(line)
      });
    }
    // Add more message types as needed
  }

  return messages;
}

function parseDataMessage(data: string): DataPoint[] {
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

function parseEventMessage(line: string): any {
  // Parse configuration messages
  // For example: "config samplingRate 1000"
  const [_, key, value] = line.split(' ');
  return { [key]: value };
}


function parseAckMessage(line: string): string {
  // parse ack message
  // return the value after recieved after the ack keyword (if one was sent)
  return line.substring(4).trim();
}

function parseErrorMessage(line: string): string {
  // Parse error messages
  // For example: "error Serial connection lost"
  return line.substring(6).trim();
}

  