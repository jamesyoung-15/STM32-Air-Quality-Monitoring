
// For responsive navbar
const navElement = document.querySelector('.nav-items');
const hamburgerElement = document.querySelector('.hamburger');
const timeElement = document.querySelector('.time-now');
// toggle hamburger icon 
hamburgerElement.addEventListener("click", ()=>{
    navElement.classList.toggle("nav-open");
    hamburgerElement.classList.toggle("hamburger-open");
});

// retrieve api info
async function getData(){
    // const response = await fetch('https://8cdgfq0r3c.execute-api.us-east-1.amazonaws.com/prod/sensor-data',{
    //     headers: {
    //         'Access-Control-Allow-Origin': '*',
    //         // "Access-Control-Allow-Credentials": true,
    //    },
    // });
    const response = await fetch('https://8cdgfq0r3c.execute-api.us-east-1.amazonaws.com/prod/sensor-data');
    const data = await response.json();
    return data;
}

// filter data by time, pass in number to specify number of data to return
async function getNumData(numData){
    let data = await getData();
    compareDates = (a, b) => {
        return new Date(b.Time) - new Date(a.Time);
      }
      
      // Sort the JSON data based on date and time
      data.sort(compareDates);
      
      // Retrieve the newest time data
      const newestTimeData = data.slice(0, numData);
      return newestTimeData;
}

// update card info by getting latest data only
async function updateCardData(){
    const temperatureElement = document.getElementById('temperature');
    const humidityElement = document.getElementById('humidity');
    const tvocElement = document.getElementById('tvoc');
    const eco2Element = document.getElementById('eco2');
    const timeMeasuredElement = document.querySelector(".time-taken");
    
    let data = await getNumData(1);
    // console.log(data);
    temperatureElement.innerHTML = `${Math.round(data[0].Temperature *10)/10}°C`;
    humidityElement.innerHTML = `${data[0].Humidity}%`;
    tvocElement.innerHTML = `${data[0].TVOC} ppb`;
    eco2Element.innerHTML = `${data[0].eCO2} ppm`;
    timeMeasuredElement.innerHTML = `${data[0].Time}`
}


// get dataset for chart
async function getChartData(numData){
    let dict = {
        "Temperature": [],
        "Time": [],
        "Humidity": [],
        "TVOC": [],
        "eCO2": [],
        // "Date": [],
    };
    let data = await getNumData(numData);
    // console.log(data);
    for(let i=0;i<numData;i++){
        // dict["Time"].push(data[i].Time); // full date
        dict["Time"].push(data[i].Time.substring(11,16)); // remove date and seconds
        dict["Temperature"].push(data[i].Temperature);
        dict["Humidity"].push(data[i].Humidity);
        dict["eCO2"].push(data[i].eCO2);
        dict["TVOC"].push(data[i].TVOC);
        // dict["Date"].push(data[i].Time.substring(0,11))
    }
    return dict;
}

let chartJs
// chartJS, currently only showing temperature chart
async function createChart(dataType){
    const ctx = document.getElementById("myChart");
    let chartData = await getChartData(8);
    // console.log(chartData);
    // Chart.defaults.backgroundColor = '#f38ba8';
    Chart.defaults.borderColor = '#FFF';
    Chart.defaults.color = '#FFF';
    chartJs = new Chart(ctx, {
        type: 'line',
        data: {
        labels: chartData["Time"].reverse(),
        datasets: [{
            label: dataType,
            data: chartData[dataType].reverse(),
            borderColor: '#cba6f7',
            backgroundColor: '#cba6f7',
        }]
        },
        options: {
            responsive: true,
            // plugins: {
            //     title: {
            //         display: true,
            //         text: 'Temperature Chart'
            //     }
            // }
        }
    });

}

// refresh chart if select changed
let changeChart = (element) =>{
    chartJs.destroy();
    createChart(element.value);
}

// For getting current time (using OS settings)
let getFormattedTime = () => {
    // Create a new Date object
    const date = new Date();
  
    // Get the UTC time offset for Hong Kong (UTC+8)
    // const localTimeOffset = 8 * 60; // in minutes
    const localTimeOffset = 0;
    // Calculate the local time in Hong Kong
    const localTimeTime = new Date(date.getTime() + localTimeOffset * 60 * 1000);
  
    // Extract the individual components of the time
    const year = localTimeTime.getFullYear();
    const month = String(localTimeTime.getMonth() + 1).padStart(2, '0');
    const day = String(localTimeTime.getDate()).padStart(2, '0');
    const hours = String(localTimeTime.getHours()).padStart(2, '0');
    const minutes = String(localTimeTime.getMinutes()).padStart(2, '0');
    const seconds = String(localTimeTime.getSeconds()).padStart(2, '0');
  
    // Format the time string
    const formattedTime = `${year}-${month}-${day} ${hours}:${minutes}:${seconds}`;
    return formattedTime;
}



window.onload = () =>{
    timeElement.innerHTML = getFormattedTime();
    updateCardData();
    createChart("Temperature");

}