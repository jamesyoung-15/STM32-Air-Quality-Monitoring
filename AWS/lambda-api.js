import { DynamoDBClient } from "@aws-sdk/client-dynamodb";
import {QueryCommand, ScanCommand,DeleteCommand, PutCommand, DynamoDBDocumentClient } from "@aws-sdk/lib-dynamodb";
import { randomUUID } from "crypto";

const REGION = "us-east-1";
const client = new DynamoDBClient({});
const docClient = DynamoDBDocumentClient.from(client);
const tableName = 'home-sensor-readings'


export const handler = async (event, context) => {
    let body;
    let statusCode = 200;
    const headers = {
        "Content-Type": "application/json",
        "Access-Control-Allow-Origin": "*",
    };
    // console.log(event);
    // console.log(event.routeKey);
    // let response;
    try{
        switch(true){
            // DELETE: delete single data
            case event.httpMethod == 'GET' && event.path == "/sensor-data/{location}/{id}":
                body = await getSingleData(event.queryStringParameters.location, event.queryStringParameters.id);
                body = `Deleted data with ID: ${event.queryStringParameters.id}$`
                break;
            // GET: retrieve all data
            case event.httpMethod == 'GET' && event.path == "/sensor-data":
                body = await getAllData();
                body = body.Items;
                break;
            // GET: retrieve all data from a specified location (filters data)
            case event.httpMethod == 'GET' && event.path == "/sensor-data/{location}":
                body = await getLocationData(event.queryStringParameters.location);
                body = body.Items;
                break;
            // PUT: create entry to db
            case event.httpMethod == 'PUT' && event.path == "/sensor-data":
                let requestJSON = JSON.parse(event.body);
                await insertData(requestJSON);
                body = `Put ${requestJSON}$`;
                break;
            case event.httpMethod == 'DELETE' && event.path == event.path == "/sensor-data/{location}/{id}":
                body = await deleteData(event.queryStringParameters.id);
                break;
            // error
            default:
                throw new Error(`Unsupported route: "${event.routeKey}"`);
        }
    } catch(err){
        statusCode = 400;
        body = err.message;
    } finally{
        body = JSON.stringify(body);
    }

    // return response;
    return {
        statusCode,
        body,
        headers,
    };
}

// Get a single specified data, using query for more efficiency
async function getSingleData(location, dataId){
    const command = new QueryCommand({
        TableName: tableName,
        KeyConditionExpression:"ID = :ID AND Location = :Location ",
        ExpressionAttributeValues:{
            ":Location": location,
            ":ID": dataId
        },
        ConsistentRead: true,
    });
    const response = await docClient.send(command);
    console.log(response);
    return response;
}

// Get all data from DB, using scan
async function getAllData(){
    const command = new ScanCommand({
        TableName: tableName
    });
    const response = await docClient.send(command);
    console.log(response);
    return response;
}

// Get and filter data using location
async function getLocationData(location){
    const command = new QueryCommand({
        TableName: tableName,
        KeyConditionExpression:"Location = :Location",
        ExpressionAttributeValues: {
            ":Location": location,
        },
        ConsistentRead: true,

    });
    const response = await docClient.send(command);
    console.log(response);
    return response;
}


// Put data to db
async function insertData(requestBody){
    const command = new PutCommand({
        TableName: tableName,
        Item:{
            ID: randomUUID(),
            Time: getFormattedTime(),
            Location: requestBody.Location,
            Temperature: requestBody.Temperature,
            Humidity: requestBody.Humidity,
            TVOC: requestBody.TVOC,
            eCO2: requestBody.eCO2,
            ExpireAt: Number(getTTL())
        },
    })
    const response = await docClient.send(command);
    // console.log(response);
    return response;
}

// Delete single data
async function deleteData(dataId){
    const command = new DeleteCommand({
        TableName: tableName,
        Key:{
            ID: dataId,
        },

    });
    const response = await docClient.send(command);
    console.log(response);
    return response;
}


// For getting current time, used to generate filename
let getFormattedTime = () => {
    // Create a new Date object
    const date = new Date();
  
    // Get the UTC time offset for Hong Kong (UTC+8)
    const timeOffset = -4 * 60; // in minutes
  
    // Calculate the local time in Hong Kong
    const localTime = new Date(date.getTime() + timeOffset * 60 * 1000);
  
    // Extract the individual components of the time
    const year = localTime.getFullYear();
    const month = String(localTime.getMonth() + 1).padStart(2, '0');
    const day = String(localTime.getDate()).padStart(2, '0');
    const hours = String(localTime.getHours()).padStart(2, '0');
    const minutes = String(localTime.getMinutes()).padStart(2, '0');
    const seconds = String(localTime.getSeconds()).padStart(2, '0');
  
    // Format the time string
    const formattedTime = `${year}-${month}-${day} ${hours}:${minutes}:${seconds}`;
    return formattedTime;
}

// compute ttl
let getTTL = () => {
    // Get the current time in epoch second format
    // const currentTime = Math.floor(new Date().getTime() / 1000);

    // Calculate the expireAt time (7 days from now) in epoch second format
    const expireAt = (Math.floor((new Date().getTime() + 7 * 24 * 60 * 60 * 1000) / 1000)).toString();
    
    return expireAt;
}