<!DOCTYPE html>
<html>

<head>
<style>
.split {
  height: 80%;
  width: 50%;
  position: fixed;
  z-index: 1;
  top: 100;
  overflow-x: hidden;
  padding-top: 20px;
}

/* Control the left side */
.left {
  left: 0;
  background-color: white;
}

/* Control the right side */
.right {
  right: 0;
  background-color: white;
}

/* If you want the content centered horizontally and vertically */
.centered {
  position: absolute;
  top: 30%;
  left: 30%;
  transform: translate(-30%, -30%);
  text-align: center;
}

/* Style the image inside the centered container, if needed */
.centered img {
  height:500px;
  width: 350px;
  //border-radius: 50%;
}
</style>
</head>

<body>
<h1> <center>Monitoring Cairan Infus</center></h1>

<?php
$mysqli=mysqli_connect('localhost','jose','josekroos10','coba');
if (!$mysqli)
die("Can't connect to MySQL: ".mysqli_connect_error());
$sql="SELECT * FROM tbinfus";
$i=0;
if($result=$mysqli->query($sql))
{
	while($row=$result->fetch_assoc())
	{
	if($i==0)
	{
		echo'<div class="split left">';
	}
	else
	{
		echo '<div class="split right">';
	}
	echo'<div class="centered">';
	echo'<img src="data:image/jpeg;base64 ,'.base64_encode($row['image']).'"/>';
	echo'</br>';
	echo 'ID=';
	echo $row['id'];
	echo'</br>';
	echo 'Volume=';
	echo $row['volume'];
	echo'</div>';
	echo'</div>';
	$i=$i+1;
	}
		
}
	


?>

</body>

</html>






