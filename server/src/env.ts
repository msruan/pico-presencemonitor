import dotenv from "dotenv";

dotenv.config();

/** InfluxDB v2 URL */
const url = process.env["INFLUX_URL"] || "http://localhost:8086";
/** InfluxDB authorization token */
const token = process.env["INFLUXDB_ADMIN_TOKEN"] || "my-token";
/** Organization within InfluxDB  */
const org = process.env["INFLUX_ORG"] || "my-org";
/**InfluxDB bucket used in examples  */
const bucket = process.env["INFLUX_BUCKET"] || "my-bucket";

export { url, token, org, bucket };
