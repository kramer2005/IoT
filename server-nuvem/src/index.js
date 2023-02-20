import Express from 'express'
import http from 'http'
import { WebSocketServer, WebSocket } from 'ws'

const app = Express()

/**
 * @type {WebSocket[]}
 */
let connections = []

app.get("/lights-on", (req, res) => {
    const data = {
        lights: true
    }

    const message = JSON.stringify(data)

    connections.forEach(conn => conn.send(message))
    console.log("Message broadcasted")

    res.sendStatus(200)
})

app.get("/lights-off", (req, res) => {
    const data = {
        lights: false
    }

    const message = JSON.stringify(data)

    connections.forEach(conn => conn.send(message))
    console.log("Message broadcasted")

    res.sendStatus(200)
})

app.get("/first-floor", (req, res) => {
    const data = {
        floor: 1
    }

    const message = JSON.stringify(data)

    connections.forEach(conn => conn.send(message))
    console.log("Message broadcasted")

    res.sendStatus(200)
})

app.get("/second-floor", (req, res) => {
    const data = {
        floor: 2
    }

    const message = JSON.stringify(data)

    connections.forEach(conn => conn.send(message))
    console.log("Message broadcasted")

    res.sendStatus(200)
})

app.get("/auto", (req, res) => {
    const data = {
        auto: true
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