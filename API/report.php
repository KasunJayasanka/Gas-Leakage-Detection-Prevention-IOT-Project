<?php
// Database credentials
$host = 'localhost';
$user = 'nodemcu_user';
$password = '';
$database = 'mobileappdb_test2';

// Create a new connection
$connection = new mysqli($host, $user, $password, $database);

// Check connection
if ($connection->connect_error) {
    die('Connection failed: ' . $connection->connect_error);
}

// Get JSON input
$jsonInput = file_get_contents('php://input');

// Decode JSON input
$inputData = json_decode($jsonInput, true);

// Extract start date and end date
$start_date = $inputData['start_date'];
$end_date = $inputData['end_date'];

// Prepare the query with placeholders to prevent SQL injection
$query = "SELECT * FROM weight_data WHERE dateTimeStamp BETWEEN ? AND ? ";
$statement = $connection->prepare($query);

// Bind the parameters to the placeholders
$statement->bind_param('ss', $start_date, $end_date);

// Execute the query
$statement->execute();

// Get the result set
$result = $statement->get_result();

// Create an array to hold the rows
$rows = array();

// Loop through the result set and fetch each row
while ($row = $result->fetch_assoc()) {
    $rows[] = $row;
}

// Convert the array to JSON format
$jsonResponse = json_encode($rows);

// Set the Content-Type header to application/json
header('Content-Type: application/json');

// Output the JSON response
echo $jsonResponse;

// Close the statement and database connection
$statement->close();
$connection->close();
?>
