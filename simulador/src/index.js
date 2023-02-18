import { WebSocket } from 'ws'

const wsClient = new WebSocket('ws://localhost:4000')

let temperature = 32;

wsClient.on("open", () => {
    setup()
    setInterval(loop, 1000)
})

const setup = () => {
    wsClient.on("message", (data) => {
        const msg = data.toString()
        const splitted = msg.split(" ")
        console.log(`Command ${splitted[0]} = ${splitted[1]} received`)
    })

    wsClient.on("close", () => {
        console.log("Disconnected")
    })
}

const loop = () => {
    const random = Math.random() < 0.5;
    if (random && temperature < 34) {
        temperature += 0.1;
    }

    if (!random && temperature > 30) {
        temperature -= 0.1;
    }

    const temp = {
        temp: temperature
    }
    wsClient.send(JSON.stringify(temp))
}