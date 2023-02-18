import { WebSocketServer, WebSocket } from 'ws'
import { InfluxDB, Point } from '@influxdata/influxdb-client'

const wsServer = new WebSocketServer({
    port: 4000
})

const url = "http://localhost:8086"
const token = "3tkckHlALSaKexCphY5mSSnEXPak5w7ktedaLUmJwbDK09VhH2cou6mQRPvyC6iyQiRCz93NtxwIuBGvdCZLng=="
const org = "KramerDev"
const bucket = "IoT"

const wsClient = new WebSocket("ws://localhost:8999")
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
            console.log(data.temp)
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

wsClient.on("message", (data) => {
    connections.forEach(conn => {
        conn.send(data)
    })

    console.log("Message broadcasted")
})