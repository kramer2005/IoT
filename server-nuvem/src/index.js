import Express from 'express'
import http from 'http'
import { WebSocketServer, WebSocket } from 'ws'

const app = Express()

/**
 * @type {WebSocket[]}
 */
let connections = []

app.get("/", (req, res) => {
    const data = {
        lights: true,
        speed: 0
    }

    const message = JSON.stringify(data)

    connections.forEach(conn => conn.send(message))
    console.log("Message broadcasted")

    res.sendStatus(200)
})

const server = http.createServer(app)

const wsServer = new WebSocketServer({ server })

wsServer.on("connection", (conn) => {
    console.log(new Date() + " new connection.")
    connections.push(conn)

    conn.on("message", (message) => {
        const data = JSON.parse(message.toString())

        console.log(data)
    })

    conn.on("close", () => {
        console.log("Client disconnected.")
        connections = connections.filter(it => it !== conn)
    })
})

server.listen(8999, () => {
    console.log("Cloud server listening")
})