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

func main() {
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
	dbName := config.Database
	dsn := fmt.Sprintf("DSN=%s;UID=%s;PWD=%s", dbName, user, pass)
	db, err := sql.Open("odbc", dsn)
	if err != nil {
		fmt.Println(err)
		return
	}
    // 如果不存在就建表
	_, err = db.Exec("CREATE TABLE IF NOT EXISTS flight_info (flight_id INTEGER PRIMARY KEY AUTO_INCREMENT, flight_number VARCHAR(10) NOT NULL, departure_city VARCHAR(20) NOT NULL, arrival_city VARCHAR(20) NOT NULL, departure_time DATETIME NOT NULL, arrival_time DATETIME NOT NULL, price DECIMAL(10,2) DEFAULT 700, departure_airport VARCHAR(20) NOT NULL, arrival_airport VARCHAR(20) NOT NULL, airline_company VARCHAR(20) NOT NULL, checkin_start_time DATETIME NOT NULL, checkin_end_time DATETIME NOT NULL, status VARCHAR(10) NOT NULL, UNIQUE(flight_number) )")
	if err != nil {
        fmt.Println(err)
        return
    }
    _, err = db.Exec("DELETE FROM flight_info")
    if err != nil {
        fmt.Println(err)
        return
    }
    for _, flight := range flights {
		if flight.Price == 0.00 {
			flight.Price = 700.00
		}
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
