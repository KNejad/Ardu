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


$sql = "SELECT command  FROM script where ID=1";
$result = $GLOBALS['conn']->query($sql);
$command = "";


if ($result->num_rows > 0) {
	// output data of each row
	while($row = $result->fetch_assoc()) {
		$command = $command . $row["command"];
	}
} else {
	if($id != null){echo "Page does not exist";}
}
if ($_SERVER["REQUEST_METHOD"] == "POST") {
	// makes sure their are no illegal (dangerous) characters. 
	$command=filter_var($_POST['command'], FILTER_SANITIZE_STRING);
}
?>
<!doctype html>


<head>
	<title> Ardu Control</title>
</head>

<body>


	<form method="post" action="<?php echo htmlspecialchars($_SERVER['PHP_SELF']);?>">

	<textarea name="command" rows="50" cols="50"><?php echo $command ?></textarea>
		<br>
		<input type="submit" class="submit" value="Submit">
	</form>


<?php
if ($_SERVER["REQUEST_METHOD"] == "POST") {


	$sql = "Update script Set command  = '$command' Where ID = 1";
	$GLOBALS['conn']->query($sql);

	//removes all spaces from command. 
	$command = str_replace(" ","", $command);	
	$command = preg_replace("/\s+/","", $command);

	$sql = "Update script Set compact = '$command' Where ID = 1";
	$GLOBALS['conn']->query($sql);


}
?>


</body>

