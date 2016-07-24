
<?php

ini_set('display_errors', 1);
ini_set('display_startup_errors', 1);
error_reporting(E_ALL);

$ardu = fopen("ardu.txt", "r") or die("Unable to open file!");
$command =  fread($ardu,filesize("ardu.txt"));
fclose($ardu);


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
	//creates and/or opens ardu.txt file and writes data to it	
	$ardu = fopen("ardu.txt", "w");

	//writes data to file and closes
	fwrite($ardu, $command);	
	fclose($ardu);



	//creates and/or opens file ardu_compact.txt. This file is smaller and is accessed by ardu to save download times and processing power of ardu
	$ardu_compact = fopen("ardu_compact.txt", "w");

	//removes all spaces from command. 
	$command = str_replace(" ","", $command);	
	$command = preg_replace("/\s+/","", $command);

	//writes data to file
	fwrite($ardu_compact, $command);	
	fclose($ardu_compact);


}
?>


</body>

