## 获取所有航班
GET /api/flights                 

成功返回
```ts
{
    "code":200,
    "message":"操作成功",
    "data":{
        "flightId":number,
        "flightNumber":string,
        "departureCity":string,
        "arrivalCity":string,
        "departureTime":string,
        "arrivalTime":string,
        "departureAirport":string,
        "arrivalAirport":string,
        "checkinStartTime":string,
        "checkinEndTime":string,
        "price":number,
        "airlineCompany":string,
        "status":string
    }[]
}
```

## 根据id获取航班
GET /api/flights/{id}

成功返回
```ts
{
    "code":200,
    "message":"操作成功",
    "data":{
        "flightId":number,
        "flightNumber":string,
        "departureCity":string,
        "arrivalCity":string,
        "departureTime":string,
        "arrivalTime":string,
        "departureAirport":string,
        "arrivalAirport":string,
        "checkinStartTime":string,
        "checkinEndTime":string,
        "price":number,
        "airlineCompany":string,
        "status":string
    }
}
```