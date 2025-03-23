import { InfluxDB, Point } from '@influxdata/influxdb-client'
import { url, token, org, bucket } from './env'

import express from "express"
import cors from "cors"
import helmet from 'helmet'

const app = express()

app.use(helmet())
app.use(cors());
app.use(express.json());

console.log(`To usando ${url} e ${token}, na ${org} com ${bucket}`)

const influxClient = new InfluxDB({ url, token }).getWriteApi(org, bucket, 's')

app.post('/motion', (req, res) => {
  console.log("Chamou plaquinha?")
  const { sensor_id, location } = req.body // Dados opcionais
  
  const point = new Point('motion_detected')
  .tag('sensor', sensor_id || 'unknown') // ID do sensor (se houver)
  .tag('location', location || 'unknown') // Localização (se houver)
  .timestamp(new Date()) // Timestamp automático
  .intField('detected', 1) // Apenas um evento simples
  
  influxClient.writePoint(point)
  res.status(200).send({ message: 'Movimentação registrada' })
})


const PORT = 3001
app.listen(PORT, () => console.log(`App de exemplo esta rodando na porta ${PORT}`))

process.on('SIGINT', async () => {
  await influxClient.close()
  process.exit(0)
})
