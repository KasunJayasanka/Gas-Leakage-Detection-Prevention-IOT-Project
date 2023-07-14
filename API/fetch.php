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

// Retrieve the first row from the "data" table
$query = "SELECT * FROM data ORDER BY id DESC LIMIT 1";
$result = $connection->query($query);

if ($result->num_rows > 0) {
    // Fetch the row as an associative array
    $row = $result->fetch_assoc();

    $weightQuery = "SELECT gasWeight FROM weight_data ORDER BY id DESC LIMIT 1";
    $weightResult = $connection->query($weightQuery);

    if ($weightResult->num_rows > 0) {
        // Fetch the gasWeight value as an associative array
        $weightRow = $weightResult->fetch_assoc();

        // Append the gasWeight value to the existing row
        $row['gasWeight'] = $weightRow['gasWeight'];
    }

    // Convert the row to JSON format
    $jsonResponse = json_encode($row);

    // Set the Content-Type header to application/json
    header('Content-Type: application/json');

    // Output the JSON response
    echo $jsonResponse;
} else {
    // No rows found in the table
    echo 'No data found.';
}

// Close the database connection
$connection->close();
?>
