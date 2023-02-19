import { WebSocket } from 'ws'
import { InfluxDB, Point } from '@influxdata/influxdb-client'
import { ReadlineParser, SerialPort } from 'serialport'

const url = "http://3.94.183.225:8086"
const token = "1OWtE4Z0iu0_cIRNxnApUvM7iU2eK8CCouPB0BoDukVxvVloFQOPSKFON14flhQ0CSPk7FvYzhBT1NyI_5Bu1w=="
const org = "UFPR"
const bucket = "IoT"

const wsClient = new WebSocket("ws://3.94.183.225:8999")
const writeApi = new InfluxDB({ url, token }).getWriteApi(org, bucket, 'ns')

const serialPort = new SerialPort({ path: '/dev/ttyACM0', baudRate: 115200 });
const parser = serialPort.pipe(new ReadlineParser({ delimiter: '\n' }));

serialPort.on("open", () => {
    console.log('Arduíno conectado');
});

parser.on('data', (data) => {
    if (!(data.startsWith("light") || data.startsWith("temperature"))) {
        return
    }

    const splitted = data.split(" ");

    const p = new Point("Arduino")
        .tag("sensor")
        .floatField(splitted[0], Number(splitted[1]))
        .timestamp(new Date())

    writeApi.writePoint(p);
    writeApi.flush();
    console.log(p.toLineProtocol());
});

let config = {
    lights: false,
    floor: 1
}

wsClient.on("open", () => {
    console.log("Connected to server");
})

wsClient.on("message", (data) => {
    const object = JSON.parse(data)

    if ("lights" in object) {
        serialPort.write(`lights ${object.lights ? "1" : "0"}`);
    }

    if ("floor" in object) {
        serialPort.write(`floor ${object.floor - 1}`);
    }

    config = {
        ...config,
        ...object
    }

    console.log(config)
})