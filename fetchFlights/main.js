const fs = require('fs');

const url = 'https://flights.ctrip.com/schedule/getScheduleByCityPair';

let flightsInfo = [];

function subtractMinutes(time, minutes) {
    const date = new Date(time);
    date.setMinutes(date.getMinutes() - minutes);
    // 北京时间
    return date;
}

const getFlights = async (departureCityCode, arriveCityCode, curPage = 1) => {
    try {
        const res = await fetch(url, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({
                departureCityCode,
                arriveCityCode,
                pageNo: curPage,
            }),
        });
        const data = await res.json();
        // 如果data为空，说明没有数据了
        if (Object.keys(data).length === 0) {
            return;
        }
        const totalPage = data['totalPage'];
        if (totalPage >= curPage) {
            const result = data['scheduleVOList'];
            if(result.length === 0) {
                return
            }
            const departure_city = data['departureCityName'];
            const arrival_city = data['arriveCityName'];
            for (const flight of result) {
                let obj = {};
                // 2024-12-18
                const YearMonthDay = flight['departDate'];
                // 10:00
                const departTime = flight['departTime'];
                const arrivalTime = flight['arriveTime'];
                const departTimeDate = new Date(`${YearMonthDay} ${departTime}`);
                const arrivalTimeDate = new Date(`${YearMonthDay} ${arrivalTime}`);
                obj['flight_number'] = flight['flightNo'];
                obj['departure_city'] = departure_city;
                obj['arrival_city'] = arrival_city;
                obj['departure_time'] = departTimeDate;
                obj['arrival_time'] = arrivalTimeDate;
                obj['price'] = flight['price'];
                obj['departure_airport'] = flight['departPortName'];
                obj['arrival_airport'] = flight['arrivePortName'];
                obj['airline_company'] = flight['airlineCompanyName'];
                obj['checkin_start_time'] = subtractMinutes(departTimeDate, 130);
                obj['checkin_end_time'] = subtractMinutes(departTimeDate, 10);
                obj['status'] = 'on Time';
                flightsInfo.push(obj);
            }
            // 如果要获取所有页数，取消下面这行的注释
            // await getFlights(departureCityCode, arriveCityCode, curPage + 1);
        }
    } catch (err) {
        console.log(err);
    }
};

const processFlights = async () => {
    try {
        const data = fs.readFileSync('nums.json', 'utf8');
        const flights = JSON.parse(data);
        const promises = [];

        for (let i = 0; i < flights.length; ++i) {
            for (let j = 0; j < flights.length; ++j) {
                if (i !== j) {
                    promises.push(getFlights(flights[i], flights[j]));
                }
            }
        }

        await Promise.all(promises);
        fs.writeFileSync('result.json', JSON.stringify(flightsInfo, null, 2));
        console.log('文件已成功写入');
    } catch (err) {
        console.log(err);
    }
};

processFlights();