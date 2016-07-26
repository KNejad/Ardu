<?php

ini_set('display_errors', 1);
ini_set('display_startup_errors', 1);
error_reporting(E_ALL);


$servername = "thenewbdevcom.ipagemysql.com";
$username = "keeyan";
$password = "password"; //ask knejad
$dbname ="ardu_44";


// Create connection
$GLOBALS['conn'] = new mysqli($servername, $username, $password, $dbname);

// Check connection
if ($GLOBALS['conn']->connect_error) {
	die("Connection failed: " . $conn->connect_error);
} 


$sql = "SELECT compact  FROM script where ID=1";
$result = $GLOBALS['conn']->query($sql);
$command = "";


if ($result->num_rows > 0) {
	// output data of each row
	while($row = $result->fetch_assoc()) {
		$command = $command . $row["compact"];
	}
}


	echo "BEGIN".$command; 
?>
