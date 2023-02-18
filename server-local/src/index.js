import { WebSocketServer, WebSocket } from 'ws'
import { InfluxDB, Point } from '@influxdata/influxdb-client'

const wsServer = new WebSocketServer({
    port: 4000
})

const url = "http://localhost:8086"
const token = "3tkckHlALSaKexCphY5mSSnEXPak5w7ktedaLUmJwbDK09VhH2cou6mQRPvyC6iyQiRCz93NtxwIuBGvdCZLng=="
const org = "KramerDev"
const bucket = "IoT"

const wsClient = new WebSocket("ws://3.94.183.225:8999")
const writeApi = new InfluxDB({ url, token }).getWriteApi(org, bucket, 'ns')

/**
 * @type {WebSocket[]}
 */
let connections = []

wsServer.on("connection", (conn) => {
    console.log(new Date() + " new connection.")
    connections.push(conn)

    conn.on("message", (message) => {
        const data = JSON.parse(message.toString())

        if ("temp" in data) {
            const p = new Point("Arduino")
                .tag("weather")
                .floatField("temperature", data.temp)
                .timestamp(new Date())

            writeApi.writePoint(p)
            writeApi.flush()
            console.log(p.toString())
        }
    })

    conn.on("close", () => {
        console.log("Client disconnected.")
        connections = connections.filter(it => it !== conn)
    })
})

wsServer.on("listening", () => {
    console.log("Server started")
})

let config = {
    lights: false,
    floor: 1
}

wsClient.on("message", (data) => {
    const object = JSON.parse(data)

    if("lights" in object) {
        connections.forEach(conn => {
            conn.send(`lights ${object.lights ? "1" : "0"}`)
        })
    }

    if("floor" in object) {
        connections.forEach(conn => {
            conn.send(`floor ${object.floor}`)
        })
    }

    const config = {
        ...config,
        object
    }

    console.log(config)
})