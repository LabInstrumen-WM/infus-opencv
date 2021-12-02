<?php
require_once('koneksi.php');
$id = $_GET['id'];
$result=array();
$text=" Volume Sisa = ";
$sql="SELECT * FROM tbinfus where id='$id'+'R'";
$r=mysqli_query($con,$sql);
while($row=mysqli_fetch_array($r))
	{
	array_push($result,array('volume'=>$row['id'].$text.$row['volume'],'id'=>$row['id'],'volume2'=>$row['volume']));
	}
echo json_encode($result);
mysqli_close($con);
?>
