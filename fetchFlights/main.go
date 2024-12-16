package main

import (
	"database/sql"
	"encoding/json"
	"fmt"
	"os"
	"time"

	_ "github.com/alexbrainman/odbc"
)

type Flight struct {
    FlightNumber     string    `json:"flight_number"`
    DepartureCity    string    `json:"departure_city"`
    ArrivalCity      string    `json:"arrival_city"`
    DepartureTime    time.Time `json:"departure_time"`
    ArrivalTime      time.Time `json:"arrival_time"`
    Price            float64   `json:"price"`
    DepartureAirport string    `json:"departure_airport"`
    ArrivalAirport   string    `json:"arrival_airport"`
    AirlineCompany   string    `json:"airline_company"`
    CheckinStartTime time.Time `json:"checkin_start_time"`
    CheckinEndTime   time.Time `json:"checkin_end_time"`
    Status           string    `json:"status"`
}

type DatabaseConfig struct {
    Hostname string `json:"hostname"`
    Port     int    `json:"port"`
    Database string `json:"database"`
    Username string `json:"username"`
    Password string `json:"password"`
}

func main(){
	file, err := os.Open("result.json")
    if err != nil {
        fmt.Println(err)
        return
    }
    defer file.Close()
    var flights []Flight
    err = json.NewDecoder(file).Decode(&flights)
    if err != nil {
        fmt.Println(err)
        return
    }

    var config DatabaseConfig
    configFile, err := os.Open("database_config.json")
    if err != nil {
        fmt.Println(err)
        return
    }
    
    err = json.NewDecoder(configFile).Decode(&config)
    if err != nil {
        fmt.Println(err)
        return
    }

	user := config.Username
	pass := config.Password
	// host := config.Hostname
    // port := config.Port
	dbName := config.Database
    // dsn := fmt.Sprintf("driver={SQL Server};server=%s;port=%d;database=%s;uid=%s;pwd=%s", host, port, dbName, user, pass)
    dsn := fmt.Sprintf("DSN=%s;UID=%s;PWD=%s", dbName, user, pass)
	db, err := sql.Open("odbc", dsn)
    if err != nil {
        fmt.Println(err)
        return
    }
    for _, flight := range flights {
        _, err := db.Exec("INSERT INTO flight_info (flight_number, departure_city, arrival_city, departure_time, arrival_time, price, departure_airport, arrival_airport, airline_company, checkin_start_time, checkin_end_time, status) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)",
            flight.FlightNumber,
            flight.DepartureCity,
            flight.ArrivalCity,
            flight.DepartureTime,
            flight.ArrivalTime,
            flight.Price,
            flight.DepartureAirport,
            flight.ArrivalAirport,
            flight.AirlineCompany,
            flight.CheckinStartTime,
            flight.CheckinEndTime,
            flight.Status,
        )
        if err != nil {
            fmt.Println(err)
            continue
        }
    }
    
}