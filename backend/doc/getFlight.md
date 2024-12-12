### 获取城市简称
GET https://flights.ctrip.com/itinerary/api/poi/get HTTP/1.1            

### 获取
POST https://flights.ctrip.com/schedule/getScheduleByCityPair HTTP/1.1         
Content-Type: application/json              

{       
    "departureCityCode": "TNA",                
    "arriveCityCode": "FOC",            
    "pageNo":1             
}                  