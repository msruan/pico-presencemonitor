from(bucket: "home")
  |> range(start: -1h) // Ajuste para o intervalo desejado
  |> filter(fn: (r) => r._measurement == "motion_detected")
  |> filter(fn: (r) => r._field == "detected")
  |> group(columns: ["sensor", "location"]) // Agrupar por sensor e localização
  |> sort(columns: ["_time"], desc: true) // Ordenar pela data mais recente
  |> yield(name: "motion_data")
